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

- [ESP32-S3](../conceitos/esp32-s3.md)

## Firmware relacionado

- [Maquina-de-Estados](../firmware/maquina-de-estados.md)
- [Fault-Handling](../firmware/fault-handling.md)
- [Telemetria](../firmware/telemetria.md)

## Protecao relacionada

- [Fail-Safe](../protecoes/fail-safe.md)
