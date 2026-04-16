#pragma once

#include "battery_monitor.h"

namespace bms {

class MockBatteryMonitor : public BatteryMonitor {
 public:
  bool begin() override;
  PackTelemetry read() override;
};

}  // namespace bms
