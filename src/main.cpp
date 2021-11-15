#include <Arduino.h>

#include "main.h"
#include "ble.h"
#include "output_handler.h"
#include "motion.h"
#include "predictor.h"
#include "emg.h"

TaskHandle_t TaskIO;
TaskHandle_t TaskMain;

// セマフォ
SemaphoreHandle_t xMutex = NULL;

char main_s[64];
long last_sample_micros = 0;
long last_process_micros = 0;

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

    if (xSemaphoreTake(xMutex, (portTickType)100) == pdTRUE)
    {
      // ブロックが必要な処理
      xSemaphoreGive(xMutex);
    }
  }
};

// Mainスレッド
void TaskMaincode(void *pvParameters)
{
  for (;;)
  {
    // 10Hz
    if ((micros() - last_process_micros) < 100 * 1000)
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
      // ブロックが必要な処理
      xSemaphoreGive(xMutex);
    }

    // 閾値判定
    motion motion = NONE;
    motion = PredictThreshold(
        extensor_value,
        flexor_value,
        rock_flexor_lower_limit,
        rock_extensor_upper_limit,
        paper_extensor_lower_limit,
        paper_flexor_upper_limit);

    // ロボットへ出力
    HandleOutput(motion);
  }
};

void setup()
{
  delay(3000);
  Serial.begin(115200);

  SetUpBLE();

  OutputSetup();

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