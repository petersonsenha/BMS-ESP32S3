# DoR e DoD do Projeto

## Objetivo

Esta pagina consolida dois artefatos de governanca do projeto:

- `DoR`: `Definition of Ready`
- `DoD`: `Definition of Done`

A ideia e criar um criterio simples para decidir:

- quando uma atividade pode comecar
- quando uma entrega pode ser considerada realmente concluida

No final, ha um `dashboard` com o estado atual do projeto e um `prompt` reutilizavel para
avaliar o que falta, o que precisa melhorar e como fazer isso.

## Dashboard executivo

<div class="d3-dashboard-grid">
  <section class="d3-panel">
    <h3>Status geral</h3>
    <p>Distribuicao macro do projeto entre entregas concluidas, em andamento e pendentes.</p>
    <div
      class="d3-status-donut"
      data-chart='{
        "title": "Estado atual do projeto",
        "items": [
          {"label": "Concluido", "value": 3, "color": "#0f766e"},
          {"label": "Em andamento", "value": 6, "color": "#f59e0b"},
          {"label": "Pendente", "value": 5, "color": "#dc2626"}
        ]
      }'
    ></div>
  </section>
  <section class="d3-panel">
    <h3>Maturidade por area</h3>
    <p>Painel resumido para localizar rapidamente onde o projeto esta mais maduro e onde ainda ha risco.</p>
    <div
      class="d3-maturity-bars"
      data-chart='{
        "title": "Maturidade por area",
        "items": [
          {"label": "Site de documentacao", "score": 100, "status": "Concluido"},
          {"label": "Documentacao tecnica", "score": 74, "status": "Em andamento"},
          {"label": "Firmware base", "score": 84, "status": "Em andamento"},
          {"label": "Estimativa de estado", "score": 58, "status": "Em andamento"},
          {"label": "Medicao de corrente", "score": 52, "status": "Em andamento"},
          {"label": "Balanceamento", "score": 58, "status": "Em andamento"},
          {"label": "Escolha do AFE", "score": 20, "status": "Pendente"},
          {"label": "Esquematico eletrico", "score": 12, "status": "Pendente"},
          {"label": "PCB", "score": 5, "status": "Pendente"},
          {"label": "Ensaios de bancada", "score": 0, "status": "Pendente"}
        ]
      }'
    ></div>
  </section>
</div>

## Mapa de modulos

<div class="d3-panel d3-panel--full">
  <h3>Modulos do projeto em Tidy tree</h3>
  <p>Estrutura do projeto mostrada como `Tidy tree` em `D3.js`, destacando modulos principais e suas dependencias de alto nivel.</p>
  <div
    class="d3-modules-tree"
    data-tree='{
      "name": "BMS-ESP32S3",
      "children": [
        {
          "name": "Hardware",
          "children": [
            {"name": "Pack 4S"},
            {"name": "AFE"},
            {"name": "Shunt"},
            {"name": "NTCs"},
            {"name": "MOSFET de carga"},
            {"name": "MOSFET de descarga"},
            {"name": "Balanceamento passivo"},
            {"name": "Fusivel"}
          ]
        },
        {
          "name": "Firmware",
          "children": [
            {"name": "Maquina de estados"},
            {"name": "Fault handling"},
            {"name": "Telemetria"},
            {"name": "Configuracao JSON"},
            {"name": "SOC e SOH"},
            {"name": "Charging e discharging"}
          ]
        },
        {
          "name": "Documentacao",
          "children": [
            {"name": "Arquitetura 4S"},
            {"name": "Funcionamento da BMS"},
            {"name": "AFE em BMS"},
            {"name": "Tecnicas de protecao"},
            {"name": "DoR e DoD"}
          ]
        },
        {
          "name": "Academico",
          "children": [
            {"name": "TCC web"},
            {"name": "Template LaTeX"},
            {"name": "ABNT"}
          ]
        },
        {
          "name": "Validacao",
          "children": [
            {"name": "Modo mock"},
            {"name": "Escolha do AFE"},
            {"name": "Esquematico"},
            {"name": "PCB"},
            {"name": "Ensaios de bancada"}
          ]
        }
      ]
    }'
  ></div>
</div>

<div class="grid cards" markdown>

- __Concluido__

  ---

  Site em `GitHub Pages`, base de documentacao tecnica e trilha academica inicial.

- __Em andamento__

  ---

  Arquitetura da BMS, maquina de estados, configuracao JSON, TCC web e aprofundamento tecnico.

- __Pendente__

  ---

  Escolha final do `AFE`, esquematico, PCB, driver real de hardware, testes de bancada e validacao.

</div>

## Dashboard detalhado

| Area | Estado | Observacao |
| --- | --- | --- |
| Arquitetura funcional da BMS | em andamento | Estrutura base definida, mas ainda pode evoluir com o AFE real |
| Escolha do AFE | pendente | Falta fechar o componente definitivo da placa |
| Estagio de potencia | pendente | Ainda falta detalhar gate drive, MOSFETs e protecoes rapidas |
| Medicao de corrente | em andamento | Conceito com `shunt` definido, mas sem front-end final validado |
| Firmware base | em andamento | Ja ha histerese, latch de falha, `SCD`, `Precharge`, snapshot estendido e config gerada por JSON, mas ainda sem AFE final validado |
| Diagnostico e logs | em andamento | Ja existe historico circular de eventos em memoria, mas o log persistente ainda nao foi fechado |
| Balanceamento | em andamento | Estrategia passiva ficou mais madura, ainda sem hardware validado |
| Estimativa de estado | em andamento | Ja existe base de `coulomb counting + OCV`, ainda sem calibracao de bancada |
| Documentacao tecnica | em andamento | Boa base criada, mas ainda faltam mais diagramas e rastreabilidade |
| TCC / monografia | em andamento | Estrutura pronta, mas o conteudo ainda precisa amadurecer |
| Site de documentacao | concluido | Publicado e navegavel |
| Esquematico eletrico | pendente | Ainda nao foi fechado |
| PCB | pendente | Depende do esquematico |
| Ensaios de bancada | pendente | Ainda nao iniciados |
| Validacao final | pendente | Depende de hardware, testes e ajustes |

## Definition of Ready

Uma atividade so deve entrar em implementacao quando atender o `DoR`.

### Checklist de Ready

- objetivo da atividade esta claro
- o escopo esta delimitado
- existe criterio tecnico para decidir a solucao
- requisitos e limites relevantes estao documentados
- dependencias tecnicas foram identificadas
- entradas necessarias ja estao disponiveis
- risco principal foi identificado
- impacto em hardware, firmware e documentacao foi pensado
- saida esperada da atividade esta definida
- existe um criterio de verificacao minima

### DoR para hardware

Uma tarefa de hardware esta `Ready` quando:

- o bloco eletrico foi definido
- a funcao no sistema esta clara
- tensao, corrente e temperatura esperadas sao conhecidas
- interfaces com `AFE`, `ESP32-S3` e potencia foram mapeadas
- referencias de componentes ja foram levantadas
- criterios de seguranca foram listados

### DoR para firmware

Uma tarefa de firmware esta `Ready` quando:

- entradas e saidas estao identificadas
- estados e transicoes envolvidos estao claros
- limites de protecao ou configuracao estao definidos
- dados de hardware que o codigo precisa consumir ja sao conhecidos
- existe estrategia de teste ou simulacao

### DoR para documentacao e TCC

Uma tarefa de documentacao esta `Ready` quando:

- o publico alvo esta claro
- o objetivo do texto esta definido
- referencias base ja foram encontradas
- a estrutura da pagina ou capitulo esta decidida
- o artefato que sera atualizado esta identificado

## Definition of Done

Uma atividade so deve ser considerada concluida quando atender o `DoD`.

### Checklist de Done

- entrega principal foi produzida
- criterio tecnico da tarefa foi atendido
- resultado foi revisado contra os requisitos
- riscos conhecidos foram tratados ou registrados
- documentacao relacionada foi atualizada
- links, diagramas e arquivos associados foram revisados
- configuracoes e parametros impactados foram alinhados
- pendencias residuais estao registradas
- proximo passo ficou claro

### DoD para hardware

Uma entrega de hardware esta `Done` quando:

- esquematico do bloco foi fechado
- componentes principais foram escolhidos
- interfaces eletricas foram revisadas
- protecoes relevantes foram consideradas
- limites e observacoes criticas ficaram documentados

### DoD para firmware

Uma entrega de firmware esta `Done` quando:

- logica principal foi implementada
- estados e falhas previstos foram tratados
- configuracao relacionada foi refletida no sistema
- simulacao, `mock` ou teste equivalente foi executado
- trilha minima de diagnostico ou eventos foi disponibilizada
- documentacao tecnica do comportamento foi atualizada

### DoD para documentacao e TCC

Uma entrega de documentacao esta `Done` quando:

- texto esta coerente e navegavel
- diagramas e figuras relevantes foram incluidos
- referencias estao ligadas ao conteudo
- pagina entrou no fluxo do site
- relacao com o restante do projeto ficou clara

## Aplicacao pratica no projeto

### O que hoje ja esta pronto o suficiente para seguir

- arquitetura geral da BMS `4S`
- ideia de usar `AFE + ESP32-S3`
- base de protecao em camadas
- maquina de estados
- configuracao inicial em `JSON`
- site de documentacao
- diagnostico em memoria por eventos

### O que ainda nao deveria entrar como "feito"

- escolha final do `AFE`
- integracao real com hardware
- definicao completa de `MOSFETs`, `shunt`, `NTCs` e alimentacao
- esquematico fechado
- PCB
- ensaio de carga, descarga e falha
- calibracao de estimativa de estado
- log persistente para analise de falha e bancada

## Como usar esta pagina no dia a dia

### Antes de comecar uma atividade

Pergunte:

- a tarefa esta `Ready` ou ainda falta contexto?

### Antes de encerrar uma atividade

Pergunte:

- a entrega esta realmente `Done` ou so parcialmente adiantada?

### Como usar com o restante da documentacao

Cruze esta pagina com:

- [docs/arquitetura-bms-4s.md](/C:/Users/peter/Documents/GitHub/BMS-ESP32S3/docs/arquitetura-bms-4s.md)
- [docs/funcionamento-da-bms.md](/C:/Users/peter/Documents/GitHub/BMS-ESP32S3/docs/funcionamento-da-bms.md)
- [docs/afe-em-bms.md](/C:/Users/peter/Documents/GitHub/BMS-ESP32S3/docs/afe-em-bms.md)
- [docs/maquina-de-estados-bms.md](/C:/Users/peter/Documents/GitHub/BMS-ESP32S3/docs/maquina-de-estados-bms.md)
- [config/bms_config.json](/C:/Users/peter/Documents/GitHub/BMS-ESP32S3/config/bms_config.json)

## Prompt de acompanhamento

Use o prompt abaixo sempre que quiser revisar o projeto com foco em maturidade de entrega:

```text
Revise o projeto BMS-ESP32S3 com base na pagina de DoR e DoD.

Quero que voce avalie 4 coisas:
1. O que ainda precisa ser feito para o projeto ficar tecnicamente mais completo.
2. O que ainda nao atende o criterio de Done.
3. O que precisa melhorar em arquitetura, hardware, firmware, documentacao e TCC.
4. Como executar essas melhorias, em ordem priorizada e com passos praticos.

Considere especialmente:
- escolha do AFE
- charging e discharging
- balanceamento passivo e ativo
- coulomb counting e estimativa de estado
- SOH, SOF, SOC, SOE e SOP
- esquematico, validacao e testes de bancada

Retorne em formato de dashboard:
- Concluido
- Em andamento
- Pendente

Depois liste:
- faltas criticas
- melhorias recomendadas
- plano de execucao sugerido
```
