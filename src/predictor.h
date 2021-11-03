#ifndef PREDICTOR_H_
#define PREDICTOR_H_

#include "motion.h"

extern motion PredictML(
    float extensor_score,
    float flexor_score,
    float extensor_threshold,
    float flexor_threshold);

motion PredictThreshold(
    float extensor_value,
    float flexor_value,
    float rock_flexor_lower_limit,
    float rock_extensor_upper_limit,
    float paper_extensor_lower_limit,
    float paper_flexor_upper_limit);

#endif