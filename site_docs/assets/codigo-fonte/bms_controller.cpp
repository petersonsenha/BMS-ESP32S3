#include "bms_controller.h"

namespace bms {

namespace {

bool is_charging(const PackTelemetry& telemetry, uint32_t threshold_ma) {
  return telemetry.pack_current_ma <= -static_cast<int32_t>(threshold_ma);
}

bool is_discharging(const PackTelemetry& telemetry, uint32_t threshold_ma) {
  return telemetry.pack_current_ma >= static_cast<int32_t>(threshold_ma);
}

uint16_t saturating_subtract_u16(uint16_t value, uint16_t delta) {
  return value > delta ? static_cast<uint16_t>(value - delta) : 0U;
}

uint32_t saturating_subtract_u32(uint32_t value, uint32_t delta) {
  return value > delta ? value - delta : 0U;
}

}  // namespace

BmsController::BmsController(BmsConfig config)
    : config_(config), state_estimator_(config) {}

BmsSnapshot BmsController::update(const PackTelemetry& telemetry, uint32_t now_ms) {
  const uint32_t idle_time_ms = update_idle_time_ms(telemetry, now_ms);
  const FaultStatus raw_faults = evaluate_faults(telemetry, now_ms);
  const FaultStatus faults = apply_fault_latch(raw_faults, now_ms);
  const BalanceCommand balance = evaluate_balancing(telemetry, faults);
  const BmsState requested_state = resolve_state(telemetry, faults, balance);
  BmsState current_state = requested_state;

  if (!startup_complete_ && requested_state != BmsState::Fault) {
    current_state = BmsState::Startup;
  }

  const OutputCommand outputs =
      apply_state_safety_guards(evaluate_outputs(faults, balance, requested_state),
                                current_state);
  const BmsStateContext state_context =
      build_state_context(telemetry, faults, raw_faults, balance, requested_state,
                          idle_time_ms, now_ms);
  const BmsStateEstimate estimate =
      state_estimator_.update(telemetry, faults, idle_time_ms, now_ms);

  BmsSnapshot snapshot{};
  snapshot.telemetry = telemetry;
  snapshot.raw_faults = raw_faults;
  snapshot.faults = faults;
  snapshot.outputs = outputs;
  snapshot.state_context = state_context;
  snapshot.estimate = estimate;

  previous_state_ = current_state;
  startup_complete_ = true;
  return snapshot;
}

void BmsController::request_fault_clear() { fault_clear_requested_ = true; }

void BmsController::set_config(BmsConfig config) {
  config_ = config;
  state_estimator_.set_config(config);
}

const BmsConfig& BmsController::config() const { return config_; }

FaultStatus BmsController::evaluate_faults(const PackTelemetry& telemetry,
                                           uint32_t now_ms) {
  FaultStatus faults = active_faults_;
  const uint16_t min_cell_mv = min_cell_voltage(telemetry);
  const uint16_t max_cell_mv = max_cell_voltage(telemetry);
  const int16_t min_temp_deci_c = min_temperature(telemetry);
  const int16_t max_temp_deci_c = max_temperature(telemetry);
  const uint32_t abs_current_ma = absolute_pack_current(telemetry);
  const bool charging =
      is_charging(telemetry, config_.operation.activity_current_threshold_ma);
  const bool discharging =
      is_discharging(telemetry, config_.operation.activity_current_threshold_ma);

  faults.afe_fault = !telemetry.afe_healthy;

  faults.cell_over_voltage =
      (max_cell_mv >= config_.protection.cell_over_voltage_mv) ||
      (active_faults_.cell_over_voltage &&
       max_cell_mv >
           saturating_subtract_u16(config_.protection.cell_over_voltage_mv,
                                   config_.fault_handling.cell_voltage_hysteresis_mv));

  faults.cell_under_voltage =
      (min_cell_mv <= config_.protection.cell_under_voltage_mv) ||
      (active_faults_.cell_under_voltage &&
       min_cell_mv <
           (config_.protection.cell_under_voltage_mv +
            config_.fault_handling.cell_voltage_hysteresis_mv));

  faults.charge_over_current =
      (charging &&
       abs_current_ma >= config_.protection.pack_over_current_charge_ma) ||
      (active_faults_.charge_over_current && telemetry.pack_current_ma < 0 &&
       abs_current_ma >
           saturating_subtract_u32(
               config_.protection.pack_over_current_charge_ma,
               config_.fault_handling.current_hysteresis_ma));

  faults.discharge_over_current =
      (discharging &&
       abs_current_ma >= config_.protection.pack_over_current_discharge_ma) ||
      (active_faults_.discharge_over_current && telemetry.pack_current_ma > 0 &&
       abs_current_ma >
           saturating_subtract_u32(
               config_.protection.pack_over_current_discharge_ma,
               config_.fault_handling.current_hysteresis_ma));

  faults.short_circuit_discharge =
      (discharging &&
       abs_current_ma >= config_.fault_handling.short_circuit_discharge_ma) ||
      (active_faults_.short_circuit_discharge && telemetry.pack_current_ma > 0 &&
       abs_current_ma >
           saturating_subtract_u32(
               config_.fault_handling.short_circuit_discharge_ma,
               config_.fault_handling.current_hysteresis_ma));

  faults.charge_under_temperature =
      (charging &&
       min_temp_deci_c <= config_.protection.charge_temp_min_deci_c) ||
      (active_faults_.charge_under_temperature && charging &&
       min_temp_deci_c <
           (config_.protection.charge_temp_min_deci_c +
            config_.fault_handling.temperature_hysteresis_deci_c));

  faults.charge_over_temperature =
      (charging &&
       max_temp_deci_c >= config_.protection.charge_temp_max_deci_c) ||
      (active_faults_.charge_over_temperature && charging &&
       max_temp_deci_c >
           (config_.protection.charge_temp_max_deci_c -
            config_.fault_handling.temperature_hysteresis_deci_c));

  faults.discharge_over_temperature =
      (max_temp_deci_c >= config_.protection.discharge_temp_max_deci_c) ||
      (active_faults_.discharge_over_temperature &&
       max_temp_deci_c >
           (config_.protection.discharge_temp_max_deci_c -
            config_.fault_handling.temperature_hysteresis_deci_c));

  if (!faults.any() &&
      config_.operation.precharge_required &&
      is_discharging(telemetry, config_.operation.activity_current_threshold_ma) &&
      !is_precharge_complete(telemetry)) {
    if (!precharge_active_) {
      precharge_active_ = true;
      precharge_started_ms_ = now_ms;
    }

    if (precharge_elapsed_ms(now_ms) >= config_.operation.precharge_timeout_ms) {
      faults.precharge_timeout = true;
      precharge_active_ = false;
    }
  } else if (faults.any() || is_precharge_complete(telemetry) ||
             !config_.operation.precharge_required ||
             !is_discharging(telemetry, config_.operation.activity_current_threshold_ma)) {
    precharge_active_ = false;
  }

  active_faults_ = faults;
  return faults;
}

FaultStatus BmsController::apply_fault_latch(const FaultStatus& raw_faults,
                                             uint32_t now_ms) {
  if (!config_.fault_handling.fault_latch_enabled) {
    return raw_faults;
  }

  if (raw_faults.any()) {
    latched_faults_.merge_from(raw_faults);
    last_fault_transition_ms_ = now_ms;
  } else if (latched_faults_.any()) {
    const uint32_t required_delay_ms =
        latched_faults_.short_circuit_discharge
            ? config_.fault_handling.short_circuit_release_delay_ms
            : config_.fault_handling.fault_release_delay_ms;
    const bool release_delay_elapsed =
        (now_ms >= last_fault_transition_ms_) &&
        ((now_ms - last_fault_transition_ms_) >= required_delay_ms);

    if ((config_.fault_handling.auto_recovery_enabled || fault_clear_requested_) &&
        release_delay_elapsed) {
      latched_faults_.clear();
      fault_clear_requested_ = false;
    }
  }

  FaultStatus effective_faults = raw_faults;
  effective_faults.merge_from(latched_faults_);
  return effective_faults;
}

BalanceCommand BmsController::evaluate_balancing(const PackTelemetry& telemetry,
                                                 const FaultStatus& faults) const {
  BalanceCommand balance{};

  if (faults.any() || should_run_precharge(telemetry, faults)) {
    return balance;
  }

  const bool charging =
      is_charging(telemetry, config_.operation.activity_current_threshold_ma);
  const bool discharging =
      is_discharging(telemetry, config_.operation.activity_current_threshold_ma);

  if (discharging) {
    return balance;
  }

  if (charging && !config_.operation.allow_balancing_while_charging) {
    return balance;
  }

  if (!charging && !config_.operation.allow_balancing_in_idle) {
    return balance;
  }

  if (absolute_pack_current(telemetry) >
      config_.operation.balance_max_pack_current_ma) {
    return balance;
  }

  if (max_temperature(telemetry) >
      config_.operation.balance_max_temperature_deci_c) {
    return balance;
  }

  const uint16_t min_cell_mv = min_cell_voltage(telemetry);

  for (std::size_t index = 0; index < telemetry.cell_voltage_mv.size(); ++index) {
    const uint16_t cell_voltage_mv = telemetry.cell_voltage_mv[index];
    const uint16_t delta_mv = static_cast<uint16_t>(cell_voltage_mv - min_cell_mv);

    if (cell_voltage_mv >= config_.protection.balance_start_mv &&
        delta_mv >= config_.protection.balance_delta_mv) {
      balance.cell_enabled[index] = true;
    }
  }

  return balance;
}

OutputCommand BmsController::evaluate_outputs(const FaultStatus& faults,
                                              const BalanceCommand& balance,
                                              BmsState requested_state) const {
  OutputCommand outputs{};

  outputs.charge_mosfet_enabled =
      !faults.afe_fault && !faults.cell_over_voltage &&
      !faults.precharge_timeout &&
      !faults.charge_over_current && !faults.short_circuit_discharge &&
      !faults.charge_under_temperature && !faults.charge_over_temperature &&
      !faults.discharge_over_temperature;

  outputs.discharge_mosfet_enabled =
      !faults.afe_fault && !faults.cell_under_voltage &&
      !faults.precharge_timeout &&
      !faults.discharge_over_current && !faults.short_circuit_discharge &&
      !faults.discharge_over_temperature;

  outputs.precharge_enabled =
      requested_state == BmsState::Precharge && !faults.any();
  outputs.balance = balance;

  if (outputs.precharge_enabled) {
    outputs.charge_mosfet_enabled = false;
    outputs.discharge_mosfet_enabled = false;
    outputs.balance = BalanceCommand{};
  }

  return outputs;
}

BmsState BmsController::resolve_state(const PackTelemetry& telemetry,
                                      const FaultStatus& faults,
                                      const BalanceCommand& balance) const {
  if (faults.any()) {
    return BmsState::Fault;
  }

  if (should_run_precharge(telemetry, faults)) {
    return BmsState::Precharge;
  }

  if (is_charging(telemetry, config_.operation.activity_current_threshold_ma)) {
    return balance.any() ? BmsState::Balancing : BmsState::Charging;
  }

  if (is_discharging(telemetry, config_.operation.activity_current_threshold_ma)) {
    return BmsState::Discharging;
  }

  if (balance.any()) {
    return BmsState::Balancing;
  }

  return BmsState::Idle;
}

BmsStateContext BmsController::build_state_context(
    const PackTelemetry& telemetry, const FaultStatus& faults,
    const FaultStatus& raw_faults, const BalanceCommand& balance,
    BmsState requested_state, uint32_t idle_time_ms, uint32_t now_ms) const {
  BmsStateContext context{};
  context.previous = previous_state_;
  context.current =
      (!startup_complete_ && requested_state != BmsState::Fault)
          ? BmsState::Startup
          : requested_state;
  context.requested = requested_state;
  context.charging_detected =
      is_charging(telemetry, config_.operation.activity_current_threshold_ma);
  context.discharging_detected =
      is_discharging(telemetry, config_.operation.activity_current_threshold_ma);
  context.balancing_requested = balance.any();
  context.charge_termination_reached = is_charge_termination_reached(telemetry);
  context.fault_active = faults.any();
  context.fault_latched = latched_faults_.any() && !raw_faults.any();
  context.precharge_active = context.current == BmsState::Precharge;
  context.precharge_complete = is_precharge_complete(telemetry);
  context.precharge_timed_out = faults.precharge_timeout;
  context.resting_for_ocv_correction =
      idle_time_ms >= config_.estimation.ocv_correction_idle_ms &&
      absolute_pack_current(telemetry) <=
          config_.estimation.ocv_correction_max_current_ma;
  context.precharge_required = config_.operation.precharge_required;
  context.idle_time_ms = idle_time_ms;
  context.precharge_elapsed_ms =
      context.precharge_active ? precharge_elapsed_ms(now_ms) : 0U;
  context.metrics.min_cell_voltage_mv = min_cell_voltage(telemetry);
  context.metrics.max_cell_voltage_mv = max_cell_voltage(telemetry);
  context.metrics.cell_voltage_delta_mv = static_cast<uint16_t>(
      context.metrics.max_cell_voltage_mv - context.metrics.min_cell_voltage_mv);
  context.metrics.output_voltage_mv = telemetry.output_voltage_mv;
  context.metrics.precharge_delta_mv =
      telemetry.pack_voltage_mv > telemetry.output_voltage_mv
          ? static_cast<uint16_t>(telemetry.pack_voltage_mv -
                                  telemetry.output_voltage_mv)
          : 0U;
  context.metrics.min_temperature_deci_c = min_temperature(telemetry);
  context.metrics.max_temperature_deci_c = max_temperature(telemetry);
  context.metrics.pack_current_ma = telemetry.pack_current_ma;
  context.metrics.absolute_pack_current_ma = absolute_pack_current(telemetry);
  return context;
}

OutputCommand BmsController::apply_state_safety_guards(
    const OutputCommand& outputs, BmsState current_state) const {
  if (current_state != BmsState::Startup && current_state != BmsState::Fault) {
    return outputs;
  }

  return OutputCommand{};
}

bool BmsController::is_charge_termination_reached(
    const PackTelemetry& telemetry) const {
  return telemetry.pack_current_ma <= 0 &&
         absolute_pack_current(telemetry) <=
             config_.operation.charge_termination_current_ma &&
         max_cell_voltage(telemetry) >=
             saturating_subtract_u16(
                 config_.protection.cell_over_voltage_mv,
                 config_.fault_handling.cell_voltage_hysteresis_mv);
}

bool BmsController::should_run_precharge(const PackTelemetry& telemetry,
                                         const FaultStatus& faults) const {
  return !faults.any() && config_.operation.precharge_required &&
         is_discharging(telemetry, config_.operation.activity_current_threshold_ma) &&
         !is_precharge_complete(telemetry);
}

bool BmsController::is_precharge_complete(
    const PackTelemetry& telemetry) const {
  if (!config_.operation.precharge_required) {
    return true;
  }

  if (!telemetry.output_voltage_valid) {
    return false;
  }

  if (telemetry.output_voltage_mv >= telemetry.pack_voltage_mv) {
    return true;
  }

  return (telemetry.pack_voltage_mv - telemetry.output_voltage_mv) <=
         config_.operation.precharge_target_delta_mv;
}

uint32_t BmsController::precharge_elapsed_ms(uint32_t now_ms) const {
  if (!precharge_active_ || now_ms < precharge_started_ms_) {
    return 0U;
  }

  return now_ms - precharge_started_ms_;
}

uint32_t BmsController::update_idle_time_ms(const PackTelemetry& telemetry,
                                            uint32_t now_ms) {
  const bool in_relaxation_window =
      absolute_pack_current(telemetry) <=
      config_.estimation.ocv_correction_max_current_ma;

  if (!in_relaxation_window) {
    idle_window_active_ = false;
    idle_started_ms_ = now_ms;
    return 0U;
  }

  if (!idle_window_active_) {
    idle_window_active_ = true;
    idle_started_ms_ = now_ms;
    return 0U;
  }

  return now_ms >= idle_started_ms_ ? (now_ms - idle_started_ms_) : 0U;
}

}  // namespace bms
