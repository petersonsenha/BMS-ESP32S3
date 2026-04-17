#include "bms_controller.h"

namespace bms {

namespace {

bool is_charging(const PackTelemetry& telemetry, uint32_t threshold_ma) {
  return telemetry.pack_current_ma <= -static_cast<int32_t>(threshold_ma);
}

bool is_discharging(const PackTelemetry& telemetry, uint32_t threshold_ma) {
  return telemetry.pack_current_ma >= static_cast<int32_t>(threshold_ma);
}

}  // namespace

BmsController::BmsController(ProtectionConfig config) : config_(config) {}

BmsSnapshot BmsController::update(const PackTelemetry& telemetry) {
  const FaultStatus faults = evaluate_faults(telemetry);
  const BalanceCommand balance = evaluate_balancing(telemetry, faults);
  const BmsState requested_state = resolve_state(telemetry, faults, balance);
  BmsState current_state = requested_state;

  if (!startup_complete_ && requested_state != BmsState::Fault) {
    current_state = BmsState::Startup;
  }

  const OutputCommand outputs =
      apply_state_safety_guards(evaluate_outputs(faults, balance), current_state);
  const BmsStateContext state_context =
      build_state_context(telemetry, faults, balance, requested_state);

  BmsSnapshot snapshot{};
  snapshot.telemetry = telemetry;
  snapshot.faults = faults;
  snapshot.outputs = outputs;
  snapshot.state_context = state_context;

  previous_state_ = current_state;
  startup_complete_ = true;
  return snapshot;
}

FaultStatus BmsController::evaluate_faults(const PackTelemetry& telemetry) const {
  FaultStatus faults{};
  faults.afe_fault = !telemetry.afe_healthy;

  for (uint16_t cell_voltage_mv : telemetry.cell_voltage_mv) {
    if (cell_voltage_mv >= config_.cell_over_voltage_mv) {
      faults.cell_over_voltage = true;
    }
    if (cell_voltage_mv <= config_.cell_under_voltage_mv) {
      faults.cell_under_voltage = true;
    }
  }

  if (telemetry.pack_current_ma < 0 &&
      static_cast<uint32_t>(-telemetry.pack_current_ma) >
          config_.pack_over_current_charge_ma) {
    faults.charge_over_current = true;
  }

  if (telemetry.pack_current_ma > 0 &&
      static_cast<uint32_t>(telemetry.pack_current_ma) >
          config_.pack_over_current_discharge_ma) {
    faults.discharge_over_current = true;
  }

  const bool charging =
      is_charging(telemetry, config_.activity_current_threshold_ma);

  for (int16_t temperature_deci_c : telemetry.temperatures_deci_c) {
    if (temperature_deci_c > config_.discharge_temp_max_deci_c) {
      faults.discharge_over_temperature = true;
    }

    if (charging && temperature_deci_c < config_.charge_temp_min_deci_c) {
      faults.charge_under_temperature = true;
    }

    if (charging && temperature_deci_c > config_.charge_temp_max_deci_c) {
      faults.charge_over_temperature = true;
    }
  }

  return faults;
}

BalanceCommand BmsController::evaluate_balancing(const PackTelemetry& telemetry,
                                                 const FaultStatus& faults) const {
  BalanceCommand balance{};

  if (faults.any()) {
    return balance;
  }

  if (is_discharging(telemetry, config_.activity_current_threshold_ma)) {
    return balance;
  }

  const uint16_t min_cell_mv = min_cell_voltage(telemetry);

  for (std::size_t index = 0; index < telemetry.cell_voltage_mv.size(); ++index) {
    const uint16_t cell_voltage_mv = telemetry.cell_voltage_mv[index];
    const uint16_t delta_mv = static_cast<uint16_t>(cell_voltage_mv - min_cell_mv);

    if (cell_voltage_mv >= config_.balance_start_mv &&
        delta_mv >= config_.balance_delta_mv) {
      balance.cell_enabled[index] = true;
    }
  }

  return balance;
}

OutputCommand BmsController::evaluate_outputs(const FaultStatus& faults,
                                              const BalanceCommand& balance) const {
  OutputCommand outputs{};

  outputs.charge_mosfet_enabled = !faults.afe_fault && !faults.cell_over_voltage &&
                                  !faults.charge_over_current &&
                                  !faults.charge_under_temperature &&
                                  !faults.charge_over_temperature;

  outputs.discharge_mosfet_enabled =
      !faults.afe_fault && !faults.cell_under_voltage &&
      !faults.discharge_over_current && !faults.discharge_over_temperature;

  outputs.balance = balance;
  return outputs;
}

BmsState BmsController::resolve_state(const PackTelemetry& telemetry,
                                      const FaultStatus& faults,
                                      const BalanceCommand& balance) const {
  if (faults.any()) {
    return BmsState::Fault;
  }

  if (is_charging(telemetry, config_.activity_current_threshold_ma)) {
    return balance.any() ? BmsState::Balancing : BmsState::Charging;
  }

  if (is_discharging(telemetry, config_.activity_current_threshold_ma)) {
    return BmsState::Discharging;
  }

  if (balance.any()) {
    return BmsState::Balancing;
  }

  return BmsState::Idle;
}

BmsStateContext BmsController::build_state_context(const PackTelemetry& telemetry,
                                                   const FaultStatus& faults,
                                                   const BalanceCommand& balance,
                                                   BmsState requested_state) const {
  BmsStateContext context{};
  context.previous = previous_state_;
  context.current =
      (!startup_complete_ && requested_state != BmsState::Fault)
          ? BmsState::Startup
          : requested_state;
  context.requested = requested_state;
  context.charging_detected =
      is_charging(telemetry, config_.activity_current_threshold_ma);
  context.discharging_detected =
      is_discharging(telemetry, config_.activity_current_threshold_ma);
  context.balancing_requested = balance.any();
  context.fault_active = faults.any();
  context.metrics.min_cell_voltage_mv = min_cell_voltage(telemetry);
  context.metrics.max_cell_voltage_mv = max_cell_voltage(telemetry);
  context.metrics.cell_voltage_delta_mv = static_cast<uint16_t>(
      context.metrics.max_cell_voltage_mv - context.metrics.min_cell_voltage_mv);
  context.metrics.min_temperature_deci_c = min_temperature(telemetry);
  context.metrics.max_temperature_deci_c = max_temperature(telemetry);
  context.metrics.pack_current_ma = telemetry.pack_current_ma;
  return context;
}

OutputCommand BmsController::apply_state_safety_guards(
    const OutputCommand& outputs, BmsState current_state) const {
  if (current_state != BmsState::Startup && current_state != BmsState::Fault) {
    return outputs;
  }

  return OutputCommand{};
}

}  // namespace bms
