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

- [config/bms_config.json](../../config/bms_config.json)

## Secoes da configuracao

### `pack`

Define a estrutura do pack:

- numero de celulas em serie
- numero de celulas em paralelo
- quimica
- capacidade

Relaciona com:

- [[Bateria-4S]]
- [[REQ-BAL-001]]

### `measurement`

Define como a BMS mede o pack:

- tipo de [[AFE]]
- interface com o [[ESP32-S3]]
- metodo de medicao por [[Shunt]]
- quantidade de [[NTC]]

Relaciona com:

- [[Telemetria]]
- [[REQ-PROT-OC]]
- [[REQ-PROT-OT]]

### `protection`

Define os limites e regras de seguranca:

- sobretensao
- subtensao
- sobrecorrente
- janela termica
- criticidade de falha do AFE
- exigencia de corte rapido de curto
- exigencia de [[Fusivel]]

Relaciona com:

- [[Sobretensao]]
- [[Subtensao]]
- [[Sobrecorrente]]
- [[Sobretemperatura]]
- [[Fail-Safe]]

### `balancing`

Define a estrategia de [[Balanceamento-Passivo]]:

- habilitado ou nao
- tipo de estrategia
- limiar de inicio
- delta minimo entre celulas

Relaciona com:

- [[REQ-BAL-001]]
- [[Maquina-de-Estados]]

### `outputs`

Define o comportamento esperado dos caminhos de potencia:

- caminho de carga
- caminho de descarga
- separacao entre caminhos

Relaciona com:

- [[MOSFET-de-Carga]]
- [[MOSFET-de-Descarga]]
- [[REQ-FAIL-001]]

### `telemetry`

Define convencoes de observabilidade:

- sinal da corrente
- periodicidade de log

Relaciona com:

- [[Telemetria]]
- [[REQ-STATE-001]]

## Uso no conhecimento do projeto

Essa nota serve como ponte entre:

- configuracao real em JSON
- comportamento do firmware
- requisitos de protecao
- blocos do esquematico logico

## Relacoes

- [[Indice-Por-Configuracao]]
- [[Matriz-Rastreabilidade-Configuracao]]
- [[Esquematico-Logico-BMS-4S]]
- [[Fault-Handling]]
- [[Maquina-de-Estados]]
