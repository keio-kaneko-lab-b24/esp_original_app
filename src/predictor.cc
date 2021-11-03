#include <Arduino.h>

#include "predictor.h"

// MLの出力から判定を行う場合
motion PredictML(
    float extensor_score,
    float flexor_score,
    float extensor_threshold,
    float flexor_threshold)
{
    if ((extensor_score > extensor_threshold) & (flexor_score > flexor_threshold))
    {
        return NONE;
    }
    else if (extensor_score > extensor_threshold)
    {
        return PAPER;
    }
    else if (flexor_score > flexor_threshold)
    {
        return ROCK;
    }
    return NONE;
}

// RMSと閾値から判定を行う場合
motion PredictThreshold(
    float extensor_value,
    float flexor_value,
    float rock_flexor_lower_limit,
    float rock_extensor_upper_limit,
    float paper_extensor_lower_limit,
    float paper_flexor_upper_limit)
{
    boolean paper = false;
    boolean rock = false;
    if ((extensor_value > paper_extensor_lower_limit) & (flexor_value < paper_flexor_upper_limit))
    {
        paper = true;
    }
    if ((flexor_value > rock_flexor_lower_limit) & (extensor_value < rock_extensor_upper_limit))
    {
        rock = true;
    }

    // paperにもrockにも判定された場合はNONEを返す。
    if ((paper == true) & (rock == false))
    {
        printf("predict paper");
        return PAPER;
    }
    else if ((rock == true) & (paper == false))
    {
        printf("predict rock");
        return ROCK;
    }
    else
    {
        return NONE;
    }
}