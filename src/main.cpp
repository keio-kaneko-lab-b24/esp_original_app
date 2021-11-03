#include <Arduino.h>

#include "main.h"
#include "ble.h"
#include "emg.h"

volatile float extensor_value = 0;
volatile float flexor_value = 0;

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