#include "mock_battery_monitor.h"

#include <Arduino.h>

namespace bms {

bool MockBatteryMonitor::begin() { return true; }

PackTelemetry MockBatteryMonitor::read() {
  const uint32_t seconds = millis() / 1000UL;
  const uint32_t phase = (seconds / 15UL) % 3UL;
  const uint16_t ripple_mv = static_cast<uint16_t>((seconds % 5UL) * 3UL);

  PackTelemetry telemetry{};
  telemetry.afe_healthy = true;

  if (phase == 0UL) {
    telemetry.cell_voltage_mv = {4092U, static_cast<uint16_t>(4110U + ripple_mv),
                                 static_cast<uint16_t>(4128U + ripple_mv), 4101U};
    telemetry.pack_current_ma = -1800;
    telemetry.temperatures_deci_c = {255, 268};
  } else if (phase == 1UL) {
    telemetry.cell_voltage_mv = {4050U, 4060U, 4065U, 4058U};
    telemetry.pack_current_ma = 0;
    telemetry.temperatures_deci_c = {248, 254};
  } else {
    telemetry.cell_voltage_mv = {
        static_cast<uint16_t>(3925U - ripple_mv),
        static_cast<uint16_t>(3936U - ripple_mv),
        static_cast<uint16_t>(3942U - ripple_mv),
        static_cast<uint16_t>(3931U - ripple_mv),
    };
    telemetry.pack_current_ma = 3200;
    telemetry.temperatures_deci_c = {292, 301};
  }

  uint32_t pack_voltage_mv = 0;
  for (uint16_t cell_voltage_mv : telemetry.cell_voltage_mv) {
    pack_voltage_mv += cell_voltage_mv;
  }

  telemetry.pack_voltage_mv = static_cast<uint16_t>(pack_voltage_mv);
  return telemetry;
}

}  // namespace bms
