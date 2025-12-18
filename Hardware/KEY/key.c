#include "key.h"

// 获取按键状态
uint8_t Key_Read(void) {
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) { // 低电平
            return 1;  // 按下
        }
    }
    return 0;  // 未按下
}

// 按键中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_14) { // PB14按键按下
		static uint8_t state = 0;

        LED_Off();
        switch(state) {
            case 0: LED_Red_On(); break;
            case 1: LED_Green_On(); break;
            case 2: LED_Blue_On(); break;
            case 3: LED_White_On(); break;
            case 4: LED_Yellow_On(); break;
            case 5: LED_Cyan_On(); break;
            case 6: LED_Purple_On(); break;
            case 7: break;
        }

		state = (state + 1) % 8;
	}
}
