#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Wi-Fi
#define WIFI_SSID "rede_wifi_do_usuario"
#define WIFI_PASSWORD "senha_wifi_do_usuario"

// Identificação deste dispositivo no banco de dados
#define DISPOSITIVO_ID "esp32-reservatorio-01"
#define DISPOSITIVO_NOME "Reservatório Principal - ETEC"

// Endpoint da API (IP do PC que roda o uvicorn, na mesma rede Wi-Fi do ESP)
// Back-end: python -m uvicorn main:app --host 0.0.0.0 --port 8000
#define API_BASE_URL "http://192.168.1.100:8000"
#define API_MEDICOES_PATH "/api/medicoes"

#define INTERVALO_LEITURA_MS 30000   // 30 segundos
#define INTERVALO_RECONEXAO_MS 10000 // 10 segundos ao perder Wi-Fi

// Capacidade do reservatório
#define CAPACIDADE_LITROS 500.0f

static const uint8_t PIN_SENSOR_NIVEL_25 = 4;  // 25%
static const uint8_t PIN_SENSOR_NIVEL_50 = 5;  // 50%
static const uint8_t PIN_SENSOR_NIVEL_75 = 6;  // 75%
static const uint8_t PIN_SENSOR_NIVEL_100 = 7; // 100%

#define HTTP_MAX_TENTATIVAS 3
#define HTTP_TIMEOUT_MS 5000

#endif
