#include <Arduino.h>
#include <stdlib.h>

#include "constants.h"
#include "emg.h"
#include "signal_processor.h"

char sp_s[64];

float *b_extensor_data = NULL;
float *b_flexor_data = NULL;

float *m_extensor_data = NULL;
float *m_flexor_data = NULL;

bool SignalProcess(int ar_extensor_data[],
                   int ar_flexor_data[],
                   const int RAW_LENGTH)
{
    // 1.正規化+ABS
    b_extensor_data = (float *)pvPortMalloc(sizeof(float) * RAW_LENGTH);
    b_flexor_data = (float *)pvPortMalloc(sizeof(float) * RAW_LENGTH);

    float extensor_mean = Mean(ar_extensor_data, RAW_LENGTH);
    float flexor_mean = Mean(ar_flexor_data, RAW_LENGTH);
    Normalization(
        ar_extensor_data,
        ar_flexor_data,
        b_extensor_data,
        b_flexor_data,
        extensor_mean,
        flexor_mean,
        RAW_LENGTH);

    // 2.移動平均
    m_extensor_data = (float *)pvPortMalloc(sizeof(float) * RAW_LENGTH);
    m_flexor_data = (float *)pvPortMalloc(sizeof(float) * RAW_LENGTH);
    RollingAverage(
        b_extensor_data,
        b_flexor_data,
        m_extensor_data,
        m_flexor_data,
        RAW_LENGTH);
    vPortFree(b_extensor_data);
    vPortFree(b_flexor_data);

    // 結果
    extensor_value = Max(m_extensor_data, RAW_LENGTH);
    flexor_value = Max(m_flexor_data, RAW_LENGTH);
    vPortFree(m_extensor_data);
    vPortFree(m_flexor_data);

    sprintf(sp_s, "e_sp: %f\nf_sp: %f", extensor_value, flexor_value);
    Serial.println(sp_s);

    return true;
}

/**
 * 整列
 */
void ArrangeArray(
    volatile int r_extensor_data[],
    volatile int r_flexor_data[],
    int ar_extensor_data[],
    int ar_flexor_data[],
    int begin_index,
    const int RAW_LENGTH)
{
    for (int i = 0; i < RAW_LENGTH; ++i)
    {
        int ring_array_index = begin_index + i - RAW_LENGTH;
        if (ring_array_index < 0)
        {
            ring_array_index += RAW_LENGTH;
        }
        ar_extensor_data[i] = r_extensor_data[ring_array_index];
        ar_flexor_data[i] = r_flexor_data[ring_array_index];
    }
}

/**
 * フィルタ＋正規化
 */
void Normalization(
    int ar_extensor_data[],
    int ar_flexor_data[],
    float b_extensor_data[],
    float b_flexor_data[],
    float extensor_mean,
    float flexor_mean,
    const int RAW_LENGTH)
{
    for (int i = 0; i < RAW_LENGTH; ++i)
    {

        float f_extensor = (float)ar_extensor_data[i];
        float f_flexor = (float)ar_flexor_data[i];

        // 正規化
        b_extensor_data[i] = f_extensor;
        b_flexor_data[i] = f_flexor;
        b_extensor_data[i] = abs(f_extensor - extensor_mean);
        b_flexor_data[i] = abs(f_flexor - flexor_mean);
    }
}

/**
 * 移動平均
 */
void RollingAverage(
    float b_extensor_data[],
    float b_flexor_data[],
    float m_extensor_data[],
    float m_flexor_data[],
    const int RAW_LENGTH)
{
    float extensor_sum = 0;
    float flexor_sum = 0;
    int m_i = 0;

    for (int i = 0; i < WINDOW_SIZE; i++)
    {
        extensor_sum += b_extensor_data[i];
        flexor_sum += b_flexor_data[i];
        if (i >= (WINDOW_SIZE / 2) - 1)
        {
            m_extensor_data[m_i] = extensor_sum / (i + 1);
            m_flexor_data[m_i] = flexor_sum / (i + 1);
            m_i += 1;
        }
    }

    for (int i = WINDOW_SIZE; i < RAW_LENGTH; i++)
    {
        extensor_sum -= b_extensor_data[i - WINDOW_SIZE];
        extensor_sum += b_extensor_data[i];
        m_extensor_data[m_i] = extensor_sum / WINDOW_SIZE;
        flexor_sum -= b_flexor_data[i - WINDOW_SIZE];
        flexor_sum += b_flexor_data[i];
        m_flexor_data[m_i] = flexor_sum / WINDOW_SIZE;
        m_i += 1;
    }

    for (int i = RAW_LENGTH; i < RAW_LENGTH + (WINDOW_SIZE / 2); i++)
    {
        extensor_sum -= b_extensor_data[i - WINDOW_SIZE];
        m_extensor_data[m_i] = extensor_sum / (WINDOW_SIZE - (i - RAW_LENGTH) + 1);
        flexor_sum -= b_flexor_data[i - WINDOW_SIZE];
        m_flexor_data[m_i] = flexor_sum / (WINDOW_SIZE - (i - RAW_LENGTH) + 1);
        m_i += 1;
        if (m_i >= RAW_LENGTH)
        {
            break;
        }
    }
}

// Max, Min取得（別ファイルにしたい）
float Max(
    float data[],
    int length)
{
    float cur_max = -99999;
    for (int i = 0; i < length; i++)
    {
        if (cur_max < data[i])
        {
            cur_max = data[i];
        }
    }
    return cur_max;
}

float Min(
    float data[],
    int length)
{
    float cur_min = 99999;
    for (int i = 0; i < length; i++)
    {
        if (cur_min > data[i])
        {
            cur_min = data[i];
        }
    }
    return cur_min;
}

int Mean(
    int data[],
    int length)
{
    int total = 0;
    for (int i = 0; i < length; i++)
    {
        total += data[i];
    }
    float mean_value = (float)total / (float)length;
    return mean_value;
}