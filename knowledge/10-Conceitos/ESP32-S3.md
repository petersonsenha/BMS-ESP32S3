---
tipo: conceito
dominio: controle
tags:
  - bms
  - esp32
  - firmware
---

# ESP32-S3

No projeto, o ESP32-S3 atua como controlador de alto nivel.

## Responsabilidades

- executar a [[Maquina-de-Estados]]
- consolidar [[Telemetria]]
- aplicar [[Fault-Handling]]
- expor comunicacao e diagnostico

## Nao deve fazer sozinho

- protecao primaria de curto-circuito
- leitura direta insegura dos taps das celulas

## Relacoes

- [[AFE]]
- [[Fail-Safe]]
- [[BMS]]
