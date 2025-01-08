#include "temperatura.h"

// Pass our oneWire reference to Dallas Temperature sensor 
// Setup a oneWire instance to communicate with any OneWire devices
Temperatura::Temperatura(uint8_t pin) : _oneWire(pin), _sensor(&_oneWire) 
{
  _ssid = "GERAL_DUE";
  _password = "compre@due";
  _server = "script.google.com";
  _GScriptId = "AKfycbx3rx2xPQu7ob2Mm-nLjNqRJ8tZ3qXEZgNPZlotsdT_zMe-Px-38psBXNyvoV18pLGA";
  _temperaturaSensor = 0.0;
  _sensor.begin();

  if (_sensor.getAddress(_sensorAddress, 0)) {
    debugar("Sensor encontrado!");
  } else {
    debugar("Erro: Nenhum sensor encontrado no construtor!");
  }
}

Temperatura::~Temperatura() {}

void Temperatura::leituraDeTemperatura() {
  Serial.println("Lendo temperatura...");

  if (!_sensor.getAddress(_sensorAddress, 0)) 
  {
    debugar("Erro: Nenhum sensor encontrado!");
    _temperaturaSensor = -127.0; //Valor padrão para erro
    return;
  }

  if (!_sensor.isConnected(_sensorAddress)) 
  {
    debugar("Erro: Sensor nao esta conectado!");
    _temperaturaSensor = -127.0; //Valor padrão para erro
    return;
  }

  _sensor.requestTemperatures();
  _temperaturaSensor = _sensor.getTempCByIndex(0);

  if (_temperaturaSensor == DEVICE_DISCONNECTED_C || _temperaturaSensor <= -30 || _temperaturaSensor >= 60.0) 
  {
    debugar("Sensor desconectado ou leitura invalida!");
    _temperaturaSensor = -127.0; //Valor padrão para erro
  } else {
    debugar("Temperatura lida: " + String(_temperaturaSensor) + " °C");
  }
}

void Temperatura::enviarMedicao(float temperatura1, float temperatura2, float temperaturaDif) {

  if(_temperaturaSensor == -127.0) 
  {
    debugar("Erro: Nao foi possivel ler temperatura.");
    blinkFalha();
    return;
  }

  if (WiFi.status() != WL_CONNECTED) 
  {
      debugar("Wi-Fi desconectado. Tentando reconectar...");
      reconnect_wifi();
      if (WiFi.status() != WL_CONNECTED) 
      {
          debugar("Erro: Não foi possível reconectar.");
          return;
      }
  }

  String url = String("https://script.google.com") + "/macros/s/" + _GScriptId + "/exec?"
               + "value1=" + String(temperatura1, 2) // Temperatura do primeiro sensor
               + "&value2=" + String(temperatura2, 2) // Temperatura do segundo sensor
               + "&value3=" + String(temperaturaDif, 2); // Diferença entre as temperaturas
  
  debugar("Enviando medicao para URL: " + url);
  _httpClient.begin(url.c_str());
  _httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  int httpCode = _httpClient.GET();
  if (httpCode > 0) {
    String payload = _httpClient.getString();
    debugar("Codigo HTTP: " + String(httpCode));
    debugar("Resposta: " + payload);
    blinkSucesso(); // Breve piscada para sucesso
  } else {
    debugar("Erro ao enviar requisicao HTTP");
    blinkFalha();
  }
  _httpClient.end();
}


void Temperatura::set_temperatura(float temperatura) {
  _temperaturaSensor = temperatura;
}

float Temperatura::get_temperatura() {
  return _temperaturaSensor;
}

/*******************************************************************************
 * WIFI
 ******************************************************************************/
void Temperatura::connect_wifi(void)
{
  
  debugar("Connecting to wifi: ");
  debugar(_ssid);
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED)
  {
  
    delay(500);
    Serial.print(WiFi.status());
    Serial.print(".");
  }

  debugar("");
  debugar("WiFi connected");
  debugar("IP address: ");
  debugar(String(WiFi.localIP()));
}

void Temperatura::reconnect_wifi() {
        WiFi.disconnect(true);
        WiFi.begin(_ssid, _password);

        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            blinkFalha();
        }

        if (WiFi.status() == WL_CONNECTED) {
            blinkSucesso(); // liga o LED
            debugar("Wi-Fi conectado!");
        } else {
            debugar("Falha ao conectar Wi-Fi!");
             blinkFalha(); // Apaga o LED
        }
    }

