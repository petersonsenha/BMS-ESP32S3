#pragma once

#include "bms_types.h"

namespace bms {

class BmsStateEstimator {
 public:
  explicit BmsStateEstimator(BmsConfig config = {});
  void set_config(BmsConfig config);

  [[nodiscard]] BmsStateEstimate update(const PackTelemetry& telemetry,
                                        const FaultStatus& faults,
                                        uint32_t idle_time_ms,
                                        uint32_t now_ms);

  private:
  [[nodiscard]] float estimate_soc_from_ocv(const PackTelemetry& telemetry) const;
  [[nodiscard]] float soc_to_pack_ocv_mv(float soc_pct) const;
  [[nodiscard]] float clamp_pct(float value) const;

  BmsConfig config_{};
  BmsStateEstimate estimate_{};
  uint32_t last_update_ms_{0};
  bool initialized_{false};
};

}  // namespace bms
