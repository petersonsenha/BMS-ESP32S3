#include "bms_state_estimator.h"

namespace bms {

namespace {

constexpr float kMilliampHoursPerAmpHour = 1000.0f;
constexpr float kMillisPerHour = 3600000.0f;

float min_float(float left, float right) {
  return left < right ? left : right;
}

float max_float(float left, float right) {
  return left > right ? left : right;
}

}  // namespace

BmsStateEstimator::BmsStateEstimator(BmsConfig config) : config_(config) {
  estimate_.estimated_internal_resistance_mohm =
      config_.estimation.nominal_internal_resistance_mohm;
}

void BmsStateEstimator::set_config(BmsConfig config) {
  config_ = config;
  if (estimate_.estimated_internal_resistance_mohm <= 0.0f) {
    estimate_.estimated_internal_resistance_mohm =
        config_.estimation.nominal_internal_resistance_mohm;
  }
}

BmsStateEstimate BmsStateEstimator::update(const PackTelemetry& telemetry,
                                          const FaultStatus& faults,
                                          uint32_t idle_time_ms,
                                          uint32_t now_ms) {
  const float nominal_capacity_mah =
      static_cast<float>(config_.estimation.nominal_capacity_mah);
  const float nominal_pack_voltage_v =
      (static_cast<float>(config_.estimation.nominal_cell_voltage_mv) *
       static_cast<float>(kSeriesCellCount)) /
      1000.0f;

  if (!initialized_) {
    estimate_.soc_pct = estimate_soc_from_ocv(telemetry);
    estimate_.remaining_capacity_mah =
        nominal_capacity_mah * estimate_.soc_pct / 100.0f;
    estimate_.soe_wh =
        (estimate_.remaining_capacity_mah / kMilliampHoursPerAmpHour) *
        nominal_pack_voltage_v;
    estimate_.soh_pct = 100.0f;
    estimate_.sof_pct = faults.any() ? 0.0f : estimate_.soc_pct;
    initialized_ = true;
    last_update_ms_ = now_ms;
  }

  const uint32_t dt_ms =
      now_ms >= last_update_ms_ ? now_ms - last_update_ms_ : 0U;
  const float dt_hours = static_cast<float>(dt_ms) / kMillisPerHour;

  if (dt_ms > 0U) {
    if (telemetry.pack_current_ma < 0) {
      const float charged_mah =
          static_cast<float>(-telemetry.pack_current_ma) * dt_hours *
          (static_cast<float>(config_.estimation.coulomb_efficiency_ppm) /
           1000000.0f);
      estimate_.remaining_capacity_mah += charged_mah;
      estimate_.cumulative_charge_mah += charged_mah;
    } else if (telemetry.pack_current_ma > 0) {
      const float discharged_mah =
          static_cast<float>(telemetry.pack_current_ma) * dt_hours;
      estimate_.remaining_capacity_mah -= discharged_mah;
      estimate_.cumulative_discharge_mah += discharged_mah;
    }

    estimate_.energy_throughput_wh +=
        (static_cast<float>(absolute_pack_current(telemetry)) / 1000.0f) *
        (static_cast<float>(telemetry.pack_voltage_mv) / 1000.0f) * dt_hours;
  }

  if (estimate_.remaining_capacity_mah < 0.0f) {
    estimate_.remaining_capacity_mah = 0.0f;
  }
  if (estimate_.remaining_capacity_mah > nominal_capacity_mah) {
    estimate_.remaining_capacity_mah = nominal_capacity_mah;
  }

  estimate_.soc_pct = clamp_pct((estimate_.remaining_capacity_mah / nominal_capacity_mah) *
                                100.0f);
  estimate_.ocv_correction_applied = false;

  if (config_.estimation.enable_ocv_correction &&
      idle_time_ms >= config_.estimation.ocv_correction_idle_ms &&
      absolute_pack_current(telemetry) <=
          config_.estimation.ocv_correction_max_current_ma) {
    const float ocv_soc_pct = estimate_soc_from_ocv(telemetry);
    estimate_.soc_pct =
        clamp_pct((estimate_.soc_pct * 0.65f) + (ocv_soc_pct * 0.35f));
    estimate_.remaining_capacity_mah =
        nominal_capacity_mah * estimate_.soc_pct / 100.0f;
    estimate_.ocv_correction_applied = true;
  }

  const uint32_t resistance_sampling_threshold_ma =
      config_.operation.activity_current_threshold_ma > 1000U
          ? config_.operation.activity_current_threshold_ma
          : 1000U;
  if (absolute_pack_current(telemetry) >= resistance_sampling_threshold_ma) {
    const float expected_ocv_mv = soc_to_pack_ocv_mv(estimate_.soc_pct);
    const float voltage_drop_mv =
        expected_ocv_mv - static_cast<float>(telemetry.pack_voltage_mv);
    const float current_a =
        static_cast<float>(absolute_pack_current(telemetry)) / 1000.0f;
    if (current_a > 0.0f && voltage_drop_mv > 0.0f) {
      const float inferred_resistance_mohm = voltage_drop_mv / current_a;
      estimate_.estimated_internal_resistance_mohm =
          (estimate_.estimated_internal_resistance_mohm * 0.85f) +
          (inferred_resistance_mohm * 0.15f);
    }
  }

  const float soh_capacity_pct = 100.0f;
  const float soh_resistance_pct =
      estimate_.estimated_internal_resistance_mohm > 0.0f
          ? clamp_pct((config_.estimation.nominal_internal_resistance_mohm /
                       estimate_.estimated_internal_resistance_mohm) *
                      100.0f)
          : 100.0f;
  estimate_.soh_pct =
      clamp_pct((soh_capacity_pct * 0.65f) + (soh_resistance_pct * 0.35f));

  estimate_.soe_wh =
      (estimate_.remaining_capacity_mah / kMilliampHoursPerAmpHour) *
      nominal_pack_voltage_v;

  const float pack_voltage_v =
      static_cast<float>(telemetry.pack_voltage_mv) / 1000.0f;
  const float charge_limit_a =
      static_cast<float>(config_.protection.pack_over_current_charge_ma) / 1000.0f;
  const float discharge_limit_a =
      static_cast<float>(config_.protection.pack_over_current_discharge_ma) /
      1000.0f;
  const float max_temp_deci_c =
      static_cast<float>(max_temperature(telemetry));
  const float min_temp_deci_c =
      static_cast<float>(min_temperature(telemetry));

  float discharge_temp_scale = 1.0f;
  const float discharge_margin_start =
      static_cast<float>(config_.protection.discharge_temp_max_deci_c - 50);
  if (max_temp_deci_c >=
      static_cast<float>(config_.protection.discharge_temp_max_deci_c)) {
    discharge_temp_scale = 0.0f;
  } else if (max_temp_deci_c > discharge_margin_start) {
    discharge_temp_scale =
        (static_cast<float>(config_.protection.discharge_temp_max_deci_c) -
         max_temp_deci_c) /
        50.0f;
  }

  float charge_temp_scale = 1.0f;
  if (min_temp_deci_c <= static_cast<float>(config_.protection.charge_temp_min_deci_c) ||
      max_temp_deci_c >= static_cast<float>(config_.protection.charge_temp_max_deci_c)) {
    charge_temp_scale = 0.0f;
  } else {
    const float low_temp_window =
        static_cast<float>(config_.protection.charge_temp_min_deci_c + 30);
    if (min_temp_deci_c < low_temp_window) {
      charge_temp_scale =
          min_float(charge_temp_scale,
                    (min_temp_deci_c -
                     static_cast<float>(config_.protection.charge_temp_min_deci_c)) /
                        30.0f);
    }

    const float high_temp_window =
        static_cast<float>(config_.protection.charge_temp_max_deci_c - 50);
    if (max_temp_deci_c > high_temp_window) {
      charge_temp_scale =
          min_float(charge_temp_scale,
                    (static_cast<float>(config_.protection.charge_temp_max_deci_c) -
                     max_temp_deci_c) /
                        50.0f);
    }
  }

  float discharge_soc_scale = 1.0f;
  if (estimate_.soc_pct < config_.estimation.minimum_soc_for_discharge_pct) {
    discharge_soc_scale =
        config_.estimation.minimum_soc_for_discharge_pct > 0.0f
            ? estimate_.soc_pct /
                  config_.estimation.minimum_soc_for_discharge_pct
            : 0.0f;
  }

  float charge_soc_scale = 1.0f;
  if (estimate_.soc_pct > 95.0f) {
    charge_soc_scale = (100.0f - estimate_.soc_pct) / 5.0f;
  }

  discharge_temp_scale = clamp_pct(discharge_temp_scale * 100.0f) / 100.0f;
  charge_temp_scale = clamp_pct(charge_temp_scale * 100.0f) / 100.0f;
  discharge_soc_scale = clamp_pct(discharge_soc_scale * 100.0f) / 100.0f;
  charge_soc_scale = clamp_pct(charge_soc_scale * 100.0f) / 100.0f;

  const float discharge_capability_pct =
      faults.any() ? 0.0f : clamp_pct(discharge_temp_scale * discharge_soc_scale * 100.0f);
  const float charge_capability_pct =
      faults.any() ? 0.0f : clamp_pct(charge_temp_scale * charge_soc_scale * 100.0f);

  estimate_.sop_discharge_w =
      pack_voltage_v * discharge_limit_a * (discharge_capability_pct / 100.0f);
  estimate_.sop_charge_w =
      pack_voltage_v * charge_limit_a * (charge_capability_pct / 100.0f);

  const float function_capability_pct =
      max_float(charge_capability_pct, discharge_capability_pct);
  estimate_.sof_pct =
      faults.any() ? 0.0f
                   : clamp_pct(min_float(
                         function_capability_pct,
                         min_float(estimate_.soc_pct, estimate_.soh_pct)));

  last_update_ms_ = now_ms;
  return estimate_;
}

float BmsStateEstimator::estimate_soc_from_ocv(
    const PackTelemetry& telemetry) const {
  const float average_cell_mv =
      static_cast<float>(telemetry.pack_voltage_mv) /
      static_cast<float>(kSeriesCellCount);
  const float span_mv = static_cast<float>(config_.estimation.full_cell_voltage_mv) -
                        static_cast<float>(config_.estimation.empty_cell_voltage_mv);
  if (span_mv <= 0.0f) {
    return 0.0f;
  }

  const float normalized =
      (average_cell_mv -
       static_cast<float>(config_.estimation.empty_cell_voltage_mv)) /
      span_mv;
  return clamp_pct(normalized * 100.0f);
}

float BmsStateEstimator::soc_to_pack_ocv_mv(float soc_pct) const {
  const float clamped_soc = clamp_pct(soc_pct) / 100.0f;
  const float cell_ocv_mv =
      static_cast<float>(config_.estimation.empty_cell_voltage_mv) +
      (static_cast<float>(config_.estimation.full_cell_voltage_mv -
                          config_.estimation.empty_cell_voltage_mv) *
       clamped_soc);
  return cell_ocv_mv * static_cast<float>(kSeriesCellCount);
}

float BmsStateEstimator::clamp_pct(float value) const {
  if (value < 0.0f) {
    return 0.0f;
  }
  if (value > 100.0f) {
    return 100.0f;
  }
  return value;
}

}  // namespace bms
