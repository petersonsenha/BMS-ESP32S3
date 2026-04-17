# Obsidian no projeto BMS

## Objetivo

Usar o Obsidian como camada de conhecimento do projeto, sem substituir a documentacao tecnica
linear que ja existe em `docs/`.

Neste repositorio, a separacao recomendada fica assim:

- `docs/`: documentacao formal, linear e orientada a implementacao
- `knowledge/`: camada de conhecimento orientada a relacoes, navegacao e exploracao visual

## Como abrir

No Obsidian, abra a raiz do repositorio como vault:

- `BMS-ESP32S3/`

Assim, o Obsidian consegue navegar tanto por `docs/` quanto por `knowledge/` com links internos
coerentes.

Motivo dessa escolha:

- links internos do Obsidian sao locais ao vault
- o Obsidian recomenda evitar vaults dentro de vaults
- os mapas `.canvas` conseguem referenciar os arquivos do projeto com caminhos consistentes

## Estrutura sugerida

A estrutura criada neste repositorio segue esta ideia:

- `00-MOCs/`: paginas centrais de navegacao
- `10-Conceitos/`: conceitos-base do dominio
- `20-Hardware/`: blocos e componentes de hardware
- `30-Protecoes/`: tecnicas e modos de protecao
- `40-Firmware/`: comportamento do software e maquina de estados
- `50-Testes/`: conhecimento de validacao
- `90-Canvas/`: mapas visuais e mapas relacionais
- `99-Templates/`: modelos para novas notas

## Como o conhecimento se conecta

O vault usa quatro mecanismos principais do Obsidian:

1. links internos ``Nota``
2. `Canvas` para mapas espaciais
3. `Graph view` para relacoes entre notas
4. `Properties` no frontmatter para classificar e filtrar

## Convencao recomendada para novas notas

Toda nota nova deve:

1. usar o template em `knowledge/99-Templates/Template-Nota.md`
2. apontar para pelo menos uma nota existente com ``link interno``
3. ser ligada a um MOC
4. evitar duplicar a documentacao formal quando a ideia for melhor como referencia visual

## Como usar com a documentacao atual

Fluxo recomendado:

1. escrever a regra tecnica principal em `docs/`
2. criar ou atualizar uma nota correspondente em `knowledge/`
3. conectar a nota ao MOC certo
4. atualizar um `.canvas` quando a relacao ficar importante para arquitetura

## Arquivos iniciais criados

O vault ja comeca com:

- MOCs para BMS, hardware, protecoes, firmware e testes
- um `Start-Here` para onboarding rapido
- indices por componente e por requisito
- indice por configuracao
- matriz de rastreabilidade entre JSON, requisitos e implementacao
- requisitos rastreaveis ligados a hardware, firmware e protecao
- notas-base para os principais conceitos da BMS 4S
- `Mapa-BMS.canvas`
- `Mapa-Protecoes.canvas`
- `Mapa-Requisitos.canvas`
- `Mapa-Esquematico-Logico.canvas`

## Beneficio pratico

Essa abordagem permite:

- navegar por topicos sem perder contexto
- enxergar relacoes entre falhas, hardware e firmware
- criar mapas mentais sem sair do repositorio
- manter todo o conhecimento em Markdown e JSON versionaveis
