#pragma once

#include "bms_types.h"

namespace bms {

class BatteryMonitor {
 public:
  virtual ~BatteryMonitor() = default;

  virtual bool begin() = 0;
  virtual PackTelemetry read() = 0;
};

}  // namespace bms
