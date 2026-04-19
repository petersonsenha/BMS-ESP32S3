# Arquitetura BMS 4S com ESP32-S3

## Objetivo

Montar uma BMS 4S para bateria de litio usando o ESP32-S3 como cerebro de supervisao,
telemetria e comunicacao.

## Ponto critico

Nao e uma boa pratica ligar os taps das 4 celulas diretamente nas entradas ADC do ESP32.
Mesmo com divisores resistivos, isso cria limites de precisao, ruido, referencia comum e risco
de dano ao microcontrolador.

Para uma BMS real, o caminho recomendado e:

- AFE dedicado para medicao das celulas
- protecao por hardware para eventos rapidos
- ESP32-S3 para controle, logica, configuracao e conectividade

## Camadas de protecao recomendadas

Para esta BMS 4S, a protecao deve ser pensada em camadas:

- `camada 1 - hardware rapido`: AFE, comparadores internos, cutoff de sobrecorrente e fusivel
- `camada 2 - firmware`: analise de limites, estados, telemetria e estrategia de balanceamento
- `camada 3 - fail-safe`: watchdog, default seguro nos MOSFETs e tratamento de falha do AFE

Se uma dessas camadas falhar, outra ainda ajuda a evitar que a bateria entre em condicao perigosa.

## Blocos recomendados

### 1. Medicao das celulas

Use um front-end proprio para bateria multicelula, por exemplo uma familia equivalente a:

- 3S-5S com balanceamento passivo integrado
- leitura por I2C ou SPI
- comparadores internos para subtensao e sobretensao

O firmware desta base foi pensado para conversar com esse tipo de bloco.

### 2. Medicao de corrente

Recomendado:

- resistor shunt no negativo do pack
- amplificador de corrente ou medicao integrada no proprio AFE

Sinal esperado no software:

- corrente positiva = descarga
- corrente negativa = carga

### 3. Temperatura

Minimo recomendado:

- 2x NTC
- um proximo das celulas
- um proximo dos MOSFETs ou area de potencia

### 4. Chaves de potencia

Topologia minima:

- MOSFET de carga
- MOSFET de descarga
- caminho ou resistor de `pre-charge`, se o barramento de saida tiver capacitancia relevante

Opcional e recomendado em versoes futuras:

- pre-charge
- fusivel
- protecao secundaria por comparador/hardware

Observacao importante:

- em curto-circuito ou sobrecorrente rapida, o desligamento ideal deve ocorrer no hardware de potencia ou no AFE, nao apenas no loop principal do ESP32-S3

### 5. Balanceamento

O projeto inicial considera balanceamento passivo:

- resistor de bleed por celula
- chaveamento por transistor/MOSFET
- habilitacao quando a celula esta acima do limiar de balanceamento e acima das demais

## Limites iniciais de firmware

Os limites padrao definidos na base atual sao:

- sobretensao de celula: 4.200 V
- subtensao de celula: 3.000 V
- sobrecorrente de carga: 5 A
- sobrecorrente de descarga: 10 A
- temperatura minima para carga: 0.0 C
- temperatura maxima para carga: 45.0 C
- temperatura maxima para descarga: 60.0 C
- inicio de balanceamento: 4.100 V
- delta minimo para balancear: 15 mV

Esses numeros sao ponto de partida. Eles precisam ser ajustados para a quimica da celula,
capacidade do pack, corrente nominal e estrategia de vida util.

## Logica de estados

A base de firmware implementa:

- `Startup`
- `Idle`
- `Charging`
- `Discharging`
- `Balancing`
- `Fault`

## O que ja esta pronto no firmware

- estruturas de telemetria para 4S
- avaliacao de falhas com histerese e latch
- deteccao de `short-circuit discharge`
- habilitacao independente de MOSFET de carga e descarga
- estado de `Precharge` com observacao do barramento de saida
- balanceamento passivo com janela operacional
- base de state estimation com `coulomb counting + OCV`
- geracao automatica do header de configuracao a partir do `bms_config.json`
- servico de override de configuracao em runtime via `Stream`
- camada de `BatteryMonitor` separada entre `AFE` e medicao auxiliar
- historico circular de eventos em memoria para diagnostico e rastreio de faults
- monitor mock para desenvolver sem hardware final
- arquivo de configuracao JSON em `config/bms_config.json`

## O que falta para virar hardware real

1. escolher o AFE real
2. adaptar o scaffold de driver para o AFE final
3. definir o mapa de pinos do ESP32-S3
4. definir o estagio de potencia e a sequencia de pre-charge
5. validar tempos de resposta e comportamento em falha
6. testar em fonte limitada antes de conectar um pack real
7. expandir o override de configuracao para BLE/Wi-Fi, se a aplicacao exigir

## Recomendacao de desenvolvimento

Sequencia segura para evoluir:

1. fechar os requisitos eletricos da bateria 4S
2. escolher o AFE e os MOSFETs
3. desenhar o esquematico
4. adaptar o `AfeBatteryMonitor` e o scaffold de driver para o AFE final
5. instrumentar exportacao de eventos por serial, BLE ou Wi-Fi
6. fazer testes com fonte e cargas controladas
