#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace bms {

constexpr std::size_t kSeriesCellCount = 4;
constexpr std::size_t kTemperatureSensorCount = 2;

enum class BmsState : uint8_t {
  Startup,
  Idle,
  Charging,
  Discharging,
  Balancing,
  Fault,
  Count,
};

struct BmsStateDefinition {
  BmsState state{BmsState::Startup};
  const char* name{"startup"};
  const char* summary{"Boot and validation phase before enabling power paths."};
  bool allows_charge_path{false};
  bool allows_discharge_path{false};
  bool allows_balancing{false};
  bool is_fault_state{false};
};

struct PackTelemetry {
  std::array<uint16_t, kSeriesCellCount> cell_voltage_mv{};
  uint16_t pack_voltage_mv{0};

  // Positive current means discharge, negative current means charge.
  int32_t pack_current_ma{0};

  std::array<int16_t, kTemperatureSensorCount> temperatures_deci_c{};
  bool afe_healthy{false};
};

struct ProtectionConfig {
  uint16_t cell_over_voltage_mv{4200};
  uint16_t cell_under_voltage_mv{3000};
  uint32_t pack_over_current_charge_ma{5000};
  uint32_t pack_over_current_discharge_ma{10000};
  int16_t charge_temp_min_deci_c{0};
  int16_t charge_temp_max_deci_c{450};
  int16_t discharge_temp_max_deci_c{600};
  uint16_t balance_start_mv{4100};
  uint16_t balance_delta_mv{15};
  uint32_t activity_current_threshold_ma{200};
};

struct FaultStatus {
  bool afe_fault{false};
  bool cell_over_voltage{false};
  bool cell_under_voltage{false};
  bool charge_over_current{false};
  bool discharge_over_current{false};
  bool charge_under_temperature{false};
  bool charge_over_temperature{false};
  bool discharge_over_temperature{false};

  [[nodiscard]] bool any() const {
    return afe_fault || cell_over_voltage || cell_under_voltage ||
           charge_over_current || discharge_over_current ||
           charge_under_temperature || charge_over_temperature ||
           discharge_over_temperature;
  }
};

struct BalanceCommand {
  std::array<bool, kSeriesCellCount> cell_enabled{};

  [[nodiscard]] bool any() const {
    for (bool enabled : cell_enabled) {
      if (enabled) {
        return true;
      }
    }
    return false;
  }
};

struct OutputCommand {
  bool charge_mosfet_enabled{false};
  bool discharge_mosfet_enabled{false};
  BalanceCommand balance{};
};

struct BmsStateMetrics {
  uint16_t min_cell_voltage_mv{0};
  uint16_t max_cell_voltage_mv{0};
  uint16_t cell_voltage_delta_mv{0};
  int16_t min_temperature_deci_c{0};
  int16_t max_temperature_deci_c{0};
  int32_t pack_current_ma{0};
};

struct BmsStateContext {
  BmsState previous{BmsState::Startup};
  BmsState current{BmsState::Startup};
  BmsState requested{BmsState::Startup};
  bool charging_detected{false};
  bool discharging_detected{false};
  bool balancing_requested{false};
  bool fault_active{false};
  BmsStateMetrics metrics{};
};

struct BmsSnapshot {
  PackTelemetry telemetry{};
  FaultStatus faults{};
  OutputCommand outputs{};
  BmsStateContext state_context{};
};

constexpr std::size_t to_index(BmsState state) {
  return static_cast<std::size_t>(state);
}

inline constexpr std::array<BmsStateDefinition, to_index(BmsState::Count)>
    kBmsStateDefinitions{{
        {
            BmsState::Startup,
            "startup",
            "Initial validation phase with power paths held safe until the first assessment completes.",
            false,
            false,
            false,
            false,
        },
        {
            BmsState::Idle,
            "idle",
            "Pack is healthy and near zero current with no balancing request active.",
            true,
            true,
            false,
            false,
        },
        {
            BmsState::Charging,
            "charging",
            "Pack is receiving current inside configured limits and charging path is allowed.",
            true,
            false,
            false,
            false,
        },
        {
            BmsState::Discharging,
            "discharging",
            "Pack is delivering current inside configured limits and discharge path is allowed.",
            false,
            true,
            false,
            false,
        },
        {
            BmsState::Balancing,
            "balancing",
            "One or more cells are above the balancing threshold and bleed resistors may be enabled.",
            true,
            true,
            true,
            false,
        },
        {
            BmsState::Fault,
            "fault",
            "At least one critical condition is active and the controller must operate conservatively.",
            false,
            false,
            false,
            true,
        },
    }};

inline constexpr const BmsStateDefinition& state_definition(BmsState state) {
  return kBmsStateDefinitions[to_index(state)];
}

inline constexpr const char* state_name(BmsState state) {
  return state_definition(state).name;
}

inline uint16_t min_cell_voltage(const PackTelemetry& telemetry) {
  uint16_t min_voltage = telemetry.cell_voltage_mv[0];
  for (std::size_t index = 1; index < telemetry.cell_voltage_mv.size(); ++index) {
    if (telemetry.cell_voltage_mv[index] < min_voltage) {
      min_voltage = telemetry.cell_voltage_mv[index];
    }
  }
  return min_voltage;
}

inline uint16_t max_cell_voltage(const PackTelemetry& telemetry) {
  uint16_t max_voltage = telemetry.cell_voltage_mv[0];
  for (std::size_t index = 1; index < telemetry.cell_voltage_mv.size(); ++index) {
    if (telemetry.cell_voltage_mv[index] > max_voltage) {
      max_voltage = telemetry.cell_voltage_mv[index];
    }
  }
  return max_voltage;
}

inline int16_t min_temperature(const PackTelemetry& telemetry) {
  int16_t min_temp = telemetry.temperatures_deci_c[0];
  for (std::size_t index = 1; index < telemetry.temperatures_deci_c.size(); ++index) {
    if (telemetry.temperatures_deci_c[index] < min_temp) {
      min_temp = telemetry.temperatures_deci_c[index];
    }
  }
  return min_temp;
}

inline int16_t max_temperature(const PackTelemetry& telemetry) {
  int16_t max_temp = telemetry.temperatures_deci_c[0];
  for (std::size_t index = 1; index < telemetry.temperatures_deci_c.size(); ++index) {
    if (telemetry.temperatures_deci_c[index] > max_temp) {
      max_temp = telemetry.temperatures_deci_c[index];
    }
  }
  return max_temp;
}

}  // namespace bms
