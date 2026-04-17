---
tipo: requisito
dominio: protecao
id: REQ-PROT-UV
criticidade: alta
tags:
  - bms
  - requisito
  - subtensao
---

# REQ-PROT-UV

## Declaracao

A BMS deve bloquear o caminho de descarga quando qualquer celula cair abaixo da tensao minima segura.

## Componentes relacionados

- [AFE](../conceitos/afe.md)
- [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md)

## Firmware relacionado

- [Fault-Handling](../firmware/fault-handling.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)

## Protecao relacionada

- [Subtensao](../protecoes/subtensao.md)
