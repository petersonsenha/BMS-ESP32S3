# TCC e Monografia em ABNT

## Objetivo

Este arquivo serve como guia pratico para transformar a documentacao do projeto em uma
`monografia` ou `TCC` no contexto brasileiro, usando a estrutura academica baseada em `ABNT`.

Observacao importante:

- `ABNT2` nao e o nome formal de uma norma; aqui estou assumindo que voce quer a
  estruturacao academica em `ABNT`
- cada instituicao pode exigir adaptacoes proprias
- a estrutura geral hoje parte de `ABNT NBR 14724:2024`

## Base normativa recomendada

Com base nas referencias que consegui verificar online em `16/04/2026`, a base normativa mais
importante para uma `monografia` ou `TCC` e esta:

- `ABNT NBR 14724:2024` - trabalhos academicos: apresentacao
- `ABNT NBR 6024:2012` - numeracao progressiva das secoes
- `ABNT NBR 6027:2012` - sumario
- `ABNT NBR 6028:2021` - resumo
- `ABNT NBR 6023:2025` - referencias
- `ABNT NBR 10520:2023` - citacoes em documentos
- `ABNT NBR 12225:2023` - lombada, quando aplicavel

Atualizacoes relevantes:

- `ABNT NBR 14724:2024` substituiu a `NBR 14724:2011`
- `ABNT NBR 6023:2025` substituiu a `ABNT NBR 6023:2018`

Como ha variacoes institucionais e guias universitarios atualizados, confirme sempre:

- manual da sua universidade
- biblioteca do curso
- orientador(a)

## O que mudou na base mais recente

Do ponto de vista pratico, as mudancas mais relevantes na atualizacao da `ABNT NBR 14724:2024`
incluem:

- consolidacao da nova edicao da norma de apresentacao de trabalhos academicos
- orientacoes atualizadas para insercao da ficha catalografica em arquivos eletronicos
- contagem de paginas pre-textuais sem numeracao impressa
- ajustes no tratamento de epigrafes e elementos pre-textuais conforme guias universitarios atualizados

Como o texto completo das normas e pago, este guia foi montado como roteiro pratico e deve ser
conferido com o manual institucional final.

## Estrutura geral da monografia

De forma resumida, uma monografia em ABNT fica organizada em:

1. elementos pre-textuais
2. elementos textuais
3. elementos pos-textuais

## Elementos pre-textuais

### Obrigatorios

- capa
- folha de rosto
- resumo em portugues
- resumo em lingua estrangeira, quando exigido
- sumario

### Podem ser obrigatorios conforme a instituicao

- folha de aprovacao
- ficha catalografica
- lista de figuras
- lista de tabelas
- lista de abreviaturas e siglas
- lista de simbolos

### Opcionais

- dedicatoria
- agradecimentos
- epigrafe
- errata

## Elementos textuais

Os elementos textuais sao sempre os mais importantes:

- introducao
- desenvolvimento
- conclusao

Na pratica, o `desenvolvimento` e dividido em capitulos e secoes.

## Elementos pos-textuais

### Obrigatorio

- referencias

### Opcionais

- glossario
- apendice
- anexo
- indice

## Ordem recomendada da monografia

```text
Capa
Folha de rosto
Ficha catalografica (se exigida)
Folha de aprovacao
Dedicatoria (opcional)
Agradecimentos (opcional)
Epigrafe (opcional)
Resumo
Abstract
Lista de figuras
Lista de tabelas
Lista de siglas
Sumario

1 Introducao
2 Fundamentacao teorica
3 Requisitos e metodologia
4 Arquitetura da BMS
5 Desenvolvimento do sistema
6 Resultados e discussao
7 Conclusao

Referencias
Apendices
Anexos
```

## Estrutura recomendada para este projeto de BMS

Para o seu tema, a monografia pode ficar muito bem organizada assim:

### 1 Introducao

Inclua:

- contexto de baterias de litio
- problema de pesquisa
- motivacao do uso de BMS
- objetivo geral
- objetivos especificos
- metodologia do trabalho
- estrutura da monografia

### 2 Fundamentacao teorica

Inclua:

- baterias de litio e packs `4S`
- conceitos de `BMS`
- tecnicas de protecao
- balanceamento de celulas
- papel do `AFE`
- controle por microcontrolador

Base direta do projeto:

- [docs/protecao-bms.md](./protecao-bms.md)
- [docs/afe-em-bms.md](./afe-em-bms.md)

### 3 Requisitos e metodologia

Inclua:

- requisitos funcionais
- requisitos de seguranca
- limites de tensao, corrente e temperatura
- metodo de pesquisa / metodo de desenvolvimento
- criterios de escolha de componentes

Base direta do projeto:

- [config/bms_config.json](../config/bms_config.json)
- [docs/maquina-de-estados-bms.md](./maquina-de-estados-bms.md)
- [knowledge/60-Requisitos/Matriz-Rastreabilidade-Configuracao.md](../knowledge/60-Requisitos/Matriz-Rastreabilidade-Configuracao.md)

### 4 Arquitetura da BMS

Inclua:

- visao geral do sistema
- blocos de hardware
- papel do `AFE`
- `ESP32-S3` como supervisor
- estagio de potencia
- sensores e medicao

Base direta do projeto:

- [docs/arquitetura-bms-4s.md](./arquitetura-bms-4s.md)
- [knowledge/20-Hardware/Esquematico-Logico-BMS-4S.md](../knowledge/20-Hardware/Esquematico-Logico-BMS-4S.md)

### 5 Desenvolvimento do sistema

Inclua:

- firmware da BMS
- maquina de estados
- fault handling
- estrategia de balanceamento
- estrutura de configuracao em JSON

Base direta do projeto:

- [src/bms_controller.cpp](../src/bms_controller.cpp)
- [include/bms_types.h](../include/bms_types.h)
- [docs/maquina-de-estados-bms.md](./maquina-de-estados-bms.md)

### 6 Resultados e discussao

Inclua:

- resultados de simulacao, mock ou bancada
- comportamento em carga, descarga e falha
- limitacoes atuais
- comparacao com requisitos definidos

### 7 Conclusao

Inclua:

- o que foi atingido
- contribuicoes do trabalho
- limitacoes
- trabalhos futuros

## Modelo de sumario sugerido

```text
1 INTRODUCAO
1.1 Contextualizacao
1.2 Problema de pesquisa
1.3 Objetivo geral
1.4 Objetivos especificos
1.5 Justificativa
1.6 Metodologia
1.7 Estrutura do trabalho

2 FUNDAMENTACAO TEORICA
2.1 Baterias de litio e configuracao 4S
2.2 Sistemas de gerenciamento de baterias
2.3 Tecnicas de protecao em BMS
2.4 Balanceamento de celulas
2.5 Analog Front End em BMS
2.6 Microcontroladores aplicados a BMS

3 REQUISITOS E METODOLOGIA DO PROJETO
3.1 Requisitos funcionais
3.2 Requisitos de seguranca
3.3 Parametros de configuracao
3.4 Metodologia de desenvolvimento

4 ARQUITETURA DA BMS 4S
4.1 Visao geral do sistema
4.2 Arquitetura de hardware
4.3 Papel do AFE
4.4 Papel do ESP32-S3
4.5 Estagio de potencia
4.6 Sensoriamento de corrente e temperatura

5 DESENVOLVIMENTO DO SISTEMA
5.1 Estrutura do firmware
5.2 Maquina de estados
5.3 Logica de protecao
5.4 Balanceamento
5.5 Estrutura de configuracao em JSON

6 RESULTADOS E DISCUSSAO
6.1 Cenarios avaliados
6.2 Resultados obtidos
6.3 Analise critica

7 CONCLUSAO

REFERENCIAS
APENDICES
ANEXOS
```

## Regras de apresentacao grafica

Estas sao as convencoes mais usadas e amplamente aceitas quando a instituicao nao define
algo diferente. Como regra de submissao, a orientacao local da universidade prevalece:

- papel `A4`
- cor preta no texto; outras cores apenas em ilustracoes
- margem superior `3 cm`
- margem esquerda `3 cm`
- margem inferior `2 cm`
- margem direita `2 cm`
- fonte tamanho `12` no texto
- fonte menor nas citacoes longas, notas, legendas e paginacao
- espacamento `1,5` no texto
- espacamento simples em citacoes longas, notas, referencias e legendas

## Regras de numeracao e seccionamento

Use numeracao progressiva:

- `1`
- `1.1`
- `1.1.1`

Evite criar secoes profundas demais se elas nao forem realmente necessarias.

## Resumo e abstract

Para TCC/monografia, normalmente voce tera:

- `Resumo` em portugues
- `Abstract` em ingles

O resumo deve apresentar, de forma concisa:

- tema
- objetivo
- metodo
- resultado principal
- conclusao principal

## Citacoes e referencias

Para a escrita do texto:

- citacao direta curta costuma ir no proprio paragrafo
- citacao direta longa fica destacada
- citacao indireta e o formato mais comum para fundamentacao

Para referencias:

- use `ABNT NBR 6023:2025`
- organize ao final em `REFERENCIAS`
- mantenha padrao consistente

## Apendice x anexo

Use assim:

- `apendice`: material produzido por voce
- `anexo`: material produzido por terceiros

Exemplos neste projeto:

- `apendice`: tabela propria de ensaios de bancada
- `anexo`: trecho de datasheet ou norma institucional, se permitido

## Exemplo de mapeamento da documentacao atual para a monografia

| Documento atual | Onde entra melhor na monografia |
| --- | --- |
| `docs/arquitetura-bms-4s.md` | capitulo de arquitetura |
| `docs/protecao-bms.md` | fundamentacao teorica e requisitos |
| `docs/maquina-de-estados-bms.md` | desenvolvimento do firmware |
| `docs/afe-em-bms.md` | fundamentacao teorica e arquitetura |
| `config/bms_config.json` | requisitos e parametros do sistema |
| `knowledge/60-Requisitos/` | rastreabilidade de requisitos |

## Esqueleto Markdown para comecar a monografia

```md
# TITULO DO TRABALHO

## Resumo

## Abstract

# 1 Introducao
## 1.1 Contextualizacao
## 1.2 Problema de pesquisa
## 1.3 Objetivo geral
## 1.4 Objetivos especificos
## 1.5 Justificativa
## 1.6 Metodologia
## 1.7 Estrutura do trabalho

# 2 Fundamentacao teorica
## 2.1 Baterias de litio e packs 4S
## 2.2 BMS
## 2.3 Tecnicas de protecao
## 2.4 AFE em BMS
## 2.5 ESP32-S3 como supervisor

# 3 Requisitos e metodologia do projeto

# 4 Arquitetura da BMS

# 5 Desenvolvimento do sistema

# 6 Resultados e discussao

# 7 Conclusao

# Referencias
```

## Template em LaTeX no repositorio

Foi criado um modelo inicial em LaTeX com `abnTeX2` neste caminho:

- [tcc-latex/main.tex](../tcc-latex/main.tex)

Arquivos principais:

- [tcc-latex/README.md](../tcc-latex/README.md)
- [tcc-latex/capitulos/01-introducao.tex](../tcc-latex/capitulos/01-introducao.tex)
- [tcc-latex/capitulos/02-fundamentacao-teorica.tex](../tcc-latex/capitulos/02-fundamentacao-teorica.tex)
- [tcc-latex/capitulos/03-requisitos-e-metodologia.tex](../tcc-latex/capitulos/03-requisitos-e-metodologia.tex)
- [tcc-latex/capitulos/04-arquitetura-da-bms.tex](../tcc-latex/capitulos/04-arquitetura-da-bms.tex)
- [tcc-latex/capitulos/05-desenvolvimento-do-sistema.tex](../tcc-latex/capitulos/05-desenvolvimento-do-sistema.tex)
- [tcc-latex/capitulos/06-resultados-e-discussao.tex](../tcc-latex/capitulos/06-resultados-e-discussao.tex)
- [tcc-latex/capitulos/07-conclusao.tex](../tcc-latex/capitulos/07-conclusao.tex)
- [tcc-latex/referencias.bib](../tcc-latex/referencias.bib)

## Recomendacao pratica para o seu caso

Como o projeto mistura `hardware`, `firmware` e `seguranca`, o formato de monografia mais
forte para voce tende a ser:

- `Capitulo 2`: fundamentacao bem forte em BMS, protecao e AFE
- `Capitulo 3`: requisitos claros e metodologia
- `Capitulo 4`: arquitetura proposta
- `Capitulo 5`: implementacao do firmware
- `Capitulo 6`: validacao e limitacoes

Isso deixa o TCC tecnicamente coerente e mais facil de defender.

## Referencias usadas para este guia

- INTERTEK INFORM. `ABNT NBR 14724:2024` - Information and documentation - Academic work - Presentation. Status: current. Disponivel em: [https://www.intertekinform.com/en-ca/standards/abnt-nbr-14724-2024-772116_saig_nbr_nbr_3516249/](https://www.intertekinform.com/en-ca/standards/abnt-nbr-14724-2024-772116_saig_nbr_nbr_3516249/)
- INTERTEK INFORM. `NBR 14724:2011` - superseded by `ABNT NBR 14724:2024`. Disponivel em: [https://www.intertekinform.com/en-us/standards/nbr-14724-2011-772116_saig_nbr_nbr_1861171/](https://www.intertekinform.com/en-us/standards/nbr-14724-2011-772116_saig_nbr_nbr_1861171/)
- INTERTEK INFORM. `ABNT NBR 6023:2025` - Information and documentation - References - Development. Disponivel em: [https://www.intertekinform.com/en-us/standards/abnt-nbr-6023-2025-779112_saig_nbr_nbr_3644196/](https://www.intertekinform.com/en-us/standards/abnt-nbr-6023-2025-779112_saig_nbr_nbr_3644196/)
- DIN MEDIA. `ABNT NBR 10520:2023-07-19` - Citations in documents - Presentation. Disponivel em: [https://www.dinmedia.de/en/standard/abnt-nbr-10520/371501342](https://www.dinmedia.de/en/standard/abnt-nbr-10520/371501342)
- DIN MEDIA. `ABNT NBR 6024:2012-02-01` - Numbering of the progressive sections of a document - Presentation. Disponivel em: [https://www.dinmedia.de/en/standard/abnt-nbr-6024/180838896](https://www.dinmedia.de/en/standard/abnt-nbr-6024/180838896)
- DIN MEDIA. `ABNT NBR 6027:2012-12-11` - Contents list - Presentation. Disponivel em: [https://www.dinmedia.de/en/standard/abnt-nbr-6027/180849330](https://www.dinmedia.de/en/standard/abnt-nbr-6027/180849330)
- DIN MEDIA. `ABNT NBR 6028:2021-05-18` - Abstracts - Presentation. Disponivel em: [https://www.dinmedia.de/en/standard/abnt-nbr-6028/341518010](https://www.dinmedia.de/en/standard/abnt-nbr-6028/341518010)
- INTERTEK INFORM. `ABNT NBR 12225:2023` - Spine - Presentation. Disponivel em: [https://www.intertekinform.com/en-gb/standards/abnt-nbr-12225-2023-774216_saig_nbr_nbr_3287879/](https://www.intertekinform.com/en-gb/standards/abnt-nbr-12225-2023-774216_saig_nbr_nbr_3287879/)
- UNIVERSIDADE FEDERAL DE UBERLANDIA. *Atualizacao da norma de apresentacao de trabalhos academicos - ABNT NBR 14724*. Disponivel em: [https://bibliotecas.ufu.br/acontece/2025/01/atualizacao-da-norma-de-apresentacao-de-trabalhos-academicos-abnt-nbr-14724](https://bibliotecas.ufu.br/acontece/2025/01/atualizacao-da-norma-de-apresentacao-de-trabalhos-academicos-abnt-nbr-14724)
- PONTIFICIA UNIVERSIDADE CATOLICA DO PARANA. *ABNT NBR 14.724 atualizada: descubra o que mudou!*. Disponivel em: [https://www.pucpr.br/noticias/abnt-nbr-14-724-atualizada-descubra-o-que-mudou/](https://www.pucpr.br/noticias/abnt-nbr-14-724-atualizada-descubra-o-que-mudou/)
- UNIVERSIDADE FEDERAL DE ALFENAS. *Manual de normalizacao e apresentacao de trabalhos academicos da UNIFAL-MG*. Atualizacao noticiada em: [https://www.unifal-mg.edu.br/bibliotecas/segunda-edicao-revista-e-atualizada-no-manual-de-normalizacao-e-apresentacao-de-trabalhos-academicos-da-unifal-mg/](https://www.unifal-mg.edu.br/bibliotecas/segunda-edicao-revista-e-atualizada-no-manual-de-normalizacao-e-apresentacao-de-trabalhos-academicos-da-unifal-mg/)
