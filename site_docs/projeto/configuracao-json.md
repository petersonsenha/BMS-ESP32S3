# Configuracao JSON da BMS

Este arquivo consolida os limites e escolhas operacionais da BMS 4S em um formato simples,
versionavel e facil de alinhar com firmware, requisitos e validacao.

## Estrutura geral

- `pack`: define topologia do pack e capacidade nominal
- `measurement`: escolhe o AFE, interface e estrategia de medicao
- `protection`: concentra limites eletricos e termicos criticos
- `balancing`: habilita e parametriza o balanceamento passivo
- `outputs`: explicita o controle dos caminhos de carga e descarga
- `telemetry`: padroniza sinal de corrente e intervalo de log

## Arquivo atual

```json
{
  "pack": {
    "series_cells": 4,
    "parallel_cells": 1,
    "chemistry": "li-ion",
    "capacity_ah": 20.0
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
    "afe_fault_is_critical": true,
    "short_circuit_hardware_cutoff_required": true,
    "fuse_required": true
  },
  "balancing": {
    "enabled": true,
    "strategy": "passive",
    "balance_start_mv": 4100,
    "balance_delta_mv": 15
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
- o JSON deve permanecer alinhado com a [matriz de rastreabilidade](../conhecimento/requisitos/matriz-rastreabilidade-configuracao.md)

## Downloads

- [Baixar bms_config.json](../assets/downloads/bms_config.json)
- [Abrir a nota de configuracao](../conhecimento/firmware/configuracao-bms.md)
