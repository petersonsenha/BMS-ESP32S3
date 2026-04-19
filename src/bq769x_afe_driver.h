#pragma once

#include <Wire.h>

#include "afe_driver.h"

namespace bms {

// Scaffold for BQ769x-style AFEs. The final register map must be confirmed
// against the exact selected part before production use.
class Bq769xAfeDriver : public AfeDriver {
 public:
  explicit Bq769xAfeDriver(TwoWire& wire = Wire, uint8_t address = 0x08);

  bool begin() override;
  bool read_sample(AfeSample& sample) override;
  const char* driver_name() const override;

 private:
  bool ping_device();
  bool read_word_register(uint8_t register_address, uint16_t& value);

  TwoWire& wire_;
  uint8_t address_{0x08};
  bool online_{false};
};

}  // namespace bms
