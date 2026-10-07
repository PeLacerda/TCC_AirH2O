#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

struct LeituraNivel {
  bool sensor25;
  bool sensor50;
  bool sensor75;
  bool sensor100;
  uint8_t nivelPercentual;
  float nivelLitros;
};

void configurarPinosSensores();
LeituraNivel lerNivelAgua();
uint8_t calcularPercentual(const LeituraNivel &leitura);
float calcularLitros(uint8_t percentual);
bool conectarWiFi();
bool enviarMedicaoAPI(const LeituraNivel &leitura);
String montarPayloadJSON(const LeituraNivel &leitura);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("  AIRH2O - Monitor ESP32-C3");
  Serial.println("========================================");

  configurarPinosSensores();
  conectarWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Conexão perdida. Reconectando...");
    conectarWiFi();
  }

  LeituraNivel leitura = lerNivelAgua();

  Serial.println("--- Leitura ---");
  Serial.printf("  25%%:  %s\n", leitura.sensor25 ? "AGUA" : "seco");
  Serial.printf("  50%%:  %s\n", leitura.sensor50 ? "AGUA" : "seco");
  Serial.printf("  75%%:  %s\n", leitura.sensor75 ? "AGUA" : "seco");
  Serial.printf(" 100%%: %s\n", leitura.sensor100 ? "AGUA" : "seco");
  Serial.printf("  Nivel: %d%% (~%.1f L)\n", leitura.nivelPercentual, leitura.nivelLitros);

  if (WiFi.status() == WL_CONNECTED) {
    enviarMedicaoAPI(leitura);
  }

  delay(INTERVALO_LEITURA_MS);
}

void configurarPinosSensores() {
  const uint8_t pinos[] = {
      PIN_SENSOR_NIVEL_25,
      PIN_SENSOR_NIVEL_50,
      PIN_SENSOR_NIVEL_75,
      PIN_SENSOR_NIVEL_100};

  for (uint8_t pino : pinos) {
    pinMode(pino, INPUT_PULLUP);
  }

  Serial.println("[Sensores] Pinos configurados.");
}

LeituraNivel lerNivelAgua() {
  LeituraNivel leitura;

  leitura.sensor25 = (digitalRead(PIN_SENSOR_NIVEL_25) == LOW);
  leitura.sensor50 = (digitalRead(PIN_SENSOR_NIVEL_50) == LOW);
  leitura.sensor75 = (digitalRead(PIN_SENSOR_NIVEL_75) == LOW);
  leitura.sensor100 = (digitalRead(PIN_SENSOR_NIVEL_100) == LOW);

  leitura.nivelPercentual = calcularPercentual(leitura);
  leitura.nivelLitros = calcularLitros(leitura.nivelPercentual);

  return leitura;
}

uint8_t calcularPercentual(const LeituraNivel &leitura) {
  if (leitura.sensor100) return 100;
  if (leitura.sensor75) return 75;
  if (leitura.sensor50) return 50;
  if (leitura.sensor25) return 25;
  return 0;
}

float calcularLitros(uint8_t percentual) {
  return (CAPACIDADE_LITROS * percentual) / 100.0f;
}

bool conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("[WiFi] Conectando a \"%s\"", WIFI_SSID);

  uint8_t tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[WiFi] Conectado! IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("[WiFi] Falha na conexão.");
  delay(INTERVALO_RECONEXAO_MS);
  return false;
}

String montarPayloadJSON(const LeituraNivel &leitura) {
  StaticJsonDocument<512> doc;

  doc["dispositivo_id"] = DISPOSITIVO_ID;
  doc["dispositivo_nome"] = DISPOSITIVO_NOME;
  doc["nivel_percentual"] = leitura.nivelPercentual;
  doc["nivel_litros_estimado"] = roundf(leitura.nivelLitros * 10.0f) / 10.0f;
  doc["capacidade_litros"] = CAPACIDADE_LITROS;

  JsonArray sensores = doc.createNestedArray("sensores_ativos");
  sensores.add(leitura.sensor25 ? 1 : 0);
  sensores.add(leitura.sensor50 ? 1 : 0);
  sensores.add(leitura.sensor75 ? 1 : 0);
  sensores.add(leitura.sensor100 ? 1 : 0);

  doc["timestamp_unix"] = (uint32_t)(millis() / 1000);

  String payload;
  serializeJson(doc, payload);
  return payload;
}

bool enviarMedicaoAPI(const LeituraNivel &leitura) {
  String url = String(API_BASE_URL) + String(API_MEDICOES_PATH);
  String payload = montarPayloadJSON(leitura);

  for (uint8_t i = 0; i < HTTP_MAX_TENTATIVAS; i++) {
    HTTPClient http;
    http.begin(url);
    http.setTimeout(HTTP_TIMEOUT_MS);
    http.addHeader("Content-Type", "application/json");

    Serial.printf("[API] POST %s\n", url.c_str());
    Serial.printf("[API] Payload: %s\n", payload.c_str());

    int httpCode = http.POST(payload);

    if (httpCode > 0) {
      Serial.printf("[API] Resposta HTTP %d: %s\n", httpCode, http.getString().c_str());
      http.end();

      if (httpCode >= 200 && httpCode < 300) {
        return true;
      }
    } else {
      Serial.printf("[API] Erro de conexão: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    delay(2000);
  }

  Serial.println("[API] Envio falhou após todas as tentativas.");
  return false;
}
