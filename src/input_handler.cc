#include <Arduino.h>

#include "input_handler.h"

#define MD_IN1 34 // GPIO #34
#define MD_IN2 35 // GPIO #35

char handle_input_s[64];

bool HandleInput(volatile int r_extensor_data[],
                 volatile int r_flexor_data[],
                 volatile int begin_index,
                 const int RAW_LENGTH)
{

    int a, b;
    b = analogRead(MD_IN1);
    a = analogRead(MD_IN2);
    r_extensor_data[begin_index] = a;
    r_flexor_data[begin_index] = b;

    // （モニタリング用）筋電を出力する
    if (begin_index % 10 == 0)
    {
        unsigned long currentMillis = xTaskGetTickCount();
        sprintf(
            handle_input_s,
            "t: %lu\ni: %d\ne: %d\nf: %d\n",
            currentMillis, begin_index, r_extensor_data[begin_index], r_flexor_data[begin_index]);
        Serial.println(handle_input_s);
    }

    return true;
}