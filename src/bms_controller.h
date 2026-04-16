#pragma once

#include "bms_config.h"

namespace bms {

class BmsController {
 public:
  explicit BmsController(ProtectionConfig config = kDefaultProtectionConfig);

  [[nodiscard]] BmsSnapshot update(const PackTelemetry& telemetry);

 private:
  [[nodiscard]] FaultStatus evaluate_faults(const PackTelemetry& telemetry) const;
  [[nodiscard]] BalanceCommand evaluate_balancing(const PackTelemetry& telemetry,
                                                  const FaultStatus& faults) const;
  [[nodiscard]] OutputCommand evaluate_outputs(const FaultStatus& faults,
                                               const BalanceCommand& balance) const;
  [[nodiscard]] BmsState resolve_state(const PackTelemetry& telemetry,
                                       const FaultStatus& faults,
                                       const BalanceCommand& balance) const;
  [[nodiscard]] BmsStateContext build_state_context(const PackTelemetry& telemetry,
                                                    const FaultStatus& faults,
                                                    const BalanceCommand& balance,
                                                    BmsState requested_state) const;
  [[nodiscard]] OutputCommand apply_state_safety_guards(const OutputCommand& outputs,
                                                        BmsState current_state) const;

  ProtectionConfig config_;
  BmsState previous_state_{BmsState::Startup};
  bool startup_complete_{false};
};

}  // namespace bms
