<div class="thesis-cover">
  <p class="thesis-kicker">Trabalho de Conclusão de Curso</p>
  <h1>Desenvolvimento de uma BMS 4S com ESP32-S3 para monitoramento, proteção e balanceamento de baterias de lítio</h1>
  <div class="thesis-meta">
    <p><strong>Autor:</strong> Seu Nome Completo</p>
    <p><strong>Local:</strong> Sua Cidade</p>
    <p><strong>Ano:</strong> 2026</p>
    <p><strong>Orientador:</strong> Prof. Nome do Orientador</p>
    <p><strong>Coorientador:</strong> Prof. Nome do Coorientador</p>
  </div>
  <p class="thesis-preamble">Monografia apresentada ao curso de X da Y como requisito parcial para obtenção do título de Z.</p>
  <p class="thesis-institution">Nome da Universidade<br>
  Nome do Centro ou Faculdade<br>
  Nome do Curso</p>
</div>

# TCC do projeto

<<<<<<< HEAD
Esta pagina apresenta a versao web da monografia do projeto, montada a partir do template em
`abnTeX2` e dos capitulos ja preparados em `tcc-latex/`.

## Elementos pre-textuais

### Resumo

Este trabalho apresenta a proposta de desenvolvimento de um sistema de gerenciamento de baterias
para um pack de lítio em configuração *4S*, utilizando um microcontrolador ESP32-S3 como
elemento de supervisão, telemetria e controle de alto nível. A pesquisa aborda os requisitos de
proteção do pack, a necessidade do uso de um *Analog Front End* para medição segura das
células, a definição da máquina de estados da BMS e a organização do firmware para tratamento de
falhas, balanceamento passivo e configuração do sistema. A metodologia combina revisão bibliográfica,
análise de arquiteturas de mercado, modelagem da solução e estruturação de uma base inicial de
firmware e documentação técnica. Como resultado, propõe-se uma arquitetura coerente para uma BMS
4S com foco em proteção, expansibilidade e futura implementação em placa dedicada.
=======
Esta página apresenta uma versão web da monografia do projeto, organizada para leitura direta no portal e alinhada ao material em `abnTeX2` disponível em `tcc-latex/`.

## Elementos pré-textuais

### Resumo

Este trabalho apresenta a proposta de desenvolvimento de um sistema de gerenciamento de baterias para um pack de lítio em configuração `4S`, utilizando um microcontrolador `ESP32-S3` como elemento de supervisão, telemetria e controle de alto nível. A pesquisa aborda os requisitos de proteção do pack, a necessidade do uso de um `Analog Front End` para medição segura das células, a definição da máquina de estados da BMS e a organização do firmware para tratamento de falhas, balanceamento passivo e configuração do sistema. A metodologia combina revisão bibliográfica, análise de arquiteturas de mercado, modelagem da solução e estruturação de uma base inicial de firmware e documentação técnica. Como resultado, propõe-se uma arquitetura coerente para uma BMS `4S` com foco em proteção, expansibilidade e futura implementação em placa dedicada.
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

**Palavras-chave**: BMS. ESP32-S3. AFE. Bateria de lítio. Balanceamento. Proteção.

### Abstract

<<<<<<< HEAD
This work presents the development proposal of a battery management system for a *4S*
lithium battery pack, using an ESP32-S3 microcontroller as the high-level supervision, telemetry,
and control element. The study addresses pack protection requirements, the need for an
*Analog Front End* for safe cell measurement, the definition of the BMS state machine,
and the firmware organization for fault handling, passive balancing, and system configuration.
The methodology combines bibliographic review, analysis of market architectures, solution modeling,
and the structuring of an initial firmware and technical documentation base. As a result, a
coherent architecture for a 4S BMS is proposed, focused on protection, expandability, and future
implementation on a dedicated printed circuit board.

**Keywords**: BMS. ESP32-S3. AFE. Lithium battery. Balancing. Protection.

## Sumario

1. [Introdução](#introduo)
1. [Fundamentação teórica](#fundamentao-terica)
1. [Requisitos e metodologia do projeto](#requisitos-e-metodologia-do-projeto)
1. [Arquitetura da BMS 4S](#arquitetura-da-bms-4s)
1. [Desenvolvimento do sistema](#desenvolvimento-do-sistema)
1. [Resultados e discussão](#resultados-e-discusso)
1. [Conclusão](#concluso)

## Texto completo

<a id="introduo"></a>
=======
This work presents the development proposal of a battery management system for a `4S` lithium battery pack, using an `ESP32-S3` microcontroller as the high-level supervision, telemetry, and control element. The study addresses pack protection requirements, the need for an `Analog Front End` for safe cell measurement, the definition of the BMS state machine, and the firmware organization for fault handling, passive balancing, and system configuration. The methodology combines bibliographic review, analysis of market architectures, solution modeling, and the structuring of an initial firmware and technical documentation base. As a result, a coherent architecture for a `4S` BMS is proposed, focused on protection, expandability, and future implementation on a dedicated printed circuit board.

**Keywords**: BMS. ESP32-S3. AFE. Lithium battery. Balancing. Protection.

## Sumário

1. [Introdução](#introducao)
2. [Fundamentação teórica](#fundamentacao-teorica)
3. [Requisitos e metodologia do projeto](#requisitos-e-metodologia-do-projeto)
4. [Arquitetura da BMS 4S](#arquitetura-da-bms-4s)
5. [Desenvolvimento do sistema](#desenvolvimento-do-sistema)
6. [Resultados e discussão](#resultados-e-discussao)
7. [Conclusão](#conclusao)
8. [Apêndice](#apendice)
9. [Anexo](#anexo)
10. [Referências](#referencias)

## Texto completo

<a id="introducao"></a>
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

## Introdução

### Contextualização

<<<<<<< HEAD
O uso de baterias de íons de lítio em sistemas embarcados, mobilidade elétrica, armazenamento
de energia e equipamentos portáteis tem crescido de forma significativa nas últimas décadas.
Com esse crescimento, aumenta também a necessidade de sistemas de gerenciamento de baterias
(*Battery Management Systems* -- BMS) capazes de monitorar, proteger e operar os packs
de forma segura  ([Davide Andrea, 2010](#ref-andrea2010); [Gregory L. Plett, 2015](#ref-plett2015v1)).

### Problema de pesquisa

Em packs multicélula, como no caso de uma bateria *4S*, a simples leitura da tensão total
do pack não é suficiente para garantir segurança operacional. É necessário monitorar cada célula
individualmente, acompanhar temperatura, corrente e estados de falha, além de aplicar estratégias
de proteção e balanceamento.

### Objetivo geral

Desenvolver a proposta de uma BMS para bateria de lítio em configuração *4S*, utilizando
um ESP32-S3 como unidade de supervisão e um *Analog Front End* dedicado para medição e
proteção primária do pack.

### Objetivos específicos

- identificar os requisitos de proteção de uma BMS para pack 4S;
- analisar o papel do AFE em sistemas de gerenciamento de baterias;
- propor a arquitetura de hardware da solução;
- estruturar a máquina de estados e a lógica de proteção do firmware;
- organizar uma base de documentação e configuração para futura implementação física.

### Justificativa

A relevância do tema se apoia na necessidade de segurança, robustez e rastreabilidade em sistemas
com células de lítio. Uma BMS mal projetada pode causar degradação precoce do pack, perda de
desempenho e, em casos severos, risco elétrico e térmico.

### Metodologia

Este trabalho adota uma abordagem de pesquisa aplicada, de natureza tecnológica, combinando:

- revisão bibliográfica sobre BMS, AFE e proteção de packs de lítio;
- análise de arquiteturas e componentes de mercado;
- modelagem da solução para uma BMS 4S;
- estruturação inicial do firmware e da documentação técnica.

### Estrutura do trabalho

Este trabalho está organizado em sete capítulos. Após esta introdução, o Capítulo 2 apresenta
a fundamentação teórica. O Capítulo 3 descreve os requisitos e a metodologia. O Capítulo 4
apresenta a arquitetura proposta. O Capítulo 5 detalha o desenvolvimento da solução. O
Capítulo 6 discute os resultados e limitações. Por fim, o Capítulo 7 apresenta a conclusão.

<a id="fundamentao-terica"></a>
=======
O uso de baterias de íons de lítio em sistemas embarcados, mobilidade elétrica, armazenamento de energia e equipamentos portáteis tem crescido de forma significativa nas últimas décadas. Com esse crescimento, aumenta também a necessidade de sistemas de gerenciamento de baterias (`Battery Management Systems` - BMS) capazes de monitorar, proteger e operar os packs de forma segura.

### Problema de pesquisa

Em packs multicélula, como no caso de uma bateria `4S`, a simples leitura da tensão total do pack não é suficiente para garantir segurança operacional. É necessário monitorar cada célula individualmente, acompanhar temperatura, corrente e estados de falha, além de aplicar estratégias de proteção e balanceamento.

### Objetivo geral

Desenvolver a proposta de uma BMS para bateria de lítio em configuração `4S`, utilizando um `ESP32-S3` como unidade de supervisão e um `Analog Front End` dedicado para medição e proteção primária do pack.

### Objetivos específicos

- Identificar os requisitos de proteção de uma BMS para pack `4S`.
- Analisar o papel do AFE em sistemas de gerenciamento de baterias.
- Propor a arquitetura de hardware da solução.
- Estruturar a máquina de estados e a lógica de proteção do firmware.
- Organizar uma base de documentação e configuração para futura implementação física.

### Justificativa

A relevância do tema se apoia na necessidade de segurança, robustez e rastreabilidade em sistemas com células de lítio. Uma BMS mal projetada pode causar degradação precoce do pack, perda de desempenho e, em casos severos, risco elétrico e térmico.

### Metodologia

Este trabalho adota uma abordagem de pesquisa aplicada, de natureza tecnológica, combinando revisão bibliográfica, análise de arquiteturas e componentes de mercado, modelagem da solução para uma BMS `4S` e estruturação inicial do firmware e da documentação técnica.

<a id="fundamentacao-teorica"></a>
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

## Fundamentação teórica

### Baterias de lítio em configuração 4S

<<<<<<< HEAD
Uma bateria em configuração *4S* possui quatro células em série. Nesse arranjo, a tensão
total do pack resulta da soma das tensões individuais das células, o que exige monitoramento
por célula para evitar condições de sobretensão e subtensão.

### Battery Management System

Um BMS é responsável por monitorar variáveis elétricas e térmicas do pack, proteger a bateria,
gerenciar carga e descarga e manter a operação dentro de limites seguros  ([Davide Andrea, 2010](#ref-andrea2010)).

### Proteção em BMS

As proteções mais relevantes em BMS incluem:

- sobretensão por célula;
- subtensão por célula;
- sobrecorrente de carga;
- sobrecorrente de descarga;
- curto-circuito;
- sobretemperatura e subtemperatura.

Segundo Andrea  ([Davide Andrea, 2010](#ref-andrea2010)), a robustez da proteção depende de uma combinação de hardware
dedicado e lógica de controle, especialmente em sistemas multicélula.

### Balanceamento de células

O balanceamento busca reduzir a divergência entre tensões individuais das células. Entre as
estratégias possíveis, o balanceamento passivo é uma das mais comuns em aplicações de menor
complexidade, por dissipar energia das células mais carregadas em resistores de bleed
 ([Gregory L. Plett, 2015](#ref-plett2015v2)).

### Analog Front End em BMS

O *Analog Front End* é o bloco responsável pela leitura segura de tensões, correntes e
temperaturas, além de frequentemente incorporar mecanismos de detecção de falhas e suporte a
balanceamento. Em sistemas reais, o AFE reduz a dependência do ADC do microcontrolador e
melhora a confiabilidade da proteção  ([Texas Instruments, 2026](#ref-ti_bq76920); [Analog Devices, 2026](#ref-ad_passive_balancing)).

### Microcontroladores na supervisão da BMS

O microcontrolador pode assumir as tarefas de:

- supervisão de alto nível;
- telemetria;
- máquina de estados;
- registro de falhas;
- comunicação com interfaces externas.

No contexto deste trabalho, o ESP32-S3 foi escolhido por sua capacidade de processamento,
interfaces de comunicação e potencial de conectividade sem fio.
=======
Uma bateria em configuração `4S` possui quatro células em série. Nesse arranjo, a tensão total do pack resulta da soma das tensões individuais das células, o que exige monitoramento por célula para evitar condições de sobretensão e subtensão.

### Battery Management System

Um BMS é responsável por monitorar variáveis elétricas e térmicas do pack, proteger a bateria, gerenciar carga e descarga e manter a operação dentro de limites seguros. Em aplicações multicélula, esse conjunto de funções passa a ser crítico para a segurança do sistema e para a vida útil do armazenamento eletroquímico.

### Proteção em BMS

As proteções mais relevantes em uma BMS incluem sobretensão por célula, subtensão por célula, sobrecorrente de carga, sobrecorrente de descarga, curto-circuito, sobretemperatura e subtemperatura. A robustez dessas funções depende de uma combinação entre hardware dedicado, supervisão por firmware e estratégia clara de falha segura.

### Balanceamento de células

O balanceamento busca reduzir a divergência entre tensões individuais das células. Entre as estratégias possíveis, o balanceamento passivo é uma das mais comuns em aplicações de menor complexidade, por dissipar energia das células mais carregadas em resistores de bleed. Essa técnica é simples, barata e coerente com a escala inicial do projeto.

### Analog Front End em BMS

O `Analog Front End` é o bloco responsável pela leitura segura de tensões, correntes e temperaturas, além de frequentemente incorporar mecanismos de detecção de falhas e suporte a balanceamento. Em sistemas reais, o AFE reduz a dependência do ADC do microcontrolador e melhora a confiabilidade da proteção.

### Microcontroladores na supervisão da BMS

O microcontrolador pode assumir as tarefas de supervisão de alto nível, telemetria, máquina de estados, registro de falhas e comunicação com interfaces externas. No contexto deste trabalho, o `ESP32-S3` foi escolhido por sua capacidade de processamento, interfaces de comunicação e potencial de conectividade sem fio.
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

<a id="requisitos-e-metodologia-do-projeto"></a>

## Requisitos e metodologia do projeto

### Requisitos funcionais

<<<<<<< HEAD
Para a proposta da BMS 4S, consideram-se como requisitos funcionais:

- leitura da tensão individual das quatro células;
- leitura de corrente do pack;
- leitura de temperatura por sensores NTC;
- controle independente dos caminhos de carga e descarga;
- suporte a balanceamento passivo;
- disponibilização de telemetria do pack.
=======
Para a proposta da BMS `4S`, consideram-se como requisitos funcionais:

- Leitura da tensão individual das quatro células.
- Leitura de corrente do pack.
- Leitura de temperatura por sensores `NTC`.
- Controle independente dos caminhos de carga e descarga.
- Suporte a balanceamento passivo.
- Disponibilização de telemetria do pack.
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

### Requisitos de segurança

Como requisitos de segurança, destacam-se:

<<<<<<< HEAD
- bloqueio por sobretensão;
- bloqueio por subtensão;
- bloqueio por sobrecorrente;
- resposta segura em condição de curto-circuito;
- bloqueio por sobretemperatura;
- comportamento *fail-safe* em caso de falha crítica.

### Parâmetros de configuração

Os parâmetros iniciais do sistema são organizados em uma estrutura de configuração contendo:

- características do pack;
- parâmetros de medição;
- limites de proteção;
- estratégia de balanceamento;
- convenções de telemetria.

### Metodologia de desenvolvimento

A metodologia adotada foi composta por:

1. levantamento bibliográfico;
1. análise de AFEs e arquiteturas de mercado;
1. definição dos requisitos do sistema;
1. modelagem da arquitetura de hardware;
1. modelagem da máquina de estados;
1. implementação da base inicial de firmware.
=======
- Bloqueio por sobretensão.
- Bloqueio por subtensão.
- Bloqueio por sobrecorrente.
- Resposta segura em condição de curto-circuito.
- Bloqueio por sobretemperatura.
- Comportamento `fail-safe` em caso de falha crítica.

### Parâmetros de configuração

Os parâmetros iniciais do sistema são organizados em uma estrutura de configuração contendo características do pack, parâmetros de medição, limites de proteção, estratégia de balanceamento, convenções de telemetria e controle de saídas. No projeto, essa camada está representada por um arquivo JSON versionado, o que favorece rastreabilidade entre requisitos, firmware e validação.

### Metodologia de desenvolvimento

A metodologia adotada foi composta por levantamento bibliográfico, análise de AFEs e arquiteturas de mercado, definição dos requisitos do sistema, modelagem da arquitetura de hardware, modelagem da máquina de estados e implementação da base inicial de firmware.
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

<a id="arquitetura-da-bms-4s"></a>

## Arquitetura da BMS 4S

<<<<<<< HEAD
### Visão geral do sistema

A arquitetura proposta divide a solução em três camadas principais:

- camada de medição e proteção primária;
- camada de controle e supervisão;
- camada de potência e comutação.

### Arquitetura de hardware

Os blocos de hardware propostos são:

- pack de células 4S;
- AFE dedicado;
- resistor shunt para medição de corrente;
- sensores NTC;
- MOSFET de carga;
- MOSFET de descarga;
- resistores de balanceamento;
- fusível.

### Papel do AFE

O AFE ocupa posição central na arquitetura, pois fornece medição multicélula, suporte à proteção
e interface segura com o microcontrolador. Para uma arquitetura 4S, famílias como a BQ76920
apresentam bom alinhamento com os requisitos de medição e proteção  ([Texas Instruments, 2026](#ref-ti_bq76920)).

### Papel do ESP32-S3

O ESP32-S3 foi definido como controlador de alto nível, responsável por:

- ler a telemetria consolidada;
- executar a máquina de estados;
- tratar falhas;
- controlar os caminhos de potência;
- expor interfaces de monitoramento e configuração.

### Estágio de potência

O estágio de potência prevê caminhos distintos para carga e descarga. Essa separação facilita
o bloqueio seletivo de eventos de falha conforme o tipo de condição detectada.

### Sensoriamento de corrente e temperatura

O sensoriamento de corrente é realizado por resistor shunt, enquanto a monitoração térmica
utiliza, no mínimo, dois termistores NTC: um próximo às células e outro próximo ao estágio
de potência.
=======
### Visão em blocos

A arquitetura proposta separa o sistema em camadas claras:

- Pack de baterias `4S` como elemento de armazenamento.
- AFE dedicado para medições por célula e suporte a proteção rápida.
- Shunt para medição de corrente.
- Sensores `NTC` para monitoramento térmico.
- MOSFETs de carga e descarga para isolamento controlado.
- `ESP32-S3` como supervisor e núcleo de telemetria.

### Justificativa arquitetural

A principal decisão de engenharia do projeto é não depender do ADC do `ESP32-S3` para medir diretamente os taps das quatro células. Em vez disso, o microcontrolador atua como orquestrador, enquanto o AFE executa a medição crítica com melhor isolamento funcional e com recursos próprios para apoio à segurança.

### Proteção em camadas

A arquitetura adota o princípio de proteção em camadas:

- Camada de hardware para reação rápida a falhas severas.
- Camada de AFE para detecção confiável de grandezas analógicas.
- Camada de firmware para regras de estado, histerese, telemetria e bloqueio coordenado.
- Camada documental para rastreabilidade de requisitos, configuração e testes.
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

<a id="desenvolvimento-do-sistema"></a>

## Desenvolvimento do sistema

### Estrutura do firmware

<<<<<<< HEAD
O firmware foi organizado com separação entre tipos, configuração, monitoramento de bateria e
lógica de controle. Essa divisão favorece manutenção, evolução e futura substituição do modo
*mock* por drivers reais de hardware.

### Máquina de estados

A máquina de estados foi definida com os seguintes estados:

- *Startup*;
- *Idle*;
- *Charging*;
- *Discharging*;
- *Balancing*;
- *Fault*.

Essa modelagem torna explícita a relação entre telemetria, falhas, saídas e transições do
sistema.

### Lógica de proteção

As proteções tratadas na base inicial do firmware incluem:

- sobretensão por célula;
- subtensão por célula;
- sobrecorrente de carga;
- sobrecorrente de descarga;
- bloqueio térmico para carga e descarga;
- falha de AFE.

### Balanceamento

O projeto adota, em sua primeira versão, balanceamento passivo. A lógica considera limiar de
início de balanceamento e delta mínimo entre a célula de maior e a de menor tensão.

### Estrutura de configuração em JSON

Os parâmetros do sistema foram organizados em arquivo JSON para permitir rastreabilidade e futura
ligação com configuração persistente do firmware. Essa estrutura inclui parâmetros do pack,
medição, proteção, balanceamento, saídas e telemetria.

<a id="resultados-e-discusso"></a>

## Resultados e discussão

### Cenários avaliados

Nesta etapa inicial, os resultados concentram-se na modelagem e na estruturação do sistema.
Os principais cenários considerados foram:

- carga com pack saudável;
- descarga com pack saudável;
- condição de desbalanceamento;
- sobretensão por célula;
- subtensão por célula;
- condição térmica crítica.

### Resultados obtidos

Como resultado do desenvolvimento, foram obtidos:

- definição de arquitetura para uma BMS 4S;
- estrutura de configuração do sistema;
- documentação técnica da proteção e do AFE;
- modelagem da máquina de estados;
- base inicial de firmware para futura integração com hardware.

### Análise crítica

Embora a base proposta seja consistente para a etapa de projeto, a solução ainda depende de:

- escolha final do AFE;
- esquemático completo;
- implementação do driver real do hardware;
- validação experimental em bancada;
- validação de proteção rápida em condição de falha.

<a id="concluso"></a>

## Conclusão

Este trabalho organizou a base conceitual, arquitetural e de firmware para o desenvolvimento de
uma BMS 4S com ESP32-S3. A análise mostrou que o uso de um AFE dedicado é essencial para medição
segura das células e para uma estratégia de proteção mais robusta.

A proposta estruturada integra:

- requisitos de proteção;
- arquitetura de hardware;
- máquina de estados;
- modelo inicial de firmware;
- rastreabilidade de configuração e documentação.

Como trabalhos futuros, destacam-se:

- escolha final do AFE;
- desenvolvimento do esquemático e da PCB;
- implementação do driver de comunicação com o AFE;
- testes em bancada com fonte limitada;
- expansão da telemetria e da validação experimental.

## Apendice

## Checklist para adaptação institucional

Antes da entrega final, recomenda-se validar:

- modelo de capa da universidade;
- exigência de folha de aprovação;
- ficha catalográfica;
- regras próprias para resumo, abstract e listas;
- padrão institucional para referências e citações.

## Anexo

## Estrutura atual da documentação do projeto
=======
O firmware foi organizado para suportar leitura de sinais, avaliação de limites, decisão por máquina de estados, balanceamento e telemetria. A base do projeto também inclui `structs` para representar estados da BMS, contexto operacional e métricas agregadas do ciclo de supervisão.

### Máquina de estados

A máquina de estados modela estados como inicialização, inatividade segura, operação normal, balanceamento e falha. O projeto dá prioridade às condições de risco, de forma que um evento crítico interrompa a progressão normal e leve o sistema para comportamento seguro.

### Configuração do sistema

A configuração da BMS foi consolidada em um arquivo `JSON` com parâmetros de pack, medição, proteção, balanceamento, saídas e telemetria. Essa escolha facilita futuras integrações com firmware, testes automatizados e calibração de bancada.

### Documentação técnica e conhecimento

Além dos arquivos de implementação, o projeto reúne uma camada documental composta por páginas técnicas, base de conhecimento relacional, mapas do Obsidian e material acadêmico. Isso transforma o repositório em um ambiente de desenvolvimento e de produção científica ao mesmo tempo.

<a id="resultados-e-discussao"></a>

## Resultados e discussão

### Resultados obtidos até o momento

Como resultado atual, o projeto dispõe de:

- Arquitetura inicial da BMS `4S` documentada.
- Estratégias de proteção descritas em nível de sistema.
- Documento específico sobre `AFE` aplicado a BMS.
- Máquina de estados formalizada.
- Configuração-base em `JSON`.
- Template acadêmico em `LaTeX` com `abnTeX2`.
- Portal web publicado no GitHub Pages.

### Limitações da etapa atual

Apesar do avanço estrutural, esta fase ainda não contempla validação experimental em hardware real, ensaios de bancada instrumentados, seleção final de um AFE específico e fabricação de uma placa dedicada. Portanto, os resultados devem ser lidos como proposta de engenharia e base organizada para implementação futura.

### Potencial de continuidade

O trabalho abre espaço para continuidade em várias frentes:

- Escolha e integração do AFE definitivo.
- Projeto esquemático e PCB da BMS.
- Ensaios elétricos e térmicos.
- Ajuste fino dos limiares de proteção.
- Expansão da telemetria local ou remota.
- Consolidação do texto acadêmico para defesa formal.

<a id="conclusao"></a>

## Conclusão

O desenvolvimento documental e arquitetural desta BMS `4S` com `ESP32-S3` demonstrou que a combinação entre supervisor digital, AFE dedicado, proteção em camadas e organização de conhecimento oferece uma base consistente para um sistema de gerenciamento de baterias seguro e evolutivo. A proposta construída neste projeto não encerra a pesquisa em um protótipo final, mas estabelece uma estrutura sólida para transição entre estudo, documentação, firmware e implementação física. Como próximo passo natural, recomenda-se selecionar o AFE real do sistema, concluir o esquemático elétrico e conduzir validações experimentais de bancada.

<a id="apendice"></a>

## Apêndice

### Checklist para adaptação institucional

Antes da entrega final, recomenda-se validar:

- Modelo de capa da universidade.
- Exigência de folha de aprovação.
- Ficha catalográfica.
- Regras próprias para resumo, abstract e listas.
- Padrão institucional para referências e citações.

<a id="anexo"></a>

## Anexo

### Estrutura atual da documentação do projeto
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74

Os documentos técnicos do projeto podem ser usados como base para a redação do TCC:

- `docs/arquitetura-bms-4s.md`
- `docs/protecao-bms.md`
- `docs/maquina-de-estados-bms.md`
- `docs/afe-em-bms.md`
- `config/bms_config.json`

<<<<<<< HEAD
## Referencias

- <a id="ref-andrea2010"></a>`andrea2010`: Davide Andrea *Battery Management Systems for Large Lithium-Ion Battery Packs* Artech House, Norwood, MA, 2010
- <a id="ref-plett2015v1"></a>`plett2015v1`: Gregory L. Plett *Battery Management Systems, Volume I: Battery Modeling* Artech House, Norwood, MA, 2015
- <a id="ref-plett2015v2"></a>`plett2015v2`: Gregory L. Plett *Battery Management Systems, Volume II: Equivalent-Circuit Methods* Artech House, Norwood, MA, 2015
- <a id="ref-warner2024"></a>`warner2024`: John T. Warner *The Handbook of Lithium-Ion Battery Pack Design: Chemistry, Components, Types, and Terminology* 2 ed., Elsevier, Oxford, 2024
- <a id="ref-ti_bq76920"></a>`ti_bq76920`: Texas Instruments *BQ76920 product page and technical documentation* 2026 Acesso em: 16 abr. 2026 https://www.ti.com/product/BQ76920
- <a id="ref-nxp_mc33771c"></a>`nxp_mc33771c`: NXP Semiconductors *MC33771C product page and technical documentation* 2026 Acesso em: 16 abr. 2026 https://www.nxp.com/products/MC33771C
- <a id="ref-adi_ltc6811"></a>`adi_ltc6811`: Analog Devices *LTC6811-1 product page and datasheet* 2026 Acesso em: 16 abr. 2026 https://www.analog.com/en/products/ltc6811-1.html
- <a id="ref-ad_passive_balancing"></a>`ad_passive_balancing`: Analog Devices *Passive Battery Cell Balancing* 2026 Acesso em: 16 abr. 2026 https://www.analog.com/en/technical-articles/passive-battery-cell-balancing.html
- <a id="ref-ti_protector_monitor_gauge"></a>`ti_protector_monitor_gauge`: Texas Instruments *Protector, Monitor or Gauge -- Selecting the Correct Battery Electronics for Your Li-ion-powered System* 2026 Acesso em: 16 abr. 2026 https://www.ti.com/document-viewer/ja-jp/lit/html/SSZT466
- <a id="ref-ctan_abntex2"></a>`ctan_abntex2`: CTAN *abntex2 -- Typeset technical and scientific Brazilian documents based on ABNT rules* 2026 Acesso em: 16 abr. 2026 https://ctan.org/pkg/abntex2?lang=en

## Arquivos de origem

- [Template principal em LaTeX](../assets/tcc-latex/main.tex)
- [Bibliografia BibTeX](../assets/tcc-latex/referencias.bib)
- [Guia de TCC e ABNT](monografia-tcc-abnt.md)
=======
## Referências

- ANDREA, Davide. *Battery Management Systems for Large Lithium-Ion Battery Packs*. Norwood: Artech House, 2010.
- PLETT, Gregory L. *Battery Management Systems, Volume I: Battery Modeling*. Norwood: Artech House, 2015.
- PLETT, Gregory L. *Battery Management Systems, Volume II: Equivalent-Circuit Methods*. Norwood: Artech House, 2015.
- WARNER, John T. *The Handbook of Lithium-Ion Battery Pack Design*. Oxford: Elsevier, 2024.
- TEXAS INSTRUMENTS. *BQ76920 product page and technical documentation*. Disponível em: <https://www.ti.com/product/BQ76920>. Acesso em: 17 abr. 2026.
- ANALOG DEVICES. *LTC6811-1 product page and technical documentation*. Disponível em: <https://www.analog.com/en/products/ltc6811-1.html>. Acesso em: 17 abr. 2026.
- NXP. *MC33771C product page and technical documentation*. Disponível em: <https://www.nxp.com/products/MC33771C>. Acesso em: 17 abr. 2026.

## Arquivos-fonte

- [Template completo em LaTeX](template-latex.md)
- [main.tex](../assets/tcc-latex/main.tex)
- [referencias.bib](../assets/tcc-latex/referencias.bib)
- [Arquitetura BMS 4S](../projeto/arquitetura-bms-4s.md)
- [AFE em BMS](../projeto/afe-em-bms.md)
- [Técnicas de proteção](../projeto/protecao-bms.md)
>>>>>>> 6cf98340600e18ed16a082d4adb388ea81806f74
