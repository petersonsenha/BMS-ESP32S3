---
tipo: firmware
dominio: configuracao
tags:
  - bms
  - firmware
  - configuracao
  - json
---

# Configuracao BMS

## Origem

Arquivo principal:

- [config/bms_config.json](../../assets/downloads/bms_config.json)

## Secoes da configuracao

### `pack`

Define a estrutura do pack:

- numero de celulas em serie
- numero de celulas em paralelo
- quimica
- capacidade

Relaciona com:

- [Bateria-4S](../conceitos/bateria-4s.md)
- [REQ-BAL-001](../requisitos/req-bal-001.md)

### `measurement`

Define como a BMS mede o pack:

- tipo de [AFE](../conceitos/afe.md)
- interface com o [ESP32-S3](../conceitos/esp32-s3.md)
- metodo de medicao por [Shunt](../hardware/shunt.md)
- quantidade de [NTC](../hardware/ntc.md)

Relaciona com:

- [Telemetria](telemetria.md)
- [REQ-PROT-OC](../requisitos/req-prot-oc.md)
- [REQ-PROT-OT](../requisitos/req-prot-ot.md)

### `protection`

Define os limites e regras de seguranca:

- sobretensao
- subtensao
- sobrecorrente
- janela termica
- criticidade de falha do AFE
- exigencia de corte rapido de curto
- exigencia de [Fusivel](../hardware/fusivel.md)

Relaciona com:

- [Sobretensao](../protecoes/sobretensao.md)
- [Subtensao](../protecoes/subtensao.md)
- [Sobrecorrente](../protecoes/sobrecorrente.md)
- [Sobretemperatura](../protecoes/sobretemperatura.md)
- [Fail-Safe](../protecoes/fail-safe.md)

### `balancing`

Define a estrategia de [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md):

- habilitado ou nao
- tipo de estrategia
- limiar de inicio
- delta minimo entre celulas

Relaciona com:

- [REQ-BAL-001](../requisitos/req-bal-001.md)
- [Maquina-de-Estados](maquina-de-estados.md)

### `outputs`

Define o comportamento esperado dos caminhos de potencia:

- caminho de carga
- caminho de descarga
- separacao entre caminhos

Relaciona com:

- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)
- [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md)
- [REQ-FAIL-001](../requisitos/req-fail-001.md)

### `telemetry`

Define convencoes de observabilidade:

- sinal da corrente
- periodicidade de log

Relaciona com:

- [Telemetria](telemetria.md)
- [REQ-STATE-001](../requisitos/req-state-001.md)

## Uso no conhecimento do projeto

Essa nota serve como ponte entre:

- configuracao real em JSON
- comportamento do firmware
- requisitos de protecao
- blocos do esquematico logico

## Relacoes

- [Indice-Por-Configuracao](../mocs/indice-por-configuracao.md)
- [Matriz-Rastreabilidade-Configuracao](../requisitos/matriz-rastreabilidade-configuracao.md)
- [Esquematico-Logico-BMS-4S](../hardware/esquematico-logico-bms-4s.md)
- [Fault-Handling](fault-handling.md)
- [Maquina-de-Estados](maquina-de-estados.md)
