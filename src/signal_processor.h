#ifndef SIGNAL_PROCESSOR_H_
#define SIGNAL_PROCESSOR_H_

extern long last_process_millis;
extern int process_count;

extern bool SignalProcess(
    int r_extensor_data[],
    int r_flexor_data[],
    const int RAW_LENGTH);

extern void ArrangeArray(
    volatile int r_extensor_data[],
    volatile int r_flexor_data[],
    int ar_extensor_data[],
    int ar_flexor_data[],
    int begin_index,
    const int RAW_LENGTH);

extern void RollingAverage(
    float a_extensor_data[],
    float a_flexor_data[],
    float m_extensor_data[],
    float m_flexor_data[],
    const int RAW_LENGTH);

extern void Normalization(
    int ar_extensor_data[],
    int ar_flexor_data[],
    float b_extensor_data[],
    float b_flexor_data[],
    float extensor_mean,
    float flexor_mean,
    const int RAW_LENGTH);

extern float Max(
    float data[],
    int length);

extern float Min(
    float data[],
    int length);

extern int Max(
    int data[],
    int length);

extern int Min(
    int data[],
    int length);

extern int Mean(
    int data[],
    int length);

#endif // SIGNAL_PROCESSOR_H_