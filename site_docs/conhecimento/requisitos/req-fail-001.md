---
tipo: requisito
dominio: seguranca
id: REQ-FAIL-001
criticidade: alta
tags:
  - bms
  - requisito
  - fail-safe
---

# REQ-FAIL-001

## Declaracao

Em falha critica ou perda de supervisao essencial, a BMS deve assumir estado seguro.

## Componentes relacionados

- [ESP32-S3](../conceitos/esp32-s3.md)
- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)
- [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md)
- [Fusivel](../hardware/fusivel.md)

## Firmware relacionado

- [Fault-Handling](../firmware/fault-handling.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)

## Protecao relacionada

- [Fail-Safe](../protecoes/fail-safe.md)
