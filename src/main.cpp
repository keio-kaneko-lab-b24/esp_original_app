#include <Arduino.h>

#include "main.h"
#include "ble.h"

#include "emg.h"

TaskHandle_t TaskIO;
TaskHandle_t TaskMain;

// セマフォ
SemaphoreHandle_t xMutex = NULL;

char main_s[64];
long last_sample_micros = 0;
long last_process_micros = 0;

volatile float extensor_value = 0;
volatile float flexor_value = 0;
volatile float rock_flexor_lower_limit = 0;
volatile float rock_extensor_upper_limit = 0;
volatile float paper_extensor_lower_limit = 0;
volatile float paper_flexor_upper_limit = 0;

// IOスレッド
void TaskIOcode(void *pvParameters)
{
  for (;;)
  {
    // 1000Hz
    if ((micros() - last_sample_micros) < 1 * 1000)
    {
      continue;
    }
    last_sample_micros = micros();

    UpdateBLEConnection();

    // ウォッチドッグのために必要
    // https://github.com/espressif/arduino-esp32/issues/3001
    // https://lang-ship.com/blog/work/esp32-freertos-l03-multitask/#toc12
    vTaskDelay(1);

    // 整列処理
    if (xSemaphoreTake(xMutex, (portTickType)100) == pdTRUE)
    {
      xSemaphoreGive(xMutex);
    }
  }
};

// Mainスレッド
void TaskMaincode(void *pvParameters)
{
  for (;;)
  {
    // 4Hz
    if ((micros() - last_process_micros) < 250 * 1000)
    {
      continue;
    }
    last_process_micros = micros();

    // ウォッチドッグのために必要
    // https://github.com/espressif/arduino-esp32/issues/3001
    // https://lang-ship.com/blog/work/esp32-freertos-l03-multitask/#toc12
    vTaskDelay(1);

    if (xSemaphoreTake(xMutex, (portTickType)100) == pdTRUE)
    {
      // do nothing
    }
  }
};

void setup()
{
  delay(3000);
  Serial.begin(115200);

  SetUpBLE();

  xMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(TaskIOcode, "TaskIO", 4096, NULL, 2, &TaskIO, 0); //Task1実行
  delay(500);
  xTaskCreatePinnedToCore(TaskMaincode, "TaskMain", 4096, NULL, 2, &TaskMain, 1); //Task2実行
  delay(500);

  Serial.println("[setup] finished.");
}

void loop()
{
  // loopは使用しないので、削除する
  vTaskDelete(NULL);
}