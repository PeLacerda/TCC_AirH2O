# AirH2O - Monitor de Nível de Água

Sistema de monitoramento de nível de reservatório desenvolvido para ESP32, com sensores de contato para detectar o nível de água e envio dos dados para uma API em rede local. O firmware lê os sensores, calcula o percentual e a quantidade aproximada de litros, e transmite as informações em formato JSON para o backend, que pode então expor os dados para um frontend.

## Visão geral

Este projeto foi pensado para um reservatório de água com sensores posicionados em pontos de nível pré-definidos:

- 25%
- 50%
- 75%
- 100%

Cada sensor indica se a água atingiu aquele nível. A partir da combinação dos sensores ativos, o firmware calcula:

- percentual de capacidade do reservatório
- volume estimado em litros
- lista dos sensores ativos
- identificação do dispositivo
- timestamp do registro

Os dados são enviados por HTTP para uma API REST, geralmente executada em um servidor local ou na mesma rede Wi‑Fi do ESP32.

## Funcionamento do firmware

O código principal em [airh2o_monitor.ino](airh2o_monitor.ino) executa os seguintes passos:

1. Inicializa a serial para debug
2. Configura os pinos dos sensores
3. Conecta o ESP32 à rede Wi‑Fi
4. Lê o estado dos sensores de nível
5. Calcula o percentual de água no reservatório
6. Converte esse percentual para litros estimados
7. Monta um payload em JSON
8. Envia os dados via HTTP POST para a API
9. Caso a rede caia, tenta reconectar automaticamente

## Estrutura dos arquivos

- [airh2o_monitor.ino](airh2o_monitor.ino): firmware principal do ESP32
- [config.h](config.h): definições do Wi‑Fi, API, pinos e capacidade do reservatório

## Hardware necessário

- ESP32 (preferencialmente ESP32-C3 ou ESP32 DevKit)
- 4 sensores de nível (contato, float switch ou sensores de presença de água)
- Resistores pull-up internos ou externos, conforme necessidade
- Cabos para ligação dos sensores ao ESP32
- Fonte de alimentação adequada para o microcontrolador
- Rede Wi‑Fi local

## Mapa dos pinos

Os pinos são definidos em [config.h](config.h):

- PIN_SENSOR_NIVEL_25 = 4
- PIN_SENSOR_NIVEL_50 = 5
- PIN_SENSOR_NIVEL_75 = 6
- PIN_SENSOR_NIVEL_100 = 7

Todos os pinos são configurados com `INPUT_PULLUP`, o que significa que o estado normal dos sensores é HIGH e o acionamento muda para LOW quando o contato é fechado.

## Configuração do projeto

Edite o arquivo [config.h](config.h) com os valores reais do seu ambiente:

```cpp
#define WIFI_SSID "rede_wifi_do_usuario"
#define WIFI_PASSWORD "senha_wifi_do_usuario"
#define DISPOSITIVO_ID "esp32-reservatorio-01"
#define DISPOSITIVO_NOME "Reservatório Principal - ETEC"
#define API_BASE_URL "http://192.168.1.100:8000"
#define API_MEDICOES_PATH "/api/medicoes"
#define CAPACIDADE_LITROS 500.0f
```

### Parâmetros importantes

- `WIFI_SSID` e `WIFI_PASSWORD`: credenciais da rede Wi‑Fi
- `DISPOSITIVO_ID`: identificador do equipamento no backend
- `DISPOSITIVO_NOME`: nome amigável exibido no sistema
- `API_BASE_URL`: endereço do backend (ex.: servidor Python com Uvicorn)
- `API_MEDICOES_PATH`: rota da API para receber as medições
- `CAPACIDADE_LITROS`: capacidade total do reservatório em litros
- `INTERVALO_LEITURA_MS`: tempo entre leituras do sensor

## Bibliotecas usadas

O projeto usa as bibliotecas oficiais do Arduino para ESP32:

- `WiFi.h`
- `HTTPClient.h`
- `ArduinoJson.h`

## Resumo

Este firmware faz a leitura dos sensores de nível de água, interpreta o estado dos mesmos, calcula o volume aproximado em litros e envia as informações para uma API para armazenamento e visualização em um frontend. Ele também tenta manter a conectividade com a rede e informar erros em caso de falha de comunicação.
