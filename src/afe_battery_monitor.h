#pragma once

#include "afe_driver.h"
#include "auxiliary_measurement_source.h"
#include "battery_monitor.h"

namespace bms {

class AfeBatteryMonitor : public BatteryMonitor {
 public:
  AfeBatteryMonitor(AfeDriver& afe_driver,
                    AuxiliaryMeasurementSource& auxiliary_source);

  bool begin() override;
  PackTelemetry read() override;

 private:
  AfeDriver& afe_driver_;
  AuxiliaryMeasurementSource& auxiliary_source_;
};

}  // namespace bms
