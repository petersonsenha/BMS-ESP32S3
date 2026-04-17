---
tipo: conceito
dominio: pack
tags:
  - bms
  - balanceamento
  - passivo
---

# Balanceamento Passivo

Balanceamento passivo usa resistores de bleed para dissipar energia das celulas mais altas.

## Quando entra

- quando a diferenca entre celulas passa do limite configurado
- quando a estrategia da [Maquina-de-Estados](../firmware/maquina-de-estados.md) permite

## Relacoes

- [Bateria-4S](bateria-4s.md)
- [AFE](afe.md)
- [MOSFET-de-Carga](../hardware/mosfet-de-carga.md)
- [Sobretemperatura](../protecoes/sobretemperatura.md)
- [Telemetria](../firmware/telemetria.md)
