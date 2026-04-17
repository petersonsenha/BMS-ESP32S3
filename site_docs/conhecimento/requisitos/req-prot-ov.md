---
tipo: requisito
dominio: protecao
id: REQ-PROT-OV
criticidade: alta
tags:
  - bms
  - requisito
  - sobretensao
---

# REQ-PROT-OV

## Declaracao

A BMS deve bloquear o caminho de carga quando qualquer celula ultrapassar a tensao maxima segura.

## Componentes relacionados

- [AFE](../conceitos/afe.md)
- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)

## Firmware relacionado

- [Fault-Handling](../firmware/fault-handling.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)
- [Telemetria](../firmware/telemetria.md)

## Protecao relacionada

- [Sobretensao](../protecoes/sobretensao.md)
