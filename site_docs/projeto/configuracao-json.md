# Configuracao JSON da BMS

Este arquivo consolida os limites e escolhas operacionais da BMS 4S em um formato simples,
versionavel e facil de alinhar com firmware, requisitos e validacao.

## Estrutura geral

- `pack`: define topologia do pack e capacidade nominal
- `measurement`: escolhe o AFE, interface e estrategia de medicao
- `protection`: concentra limites eletricos e termicos criticos
- `fault_handling`: define histerese, latch e politica de liberacao de falha
- `operation`: concentra limiares operacionais de carga, descarga e balanceamento
- `estimation`: define a base de `SOC`, `SOE`, `SOH`, `SOP` e `SOF`

## Arquivo atual

```json
{
  "pack": {
    "series_cells": 4,
    "parallel_cells": 1,
    "chemistry": "li-ion",
    "capacity_ah": 20.0,
    "nominal_cell_voltage_mv": 3700,
    "full_cell_voltage_mv": 4200,
    "empty_cell_voltage_mv": 3000
  },
  "measurement": {
    "afe_type": "to_be_defined",
    "afe_interface": "i2c",
    "current_sense": {
      "method": "shunt",
      "shunt_resistance_milliohm": 2.0
    },
    "temperature_sensors": {
      "ntc_count": 2
    }
  },
  "protection": {
    "cell_over_voltage_mv": 4200,
    "cell_under_voltage_mv": 3000,
    "pack_over_current_charge_ma": 5000,
    "pack_over_current_discharge_ma": 10000,
    "charge_temp_min_deci_c": 0,
    "charge_temp_max_deci_c": 450,
    "discharge_temp_max_deci_c": 600,
    "balance_start_mv": 4100,
    "balance_delta_mv": 15,
    "afe_fault_is_critical": true,
    "short_circuit_hardware_cutoff_required": true,
    "fuse_required": true
  },
  "fault_handling": {
    "cell_voltage_hysteresis_mv": 25,
    "current_hysteresis_ma": 400,
    "temperature_hysteresis_deci_c": 20,
    "short_circuit_discharge_ma": 18000,
    "fault_latch_enabled": true,
    "auto_recovery_enabled": false,
    "fault_release_delay_ms": 3000,
    "short_circuit_release_delay_ms": 10000
  },
  "operation": {
    "activity_current_threshold_ma": 200,
    "charge_termination_current_ma": 250,
    "balance_max_pack_current_ma": 1500,
    "balance_max_temperature_deci_c": 420,
    "allow_balancing_while_charging": true,
    "allow_balancing_in_idle": true,
    "precharge_required": false,
    "precharge_timeout_ms": 1500,
    "precharge_target_delta_mv": 500
  },
  "estimation": {
    "nominal_capacity_mah": 20000,
    "nominal_cell_voltage_mv": 3700,
    "full_cell_voltage_mv": 4200,
    "empty_cell_voltage_mv": 3000,
    "ocv_correction_idle_ms": 30000,
    "ocv_correction_max_current_ma": 150,
    "coulomb_efficiency_ppm": 995000,
    "nominal_internal_resistance_milliohm": 18.0,
    "minimum_soc_for_discharge_pct": 5.0,
    "enable_ocv_correction": true
  },
  "outputs": {
    "charge_mosfet_enabled": true,
    "discharge_mosfet_enabled": true,
    "separate_charge_discharge_paths": true
  },
  "telemetry": {
    "current_sign_convention": {
      "negative_ma": "charge",
      "positive_ma": "discharge"
    },
    "serial_log_interval_ms": 1000
  }
}
```

## Observacoes de engenharia

- o projeto assume `4S` com `1P`
- o AFE ainda esta como `to_be_defined`, o que combina com a fase atual de arquitetura
- a protecao de curto em hardware segue marcada como obrigatoria
- o `PlatformIO` gera `include/generated_bms_config.h` antes do build a partir deste JSON
- o firmware aceita override em runtime via um servico de comandos baseado em `Stream`
- o JSON deve permanecer alinhado com a [matriz de rastreabilidade](../conhecimento/requisitos/matriz-rastreabilidade-configuracao.md)

## Downloads

- [Baixar bms_config.json](../assets/downloads/bms_config.json)
- [Header gerado para o firmware](../assets/codigo-fonte/generated_bms_config.h)
- [Servico de runtime config](../assets/codigo-fonte/bms_runtime_config_service.cpp)
- [Camada de monitor para AFE](../assets/codigo-fonte/afe_battery_monitor.cpp)
- [Scaffold de driver BQ769x](../assets/codigo-fonte/bq769x_afe_driver.cpp)
- [Script gerador da configuracao](../assets/site/generate_bms_config_header.py)
- [Abrir a nota de configuracao](../conhecimento/firmware/configuracao-bms.md)
