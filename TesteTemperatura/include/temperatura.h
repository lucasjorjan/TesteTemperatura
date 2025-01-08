#ifndef TEMPERATURA_H
#define TEMPERATURA_H

#include <Arduino.h>      // Inclui a biblioteca Arduino para uso de funções básicas
#include "utils.h"      // inclui funções de debug
#include <HTTPClient.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class Temperatura {
private:
  OneWire _oneWire;
  DallasTemperature _sensor;
  float _temperaturaSensor;
  DeviceAddress _sensorAddress;
  long _tempoUltimoEnvio;
  const char* _ssid;     // your network SSID (name of wifi network)
  const char* _password; // your network password
  const char* _server;  // Server URL
  const char* _GScriptId; //planilha teste
  const int _httpsPort = 443;
  WiFiClientSecure _client;
  float _ultimaTemperaturaEnviada = -127.0; // Valor inicial fora do intervalo usual de temperatura
  HTTPClient _httpClient;

public:
  Temperatura(uint8_t pin); // Construtor com pino para o sensor
  ~Temperatura();

  void setup();
  void set_temperatura(float temperatura);
  float get_temperatura();
  void leituraDeTemperatura();
  void enviarMedicao(float temperatura1, float temperatura2, float temperaturaDif);
  void connect_wifi(void);
  void reconnect_wifi();
  static void Task1codeStatic(void *pvParameters);
  static void Task2codeStatic(void *pvParameters);

};

#endif  // TEMPERATURA_H