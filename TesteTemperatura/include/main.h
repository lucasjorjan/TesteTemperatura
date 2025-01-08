#ifndef MAIN_H
#define MAIN_H

#include "string.h"
#include <Arduino.h>      // Inclui a biblioteca Arduino para uso de funções básicas
#include <OneWire.h>
#include <DallasTemperature.h>
#include "utils.h"      // inclui funções de debug
#include "temperatura.h"      // inclui funções de debug
#include <esp_task_wdt.h>

#define PINO_ONEWIRE_SENSOR1 23 // Define pino do sensor
#define PINO_ONEWIRE_SENSOR2 22 // Define pino do sensor
#define LED_BUILTIN 2

/*******************************************************************************
 * Variaveis e defines BLUETOOH
 ******************************************************************************/

class Sistema {
private:
    Temperatura * _temperatura1;
    Temperatura * _temperatura2;

    static void Task1codeWrapper(void *pvParameters);
    static void Task2codeWrapper(void *pvParameters);
    TaskHandle_t _Task1; // Declaração de Task1
    TaskHandle_t _Task2; // Declaração de Task2

    // Semáforo para proteger o acesso às variáveis compartilhadas
    SemaphoreHandle_t _xMutex;

    // Variáveis para armazenar os valores mais recentes
    float _temperaturaDif;
    float _temperatura1Ultima = -127.0;
    float _temperatura2Ultima = -127.0;
    float _leitura_temperatura1;
    float _leitura_temperatura2;

public:
    Sistema();
    ~Sistema();

    void setup();
    void Task1code(void *pvParameters); // Adicionada declaração
    void Task2code(void *pvParameters); // Adicionada declaração
};

#endif // MAIN_H
