# Template LaTeX do TCC

Esta pagina organiza o material academico do projeto para quem quiser transformar a documentacao
tecnica em uma monografia com `abnTeX2`.

## O que esta pronto

- estrutura base de TCC em `abnTeX2`
- capitulos iniciais alinhados ao tema da BMS 4S
- bibliografia inicial em formato `BibTeX`
- separacao entre introducao, fundamentacao, metodologia, arquitetura, desenvolvimento e conclusao

## Arquivos principais

- [main.tex](../assets/tcc-latex/main.tex)
- [referencias.bib](../assets/tcc-latex/referencias.bib)
- [01-introducao.tex](../assets/tcc-latex/capitulos/01-introducao.tex)
- [02-fundamentacao-teorica.tex](../assets/tcc-latex/capitulos/02-fundamentacao-teorica.tex)
- [03-requisitos-e-metodologia.tex](../assets/tcc-latex/capitulos/03-requisitos-e-metodologia.tex)
- [04-arquitetura-da-bms.tex](../assets/tcc-latex/capitulos/04-arquitetura-da-bms.tex)
- [05-desenvolvimento-do-sistema.tex](../assets/tcc-latex/capitulos/05-desenvolvimento-do-sistema.tex)
- [06-resultados-e-discussao.tex](../assets/tcc-latex/capitulos/06-resultados-e-discussao.tex)
- [07-conclusao.tex](../assets/tcc-latex/capitulos/07-conclusao.tex)

## Como usar

1. Ajuste capa, curso, instituicao, orientacao e elementos pre-textuais no `main.tex`.
2. Reaproveite os capitulos tecnicos do site para alimentar o texto da monografia.
3. Revise citacoes e referencias com base no manual institucional da sua universidade.
4. Compile com `pdflatex` e `bibtex`, ou com `latexmk` quando o ambiente tiver as ferramentas.

## Referencias oficiais

- [CTAN - abnTeX2](https://ctan.org/pkg/abntex2?lang=en)
- [GitHub - abnTeX2](https://github.com/abntex/abntex2)

## Relacao com o restante da documentacao

- a base normativa esta em [TCC e ABNT](monografia-tcc-abnt.md)
- a parte tecnica da bateria esta em [Arquitetura BMS 4S](../projeto/arquitetura-bms-4s.md)
- a trilha de protecao esta em [Tecnicas de protecao](../projeto/protecao-bms.md)
- o recorte de AFE esta em [AFE em BMS](../projeto/afe-em-bms.md)
