#pragma once

#include "bms_config.h"
#include "bms_state_estimator.h"

namespace bms {

class BmsController {
 public:
  explicit BmsController(BmsConfig config = kDefaultBmsConfig);

  [[nodiscard]] BmsSnapshot update(const PackTelemetry& telemetry, uint32_t now_ms);
  void request_fault_clear();
  void set_config(BmsConfig config);
  [[nodiscard]] const BmsConfig& config() const;

 private:
  [[nodiscard]] FaultStatus evaluate_faults(const PackTelemetry& telemetry,
                                            uint32_t now_ms);
  [[nodiscard]] FaultStatus apply_fault_latch(const FaultStatus& raw_faults,
                                              uint32_t now_ms);
  [[nodiscard]] BalanceCommand evaluate_balancing(const PackTelemetry& telemetry,
                                                  const FaultStatus& faults) const;
  [[nodiscard]] OutputCommand evaluate_outputs(const FaultStatus& faults,
                                               const BalanceCommand& balance,
                                               BmsState requested_state) const;
  [[nodiscard]] BmsState resolve_state(const PackTelemetry& telemetry,
                                       const FaultStatus& faults,
                                       const BalanceCommand& balance) const;
  [[nodiscard]] BmsStateContext build_state_context(const PackTelemetry& telemetry,
                                                    const FaultStatus& faults,
                                                    const FaultStatus& raw_faults,
                                                    const BalanceCommand& balance,
                                                    BmsState requested_state,
                                                    uint32_t idle_time_ms,
                                                    uint32_t now_ms) const;
  [[nodiscard]] OutputCommand apply_state_safety_guards(const OutputCommand& outputs,
                                                        BmsState current_state) const;
  [[nodiscard]] bool is_charge_termination_reached(
      const PackTelemetry& telemetry) const;
  [[nodiscard]] bool should_run_precharge(const PackTelemetry& telemetry,
                                          const FaultStatus& faults) const;
  [[nodiscard]] bool is_precharge_complete(const PackTelemetry& telemetry) const;
  [[nodiscard]] uint32_t precharge_elapsed_ms(uint32_t now_ms) const;
  [[nodiscard]] uint32_t update_idle_time_ms(const PackTelemetry& telemetry,
                                             uint32_t now_ms);

  BmsConfig config_{};
  BmsStateEstimator state_estimator_{};
  BmsState previous_state_{BmsState::Startup};
  FaultStatus active_faults_{};
  FaultStatus latched_faults_{};
  uint32_t last_fault_transition_ms_{0};
  uint32_t idle_started_ms_{0};
  uint32_t precharge_started_ms_{0};
  bool startup_complete_{false};
  bool idle_window_active_{false};
  bool precharge_active_{false};
  bool fault_clear_requested_{false};
};

}  // namespace bms
