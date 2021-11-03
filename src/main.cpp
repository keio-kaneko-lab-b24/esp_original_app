#include <Arduino.h>

#include "main.h"
#include "ble.h"

void setup()
{
  delay(3000);
  Serial.begin(115200);

  SetUpBLE();

  Serial.println("[setup] finished.");
}

void loop()
{
  UpdateBLEConnection();
}