# Template LaTeX do TCC

Este diretório contém um modelo inicial de monografia/TCC em LaTeX usando `abnTeX2`.

## Estrutura

- `main.tex`: arquivo principal
- `pretextuais/`: resumo e abstract
- `capitulos/`: capítulos do trabalho
- `referencias.bib`: base bibliográfica inicial

## Requisito principal

O template foi pensado para compilar com a classe `abntex2`.

Referência oficial:

- [CTAN - abntex2](https://ctan.org/pkg/abntex2?lang=en)
- [GitHub - abntex2](https://github.com/abntex/abntex2)

## Compilação

Fluxo mais comum:

```powershell
pdflatex main.tex
bibtex main
pdflatex main.tex
pdflatex main.tex
```

Se você usar `latexmk`:

```powershell
latexmk -pdf main.tex
```

## Observação importante

O `abnTeX2` é a base prática mais comum para documentos acadêmicos brasileiros em LaTeX, mas o
manual da sua universidade ainda prevalece para detalhes institucionais como:

- capa
- folha de aprovação
- ficha catalográfica
- identificação do curso
- estilo de citações e listas
