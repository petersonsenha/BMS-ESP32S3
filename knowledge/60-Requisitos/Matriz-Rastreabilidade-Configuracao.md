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
| `pack.series_cells` | [[REQ-BAL-001]] | [[Bateria-4S]], [[Esquematico-Logico-BMS-4S]] | define que o pack e 4S e orienta medicao por celula |
| `pack.parallel_cells` | [[REQ-BAL-001]] | [[Bateria-4S]] | influencia capacidade e estrategia operacional do pack |
| `pack.chemistry` | [[REQ-PROT-OT]] | [[Configuracao-BMS]], [[Sobretemperatura]] | condiciona limites termicos e de operacao |
| `pack.capacity_ah` | [[REQ-PROT-OC]] | [[Configuracao-BMS]], [[Sobrecorrente]] | ajuda a calibrar corrente segura de carga e descarga |
| `measurement.afe_type` | [[REQ-PROT-OV]], [[REQ-PROT-UV]] | [[AFE]], [[Telemetria]] | define a base da leitura segura por celula |
| `measurement.afe_interface` | [[REQ-STATE-001]] | [[ESP32-S3]], [[Telemetria]] | define como o firmware recebe medicao do AFE |
| `measurement.current_sense.method` | [[REQ-PROT-OC]] | [[Shunt]], [[Telemetria]] | define a estrategia de medicao de corrente |
| `measurement.current_sense.shunt_resistance_milliohm` | [[REQ-PROT-OC]] | [[Shunt]], [[Sobrecorrente]] | afeta escala e precisao da corrente medida |
| `measurement.temperature_sensors.ntc_count` | [[REQ-PROT-OT]] | [[NTC]], [[Sobretemperatura]] | define cobertura minima de monitoracao termica |
| `protection.cell_over_voltage_mv` | [[REQ-PROT-OV]] | [[Sobretensao]], [[Fault-Handling]] | define o limiar de bloqueio da carga |
| `protection.cell_under_voltage_mv` | [[REQ-PROT-UV]] | [[Subtensao]], [[Fault-Handling]] | define o limiar de bloqueio da descarga |
| `protection.pack_over_current_charge_ma` | [[REQ-PROT-OC]] | [[Sobrecorrente]], [[Fault-Handling]] | limita corrente segura de carga |
| `protection.pack_over_current_discharge_ma` | [[REQ-PROT-OC]] | [[Sobrecorrente]], [[Curto-Circuito]] | limita corrente segura de descarga |
| `protection.charge_temp_min_deci_c` | [[REQ-PROT-OT]] | [[Sobretemperatura]], [[Fault-Handling]] | impede carga abaixo da faixa segura |
| `protection.charge_temp_max_deci_c` | [[REQ-PROT-OT]] | [[Sobretemperatura]], [[Fault-Handling]] | impede carga acima da faixa segura |
| `protection.discharge_temp_max_deci_c` | [[REQ-PROT-OT]] | [[Sobretemperatura]], [[Fault-Handling]] | impede descarga acima da faixa segura |
| `protection.afe_fault_is_critical` | [[REQ-FAIL-001]] | [[AFE]], [[Fail-Safe]], [[Fault-Handling]] | define se falha do AFE derruba a operacao |
| `protection.short_circuit_hardware_cutoff_required` | [[REQ-PROT-OC]], [[REQ-FAIL-001]] | [[Curto-Circuito]], [[Fusivel]], [[Esquematico-Logico-BMS-4S]] | exige camada rapida de protecao em hardware |
| `protection.fuse_required` | [[REQ-PROT-OC]], [[REQ-FAIL-001]] | [[Fusivel]], [[Esquematico-Logico-BMS-4S]] | exige barreira fisica adicional no pack |
| `balancing.enabled` | [[REQ-BAL-001]] | [[Balanceamento-Passivo]], [[Maquina-de-Estados]] | habilita a funcionalidade de balanceamento |
| `balancing.strategy` | [[REQ-BAL-001]] | [[Balanceamento-Passivo]] | define o tipo de estrategia aplicada |
| `balancing.balance_start_mv` | [[REQ-BAL-001]] | [[Balanceamento-Passivo]], [[Telemetria]] | define o limiar de inicio do bleed |
| `balancing.balance_delta_mv` | [[REQ-BAL-001]] | [[Balanceamento-Passivo]], [[Maquina-de-Estados]] | define o desbalanceamento minimo para atuar |
| `outputs.charge_mosfet_enabled` | [[REQ-FAIL-001]], [[REQ-STATE-001]] | [[MOSFET-de-Carga]], [[Maquina-de-Estados]] | controla disponibilidade do caminho de carga |
| `outputs.discharge_mosfet_enabled` | [[REQ-FAIL-001]], [[REQ-STATE-001]] | [[MOSFET-de-Descarga]], [[Maquina-de-Estados]] | controla disponibilidade do caminho de descarga |
| `outputs.separate_charge_discharge_paths` | [[REQ-STATE-001]] | [[MOSFET-de-Carga]], [[MOSFET-de-Descarga]], [[Esquematico-Logico-BMS-4S]] | explicita arquitetura com caminhos distintos |
| `telemetry.current_sign_convention.negative_ma` | [[REQ-STATE-001]] | [[Telemetria]], [[Maquina-de-Estados]] | define interpretacao da corrente de carga |
| `telemetry.current_sign_convention.positive_ma` | [[REQ-STATE-001]] | [[Telemetria]], [[Maquina-de-Estados]] | define interpretacao da corrente de descarga |
| `telemetry.serial_log_interval_ms` | [[REQ-STATE-001]] | [[Telemetria]], [[Fault-Handling]] | regula observabilidade do estado da BMS |

## Leitura recomendada

Comece por:

1. [[Configuracao-BMS]]
2. [[Indice-Por-Requisito]]
3. [[Esquematico-Logico-BMS-4S]]

## Relacoes

- [[Configuracao-BMS]]
- [[Indice-Por-Configuracao]]
- [[Indice-Por-Requisito]]
- [[Maquina-de-Estados]]
- [[Fault-Handling]]
