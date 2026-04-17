---
tipo: requisito
dominio: protecao
id: REQ-PROT-OT
criticidade: alta
tags:
  - bms
  - requisito
  - temperatura
---

# REQ-PROT-OT

## Declaracao

A BMS deve bloquear operacao fora da faixa termica segura para proteger celulas e estagio de potencia.

## Componentes relacionados

- [NTC](../hardware/ntc.md)
- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)
- [MOSFET-de-Descarga](../hardware/mosfet-de-descarga.md)

## Firmware relacionado

- [Fault-Handling](../firmware/fault-handling.md)
- [Maquina-de-Estados](../firmware/maquina-de-estados.md)
- [Telemetria](../firmware/telemetria.md)

## Protecao relacionada

- [Sobretemperatura](../protecoes/sobretemperatura.md)
