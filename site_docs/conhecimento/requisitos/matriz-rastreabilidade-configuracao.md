---
tipo: matriz
dominio: rastreabilidade
tags:
  - bms
  - configuracao
  - requisitos
  - rastreabilidade
---

# Matriz Rastreabilidade Configuracao

## Objetivo

Ligar o `config/bms_config.json` aos requisitos, aos componentes e ao comportamento do firmware.

## Como ler

- `json path`: campo da configuracao
- `requisito`: requisito impactado
- `implementacao`: notas de firmware ou hardware diretamente relacionadas
- `impacto`: motivo da ligacao

## Matriz

| JSON path | Requisito | Implementacao | Impacto |
| --- | --- | --- | --- |
| `pack.series_cells` | [REQ-BAL-001](req-bal-001.md) | [Bateria-4S](../conceitos/bateria-4s.md), [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md) | define que o pack e 4S e orienta medicao por celula |
| `pack.parallel_cells` | [REQ-BAL-001](req-bal-001.md) | [Bateria-4S](../conceitos/bateria-4s.md) | influencia capacidade e estrategia operacional do pack |
| `pack.chemistry` | [REQ-PROT-OT](req-prot-ot.md) | [Configuracao-BMS](../firmware/configuracao-bms.md), [Sobretemperatura](../protecoes/sobretemperatura.md) | condiciona limites termicos e de operacao |
| `pack.capacity_ah` | [REQ-PROT-OC](req-prot-oc.md) | [Configuracao-BMS](../firmware/configuracao-bms.md), [Sobrecorrente](../protecoes/sobrecorrente.md) | ajuda a calibrar corrente segura de carga e descarga |
| `measurement.afe_type` | [REQ-PROT-OV](req-prot-ov.md), [REQ-PROT-UV](req-prot-uv.md) | [AFE](../conceitos/afe.md), [Telemetria](../firmware/telemetria.md) | define a base da leitura segura por celula |
| `measurement.afe_interface` | [REQ-STATE-001](req-state-001.md) | [ESP32-S3](../conceitos/esp32-s3.md), [Telemetria](../firmware/telemetria.md) | define como o firmware recebe medicao do AFE |
| `measurement.current_sense.method` | [REQ-PROT-OC](req-prot-oc.md) | [Shunt](../hardware/shunt.md), [Telemetria](../firmware/telemetria.md) | define a estrategia de medicao de corrente |
| `measurement.current_sense.shunt_resistance_milliohm` | [REQ-PROT-OC](req-prot-oc.md) | [Shunt](../hardware/shunt.md), [Sobrecorrente](../protecoes/sobrecorrente.md) | afeta escala e precisao da corrente medida |
| `measurement.temperature_sensors.ntc_count` | [REQ-PROT-OT](req-prot-ot.md) | [NTC](../hardware/ntc.md), [Sobretemperatura](../protecoes/sobretemperatura.md) | define cobertura minima de monitoracao termica |
| `protection.cell_over_voltage_mv` | [REQ-PROT-OV](req-prot-ov.md) | [Sobretensao](../protecoes/sobretensao.md), [Fault-Handling](../firmware/fault-handling.md) | define o limiar de bloqueio da carga |
| `protection.cell_under_voltage_mv` | [REQ-PROT-UV](req-prot-uv.md) | [Subtensao](../protecoes/subtensao.md), [Fault-Handling](../firmware/fault-handling.md) | define o limiar de bloqueio da descarga |
| `protection.pack_over_current_charge_ma` | [REQ-PROT-OC](req-prot-oc.md) | [Sobrecorrente](../protecoes/sobrecorrente.md), [Fault-Handling](../firmware/fault-handling.md) | limita corrente segura de carga |
| `protection.pack_over_current_discharge_ma` | [REQ-PROT-OC](req-prot-oc.md) | [Sobrecorrente](../protecoes/sobrecorrente.md), [Curto-Circuito](../protecoes/curto-circuito.md) | limita corrente segura de descarga |
| `protection.charge_temp_min_deci_c` | [REQ-PROT-OT](req-prot-ot.md) | [Sobretemperatura](../protecoes/sobretemperatura.md), [Fault-Handling](../firmware/fault-handling.md) | impede carga abaixo da faixa segura |
| `protection.charge_temp_max_deci_c` | [REQ-PROT-OT](req-prot-ot.md) | [Sobretemperatura](../protecoes/sobretemperatura.md), [Fault-Handling](../firmware/fault-handling.md) | impede carga acima da faixa segura |
| `protection.discharge_temp_max_deci_c` | [REQ-PROT-OT](req-prot-ot.md) | [Sobretemperatura](../protecoes/sobretemperatura.md), [Fault-Handling](../firmware/fault-handling.md) | impede descarga acima da faixa segura |
| `protection.afe_fault_is_critical` | [REQ-FAIL-001](req-fail-001.md) | [AFE](../conceitos/afe.md), [Fail-Safe](../protecoes/fail-safe.md), [Fault-Handling](../firmware/fault-handling.md) | define se falha do AFE derruba a operacao |
| `protection.short_circuit_hardware_cutoff_required` | [REQ-PROT-OC](req-prot-oc.md), [REQ-FAIL-001](req-fail-001.md) | [Curto-Circuito](../protecoes/curto-circuito.md), [Fusivel](../hardware/fusivel.md), [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md) | exige camada rapida de protecao em hardware |
| `protection.fuse_required` | [REQ-PROT-OC](req-prot-oc.md), [REQ-FAIL-001](req-fail-001.md) | [Fusivel](../hardware/fusivel.md), [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md) | exige barreira fisica adicional no pack |
| `balancing.enabled` | [REQ-BAL-001](req-bal-001.md) | [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | habilita a funcionalidade de balanceamento |
| `balancing.strategy` | [REQ-BAL-001](req-bal-001.md) | [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md) | define o tipo de estrategia aplicada |
| `balancing.balance_start_mv` | [REQ-BAL-001](req-bal-001.md) | [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md), [Telemetria](../firmware/telemetria.md) | define o limiar de inicio do bleed |
| `balancing.balance_delta_mv` | [REQ-BAL-001](req-bal-001.md) | [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | define o desbalanceamento minimo para atuar |
| `outputs.charge_mosfet_enabled` | [REQ-FAIL-001](req-fail-001.md), [REQ-STATE-001](req-state-001.md) | [MOSFET-de-Carga](../hardware/mosfet-de-carga.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | controla disponibilidade do caminho de carga |
| `outputs.discharge_mosfet_enabled` | [REQ-FAIL-001](req-fail-001.md), [REQ-STATE-001](req-state-001.md) | [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | controla disponibilidade do caminho de descarga |
| `outputs.separate_charge_discharge_paths` | [REQ-STATE-001](req-state-001.md) | [MOSFET-de-Carga](../hardware/mosfet-de-carga.md), [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md), [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md) | explicita arquitetura com caminhos distintos |
| `telemetry.current_sign_convention.negative_ma` | [REQ-STATE-001](req-state-001.md) | [Telemetria](../firmware/telemetria.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | define interpretacao da corrente de carga |
| `telemetry.current_sign_convention.positive_ma` | [REQ-STATE-001](req-state-001.md) | [Telemetria](../firmware/telemetria.md), [Maquina-de-Estados](../firmware/maquina-de-estados.md) | define interpretacao da corrente de descarga |
| `telemetry.serial_log_interval_ms` | [REQ-STATE-001](req-state-001.md) | [Telemetria](../firmware/telemetria.md), [Fault-Handling](../firmware/fault-handling.md) | regula observabilidade do estado da BMS |

## Leitura recomendada

Comece por:

1. [Configuracao-BMS](../firmware/configuracao-bms.md)
2. [Indice-Por-Requisito](../mocs/indice-por-requisito.md)
3. [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md)

## Relacoes

- [Configuracao-BMS](../firmware/configuracao-bms.md)
- [Indice-Por-Configuracao](../mocs/indice-por-configuracao.md)
- [Indice-Por-Requisito](../mocs/indice-por-requisito.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)
- [Fault-Handling](../firmware/fault-handling.md)
