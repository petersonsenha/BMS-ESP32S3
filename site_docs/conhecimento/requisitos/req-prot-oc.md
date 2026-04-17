---
tipo: requisito
dominio: protecao
id: REQ-PROT-OC
criticidade: alta
tags:
  - bms
  - requisito
  - sobrecorrente
---

# REQ-PROT-OC

## Declaracao

A BMS deve detectar sobrecorrente e levar o sistema a um estado seguro no caminho afetado.

## Componentes relacionados

- [Shunt](../hardware/shunt.md)
- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)
- [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md)
- [Fusivel](../hardware/fusivel.md)

## Firmware relacionado

- [Fault-Handling](../firmware/fault-handling.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)
- [Telemetria](../firmware/telemetria.md)

## Protecao relacionada

- [Sobrecorrente](../protecoes/sobrecorrente.md)
- [Curto-Circuito](../protecoes/curto-circuito.md)
