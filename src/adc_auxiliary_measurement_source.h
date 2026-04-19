#pragma once

#include <Arduino.h>

#include "auxiliary_measurement_source.h"

namespace bms {

struct AdcAuxiliaryMeasurementConfig {
  uint8_t current_adc_pin{1};
  uint8_t output_voltage_adc_pin{2};
  uint16_t adc_reference_mv{3300};
  uint16_t adc_full_scale_count{4095};
  uint16_t current_zero_mv{1650};
  float current_sensitivity_mv_per_a{66.0f};
  float output_divider_ratio{6.0f};
};

class AdcAuxiliaryMeasurementSource : public AuxiliaryMeasurementSource {
 public:
  explicit AdcAuxiliaryMeasurementSource(
      AdcAuxiliaryMeasurementConfig config = {});

  bool begin() override;
  bool read_sample(AuxiliarySample& sample) override;
  const char* source_name() const override;

 private:
  float counts_to_mv(int raw_counts) const;

  AdcAuxiliaryMeasurementConfig config_{};
};

}  // namespace bms
