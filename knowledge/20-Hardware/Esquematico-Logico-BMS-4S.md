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
- taps das celulas ate o [[AFE]]
- [[Shunt]] para medicao de corrente
- [[MOSFET-de-Carga]]
- [[MOSFET-de-Descarga]]
- [[Fusivel]]
- `regulacao de alimentacao` para o [[ESP32-S3]]
- [[NTC]] para temperatura
- interface entre [[ESP32-S3]] e [[AFE]]

## Fluxo logico

1. o pack entrega taps das celulas ao [[AFE]]
2. a corrente passa pelo [[Shunt]]
3. o estagio de potencia usa [[MOSFET-de-Carga]] e [[MOSFET-de-Descarga]]
4. o [[ESP32-S3]] le o estado do sistema e executa a [[Maquina-de-Estados]]
5. o firmware aplica [[Fault-Handling]] conforme os requisitos

## Relacoes

- [[AFE]]
- [[ESP32-S3]]
- [[Shunt]]
- [[MOSFET-de-Carga]]
- [[MOSFET-de-Descarga]]
- [[NTC]]
- [[Fusivel]]
- [[Configuracao-BMS]]
- [[REQ-PROT-OV]]
- [[REQ-PROT-UV]]
- [[REQ-PROT-OC]]
- [[REQ-PROT-OT]]
