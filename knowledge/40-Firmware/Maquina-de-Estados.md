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

- [[Fault-Handling]]
- [[Telemetria]]
- [[Balanceamento-Passivo]]
- [[Fail-Safe]]
- [[MOC-Firmware]]

## Referencia formal

- [Documento tecnico](../../docs/maquina-de-estados-bms.md)
