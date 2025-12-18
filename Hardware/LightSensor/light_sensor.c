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

/******************************************************************
 * 函 数 说 明：读取光敏电阻值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 备      注：最亮100  最暗0
******************************************************************/
unsigned int Get_illume_Percentage_value(void) {
    //ADC精度都是12位
    //2的12次方 = 4096
    //因为单片机是从0开始算，所以要4096-1=4095
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = Get_Adc_Value(10);
    //百分比 = （ 当前值 / 最大值 ）* 100
    Percentage_value = ( 1 - ( (float)adc_new / adc_max ) ) * 100;
    return Percentage_value;
}
