<div class="hero">
  <p class="hero-eyebrow">BMS-ESP32S3</p>
  <h1>Documentacao online da BMS 4S com ESP32-S3</h1>
  <p class="hero-lead">
    Um portal unico para arquitetura, protecoes, AFE, maquina de estados, base de conhecimento
    e trilha academica do projeto.
  </p>
  <div class="hero-actions">
    <a class="md-button md-button--primary" href="projeto/visao-geral/">Explorar o projeto</a>
    <a class="md-button" href="projeto/afe-em-bms/">Estudar o AFE</a>
    <a class="md-button" href="academico/tcc-do-projeto/">Ler o TCC</a>
    <a class="md-button" href="https://github.com/petersonsenha/BMS-ESP32S3">Abrir repositorio</a>
  </div>
</div>

<div class="summary-strip">
  <span>4S</span>
  <span>ESP32-S3</span>
  <span>AFE dedicado</span>
  <span>Protecao em camadas</span>
  <span>GitHub Pages</span>
</div>

## O que voce encontra aqui

<div class="grid cards" markdown>

- :material-battery-high: __Projeto__

  ---

  A base tecnica da BMS: visao geral, hardware necessario, configuracao e caminho de firmware.

  [Abrir secao](projeto/visao-geral.md)

- :material-shield-check: __Protecoes__

  ---

  Sobretensao, subtensao, sobrecorrente, curto, temperatura e fail-safe tratados como arquitetura.

  [Ver tecnicas](projeto/protecao-bms.md)

- :material-chip: __AFE em foco__

  ---

  O papel do front-end analogico, criterios de selecao, exemplos e referencias para packs 4S.

  [Ler documentacao](projeto/afe-em-bms.md)

- :material-state-machine: __Controle e estados__

  ---

  Maquina de estados, contexto de ciclo, condicoes de falha e comportamento seguro da BMS.

  [Entrar no fluxo](projeto/maquina-de-estados-bms.md)

- :material-graph-outline: __Base de conhecimento__

  ---

  A camada relacional do projeto, com MOCs, rastreabilidade, Obsidian e mapas de conhecimento.

  [Abrir conhecimento](conhecimento/visao-geral.md)

- :material-source-branch: __Publicacao__

  ---

  Como o portal e gerado, validado e publicado automaticamente no GitHub Pages.

  [Ver deploy](projeto/publicacao-github-pages.md)

- :material-school: __Trilha academica__

  ---

  Monografia web do projeto, guia ABNT e template LaTeX usando abnTeX2.

  [Abrir TCC](academico/tcc-do-projeto.md)

</div>

## Fluxo recomendado

1. Comece por [Visao geral do projeto](projeto/visao-geral.md).
2. Em seguida, passe por [Arquitetura BMS 4S](projeto/arquitetura-bms-4s.md) e [Tecnicas de protecao](projeto/protecao-bms.md).
3. Se o foco for hardware, aprofunde em [AFE em BMS](projeto/afe-em-bms.md) e no [Esquematico logico](conhecimento/hardware/esquematico-logico-bms-4s.md).
4. Se o foco for firmware, siga para [Maquina de estados](projeto/maquina-de-estados-bms.md) e [Configuracao JSON](projeto/configuracao-json.md).
5. Para os mapas relacionais, abra [Mapas relacionais](conhecimento/mapas-relacionais.md).
6. Para pesquisa ou TCC, siga para [TCC do projeto](academico/tcc-do-projeto.md), depois [TCC e ABNT](academico/monografia-tcc-abnt.md) e [Template LaTeX](academico/template-latex.md).

## Destaques de engenharia

- O `ESP32-S3` atua como supervisor, telemetria e logica de alto nivel.
- O `AFE` fica no caminho critico de medicao segura e suporte a protecoes rapidas.
- A documentacao esta organizada para servir tanto desenvolvimento quanto monografia.
- O site e gerado a partir dos Markdown do proprio repositorio para evitar divergencia entre codigo e documentacao.
