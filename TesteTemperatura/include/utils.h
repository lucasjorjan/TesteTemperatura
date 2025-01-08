#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>      // Inclui a biblioteca Arduino para uso de funções básicas

/*******************************************************************************
 * FUNÇÃO PARA DEBUG
 ******************************************************************************/

#define debugSerial // Define macro para habilitar debug serial
#define DEBUG_MODE // Define macro para habilitar debug serial
#define LED_BUILTIN 2

void debugar(String msgUSB);
void piscarLed(int vezes, int tempoOn, int tempoOff);
void blinkFalha();
void blinkSucesso();

#endif  // UTILS_H