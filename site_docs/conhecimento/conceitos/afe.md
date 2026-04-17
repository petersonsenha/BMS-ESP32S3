---
tipo: conceito
dominio: hardware
tags:
  - bms
  - afe
  - medicao
---

# AFE

AFE e o front-end analogico dedicado para medicao segura das celulas e, em muitos casos,
apoio a protecoes rapidas do pack.

## No contexto da BMS

- mede tensao por celula
- pode apoiar o [Balanceamento-Passivo](balanceamento-passivo.md)
- pode detectar falhas de forma mais rapida que o loop do microcontrolador

## Relacoes

- [Bateria-4S](bateria-4s.md)
- [Sobretensao](../protecoes/sobretensao.md)
- [Subtensao](../protecoes/subtensao.md)
- [Curto-Circuito](../protecoes/curto-circuito.md)
- [Fault-Handling](../firmware/fault-handling.md)
