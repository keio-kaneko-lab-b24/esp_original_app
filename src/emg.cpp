#include <Arduino.h>

#include "emg.h"

volatile float extensor_value = 0;
volatile float flexor_value = 0;

volatile float rock_flexor_lower_limit = 0;
volatile float rock_extensor_upper_limit = 0;
volatile float paper_extensor_lower_limit = 0;
volatile float paper_flexor_upper_limit = 0;

// RMS値を更新する
// @value （例）"0.012345, F: 0.056789, E5"
void updataRMSFromString(
    std::string value)
{
    size_t pos = 0;
    std::string delimiter = ",";

    // TODO: もっとよい書き方
    // ExtensorのRMS値を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string e_token = value.substr(2, 100); // 多めに100文字目まで取得
        const char *e_str = e_token.c_str();
        extensor_value = atof(e_str);
        value.erase(0, pos + delimiter.length());
        break;
    }

    // FlexorのRMS値を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string f_token = value.substr(3, 100); // 多めに100文字目まで取得
        const char *f_str = f_token.c_str();
        flexor_value = atof(f_str);
        value.erase(0, pos + delimiter.length());
        break;
    }
}

// 閾値判定の閾値を更新
// @value （例）"RE:LT0.001,RF:GT0.002,PE:GT0.003,PF:LT0.001"
void updateThresholdFromString(
    std::string value)
{
    size_t pos = 0;
    std::string delimiter = ",";

    // TODO: もっとよい書き方
    // RockのExtensor上限を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string e_token = value.substr(5, 100); // 多めに100文字目まで取得
        const char *e_str = e_token.c_str();
        if (atof(e_str) < 0)
        {
            rock_extensor_upper_limit = 99999;
        }
        else
        {
            rock_extensor_upper_limit = atof(e_str);
        }
        value.erase(0, pos + delimiter.length());
        break;
    }

    // RockのFlexor下限を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string f_token = value.substr(5, 100); // 多めに100文字目まで取得
        const char *f_str = f_token.c_str();
        rock_flexor_lower_limit = atof(f_str);
        value.erase(0, pos + delimiter.length());
        break;
    }

    // PaperのExtensor下限を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string e_token = value.substr(5, 100); // 多めに100文字目まで取得
        const char *e_str = e_token.c_str();
        paper_extensor_lower_limit = atof(e_str);
        value.erase(0, pos + delimiter.length());
        break;
    }

    // PaperのFlexor上限を取得
    std::string f_token = value.substr(5, 100); // 多めに100文字目まで取得
    const char *f_str = f_token.c_str();
    if (atof(f_str) < 0)
    {
        paper_flexor_upper_limit = 99999;
    }
    else
    {
        paper_flexor_upper_limit = atof(f_str);
    }
    value.erase(0, pos + delimiter.length());
}