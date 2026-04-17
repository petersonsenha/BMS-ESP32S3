---
tipo: hardware
dominio: arquitetura
tags:
  - bms
  - hardware
  - esquematico
  - logico
---

# Esquematico Logico BMS 4S

## Objetivo

Representar os blocos principais do circuito da BMS 4S sem entrar ainda no nivel de componente final.

## Blocos principais

- pack de celulas `4S`
- taps das celulas ate o [AFE](../conceitos/afe.md)
- [Shunt](shunt.md) para medicao de corrente
- [MOSFET-de-Carga](mosfet-de-carga.md)
- [MOSFET-de-Descarga](mosfet-de-descarga.md)
- [Fusivel](fusivel.md)
- `regulacao de alimentacao` para o [ESP32-S3](../conceitos/esp32-s3.md)
- [NTC](ntc.md) para temperatura
- interface entre [ESP32-S3](../conceitos/esp32-s3.md) e [AFE](../conceitos/afe.md)

## Fluxo logico

1. o pack entrega taps das celulas ao [AFE](../conceitos/afe.md)
2. a corrente passa pelo [Shunt](shunt.md)
3. o estagio de potencia usa [MOSFET-de-Carga](mosfet-de-carga.md) e [MOSFET-de-Descarga](mosfet-de-descarga.md)
4. o [ESP32-S3](../conceitos/esp32-s3.md) le o estado do sistema e executa a [Maquina-de-Estados](../firmware/maquina-de-estados.md)
5. o firmware aplica [Fault-Handling](../firmware/fault-handling.md) conforme os requisitos

## Relacoes

- [AFE](../conceitos/afe.md)
- [ESP32-S3](../conceitos/esp32-s3.md)
- [Shunt](shunt.md)
- [MOSFET-de-Carga](mosfet-de-carga.md)
- [MOSFET-de-Descarga](mosfet-de-descarga.md)
- [NTC](ntc.md)
- [Fusivel](fusivel.md)
- [Configuracao-BMS](../firmware/configuracao-bms.md)
- [REQ-PROT-OV](../requisitos/req-prot-ov.md)
- [REQ-PROT-UV](../requisitos/req-prot-uv.md)
- [REQ-PROT-OC](../requisitos/req-prot-oc.md)
- [REQ-PROT-OT](../requisitos/req-prot-ot.md)
