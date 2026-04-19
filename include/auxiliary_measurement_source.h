#pragma once

#include "bms_types.h"

namespace bms {

struct AuxiliarySample {
  int32_t pack_current_ma{0};
  uint16_t output_voltage_mv{0};
  bool output_voltage_valid{false};
};

class AuxiliaryMeasurementSource {
 public:
  virtual ~AuxiliaryMeasurementSource() = default;

  virtual bool begin() = 0;
  virtual bool read_sample(AuxiliarySample& sample) = 0;
  virtual const char* source_name() const = 0;
};

}  // namespace bms
