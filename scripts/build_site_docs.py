from __future__ import annotations

import json
import os
import re
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SITE_DOCS = ROOT / "site_docs"

README_SOURCE = ROOT / "README.md"

KNOWLEDGE_GROUPS = {
    "00-MOCs": "conhecimento/mocs",
    "10-Conceitos": "conhecimento/conceitos",
    "20-Hardware": "conhecimento/hardware",
    "30-Protecoes": "conhecimento/protecoes",
    "40-Firmware": "conhecimento/firmware",
    "50-Testes": "conhecimento/testes",
    "60-Requisitos": "conhecimento/requisitos",
}

MANUAL_PAGE_TARGETS = {
    SITE_DOCS / "index.md",
    SITE_DOCS / "academico" / "template-latex.md",
}

MARKDOWN_LINK_RE = re.compile(r"(!?)\[([^\]]+)\]\(([^)]+)\)")
WIKI_LINK_RE = re.compile(r"\[\[([^\]|#]+)(?:#([^\]|]+))?(?:\|([^\]]+))?\]\]")
WINDOWS_ABS_RE = re.compile(r"^/?[A-Za-z]:/")
LATEX_INLINE_COMMAND_RE = re.compile(r"\\(textit|textbf|texttt)\{([^{}]+)\}")
LATEX_COMMAND_WITH_ARG_RE = re.compile(r"\\([a-zA-Z*]+)\{([^{}]*)\}")


def slugify_name(name: str) -> str:
    return name.lower()


def relative_repo_path(path: Path) -> Path:
    return path.resolve().relative_to(ROOT)


def build_markdown_map() -> dict[Path, Path]:
    mapping: dict[Path, Path] = {
        README_SOURCE.resolve(): Path("projeto/visao-geral.md"),
        (ROOT / "knowledge" / "README.md").resolve(): Path("conhecimento/visao-geral.md"),
        (ROOT / "docs" / "arquitetura-bms-4s.md").resolve(): Path("projeto/arquitetura-bms-4s.md"),
        (ROOT / "docs" / "funcionamento-da-bms.md").resolve(): Path("projeto/funcionamento-da-bms.md"),
        (ROOT / "docs" / "protecao-bms.md").resolve(): Path("projeto/protecao-bms.md"),
        (ROOT / "docs" / "maquina-de-estados-bms.md").resolve(): Path("projeto/maquina-de-estados-bms.md"),
        (ROOT / "docs" / "diagnostico-e-eventos.md").resolve(): Path("projeto/diagnostico-e-eventos.md"),
        (ROOT / "docs" / "afe-em-bms.md").resolve(): Path("projeto/afe-em-bms.md"),
        (ROOT / "docs" / "dor-e-dod.md").resolve(): Path("projeto/dor-e-dod.md"),
        (ROOT / "docs" / "obsidian-vault-bms.md").resolve(): Path("conhecimento/obsidian-vault-bms.md"),
        (ROOT / "docs" / "monografia-tcc-abnt.md").resolve(): Path("academico/monografia-tcc-abnt.md"),
    }

    for source_dir, target_dir in KNOWLEDGE_GROUPS.items():
        for source_path in sorted((ROOT / "knowledge" / source_dir).glob("*.md")):
            if source_path.name == "Template-Nota.md":
                continue
            mapping[source_path.resolve()] = Path(target_dir) / f"{slugify_name(source_path.stem)}.md"

    return mapping


MARKDOWN_MAP = build_markdown_map()
NOTE_MAP = {source.stem.lower(): target for source, target in MARKDOWN_MAP.items()}

STATIC_FILE_MAP: dict[Path, Path] = {
    (ROOT / "config" / "bms_config.json").resolve(): Path("assets/downloads/bms_config.json"),
    (ROOT / "src" / "bms_controller.cpp").resolve(): Path("assets/codigo-fonte/bms_controller.cpp"),
    (ROOT / "include" / "bms_types.h").resolve(): Path("assets/codigo-fonte/bms_types.h"),
    (ROOT / "include" / "generated_bms_config.h").resolve(): Path("assets/codigo-fonte/generated_bms_config.h"),
    (ROOT / "include" / "bms_event_log.h").resolve(): Path("assets/codigo-fonte/bms_event_log.h"),
    (ROOT / "src" / "bms_runtime_config_service.cpp").resolve(): Path("assets/codigo-fonte/bms_runtime_config_service.cpp"),
    (ROOT / "src" / "bms_event_log.cpp").resolve(): Path("assets/codigo-fonte/bms_event_log.cpp"),
    (ROOT / "src" / "afe_battery_monitor.cpp").resolve(): Path("assets/codigo-fonte/afe_battery_monitor.cpp"),
    (ROOT / "src" / "bq769x_afe_driver.cpp").resolve(): Path("assets/codigo-fonte/bq769x_afe_driver.cpp"),
    (ROOT / "src" / "main.cpp").resolve(): Path("assets/codigo-fonte/main.cpp"),
    (ROOT / "mkdocs.yml").resolve(): Path("assets/site/mkdocs.yml"),
    (ROOT / ".github" / "workflows" / "github-pages.yml").resolve(): Path("assets/site/github-pages.yml"),
    (ROOT / "scripts" / "build_site_docs.py").resolve(): Path("assets/site/build_site_docs.py"),
    (ROOT / "scripts" / "generate_bms_config_header.py").resolve(): Path("assets/site/generate_bms_config_header.py"),
    (ROOT / "requirements-docs.txt").resolve(): Path("assets/site/requirements-docs.txt"),
}

CANVAS_MAP = {
    source.resolve(): Path("conhecimento/canvas") / source.name
    for source in sorted((ROOT / "knowledge" / "90-Canvas").glob("*.canvas"))
}

PROJECT_CONFIG_PAGE = Path("projeto/configuracao-json.md")
TCC_TEMPLATE_PAGE = Path("academico/template-latex.md")
TCC_WEB_PAGE = Path("academico/tcc-do-projeto.md")
GITHUB_PAGES_PAGE = Path("projeto/publicacao-github-pages.md")
KNOWLEDGE_MAPS_PAGE = Path("conhecimento/mapas-relacionais.md")


def clean_generated_content() -> None:
    for directory in [
        SITE_DOCS / "projeto",
        SITE_DOCS / "conhecimento",
        SITE_DOCS / "assets" / "downloads",
        SITE_DOCS / "assets" / "codigo-fonte",
        SITE_DOCS / "assets" / "site",
        SITE_DOCS / "assets" / "tcc-latex",
    ]:
        if directory.exists():
            shutil.rmtree(directory)

    generated_academic_page = SITE_DOCS / "academico" / "monografia-tcc-abnt.md"
    if generated_academic_page.exists():
        generated_academic_page.unlink()

    for manual_path in MANUAL_PAGE_TARGETS:
        manual_path.parent.mkdir(parents=True, exist_ok=True)


def ensure_parent(path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)


def copy_static_assets() -> None:
    for source_path, target_path in STATIC_FILE_MAP.items():
        destination = SITE_DOCS / target_path
        ensure_parent(destination)
        shutil.copy2(source_path, destination)

    tcc_source = ROOT / "tcc-latex"
    tcc_destination = SITE_DOCS / "assets" / "tcc-latex"
    shutil.copytree(tcc_source, tcc_destination, dirs_exist_ok=True)
    readme_in_assets = tcc_destination / "README.md"
    if readme_in_assets.exists():
        readme_in_assets.unlink()

    for source_path, target_path in CANVAS_MAP.items():
        destination = SITE_DOCS / target_path
        ensure_parent(destination)
        shutil.copy2(source_path, destination)


def resolve_local_target(source_path: Path, href: str) -> Path | None:
    if href.startswith(("http://", "https://", "mailto:", "#")):
        return None

    cleaned = href.split("#", 1)[0].split("?", 1)[0].strip()
    if not cleaned:
        return None

    if WINDOWS_ABS_RE.match(cleaned):
        target = Path(cleaned.lstrip("/"))
    else:
        target = (source_path.parent / cleaned).resolve()

    try:
        target.relative_to(ROOT)
    except ValueError:
        return None

    return target


def rewrite_anchor(anchor: str | None) -> str:
    if not anchor:
        return ""
    normalized = re.sub(r"[^a-z0-9\- ]", "", anchor.lower())
    normalized = normalized.replace(" ", "-")
    normalized = re.sub(r"-{2,}", "-", normalized).strip("-")
    return f"#{normalized}" if normalized else ""


def relative_href(current_target: Path, destination: Path) -> str:
    current_directory = (SITE_DOCS / current_target).parent
    target_file = SITE_DOCS / destination
    return Path(os.path.relpath(target_file, start=current_directory)).as_posix()


def resolve_href(source_path: Path, current_target: Path, href: str) -> str | None:
    if href.startswith(("http://", "https://", "mailto:", "#")):
        return href

    local_target = resolve_local_target(source_path, href)
    if local_target is None:
        return href

    anchor = rewrite_anchor(href.split("#", 1)[1] if "#" in href else None)

    if local_target in MARKDOWN_MAP:
        return f"{relative_href(current_target, MARKDOWN_MAP[local_target])}{anchor}"

    if local_target in STATIC_FILE_MAP:
        return f"{relative_href(current_target, STATIC_FILE_MAP[local_target])}{anchor}"

    if local_target in CANVAS_MAP:
        return f"{relative_href(current_target, CANVAS_MAP[local_target])}{anchor}"

    relative_target = relative_repo_path(local_target)

    if relative_target == Path("config/bms_config.json"):
        return f"{relative_href(current_target, PROJECT_CONFIG_PAGE)}{anchor}"

    if relative_target.parts and relative_target.parts[0] == "tcc-latex":
        if relative_target.name == "README.md":
            return f"{relative_href(current_target, TCC_TEMPLATE_PAGE)}{anchor}"
        copied_asset = Path("assets") / relative_target
        return f"{relative_href(current_target, copied_asset)}{anchor}"

    return None


def replace_markdown_links(source_path: Path, current_target: Path, text: str) -> str:
    def replacement(match: re.Match[str]) -> str:
        is_image = match.group(1) == "!"
        label = match.group(2)
        href = match.group(3).strip()
        rewritten = resolve_href(source_path, current_target, href)

        if rewritten is None:
            return match.group(0) if is_image else f"`{label}`"

        return f"{match.group(1)}[{label}]({rewritten})"

    return MARKDOWN_LINK_RE.sub(replacement, text)


def replace_wiki_links(current_target: Path, text: str) -> str:
    def replacement(match: re.Match[str]) -> str:
        note_name = match.group(1).strip()
        anchor = match.group(2)
        label = match.group(3) or note_name
        destination = NOTE_MAP.get(note_name.lower())
        if destination is None:
            return f"`{label}`"

        href = relative_href(current_target, destination)
        return f"[{label}]({href}{rewrite_anchor(anchor)})"

    return WIKI_LINK_RE.sub(replacement, text)


def render_markdown_page(source_path: Path, target_path: Path) -> None:
    raw_text = source_path.read_text(encoding="utf-8")
    rewritten = replace_markdown_links(source_path, target_path, raw_text)
    rewritten = replace_wiki_links(target_path, rewritten)

    destination = SITE_DOCS / target_path
    ensure_parent(destination)
    destination.write_text(rewritten, encoding="utf-8")


def extract_tex_command(text: str, command: str) -> str:
    match = re.search(rf"\\{command}\{{([\s\S]*?)\}}", text)
    return match.group(1).strip() if match else ""


def extract_tex_environment(text: str, environment: str, options: str = "") -> str:
    pattern = rf"\\begin\{{{environment}\}}{options}([\s\S]*?)\\end\{{{environment}\}}"
    match = re.search(pattern, text)
    return match.group(1).strip() if match else ""


def maybe_fix_mojibake(text: str) -> str:
    if "Ã" not in text and "Â" not in text:
        return text

    try:
        return text.encode("latin-1").decode("utf-8")
    except UnicodeError:
        return text


def parse_bib_entries(bib_text: str) -> dict[str, dict[str, str]]:
    entries: dict[str, dict[str, str]] = {}
    for match in re.finditer(r"@(\w+)\{([^,]+),([\s\S]*?)\n\}", bib_text):
        entry_type = match.group(1).strip()
        key = match.group(2).strip()
        body = match.group(3)
        fields = {"type": entry_type}
        for field_match in re.finditer(r"(\w+)\s*=\s*\{([\s\S]*?)\}", body):
            fields[field_match.group(1).strip().lower()] = " ".join(field_match.group(2).split())
        entries[key] = fields
    return entries


def citation_label(key: str, bib_entries: dict[str, dict[str, str]]) -> str:
    entry = bib_entries.get(key, {})
    author = entry.get("author", key)
    year = entry.get("year", "s.d.")
    author = author.replace("{", "").replace("}", "")
    short_author = author.split(" and ")[0].split(",")[0].strip()
    return f"{short_author}, {year}"


def replace_citations(text: str, bib_entries: dict[str, dict[str, str]]) -> str:
    def replacement(match: re.Match[str]) -> str:
        keys = [key.strip() for key in match.group(2).split(",") if key.strip()]
        rendered = [
            f"[{citation_label(key, bib_entries)}](#ref-{slugify_name(key)})"
            for key in keys
        ]
        if not rendered:
            return ""
        return f" ({'; '.join(rendered)})"

    return re.sub(r"\\(cite|citeonline)\{([^}]+)\}", replacement, text)


def replace_inline_latex(text: str) -> str:
    previous = ""
    current = text
    while previous != current:
        previous = current

        def inline_replacement(match: re.Match[str]) -> str:
            command = match.group(1)
            value = match.group(2)
            if command == "textbf":
                return f"**{value}**"
            if command == "texttt":
                return f"`{value}`"
            return f"*{value}*"

        current = LATEX_INLINE_COMMAND_RE.sub(inline_replacement, current)

    return current


def convert_latex_lists(text: str) -> str:
    def replace_environment(environment: str, marker: str) -> None:
        nonlocal text

        pattern = re.compile(
            rf"\\begin\{{{environment}\}}([\s\S]*?)\\end\{{{environment}\}}",
            re.MULTILINE,
        )

        def env_replacement(match: re.Match[str]) -> str:
            body = match.group(1).strip()
            items = re.findall(r"\\item\s+([\s\S]*?)(?=(\\item|$))", body)
            lines = []
            for item_text, _ in items:
                item_text = " ".join(item_text.strip().split())
                if item_text:
                    lines.append(f"{marker} {item_text}")
            return "\n".join(lines)

        text = pattern.sub(env_replacement, text)

    replace_environment("itemize", "-")
    replace_environment("enumerate", "1.")
    return text


def cleanup_latex_text(text: str) -> str:
    cleaned = text
    cleaned = cleaned.replace(r"\textit", r"\textit")
    cleaned = cleaned.replace(r"\_", "_")
    cleaned = cleaned.replace(r"\%", "%")
    cleaned = cleaned.replace(r"\&", "&")
    cleaned = cleaned.replace(r"~", " ")
    cleaned = cleaned.replace(r"``", '"').replace(r"''", '"')
    cleaned = cleaned.replace(r"\ ", " ")
    cleaned = re.sub(r"\\begin\{otherlanguage\*\}\{[^}]+\}", "", cleaned)
    cleaned = cleaned.replace(r"\end{otherlanguage*}", "")
    cleaned = replace_inline_latex(cleaned)
    cleaned = convert_latex_lists(cleaned)
    cleaned = re.sub(r"\\chapter\{([^}]+)\}", r"## \1", cleaned)
    cleaned = re.sub(r"\\section\{([^}]+)\}", r"### \1", cleaned)
    cleaned = re.sub(r"\\subsection\{([^}]+)\}", r"#### \1", cleaned)
    cleaned = re.sub(r"\\partapendices|\\partanexos", "", cleaned)
    cleaned = re.sub(r"\\chapter\*\{([^}]+)\}", r"## \1", cleaned)
    cleaned = re.sub(r"\\[a-zA-Z]+\*?(?:\[[^\]]*\])?\{[^{}]*\}", "", cleaned)
    cleaned = re.sub(r"\\[a-zA-Z]+\*?", "", cleaned)
    cleaned = re.sub(r"\n{3,}", "\n\n", cleaned)
    return cleaned.strip()


def format_bib_entry(key: str, entry: dict[str, str]) -> str:
    parts = []
    if author := entry.get("author"):
        parts.append(author.replace("{", "").replace("}", ""))
    if title := entry.get("title"):
        parts.append(f"*{title}*")
    edition = entry.get("edition")
    publisher = entry.get("publisher")
    address = entry.get("address")
    year = entry.get("year")
    note = entry.get("note")
    url = entry.get("url")

    trailing = ", ".join([value for value in [edition and f"{edition} ed.", publisher, address, year] if value])
    if trailing:
        parts.append(trailing)
    if note:
        parts.append(note)
    if url:
        parts.append(url)

    return " ".join(parts)


def render_tcc_web_page() -> None:
    main_tex = maybe_fix_mojibake((ROOT / "tcc-latex" / "main.tex").read_text(encoding="utf-8"))
    bib_text = maybe_fix_mojibake((ROOT / "tcc-latex" / "referencias.bib").read_text(encoding="utf-8"))
    bib_entries = parse_bib_entries(bib_text)

    title = extract_tex_command(main_tex, "title")
    author = extract_tex_command(main_tex, "author")
    local = extract_tex_command(main_tex, "local")
    year = extract_tex_command(main_tex, "date")
    institution = extract_tex_command(main_tex, "instituicao").lstrip("%").strip().replace("\\\\", "<br>")
    work_type = extract_tex_command(main_tex, "tipotrabalho")
    preamble = extract_tex_command(main_tex, "preambulo")
    advisor = extract_tex_command(main_tex, "orientador")
    coadvisor = extract_tex_command(main_tex, "coorientador")

    resumo = cleanup_latex_text(replace_citations(extract_tex_environment(main_tex, "resumo"), bib_entries))
    abstract = cleanup_latex_text(
        replace_citations(extract_tex_environment(main_tex, "resumo", r"\[Abstract\]"), bib_entries)
    )

    chapter_paths = [
        ROOT / "tcc-latex" / "capitulos" / "01-introducao.tex",
        ROOT / "tcc-latex" / "capitulos" / "02-fundamentacao-teorica.tex",
        ROOT / "tcc-latex" / "capitulos" / "03-requisitos-e-metodologia.tex",
        ROOT / "tcc-latex" / "capitulos" / "04-arquitetura-da-bms.tex",
        ROOT / "tcc-latex" / "capitulos" / "05-desenvolvimento-do-sistema.tex",
        ROOT / "tcc-latex" / "capitulos" / "06-resultados-e-discussao.tex",
        ROOT / "tcc-latex" / "capitulos" / "07-conclusao.tex",
    ]

    rendered_chapters = []
    chapter_titles = []
    for chapter_path in chapter_paths:
        chapter_text = maybe_fix_mojibake(chapter_path.read_text(encoding="utf-8"))
        chapter_text = replace_citations(chapter_text, bib_entries)
        chapter_markdown = cleanup_latex_text(chapter_text)
        chapter_heading_match = re.search(r"^##\s+(.+)$", chapter_markdown, re.MULTILINE)
        if chapter_heading_match:
            chapter_title = chapter_heading_match.group(1).strip()
            chapter_titles.append(chapter_title)
            chapter_markdown = f'<a id="{rewrite_anchor(chapter_title).lstrip("#")}"></a>\n\n{chapter_markdown}'
        rendered_chapters.append(chapter_markdown)

    appendix = cleanup_latex_text(
        replace_citations(
            extract_tex_environment(main_tex, "apendicesenv"),
            bib_entries,
        )
    )
    annex = cleanup_latex_text(
        replace_citations(
            extract_tex_environment(main_tex, "anexosenv"),
            bib_entries,
        )
    )

    references_markdown = "\n".join(
        f'- <a id="ref-{slugify_name(key)}"></a>`{key}`: {format_bib_entry(key, entry)}'
        for key, entry in bib_entries.items()
    )

    summary_markdown = "\n".join(
        f"1. [{chapter_title}]({rewrite_anchor(chapter_title)})"
        for chapter_title in chapter_titles
    )

    content = f"""<div class="thesis-cover">
  <p class="thesis-kicker">{work_type}</p>
  <h1>{title}</h1>
  <div class="thesis-meta">
    <p><strong>Autor:</strong> {author}</p>
    <p><strong>Local:</strong> {local}</p>
    <p><strong>Ano:</strong> {year}</p>
    <p><strong>Orientador:</strong> {advisor}</p>
    <p><strong>Coorientador:</strong> {coadvisor}</p>
  </div>
  <p class="thesis-preamble">{preamble}</p>
  <p class="thesis-institution">{institution}</p>
</div>

# TCC do projeto

Esta pagina apresenta a versao web da monografia do projeto, montada a partir do template em
`abnTeX2` e dos capitulos ja preparados em `tcc-latex/`.

## Elementos pre-textuais

### Resumo

{resumo}

### Abstract

{abstract}

## Sumario

{summary_markdown}

## Texto completo

{"\n\n".join(rendered_chapters)}

## Apendice

{appendix}

## Anexo

{annex}

## Referencias

{references_markdown}

## Arquivos de origem

- [Template principal em LaTeX](../assets/tcc-latex/main.tex)
- [Bibliografia BibTeX](../assets/tcc-latex/referencias.bib)
- [Guia de TCC e ABNT](monografia-tcc-abnt.md)
"""

    destination = SITE_DOCS / TCC_WEB_PAGE
    ensure_parent(destination)
    destination.write_text(content, encoding="utf-8")


def render_config_page() -> None:
    config_source = ROOT / "config" / "bms_config.json"
    config_text = config_source.read_text(encoding="utf-8")
    config = json.loads(config_text)

    content = f"""# Configuracao JSON da BMS

Este arquivo consolida os limites e escolhas operacionais da BMS 4S em um formato simples,
versionavel e facil de alinhar com firmware, requisitos e validacao.

## Estrutura geral

- `pack`: define topologia do pack e capacidade nominal
- `measurement`: escolhe o AFE, interface e estrategia de medicao
- `protection`: concentra limites eletricos e termicos criticos
- `fault_handling`: define histerese, latch e politica de liberacao de falha
- `operation`: concentra limiares operacionais de carga, descarga e balanceamento
- `estimation`: define a base de `SOC`, `SOE`, `SOH`, `SOP` e `SOF`

## Arquivo atual

```json
{config_text.rstrip()}
```

## Observacoes de engenharia

- o projeto assume `{config["pack"]["series_cells"]}S` com `{config["pack"]["parallel_cells"]}P`
- o AFE ainda esta como `to_be_defined`, o que combina com a fase atual de arquitetura
- a protecao de curto em hardware segue marcada como obrigatoria
- o `PlatformIO` gera `include/generated_bms_config.h` antes do build a partir deste JSON
- o firmware aceita override em runtime via um servico de comandos baseado em `Stream`
- o JSON deve permanecer alinhado com a [matriz de rastreabilidade](../conhecimento/requisitos/matriz-rastreabilidade-configuracao.md)

## Downloads

- [Baixar bms_config.json](../assets/downloads/bms_config.json)
- [Header gerado para o firmware](../assets/codigo-fonte/generated_bms_config.h)
- [Servico de runtime config](../assets/codigo-fonte/bms_runtime_config_service.cpp)
- [Camada de monitor para AFE](../assets/codigo-fonte/afe_battery_monitor.cpp)
- [Scaffold de driver BQ769x](../assets/codigo-fonte/bq769x_afe_driver.cpp)
- [Script gerador da configuracao](../assets/site/generate_bms_config_header.py)
- [Abrir a nota de configuracao](../conhecimento/firmware/configuracao-bms.md)
"""

    destination = SITE_DOCS / PROJECT_CONFIG_PAGE
    ensure_parent(destination)
    destination.write_text(content, encoding="utf-8")


def render_github_pages_page() -> None:
    content = """# Publicacao no GitHub Pages

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
C:\\Users\\peter\\AppData\\Local\\Programs\\Python\\Python313\\python.exe scripts\\build_site_docs.py
C:\\Users\\peter\\AppData\\Local\\Programs\\Python\\Python313\\python.exe -m mkdocs build --strict
```

## Boas praticas

- manter a documentacao tecnica em `docs/`
- manter a base relacional em `knowledge/`
- evitar editar diretamente paginas geradas quando a fonte original ja existe no repositorio
- revisar links e imagens antes de cada publicacao
"""

    destination = SITE_DOCS / GITHUB_PAGES_PAGE
    ensure_parent(destination)
    destination.write_text(content, encoding="utf-8")


def render_knowledge_maps_page() -> None:
    canvas_links = "\n".join(
        f"- [{path.name}](canvas/{path.name})"
        for path in sorted((ROOT / "knowledge" / "90-Canvas").glob("*.canvas"))
    )

    content = f"""# Mapas relacionais

Esta pagina centraliza os artefatos visuais da camada de conhecimento do projeto.

## Objetivo

Os mapas ajudam a navegar pela documentacao como rede de conceitos, componentes, requisitos
e caminhos de implementacao, complementando a documentacao linear.

## Arquivos Canvas

{canvas_links}

## Como usar

- abra os `.canvas` no Obsidian para edicao visual
- use esta documentacao web como camada de leitura e referencia publica
- conecte cada mapa com as notas formais e as paginas de projeto

## Pontos de entrada recomendados

- [Start Here](mocs/start-here.md)
- [MOC BMS](mocs/moc-bms.md)
- [Indice por componente](mocs/indice-por-componente.md)
- [Indice por requisito](mocs/indice-por-requisito.md)
- [Matriz de rastreabilidade](requisitos/matriz-rastreabilidade-configuracao.md)
"""

    destination = SITE_DOCS / KNOWLEDGE_MAPS_PAGE
    ensure_parent(destination)
    destination.write_text(content, encoding="utf-8")


def build_site_docs() -> None:
    clean_generated_content()
    copy_static_assets()

    for source_path, target_path in MARKDOWN_MAP.items():
        render_markdown_page(source_path, target_path)

    render_config_page()
    render_github_pages_page()
    render_knowledge_maps_page()
    render_tcc_web_page()


if __name__ == "__main__":
    build_site_docs()
    print(f"Generated {len(MARKDOWN_MAP)} markdown pages into {SITE_DOCS}")
