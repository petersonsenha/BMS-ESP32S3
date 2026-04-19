#pragma once

#include "bms_types.h"

namespace bms {

struct AfeSample {
  std::array<uint16_t, kSeriesCellCount> cell_voltage_mv{};
  std::array<int16_t, kTemperatureSensorCount> temperatures_deci_c{};
  bool healthy{false};
};

class AfeDriver {
 public:
  virtual ~AfeDriver() = default;

  virtual bool begin() = 0;
  virtual bool read_sample(AfeSample& sample) = 0;
  virtual const char* driver_name() const = 0;
};

}  // namespace bms
