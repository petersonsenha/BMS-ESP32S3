#include <Arduino.h>

#include "bms_controller.h"
#include "mock_battery_monitor.h"

namespace {

bms::MockBatteryMonitor g_battery_monitor;
bms::BmsController g_bms_controller;
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
  Serial.print(") balance=[");

  for (std::size_t index = 0; index < snapshot.outputs.balance.cell_enabled.size();
       ++index) {
    if (index > 0) {
      Serial.print(", ");
    }
    Serial.print(snapshot.outputs.balance.cell_enabled[index] ? "1" : "0");
  }

  Serial.print("] faults=");
  print_faults(snapshot.faults);
  Serial.println();
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

  Serial.println("Battery monitor mock started");
}

void loop() {
  const bms::PackTelemetry telemetry = g_battery_monitor.read();
  const bms::BmsSnapshot snapshot = g_bms_controller.update(telemetry);

  if ((millis() - g_last_log_ms) >= 1000UL) {
    g_last_log_ms = millis();
    print_snapshot(snapshot);
  }

  delay(100);
}
