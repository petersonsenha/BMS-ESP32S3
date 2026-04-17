# Tecnicas de Protecao para uma BMS 4S

## Visao geral

Uma BMS precisa assumir que falhas vao acontecer:

- celula pode sair do limite
- sensor pode falhar
- MOSFET pode aquecer
- firmware pode travar
- usuario pode conectar uma carga agressiva

Por isso, a protecao deve ser distribuida em mais de uma camada.

## 1. Protecao por tensao de celula

Essa e a protecao mais basica e mais importante.

### Sobretensao

Objetivo:

- impedir que qualquer celula ultrapasse a tensao maxima segura durante a carga

Tecnica recomendada:

- medir cada celula com AFE dedicado
- desligar o MOSFET de carga quando qualquer celula exceder o limite
- manter balanceamento nas celulas mais altas quando permitido pela estrategia

### Subtensao

Objetivo:

- impedir descarga profunda, que degrada ou danifica a celula

Tecnica recomendada:

- desligar o MOSFET de descarga quando qualquer celula cair abaixo do limite
- opcionalmente usar dois niveis: alerta e corte

## 2. Protecao por corrente

### Sobrecorrente de carga

Objetivo:

- evitar aquecimento, estresse nas celulas e estresse nos MOSFETs durante a carga

Tecnica recomendada:

- medir corrente no shunt
- cortar o caminho de carga acima do limite
- usar comparacao rapida por hardware quando o AFE suportar

### Sobrecorrente de descarga

Objetivo:

- proteger o pack contra carga excessiva ou falha no sistema alimentado

Tecnica recomendada:

- usar limite de descarga no firmware
- usar protecao rapida por hardware para eventos abruptos

### Curto-circuito

Objetivo:

- abrir o circuito antes que a trilha, o shunt ou os MOSFETs entrem em regime destrutivo

Tecnica recomendada:

- nao depender apenas do loop de software
- usar AFE/comparador com resposta rapida
- considerar fusivel como ultima barreira fisica

## 3. Protecao por temperatura

Temperatura deve ser observada tanto nas celulas quanto no estagio de potencia.

### Carga em baixa temperatura

Objetivo:

- impedir carga abaixo da faixa segura, especialmente em quimicas de litio

Tecnica recomendada:

- bloquear carga quando a temperatura estiver abaixo do limite configurado

### Sobretemperatura

Objetivo:

- evitar dano termico e reduzir risco de runaway

Tecnica recomendada:

- usar pelo menos 2 NTC
- um perto das celulas
- um perto dos MOSFETs
- bloquear carga e descarga conforme a estrategia definida

## 4. Balanceamento de celulas

Balanceamento nao substitui protecao, mas ajuda a manter o pack dentro de uma operacao segura.

### Balanceamento passivo

Tecnica recomendada para esta primeira versao:

- resistor de bleed por celula
- acionamento controlado
- habilitar apenas quando a celula estiver acima do limiar e acima das demais

Cuidados:

- nao balancear durante falha critica
- evitar balanceamento agressivo com temperatura elevada

## 5. Protecao em caso de falha do sistema

Uma BMS segura precisa prever o que acontece se o microcontrolador ou o AFE falhar.

Tecnicas recomendadas:

- `watchdog` no ESP32-S3
- estado seguro por padrao nos gates dos MOSFETs
- tratar falha de comunicacao com o AFE como falha critica
- desligar carga e descarga em caso de leitura invalida
- manter trilha de falha latched quando necessario

## 6. Protecao mecanica e eletrica complementar

Mesmo com firmware e AFE adequados, algumas protecoes devem existir no hardware de potencia:

- fusivel no pack
- TVS ou supressor, se a aplicacao exigir
- layout com retorno de corrente bem controlado
- separacao entre sinal e potencia
- dimensionamento termico de trilhas, shunt e MOSFETs

## 7. Filosofia recomendada para este projeto

Para esta BMS 4S com ESP32-S3:

- o `AFE` deve cuidar da leitura confiavel das celulas
- o `ESP32-S3` deve cuidar da logica, telemetria e estrategia
- o corte de falhas rapidas nao deve depender so do firmware
- o sistema deve assumir estado seguro se qualquer medicao essencial falhar

## 8. O que o firmware atual ja cobre

A base atual do repositorio ja contempla:

- deteccao de sobretensao por celula
- deteccao de subtensao por celula
- sobrecorrente de carga
- sobrecorrente de descarga
- bloqueio por temperatura de carga
- bloqueio por temperatura de descarga
- falha de AFE
- balanceamento passivo simples

## 9. O que ainda vale adicionar depois

Evolucoes importantes para versoes futuras:

1. protecao de curto com temporizacao em hardware
2. histerese e temporizacao por falha
3. estados latched com politica de recuperacao
4. estimativa de SOC e SOH
5. pre-charge
6. log persistente de eventos de falha
