#include "light_sensor.h"

extern ADC_HandleTypeDef hadc1; // 引用CubeMx生成的ADC句柄

// 读取DO引脚（数字输出）
// 返回值：0=光强，1=光弱
uint8_t LightSensor_ReadDO(void) {
    return HAL_GPIO_ReadPin(LIGHT_DO_GPIO_Port, LIGHT_DO_Pin);
}

// 读取AO引脚（模拟输出）
// 返回值：0~4095，值越大光越暗
uint8_t LightSensor_ReadAO(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}
