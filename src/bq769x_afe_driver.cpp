#include "bq769x_afe_driver.h"

namespace bms {

namespace {

constexpr uint8_t kCellVoltageRegisters[kSeriesCellCount] = {
    0x14, 0x16, 0x18, 0x1A,
};
constexpr uint8_t kTemperatureRegisters[kTemperatureSensorCount] = {
    0x2A, 0x2C,
};

}  // namespace

Bq769xAfeDriver::Bq769xAfeDriver(TwoWire& wire, uint8_t address)
    : wire_(wire), address_(address) {}

bool Bq769xAfeDriver::begin() {
  wire_.begin();
  online_ = ping_device();
  return online_;
}

bool Bq769xAfeDriver::read_sample(AfeSample& sample) {
  if (!online_) {
    online_ = ping_device();
    if (!online_) {
      sample = AfeSample{};
      return false;
    }
  }

  for (std::size_t index = 0; index < kSeriesCellCount; ++index) {
    uint16_t value = 0;
    if (!read_word_register(kCellVoltageRegisters[index], value)) {
      online_ = false;
      sample = AfeSample{};
      return false;
    }
    sample.cell_voltage_mv[index] = value;
  }

  for (std::size_t index = 0; index < kTemperatureSensorCount; ++index) {
    uint16_t value = 0;
    if (!read_word_register(kTemperatureRegisters[index], value)) {
      online_ = false;
      sample = AfeSample{};
      return false;
    }

    sample.temperatures_deci_c[index] = static_cast<int16_t>(value);
  }

  sample.healthy = true;
  return true;
}

const char* Bq769xAfeDriver::driver_name() const { return "bq769x-scaffold"; }

bool Bq769xAfeDriver::ping_device() {
  wire_.beginTransmission(address_);
  return wire_.endTransmission() == 0;
}

bool Bq769xAfeDriver::read_word_register(uint8_t register_address,
                                         uint16_t& value) {
  wire_.beginTransmission(address_);
  wire_.write(register_address);
  if (wire_.endTransmission(false) != 0) {
    return false;
  }

  if (wire_.requestFrom(static_cast<int>(address_), 2) != 2) {
    return false;
  }

  const uint8_t low_byte = wire_.read();
  const uint8_t high_byte = wire_.read();
  value = static_cast<uint16_t>((static_cast<uint16_t>(high_byte) << 8) |
                                low_byte);
  return true;
}

}  // namespace bms
