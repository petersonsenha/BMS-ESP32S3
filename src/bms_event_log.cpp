#include "bms_event_log.h"

#include <cstdio>
#include <cstring>

namespace bms {

namespace {

bool faults_equal(const FaultStatus& left, const FaultStatus& right) {
  return left.afe_fault == right.afe_fault &&
         left.cell_over_voltage == right.cell_over_voltage &&
         left.cell_under_voltage == right.cell_under_voltage &&
         left.charge_over_current == right.charge_over_current &&
         left.discharge_over_current == right.discharge_over_current &&
         left.short_circuit_discharge == right.short_circuit_discharge &&
         left.precharge_timeout == right.precharge_timeout &&
         left.charge_under_temperature == right.charge_under_temperature &&
         left.charge_over_temperature == right.charge_over_temperature &&
         left.discharge_over_temperature == right.discharge_over_temperature;
}

}  // namespace

void BmsEventLog::record_boot(uint32_t timestamp_ms) {
  append_event(timestamp_ms, BmsEventCode::Boot, BmsState::Startup,
               BmsState::Startup, FaultStatus{}, 0U, 0, "system boot");
}

void BmsEventLog::capture_snapshot(const BmsSnapshot& snapshot,
                                   uint32_t timestamp_ms) {
  if (!initialized_) {
    initialized_ = true;
    last_state_ = snapshot.state_context.current;
    last_faults_ = snapshot.faults;
    last_precharge_active_ = snapshot.state_context.precharge_active;
    last_precharge_complete_ = snapshot.state_context.precharge_complete;
    last_precharge_timed_out_ = snapshot.state_context.precharge_timed_out;
    last_ocv_correction_applied_ = snapshot.estimate.ocv_correction_applied;
    return;
  }

  if (snapshot.state_context.current != last_state_) {
    char detail[64]{};
    std::snprintf(detail, sizeof(detail), "%s -> %s",
                  state_name(last_state_),
                  state_name(snapshot.state_context.current));
    append_event(timestamp_ms, BmsEventCode::StateTransition, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, detail);
  }

  if (!faults_equal(last_faults_, snapshot.faults) && snapshot.faults.any()) {
    char detail[64]{};
    format_faults(snapshot.faults, detail, sizeof(detail));
    const BmsEventCode code =
        last_faults_.any() ? BmsEventCode::FaultChanged
                           : BmsEventCode::FaultRaised;
    append_event(timestamp_ms, code, last_state_, snapshot.state_context.current,
                 snapshot.faults, snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, detail);
  } else if (!faults_equal(last_faults_, snapshot.faults) && last_faults_.any() &&
             !snapshot.faults.any()) {
    append_event(timestamp_ms, BmsEventCode::FaultCleared, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, "faults cleared");
  }

  if (!last_precharge_active_ && snapshot.state_context.precharge_active) {
    append_event(timestamp_ms, BmsEventCode::PrechargeStarted, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, "precharge active");
  }

  if (!last_precharge_complete_ && snapshot.state_context.precharge_complete &&
      snapshot.state_context.precharge_required) {
    append_event(timestamp_ms, BmsEventCode::PrechargeCompleted, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, "bus charged");
  }

  if (!last_precharge_timed_out_ && snapshot.state_context.precharge_timed_out) {
    append_event(timestamp_ms, BmsEventCode::PrechargeTimedOut, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, "precharge timeout");
  }

  if (!last_ocv_correction_applied_ && snapshot.estimate.ocv_correction_applied) {
    append_event(timestamp_ms, BmsEventCode::OcvCorrectionApplied, last_state_,
                 snapshot.state_context.current, snapshot.faults,
                 snapshot.telemetry.pack_voltage_mv,
                 snapshot.telemetry.pack_current_ma, "ocv correction");
  }

  last_state_ = snapshot.state_context.current;
  last_faults_ = snapshot.faults;
  last_precharge_active_ = snapshot.state_context.precharge_active;
  last_precharge_complete_ = snapshot.state_context.precharge_complete;
  last_precharge_timed_out_ = snapshot.state_context.precharge_timed_out;
  last_ocv_correction_applied_ = snapshot.estimate.ocv_correction_applied;
}

void BmsEventLog::record_config_update(uint32_t timestamp_ms,
                                       const char* path,
                                       const char* value) {
  char detail[64]{};
  std::snprintf(detail, sizeof(detail), "%s=%s", path, value);
  append_event(timestamp_ms, BmsEventCode::ConfigUpdated, BmsState::Startup,
               BmsState::Startup, FaultStatus{}, 0U, 0, detail);
}

void BmsEventLog::record_manual_fault_clear_request(uint32_t timestamp_ms) {
  append_event(timestamp_ms, BmsEventCode::ManualFaultClearRequested,
               BmsState::Startup, BmsState::Startup, FaultStatus{}, 0U, 0,
               "clear-faults");
}

void BmsEventLog::print(Stream& io, std::size_t max_entries) const {
  const std::size_t entries_to_print =
      max_entries < count_ ? max_entries : count_;
  io.print("event-count=");
  io.println(count_);

  for (std::size_t index = 0; index < entries_to_print; ++index) {
    const BmsEventEntry& entry =
        event_at(count_ - entries_to_print + index);
    char faults[64]{};
    format_faults(entry.faults, faults, sizeof(faults));
    io.print("[");
    io.print(index);
    io.print("] t=");
    io.print(entry.timestamp_ms);
    io.print("ms type=");
    io.print(event_code_name(entry.code));
    io.print(" from=");
    io.print(state_name(entry.from_state));
    io.print(" to=");
    io.print(state_name(entry.to_state));
    io.print(" pack=");
    io.print(entry.pack_voltage_mv);
    io.print("mV current=");
    io.print(entry.pack_current_ma);
    io.print("mA faults=");
    io.print(faults[0] != '\0' ? faults : "none");
    io.print(" detail=");
    io.println(entry.detail);
  }
}

void BmsEventLog::clear() {
  next_index_ = 0;
  count_ = 0;
  initialized_ = false;
  last_state_ = BmsState::Startup;
  last_faults_.clear();
  last_precharge_active_ = false;
  last_precharge_complete_ = false;
  last_precharge_timed_out_ = false;
  last_ocv_correction_applied_ = false;
  std::memset(entries_, 0, sizeof(entries_));
}

void BmsEventLog::append_event(uint32_t timestamp_ms,
                               BmsEventCode code,
                               BmsState from_state,
                               BmsState to_state,
                               const FaultStatus& faults,
                               uint16_t pack_voltage_mv,
                               int32_t pack_current_ma,
                               const char* detail) {
  BmsEventEntry& entry = entries_[next_index_];
  entry.timestamp_ms = timestamp_ms;
  entry.code = code;
  entry.from_state = from_state;
  entry.to_state = to_state;
  entry.faults = faults;
  entry.pack_voltage_mv = pack_voltage_mv;
  entry.pack_current_ma = pack_current_ma;
  std::snprintf(entry.detail, sizeof(entry.detail), "%s", detail);

  next_index_ = (next_index_ + 1U) % kCapacity;
  if (count_ < kCapacity) {
    ++count_;
  }
}

const BmsEventEntry& BmsEventLog::event_at(std::size_t chronological_index) const {
  const std::size_t oldest_index =
      count_ < kCapacity ? 0U : next_index_;
  const std::size_t physical_index =
      (oldest_index + chronological_index) % kCapacity;
  return entries_[physical_index];
}

const char* BmsEventLog::event_code_name(BmsEventCode code) {
  switch (code) {
    case BmsEventCode::Boot:
      return "boot";
    case BmsEventCode::StateTransition:
      return "state";
    case BmsEventCode::FaultRaised:
      return "fault-raised";
    case BmsEventCode::FaultChanged:
      return "fault-changed";
    case BmsEventCode::FaultCleared:
      return "fault-cleared";
    case BmsEventCode::PrechargeStarted:
      return "precharge-start";
    case BmsEventCode::PrechargeCompleted:
      return "precharge-done";
    case BmsEventCode::PrechargeTimedOut:
      return "precharge-timeout";
    case BmsEventCode::OcvCorrectionApplied:
      return "ocv-correction";
    case BmsEventCode::ConfigUpdated:
      return "config";
    case BmsEventCode::ManualFaultClearRequested:
      return "fault-clear-request";
  }

  return "unknown";
}

void BmsEventLog::format_faults(const FaultStatus& faults,
                                char* buffer,
                                std::size_t buffer_size) {
  const char* separator = "";
  std::snprintf(buffer, buffer_size, "");

  auto append_fault = [&](const char* name, bool active) {
    if (!active) {
      return;
    }

    const std::size_t used = std::strlen(buffer);
    if (used >= buffer_size) {
      return;
    }

    std::snprintf(buffer + used, buffer_size - used, "%s%s", separator, name);
    separator = "|";
  };

  append_fault("afe", faults.afe_fault);
  append_fault("cell_ov", faults.cell_over_voltage);
  append_fault("cell_uv", faults.cell_under_voltage);
  append_fault("chg_oc", faults.charge_over_current);
  append_fault("dsg_oc", faults.discharge_over_current);
  append_fault("scd", faults.short_circuit_discharge);
  append_fault("precharge_to", faults.precharge_timeout);
  append_fault("chg_ut", faults.charge_under_temperature);
  append_fault("chg_ot", faults.charge_over_temperature);
  append_fault("dsg_ot", faults.discharge_over_temperature);
}

}  // namespace bms
