#include "adc_auxiliary_measurement_source.h"

namespace bms {

AdcAuxiliaryMeasurementSource::AdcAuxiliaryMeasurementSource(
    AdcAuxiliaryMeasurementConfig config)
    : config_(config) {}

bool AdcAuxiliaryMeasurementSource::begin() {
  pinMode(config_.current_adc_pin, INPUT);
  pinMode(config_.output_voltage_adc_pin, INPUT);
  return true;
}

bool AdcAuxiliaryMeasurementSource::read_sample(AuxiliarySample& sample) {
  const int current_counts = analogRead(config_.current_adc_pin);
  const int output_counts = analogRead(config_.output_voltage_adc_pin);

  const float current_mv = counts_to_mv(current_counts);
  const float output_mv =
      counts_to_mv(output_counts) * config_.output_divider_ratio;

  const float delta_mv = current_mv - static_cast<float>(config_.current_zero_mv);
  const float current_a = config_.current_sensitivity_mv_per_a > 0.0f
                              ? (delta_mv / config_.current_sensitivity_mv_per_a)
                              : 0.0f;

  sample.pack_current_ma = static_cast<int32_t>(current_a * 1000.0f);
  sample.output_voltage_mv = output_mv > 0.0f
                                 ? static_cast<uint16_t>(output_mv)
                                 : 0U;
  sample.output_voltage_valid = sample.output_voltage_mv > 0U;
  return true;
}

const char* AdcAuxiliaryMeasurementSource::source_name() const {
  return "adc-auxiliary";
}

float AdcAuxiliaryMeasurementSource::counts_to_mv(int raw_counts) const {
  return (static_cast<float>(raw_counts) *
          static_cast<float>(config_.adc_reference_mv)) /
         static_cast<float>(config_.adc_full_scale_count);
}

}  // namespace bms
