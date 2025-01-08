#include "main.h"

Sistema sistema;

Sistema::Sistema() {}

Sistema::~Sistema() {}

void Sistema::setup() {
  // esp_task_wdt_deinit();  // Desativa o Watchdog Timer
  digitalWrite(LED_BUILTIN, HIGH);   // Acende o LED de indicação


  Serial.begin(115200);

  _temperatura1 = new Temperatura(PINO_ONEWIRE_SENSOR1); // Sensor no pino 23
  _temperatura2 = new Temperatura(PINO_ONEWIRE_SENSOR2); // Sensor no pino 14

  _temperatura1->connect_wifi();

  // Inicializa o mutex
  _xMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    Task1codeWrapper,
    "Task1",
    4096,
    this,
    1,
    &_Task1,
    1);

  xTaskCreatePinnedToCore(
    Task2codeWrapper,
    "Task2",
    8192,
    this,
    1,
    &_Task2,
    0);

  // Suspende Task2 inicialmente
  vTaskSuspend(_Task2);
}

void Sistema::Task1codeWrapper(void *pvParameters) {
  Sistema *sistema = static_cast<Sistema *>(pvParameters);
  sistema->Task1code(pvParameters);
}

void Sistema::Task2codeWrapper(void *pvParameters) {
  Sistema *sistema = static_cast<Sistema *>(pvParameters);
  sistema->Task2code(pvParameters);
}

// Task1: Lê as temperaturas de ambos os sensores, calcula a diferença entre elas, e armazena essas informações.

void Sistema::Task1code(void *pvParameters) {
  const uint32_t intervaloLeitura = 10000; // 10 segundos
  static uint32_t ultimaLeitura = 0;

  while (true) {
    uint32_t agora = millis();

    // Verifica se já passou o intervalo desde a última leitura
    if (agora - ultimaLeitura >= intervaloLeitura) {
      ultimaLeitura = agora;

      // Lê as temperaturas dos sensores
      _temperatura1->leituraDeTemperatura();
      _temperatura2->leituraDeTemperatura();

      // Atualiza os valores compartilhados
      if (xSemaphoreTake(_xMutex, portMAX_DELAY)) {
        _leitura_temperatura1= _temperatura1->get_temperatura();
        _leitura_temperatura2 = _temperatura2->get_temperatura();
        _temperaturaDif = abs(_leitura_temperatura1 - _leitura_temperatura2);
        xSemaphoreGive(_xMutex);
      }

      // Retoma a Task2 para enviar os dados
      vTaskResume(_Task2);
    }

    // Aguarda brevemente para evitar uso excessivo da CPU
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Sistema::Task2code(void *pvParameters) {
  while (true) {
    // Suspende até ser retomada pela Task1
    vTaskSuspend(nullptr);

    // Envia os dados atualizados
    if (xSemaphoreTake(_xMutex, portMAX_DELAY)) {
      // Verifica se é a primeira execução
      if (_temperatura1Ultima == -127.0 || _temperatura2Ultima == -127.0) {
        debugar("Primeira execucoo, enviando dados...");
        _temperatura1->enviarMedicao(_leitura_temperatura1, _leitura_temperatura2, _temperaturaDif);
        _temperatura1Ultima = _leitura_temperatura1;
        _temperatura2Ultima = _leitura_temperatura2;
        xSemaphoreGive(_xMutex);
        continue; // Pula a lógica de comparação na primeira execução
      }

      // Verifica se a mudança de temperatura variou mais do que 5 graus para envio
      if (abs(_leitura_temperatura1 - _temperatura1Ultima) >= 5.0 || abs(_leitura_temperatura2 - _temperatura2Ultima) >= 5.0) {
        debugar("Mudanca de temperatura variou mais do que 5 graus para envio.");
        xSemaphoreGive(_xMutex);
        continue;
      }

      // Envia os dados
      _temperatura1->enviarMedicao(_leitura_temperatura1, _leitura_temperatura2, _temperaturaDif);
      _temperatura1Ultima = _leitura_temperatura1;
      _temperatura2Ultima = _leitura_temperatura2;
      xSemaphoreGive(_xMutex);
    }
  }
}

void setup() 
{
  sistema.setup();
}

void loop() 
{
}
