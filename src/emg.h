#ifndef EMG_H_
#define EMG_H_

// 共通：信号処理後の値
extern volatile float extensor_value;
extern volatile float flexor_value;

// 閾値判定：閾値
extern volatile float rock_flexor_lower_limit;
extern volatile float rock_extensor_upper_limit;
extern volatile float paper_extensor_lower_limit;
extern volatile float paper_flexor_upper_limit;

// AI判定：閾値
// TODO

extern void getRMS(
    std::string value);

extern void getThreshold(
    std::string value);

#endif // EMG_H_