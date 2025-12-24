#include "obstacle.h"

// 检测是否有障碍物
uint8_t Obstacle_Detected(void) {
    return (HAL_GPIO_ReadPin(IR_OBS_GPIO_Port, IR_OBS_Pin) == GPIO_PIN_RESET);
}
