# BMS-ESP32S3

Projeto inicial de uma BMS 4S com ESP32-S3.

## Hardware Necessario

Para sair do conceito e chegar a uma BMS 4S funcional, o conjunto minimo recomendado e:

- `ESP32-S3` para supervisao, telemetria, BLE/Wi-Fi e atualizacao de firmware
- `AFE` dedicado para 4S ou faixa 3S-5S para leitura de tensao das celulas
- `shunt resistor` para medicao de corrente
- `amplificador de corrente` ou leitura de corrente integrada no AFE
- `2x NTC` para temperatura de celulas e estagio de potencia
- `MOSFET de carga`
- `MOSFET de descarga`
- `resistores de balanceamento` por celula
- `fusivel` no pack
- `fonte auxiliar` ou regulacao adequada para alimentar o ESP32-S3 a partir do pack

Sem esse conjunto, o firmware sozinho nao entrega a seguranca que uma BMS precisa.

## Protecao da BMS

Uma BMS segura precisa trabalhar em camadas. O firmware ajuda bastante, mas a protecao principal
deve nascer no hardware.

Tecnicas de protecao que merecem prioridade neste projeto:

- `sobretensao por celula`: cortar a carga quando qualquer celula ultrapassar o limite configurado
- `subtensao por celula`: cortar a descarga antes que a celula entre em zona de dano
- `sobrecorrente de carga`: desligar o caminho de carga quando a corrente exceder o limite seguro
- `sobrecorrente de descarga`: desligar o caminho de descarga em sobrecarga ou curto
- `curto-circuito`: protecao rapida por hardware, sem depender apenas do loop do ESP32
- `sobretemperatura`: bloquear carga e descarga quando celulas ou MOSFETs aquecerem demais
- `subtemperatura de carga`: impedir carga abaixo da temperatura minima segura
- `falha do AFE`: tratar perda de comunicacao ou leitura invalida como falha critica
- `balanceamento`: drenar energia das celulas mais altas para evitar divergencia de tensao
- `fail-safe`: desligar MOSFETs em caso de reset, watchdog ou perda de supervisao

Detalhamento das tecnicas de protecao:

- [docs/protecao-bms.md](protecao-bms.md)
- [docs/arquitetura-bms-4s.md](arquitetura-bms-4s.md)
- [docs/maquina-de-estados-bms.md](maquina-de-estados-bms.md)
- [docs/afe-em-bms.md](afe-em-bms.md)
- [docs/obsidian-vault-bms.md](../conhecimento/obsidian-vault-bms.md)
- [docs/monografia-tcc-abnt.md](../academico/monografia-tcc-abnt.md)
- [tcc-latex/main.tex](../assets/tcc-latex/main.tex)

## Referencias Visuais

As imagens abaixo sao referencias publicas de fabricantes e servem para ilustrar os tipos de circuitos
que estamos discutindo para esta BMS 4S. Elas nao representam ainda o esquematico final da nossa placa.

### Balanceamento passivo por resistor

![Balanceamento passivo por resistor bleed](https://www.analog.com/en/_/media/analog/en/landing-pages/technical-articles/passive-balancing-allows-all-cells-to-appear-to-have-the-same-capacity/227546_fig_01.jpg?la=en&rev=3cfd03b9823346cfaca20d8f8acc410f&sc_lang=en)

Fonte: [Analog Devices - Passive Battery Cell Balancing](https://www.analog.com/en/resources/technical-articles/passive-battery-cell-balancing.html)

### Exemplo de AFE multicelula com balanceamento passivo externo

![Aplicacao multicelula com AFE e balanceamento passivo externo](https://www.analog.com/en/_/media/analog/en/landing-pages/technical-articles/passive-balancing-allows-all-cells-to-appear-to-have-the-same-capacity/227546_fig_02.jpg?la=en&rev=a3aaf0909965470cba2cdb41d798a80c&sc_lang=en)

Fonte: [Analog Devices - Passive Battery Cell Balancing](https://www.analog.com/en/resources/technical-articles/passive-battery-cell-balancing.html)

### Medicao de corrente por shunt

![Aplicacao tipica de medicao de corrente por resistor shunt](https://www.analog.com/cn/_/media/analog/en/landing-pages/technical-articles/versatile-current-sense-amplifiers-offer-railtorail-input/figure1.png?la=en&rev=ce8011b8eee44706b9b4f919116d84c6&sc_lang=zh)

Fonte: [Analog Devices - Versatile Current Sense Amplifiers Offer Rail-to-Rail Input, 150C Operating Temperature](https://www.analog.com/cn/resources/technical-articles/versatile-current-sense-amplifiers-offer-railtorail-input.html)

### Monitoramento sobre fusivel ou chave de potencia

![Monitoramento de corrente e queda de tensao sobre fusivel ou chave](https://www.analog.com/cn/_/media/analog/en/landing-pages/technical-articles/versatile-current-sense-amplifiers-offer-railtorail-input/figure4.png?la=en&rev=92f0166e358044788831b0a4288b5dd2&sc_lang=zh)

Fonte: [Analog Devices - Versatile Current Sense Amplifiers Offer Rail-to-Rail Input, 150C Operating Temperature](https://www.analog.com/cn/resources/technical-articles/versatile-current-sense-amplifiers-offer-railtorail-input.html)

Objetivo deste repositorio:

- definir a arquitetura eletrica da BMS 4S
- criar a base do firmware de supervisao
- preparar o caminho para a futura PCB

Importante: o ESP32 nao deve ser o unico elemento de protecao de uma bateria de litio.
Neste projeto, a recomendacao e usar o ESP32-S3 como controlador, telemetria e logica de alto nivel,
enquanto um AFE dedicado faz a medicao segura das celulas e apoia a protecao primaria.

## Arquitetura recomendada

Detalhes da arquitetura eletrica:

- [docs/arquitetura-bms-4s.md](arquitetura-bms-4s.md)

## Estrutura do firmware

- `include/`: tipos, configuracao e interfaces
- `src/`: logica da BMS, monitor mock e `main.cpp`
- `platformio.ini`: ponto de partida para ESP32-S3 com PlatformIO
- `config/bms_config.json`: configuracao base da BMS 4S
- `knowledge/`: camada do Obsidian para mapas relacionais, MOCs, requisitos e Canvas

## Estado atual

Esta primeira base ja contem:

- maquina de estados da BMS
- verificacao de falhas principais
- logica de balanceamento passivo
- modo mock para desenvolvimento sem placa pronta

Documentacao da maquina de estados:

- [docs/maquina-de-estados-bms.md](maquina-de-estados-bms.md)

## Site da documentacao

O projeto agora tambem tem uma base pronta para documentacao online via `GitHub Pages`,
usando `MkDocs` com tema `Material`.

Arquivos principais:

- `mkdocs.yml`: configuracao do site
- `site_docs/`: conteudo publicado pelo site
- `scripts/build_site_docs.py`: gera as paginas do site a partir da documentacao do repositorio
- `.github/workflows/github-pages.yml`: deploy automatico no GitHub Pages
- `requirements-docs.txt`: dependencias da documentacao

Fluxo local recomendado:

```powershell
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe scripts\build_site_docs.py
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe -m pip install -r requirements-docs.txt
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe -m mkdocs serve
```

Para gerar a versao estatica:

```powershell
C:\Users\peter\AppData\Local\Programs\Python\Python313\python.exe -m mkdocs build --strict
```

Antes de publicar no `GitHub Pages`, vale ajustar o `site_url` em `mkdocs.yml` com a URL final
do repositorio publicado.

## Proximos passos

1. escolher o AFE real da placa
2. trocar o monitor mock por um driver de hardware
3. definir o esquema dos MOSFETs, shunt e NTCs
4. adicionar persistencia, telemetria e testes de bancada
