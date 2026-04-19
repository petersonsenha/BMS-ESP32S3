#include "afe_battery_monitor.h"

namespace bms {

AfeBatteryMonitor::AfeBatteryMonitor(
    AfeDriver& afe_driver, AuxiliaryMeasurementSource& auxiliary_source)
    : afe_driver_(afe_driver), auxiliary_source_(auxiliary_source) {}

bool AfeBatteryMonitor::begin() {
  const bool afe_ready = afe_driver_.begin();
  const bool auxiliary_ready = auxiliary_source_.begin();
  return afe_ready && auxiliary_ready;
}

PackTelemetry AfeBatteryMonitor::read() {
  PackTelemetry telemetry{};
  AfeSample afe_sample{};
  AuxiliarySample auxiliary_sample{};

  const bool afe_ok = afe_driver_.read_sample(afe_sample);
  const bool aux_ok = auxiliary_source_.read_sample(auxiliary_sample);

  telemetry.afe_healthy = afe_ok && afe_sample.healthy;
  telemetry.cell_voltage_mv = afe_sample.cell_voltage_mv;
  telemetry.temperatures_deci_c = afe_sample.temperatures_deci_c;
  telemetry.pack_current_ma = aux_ok ? auxiliary_sample.pack_current_ma : 0;
  telemetry.output_voltage_mv =
      aux_ok ? auxiliary_sample.output_voltage_mv : 0U;
  telemetry.output_voltage_valid =
      aux_ok && auxiliary_sample.output_voltage_valid;

  uint32_t pack_voltage_mv = 0;
  for (uint16_t cell_voltage_mv : telemetry.cell_voltage_mv) {
    pack_voltage_mv += cell_voltage_mv;
  }
  telemetry.pack_voltage_mv = static_cast<uint16_t>(pack_voltage_mv);

  return telemetry;
}

}  // namespace bms
