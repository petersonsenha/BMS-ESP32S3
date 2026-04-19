#pragma once

#include <cstddef>
#include <cstdint>

#include <Arduino.h>

#include "bms_types.h"

namespace bms {

enum class BmsEventCode : uint8_t {
  Boot,
  StateTransition,
  FaultRaised,
  FaultChanged,
  FaultCleared,
  PrechargeStarted,
  PrechargeCompleted,
  PrechargeTimedOut,
  OcvCorrectionApplied,
  ConfigUpdated,
  ManualFaultClearRequested,
};

struct BmsEventEntry {
  uint32_t timestamp_ms{0};
  BmsEventCode code{BmsEventCode::Boot};
  BmsState from_state{BmsState::Startup};
  BmsState to_state{BmsState::Startup};
  FaultStatus faults{};
  uint16_t pack_voltage_mv{0};
  int32_t pack_current_ma{0};
  char detail[64]{};
};

class BmsEventLog {
 public:
  static constexpr std::size_t kCapacity = 32;

  void record_boot(uint32_t timestamp_ms);
  void capture_snapshot(const BmsSnapshot& snapshot, uint32_t timestamp_ms);
  void record_config_update(uint32_t timestamp_ms,
                            const char* path,
                            const char* value);
  void record_manual_fault_clear_request(uint32_t timestamp_ms);
  void print(Stream& io, std::size_t max_entries = kCapacity) const;
  void clear();

 private:
  void append_event(uint32_t timestamp_ms,
                    BmsEventCode code,
                    BmsState from_state,
                    BmsState to_state,
                    const FaultStatus& faults,
                    uint16_t pack_voltage_mv,
                    int32_t pack_current_ma,
                    const char* detail);
  const BmsEventEntry& event_at(std::size_t chronological_index) const;
  static const char* event_code_name(BmsEventCode code);
  static void format_faults(const FaultStatus& faults,
                            char* buffer,
                            std::size_t buffer_size);

  BmsEventEntry entries_[kCapacity]{};
  std::size_t next_index_{0};
  std::size_t count_{0};
  bool initialized_{false};
  BmsState last_state_{BmsState::Startup};
  FaultStatus last_faults_{};
  bool last_precharge_active_{false};
  bool last_precharge_complete_{false};
  bool last_precharge_timed_out_{false};
  bool last_ocv_correction_applied_{false};
};

}  // namespace bms
