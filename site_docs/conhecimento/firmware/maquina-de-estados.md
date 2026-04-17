---
tipo: firmware
dominio: estados
tags:
  - bms
  - firmware
  - maquina-de-estados
---

# Maquina de Estados

Representa o modo operacional atual da BMS.

## Estados

- `Startup`
- `Idle`
- `Charging`
- `Discharging`
- `Balancing`
- `Fault`

## Relacoes

- [Fault-Handling](fault-handling.md)
- [Telemetria](telemetria.md)
- [Balanceamento-Passivo](../conceitos/balanceamento-passivo.md)
- [Fail-Safe](../protecoes/fail-safe.md)
- [MOC-Firmware](../mocs/moc-firmware.md)

## Referencia formal

- [Documento tecnico](../../projeto/maquina-de-estados-bms.md)
