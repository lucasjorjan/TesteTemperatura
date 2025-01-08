#include "utils.h"     

/*******************************************************************************
 * FUNÇÃO PARA DEBUG
 ******************************************************************************/

// #ifdef DEBUG_MODE
//   #define debugar(x) Serial.println(x)
// #else
//   #define debugar(x)
// #endif


void debugar(String msgUSB)
{
#ifdef debugSerial

  Serial.print("[DEBUG] ");
  Serial.println(msgUSB);

#endif
}

void blinkSucesso()
{
  // Padrão 1
  piscarLed(3, 200, 200); // 3 piscadas com 100ms de intervalo
  digitalWrite(LED_BUILTIN, HIGH);
}

void piscarLed(int vezes, int tempoOn, int tempoOff) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(tempoOn);
    digitalWrite(LED_BUILTIN, LOW);
    delay(tempoOff);
  }
}

void blinkFalha() {
  // Padrão 1
  piscarLed(3, 250, 150); // 3 piscadas com 100ms de intervalo
  
  // Padrão 2
  piscarLed(6, 100, 50);   // 6 piscadas rápidas com 50ms de intervalo

  // Padrão 3
  piscarLed(3, 150, 150);   // 3 piscadas rápidas com 75ms de intervalo
}
