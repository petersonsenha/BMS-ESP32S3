#include "bms_runtime_config_service.h"

#include <cstdlib>

namespace bms {

BmsRuntimeConfigService::BmsRuntimeConfigService(BmsController& controller,
                                                 BmsConfig initial_config,
                                                 BmsEventLog* event_log)
    : controller_(controller),
      active_config_(initial_config),
      event_log_(event_log) {}

void BmsRuntimeConfigService::begin(Stream& io) {
  controller_.set_config(active_config_);
  io.println("Runtime config service ready. Type 'help' for commands.");
}

void BmsRuntimeConfigService::poll(Stream& io) {
  while (io.available() > 0) {
    const char next_char = static_cast<char>(io.read());
    if (next_char == '\r') {
      continue;
    }
    if (next_char == '\n') {
      handle_line(input_buffer_, io);
      input_buffer_ = "";
      continue;
    }
    input_buffer_ += next_char;
  }
}

const BmsConfig& BmsRuntimeConfigService::active_config() const {
  return active_config_;
}

void BmsRuntimeConfigService::handle_line(const String& line, Stream& io) {
  const String trimmed_line = normalize(line);
  if (trimmed_line.length() == 0) {
    return;
  }

  if (trimmed_line == "help") {
    print_help(io);
    return;
  }

  if (trimmed_line == "show-config") {
    print_config(io);
    return;
  }

  if (trimmed_line == "show-events") {
    if (event_log_ == nullptr) {
      io.println("error event log unavailable");
      return;
    }
    event_log_->print(io, 16);
    return;
  }

  if (trimmed_line == "clear-events") {
    if (event_log_ == nullptr) {
      io.println("error event log unavailable");
      return;
    }
    event_log_->clear();
    io.println("ok events cleared");
    return;
  }

  if (trimmed_line == "clear-faults") {
    controller_.request_fault_clear();
    if (event_log_ != nullptr) {
      event_log_->record_manual_fault_clear_request(millis());
    }
    io.println("ok clear-faults requested");
    return;
  }

  if (trimmed_line.startsWith("set ")) {
    const int first_space = trimmed_line.indexOf(' ');
    const int second_space = trimmed_line.indexOf(' ', first_space + 1);
    if (second_space < 0) {
      io.println("error usage: set <path> <value>");
      return;
    }

    const String path =
        normalize(trimmed_line.substring(first_space + 1, second_space));
    const String value = normalize(trimmed_line.substring(second_space + 1));
    String message{};
    if (!apply_set_command(path, value, message)) {
      io.print("error ");
      io.println(message);
      return;
    }

    controller_.set_config(active_config_);
    if (event_log_ != nullptr) {
      event_log_->record_config_update(millis(), path.c_str(), value.c_str());
    }
    io.print("ok ");
    io.println(message);
    return;
  }

  io.println("error unknown command");
}

bool BmsRuntimeConfigService::apply_set_command(const String& path,
                                                const String& value,
                                                String& message) {
  uint16_t u16_value = 0;
  uint32_t u32_value = 0;
  int16_t i16_value = 0;
  float float_value = 0.0f;
  bool bool_value = false;

  if (path == "protection.cell_over_voltage_mv" &&
      parse_u16(value, u16_value)) {
    active_config_.protection.cell_over_voltage_mv = u16_value;
  } else if (path == "protection.cell_under_voltage_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.protection.cell_under_voltage_mv = u16_value;
  } else if (path == "protection.pack_over_current_charge_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.protection.pack_over_current_charge_ma = u32_value;
  } else if (path == "protection.pack_over_current_discharge_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.protection.pack_over_current_discharge_ma = u32_value;
  } else if (path == "protection.charge_temp_min_deci_c" &&
             parse_i16(value, i16_value)) {
    active_config_.protection.charge_temp_min_deci_c = i16_value;
  } else if (path == "protection.charge_temp_max_deci_c" &&
             parse_i16(value, i16_value)) {
    active_config_.protection.charge_temp_max_deci_c = i16_value;
  } else if (path == "protection.discharge_temp_max_deci_c" &&
             parse_i16(value, i16_value)) {
    active_config_.protection.discharge_temp_max_deci_c = i16_value;
  } else if (path == "protection.balance_start_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.protection.balance_start_mv = u16_value;
  } else if (path == "protection.balance_delta_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.protection.balance_delta_mv = u16_value;
  } else if (path == "fault_handling.cell_voltage_hysteresis_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.fault_handling.cell_voltage_hysteresis_mv = u16_value;
  } else if (path == "fault_handling.current_hysteresis_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.fault_handling.current_hysteresis_ma = u32_value;
  } else if (path == "fault_handling.temperature_hysteresis_deci_c" &&
             parse_i16(value, i16_value)) {
    active_config_.fault_handling.temperature_hysteresis_deci_c = i16_value;
  } else if (path == "fault_handling.short_circuit_discharge_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.fault_handling.short_circuit_discharge_ma = u32_value;
  } else if (path == "fault_handling.fault_latch_enabled" &&
             parse_bool(value, bool_value)) {
    active_config_.fault_handling.fault_latch_enabled = bool_value;
  } else if (path == "fault_handling.auto_recovery_enabled" &&
             parse_bool(value, bool_value)) {
    active_config_.fault_handling.auto_recovery_enabled = bool_value;
  } else if (path == "fault_handling.fault_release_delay_ms" &&
             parse_u32(value, u32_value)) {
    active_config_.fault_handling.fault_release_delay_ms = u32_value;
  } else if (path == "fault_handling.short_circuit_release_delay_ms" &&
             parse_u32(value, u32_value)) {
    active_config_.fault_handling.short_circuit_release_delay_ms = u32_value;
  } else if (path == "operation.activity_current_threshold_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.operation.activity_current_threshold_ma = u32_value;
  } else if (path == "operation.charge_termination_current_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.operation.charge_termination_current_ma = u32_value;
  } else if (path == "operation.balance_max_pack_current_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.operation.balance_max_pack_current_ma = u32_value;
  } else if (path == "operation.balance_max_temperature_deci_c" &&
             parse_i16(value, i16_value)) {
    active_config_.operation.balance_max_temperature_deci_c = i16_value;
  } else if (path == "operation.allow_balancing_while_charging" &&
             parse_bool(value, bool_value)) {
    active_config_.operation.allow_balancing_while_charging = bool_value;
  } else if (path == "operation.allow_balancing_in_idle" &&
             parse_bool(value, bool_value)) {
    active_config_.operation.allow_balancing_in_idle = bool_value;
  } else if (path == "operation.precharge_required" &&
             parse_bool(value, bool_value)) {
    active_config_.operation.precharge_required = bool_value;
  } else if (path == "operation.precharge_timeout_ms" &&
             parse_u32(value, u32_value)) {
    active_config_.operation.precharge_timeout_ms = u32_value;
  } else if (path == "operation.precharge_target_delta_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.operation.precharge_target_delta_mv = u16_value;
  } else if (path == "estimation.nominal_capacity_mah" &&
             parse_u32(value, u32_value)) {
    active_config_.estimation.nominal_capacity_mah = u32_value;
  } else if (path == "estimation.nominal_cell_voltage_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.estimation.nominal_cell_voltage_mv = u16_value;
  } else if (path == "estimation.full_cell_voltage_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.estimation.full_cell_voltage_mv = u16_value;
  } else if (path == "estimation.empty_cell_voltage_mv" &&
             parse_u16(value, u16_value)) {
    active_config_.estimation.empty_cell_voltage_mv = u16_value;
  } else if (path == "estimation.ocv_correction_idle_ms" &&
             parse_u32(value, u32_value)) {
    active_config_.estimation.ocv_correction_idle_ms = u32_value;
  } else if (path == "estimation.ocv_correction_max_current_ma" &&
             parse_u32(value, u32_value)) {
    active_config_.estimation.ocv_correction_max_current_ma = u32_value;
  } else if (path == "estimation.coulomb_efficiency_ppm" &&
             parse_u32(value, u32_value)) {
    active_config_.estimation.coulomb_efficiency_ppm = u32_value;
  } else if ((path == "estimation.nominal_internal_resistance_mohm" ||
              path == "estimation.nominal_internal_resistance_milliohm") &&
             parse_float(value, float_value)) {
    active_config_.estimation.nominal_internal_resistance_mohm = float_value;
  } else if (path == "estimation.minimum_soc_for_discharge_pct" &&
             parse_float(value, float_value)) {
    active_config_.estimation.minimum_soc_for_discharge_pct = float_value;
  } else if (path == "estimation.enable_ocv_correction" &&
             parse_bool(value, bool_value)) {
    active_config_.estimation.enable_ocv_correction = bool_value;
  } else {
    message = "invalid path or value";
    return false;
  }

  message = path + "=" + value;
  return true;
}

void BmsRuntimeConfigService::print_help(Stream& io) const {
  io.println("Commands:");
  io.println("  help");
  io.println("  show-config");
  io.println("  show-events");
  io.println("  clear-events");
  io.println("  clear-faults");
  io.println("  set <path> <value>");
  io.println("Examples:");
  io.println("  set operation.precharge_required true");
  io.println("  set operation.precharge_timeout_ms 2000");
  io.println("  set protection.pack_over_current_discharge_ma 12000");
  io.println("  set estimation.enable_ocv_correction false");
}

void BmsRuntimeConfigService::print_config(Stream& io) const {
  io.println("{");
  io.print("  \"protection\": {\"cell_over_voltage_mv\": ");
  io.print(active_config_.protection.cell_over_voltage_mv);
  io.print(", \"cell_under_voltage_mv\": ");
  io.print(active_config_.protection.cell_under_voltage_mv);
  io.print(", \"pack_over_current_charge_ma\": ");
  io.print(active_config_.protection.pack_over_current_charge_ma);
  io.print(", \"pack_over_current_discharge_ma\": ");
  io.print(active_config_.protection.pack_over_current_discharge_ma);
  io.println("},");

  io.print("  \"fault_handling\": {\"fault_latch_enabled\": ");
  io.print(active_config_.fault_handling.fault_latch_enabled ? "true" : "false");
  io.print(", \"auto_recovery_enabled\": ");
  io.print(active_config_.fault_handling.auto_recovery_enabled ? "true" : "false");
  io.print(", \"short_circuit_discharge_ma\": ");
  io.print(active_config_.fault_handling.short_circuit_discharge_ma);
  io.println("},");

  io.print("  \"operation\": {\"precharge_required\": ");
  io.print(active_config_.operation.precharge_required ? "true" : "false");
  io.print(", \"precharge_timeout_ms\": ");
  io.print(active_config_.operation.precharge_timeout_ms);
  io.print(", \"precharge_target_delta_mv\": ");
  io.print(active_config_.operation.precharge_target_delta_mv);
  io.println("},");

  io.print("  \"estimation\": {\"nominal_capacity_mah\": ");
  io.print(active_config_.estimation.nominal_capacity_mah);
  io.print(", \"ocv_correction_idle_ms\": ");
  io.print(active_config_.estimation.ocv_correction_idle_ms);
  io.print(", \"enable_ocv_correction\": ");
  io.print(active_config_.estimation.enable_ocv_correction ? "true" : "false");
  io.println("}");
  io.println("}");
}

String BmsRuntimeConfigService::normalize(String value) {
  value.trim();
  return value;
}

bool BmsRuntimeConfigService::parse_bool(const String& value,
                                         bool& parsed_value) {
  const String normalized = normalize(value);
  if (normalized == "true" || normalized == "1" || normalized == "on") {
    parsed_value = true;
    return true;
  }
  if (normalized == "false" || normalized == "0" || normalized == "off") {
    parsed_value = false;
    return true;
  }
  return false;
}

bool BmsRuntimeConfigService::parse_u16(const String& value,
                                        uint16_t& parsed_value) {
  char* end_pointer = nullptr;
  const unsigned long parsed = strtoul(value.c_str(), &end_pointer, 10);
  if (end_pointer == value.c_str() || *end_pointer != '\0' ||
      parsed > 65535UL) {
    return false;
  }
  parsed_value = static_cast<uint16_t>(parsed);
  return true;
}

bool BmsRuntimeConfigService::parse_u32(const String& value,
                                        uint32_t& parsed_value) {
  char* end_pointer = nullptr;
  const unsigned long parsed = strtoul(value.c_str(), &end_pointer, 10);
  if (end_pointer == value.c_str() || *end_pointer != '\0') {
    return false;
  }
  parsed_value = static_cast<uint32_t>(parsed);
  return true;
}

bool BmsRuntimeConfigService::parse_i16(const String& value,
                                        int16_t& parsed_value) {
  char* end_pointer = nullptr;
  const long parsed = strtol(value.c_str(), &end_pointer, 10);
  if (end_pointer == value.c_str() || *end_pointer != '\0' ||
      parsed < -32768L || parsed > 32767L) {
    return false;
  }
  parsed_value = static_cast<int16_t>(parsed);
  return true;
}

bool BmsRuntimeConfigService::parse_float(const String& value,
                                          float& parsed_value) {
  char* end_pointer = nullptr;
  const float parsed = strtof(value.c_str(), &end_pointer);
  if (end_pointer == value.c_str() || *end_pointer != '\0') {
    return false;
  }
  parsed_value = parsed;
  return true;
}

}  // namespace bms
