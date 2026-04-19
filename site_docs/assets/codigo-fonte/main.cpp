#include <Arduino.h>

#include "battery_monitor.h"
#include "bms_controller.h"
#include "bms_event_log.h"
#include "bms_runtime_config_service.h"
#include "mock_battery_monitor.h"

#if BMS_USE_AFE_MONITOR
#include "adc_auxiliary_measurement_source.h"
#include "afe_battery_monitor.h"
#include "bq769x_afe_driver.h"
#endif

namespace {

#if BMS_USE_AFE_MONITOR
bms::Bq769xAfeDriver g_afe_driver;
bms::AdcAuxiliaryMeasurementSource g_auxiliary_measurements;
bms::AfeBatteryMonitor g_afe_battery_monitor(g_afe_driver,
                                             g_auxiliary_measurements);
bms::BatteryMonitor& g_battery_monitor = g_afe_battery_monitor;
#else
bms::MockBatteryMonitor g_mock_battery_monitor;
bms::BatteryMonitor& g_battery_monitor = g_mock_battery_monitor;
#endif

bms::BmsController g_bms_controller;
bms::BmsEventLog g_event_log;
bms::BmsRuntimeConfigService g_runtime_config_service(g_bms_controller,
                                                      bms::kDefaultBmsConfig,
                                                      &g_event_log);
uint32_t g_last_log_ms = 0;

void print_faults(const bms::FaultStatus& faults) {
  if (!faults.any()) {
    Serial.print("none");
    return;
  }

  if (faults.afe_fault) {
    Serial.print("afe ");
  }
  if (faults.cell_over_voltage) {
    Serial.print("cell_ov ");
  }
  if (faults.cell_under_voltage) {
    Serial.print("cell_uv ");
  }
  if (faults.charge_over_current) {
    Serial.print("chg_oc ");
  }
  if (faults.discharge_over_current) {
    Serial.print("dsg_oc ");
  }
  if (faults.short_circuit_discharge) {
    Serial.print("scd ");
  }
  if (faults.precharge_timeout) {
    Serial.print("precharge_to ");
  }
  if (faults.charge_under_temperature) {
    Serial.print("chg_ut ");
  }
  if (faults.charge_over_temperature) {
    Serial.print("chg_ot ");
  }
  if (faults.discharge_over_temperature) {
    Serial.print("dsg_ot ");
  }
}

void print_snapshot(const bms::BmsSnapshot& snapshot) {
  Serial.print("state=");
  Serial.print(bms::state_name(snapshot.state_context.current));
  Serial.print(" requested=");
  Serial.print(bms::state_name(snapshot.state_context.requested));
  Serial.print(" prev=");
  Serial.print(bms::state_name(snapshot.state_context.previous));
  Serial.print(" pack=");
  Serial.print(snapshot.telemetry.pack_voltage_mv);
  Serial.print("mV output=");
  Serial.print(snapshot.telemetry.output_voltage_mv);
  Serial.print("mV current=");
  Serial.print(snapshot.telemetry.pack_current_ma);
  Serial.print("mA cells=[");

  for (std::size_t index = 0; index < snapshot.telemetry.cell_voltage_mv.size();
       ++index) {
    if (index > 0) {
      Serial.print(", ");
    }
    Serial.print(snapshot.telemetry.cell_voltage_mv[index]);
  }

  Serial.print("] temps=[");
  for (std::size_t index = 0;
       index < snapshot.telemetry.temperatures_deci_c.size(); ++index) {
    if (index > 0) {
      Serial.print(", ");
    }
    Serial.print(snapshot.telemetry.temperatures_deci_c[index] / 10.0f, 1);
  }

  Serial.print("]C mosfets(chg=");
  Serial.print(snapshot.outputs.charge_mosfet_enabled ? "on" : "off");
  Serial.print(", dsg=");
  Serial.print(snapshot.outputs.discharge_mosfet_enabled ? "on" : "off");
  Serial.print(", precharge=");
  Serial.print(snapshot.outputs.precharge_enabled ? "on" : "off");
  Serial.print(") balance=[");

  for (std::size_t index = 0; index < snapshot.outputs.balance.cell_enabled.size();
       ++index) {
    if (index > 0) {
      Serial.print(", ");
    }
    Serial.print(snapshot.outputs.balance.cell_enabled[index] ? "1" : "0");
  }

  Serial.print("] raw_faults=");
  print_faults(snapshot.raw_faults);
  Serial.print(" effective_faults=");
  print_faults(snapshot.faults);
  Serial.print(" idle_ms=");
  Serial.print(snapshot.state_context.idle_time_ms);
  Serial.print(" precharge(ms=");
  Serial.print(snapshot.state_context.precharge_elapsed_ms);
  Serial.print(", active=");
  Serial.print(snapshot.state_context.precharge_active ? "yes" : "no");
  Serial.print(", done=");
  Serial.print(snapshot.state_context.precharge_complete ? "yes" : "no");
  Serial.print(", timeout=");
  Serial.print(snapshot.state_context.precharge_timed_out ? "yes" : "no");
  Serial.print(") ocv_ready=");
  Serial.print(snapshot.state_context.resting_for_ocv_correction ? "yes" : "no");
  Serial.print(" charge_done=");
  Serial.print(snapshot.state_context.charge_termination_reached ? "yes" : "no");
  Serial.print(" soc=");
  Serial.print(snapshot.estimate.soc_pct, 1);
  Serial.print("% soe=");
  Serial.print(snapshot.estimate.soe_wh, 2);
  Serial.print("Wh soh=");
  Serial.print(snapshot.estimate.soh_pct, 1);
  Serial.print("% sop(chg=");
  Serial.print(snapshot.estimate.sop_charge_w, 1);
  Serial.print("W,dsg=");
  Serial.print(snapshot.estimate.sop_discharge_w, 1);
  Serial.print("W) sof=");
  Serial.print(snapshot.estimate.sof_pct, 1);
  Serial.print("% r_est=");
  Serial.print(snapshot.estimate.estimated_internal_resistance_mohm, 2);
  Serial.print("mOhm throughput=");
  Serial.print(snapshot.estimate.energy_throughput_wh, 3);
  Serial.println("Wh");
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1200);

  Serial.println();
  Serial.println("BMS 4S bootstrap");

  if (!g_battery_monitor.begin()) {
    Serial.println("Battery monitor failed to start");
    return;
  }

#if BMS_USE_AFE_MONITOR
  Serial.println("Battery monitor AFE scaffold started");
#else
  Serial.println("Battery monitor mock started");
#endif

  g_runtime_config_service.begin(Serial);
  g_event_log.record_boot(millis());
}

void loop() {
  g_runtime_config_service.poll(Serial);

  const bms::PackTelemetry telemetry = g_battery_monitor.read();
  const bms::BmsSnapshot snapshot =
      g_bms_controller.update(telemetry, millis());
  g_event_log.capture_snapshot(snapshot, millis());

  if ((millis() - g_last_log_ms) >= 1000UL) {
    g_last_log_ms = millis();
    print_snapshot(snapshot);
  }

  delay(100);
}
