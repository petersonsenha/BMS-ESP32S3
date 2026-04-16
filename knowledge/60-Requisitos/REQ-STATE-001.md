---
tipo: requisito
dominio: firmware
id: REQ-STATE-001
criticidade: alta
tags:
  - bms
  - requisito
  - estados
---

# REQ-STATE-001

## Declaracao

A logica da BMS deve ser organizada por maquina de estados explicita, com transicoes auditaveis e comportamento seguro em `Startup` e `Fault`.

## Componentes relacionados

- [[ESP32-S3]]

## Firmware relacionado

- [[Maquina-de-Estados]]
- [[Fault-Handling]]
- [[Telemetria]]

## Protecao relacionada

- [[Fail-Safe]]
