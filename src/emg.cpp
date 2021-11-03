#include <Arduino.h>

#include "ble.h"
#include "emg.h"

void getRMS(
    std::string value)
{
    size_t pos = 0;
    std::string delimiter = ",";

    // ExtensorのRMS値を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string e_token = value.substr(3, 100); // 多めに100文字目まで取得
        const char *e_str = e_token.c_str();
        extensor_value = atof(e_str);
        value.erase(0, pos + delimiter.length());
        break;
    }

    // FlexorのRMS値を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string f_token = value.substr(4, 100); // 多めに100文字目まで取得
        const char *f_str = f_token.c_str();
        flexor_value = atof(f_str);
        value.erase(0, pos + delimiter.length());
        break;
    }
}

void getThreshold(
    std::string value)
{
    size_t pos = 0;
    std::string delimiter = ",";

    // RockのExtensor上限を取得
    while ((pos = value.find(delimiter)) != std::string::npos)
    {
        std::string e_token = value.substr(5, 100); // 多めに100文字目まで取得
        const char *e_str = e_token.c_str();
        rock_extensor_upper_limit = atof(e_str);
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
    paper_flexor_upper_limit = atof(f_str);
    value.erase(0, pos + delimiter.length());
}