# Publicacao no GitHub Pages

Esta pagina resume como a documentacao online do projeto e publicada automaticamente.

## Fluxo adotado

1. o conteudo fonte fica no proprio repositorio
2. o script `scripts/build_site_docs.py` reorganiza os Markdown para o site
3. o `MkDocs` gera a versao estatica final
4. o GitHub Actions publica a pasta `site/` no GitHub Pages

## Arquivos centrais

- [mkdocs.yml](../assets/site/mkdocs.yml)
- [Workflow de deploy](../assets/site/github-pages.yml)
- [Gerador do site](../assets/site/build_site_docs.py)
- [Dependencias da documentacao](../assets/site/requirements-docs.txt)

## URL esperada

- Site publicado: [https://petersonsenha.github.io/BMS-ESP32S3/](https://petersonsenha.github.io/BMS-ESP32S3/)
- Repositorio: [https://github.com/petersonsenha/BMS-ESP32S3](https://github.com/petersonsenha/BMS-ESP32S3)

## Como publicar

1. envie as alteracoes para a branch `main`
2. no GitHub, mantenha o Pages configurado para usar `GitHub Actions`
3. aguarde o workflow `docs` finalizar
4. abra a URL publicada do projeto

## Validacao local

```powershell
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe scripts\build_site_docs.py
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe -m mkdocs build --strict
```

## Boas praticas

- manter a documentacao tecnica em `docs/`
- manter a base relacional em `knowledge/`
- evitar editar diretamente paginas geradas quando a fonte original ja existe no repositorio
- revisar links e imagens antes de cada publicacao
