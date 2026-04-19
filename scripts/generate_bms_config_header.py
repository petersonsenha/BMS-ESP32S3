from __future__ import annotations

import json
from pathlib import Path


def resolve_root() -> Path:
    if "__file__" in globals():
        return Path(__file__).resolve().parents[1]

    current = Path.cwd().resolve()
    if (current / "platformio.ini").exists():
        return current
    if (current.parent / "platformio.ini").exists():
        return current.parent
    return current


ROOT = resolve_root()
CONFIG_PATH = ROOT / "config" / "bms_config.json"
OUTPUT_PATH = ROOT / "include" / "generated_bms_config.h"


def as_cpp_bool(value: bool) -> str:
    return "true" if value else "false"


def as_cpp_float(value: float) -> str:
    rendered = f"{value:.6g}"
    if "." not in rendered and "e" not in rendered.lower():
        rendered = f"{rendered}.0"
    return f"{rendered}f"


def load_config() -> dict:
    return json.loads(CONFIG_PATH.read_text(encoding="utf-8"))


def build_header(config: dict) -> str:
    protection = config["protection"]
    fault_handling = config["fault_handling"]
    operation = config["operation"]
    estimation = config["estimation"]

    return f"""#pragma once

#include "bms_types.h"

namespace bms {{

inline constexpr BmsConfig kGeneratedBmsConfig{{
    ProtectionConfig{{
        {protection["cell_over_voltage_mv"]},
        {protection["cell_under_voltage_mv"]},
        {protection["pack_over_current_charge_ma"]},
        {protection["pack_over_current_discharge_ma"]},
        {protection["charge_temp_min_deci_c"]},
        {protection["charge_temp_max_deci_c"]},
        {protection["discharge_temp_max_deci_c"]},
        {protection["balance_start_mv"]},
        {protection["balance_delta_mv"]},
    }},
    FaultHandlingConfig{{
        {fault_handling["cell_voltage_hysteresis_mv"]},
        {fault_handling["current_hysteresis_ma"]},
        {fault_handling["temperature_hysteresis_deci_c"]},
        {fault_handling["short_circuit_discharge_ma"]},
        {as_cpp_bool(fault_handling["fault_latch_enabled"])},
        {as_cpp_bool(fault_handling["auto_recovery_enabled"])},
        {fault_handling["fault_release_delay_ms"]},
        {fault_handling["short_circuit_release_delay_ms"]},
    }},
    OperationalConfig{{
        {operation["activity_current_threshold_ma"]},
        {operation["charge_termination_current_ma"]},
        {operation["balance_max_pack_current_ma"]},
        {operation["balance_max_temperature_deci_c"]},
        {as_cpp_bool(operation["allow_balancing_while_charging"])},
        {as_cpp_bool(operation["allow_balancing_in_idle"])},
        {as_cpp_bool(operation["precharge_required"])},
        {operation["precharge_timeout_ms"]},
        {operation["precharge_target_delta_mv"]},
    }},
    EstimationConfig{{
        {estimation["nominal_capacity_mah"]},
        {estimation["nominal_cell_voltage_mv"]},
        {estimation["full_cell_voltage_mv"]},
        {estimation["empty_cell_voltage_mv"]},
        {estimation["ocv_correction_idle_ms"]},
        {estimation["ocv_correction_max_current_ma"]},
        {estimation["coulomb_efficiency_ppm"]},
        {as_cpp_float(estimation["nominal_internal_resistance_milliohm"])},
        {as_cpp_float(estimation["minimum_soc_for_discharge_pct"])},
        {as_cpp_bool(estimation["enable_ocv_correction"])},
    }},
}};

}}  // namespace bms
"""


def main() -> None:
    config = load_config()
    OUTPUT_PATH.write_text(build_header(config), encoding="utf-8")
    print(f"Generated {OUTPUT_PATH.relative_to(ROOT)} from {CONFIG_PATH.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
