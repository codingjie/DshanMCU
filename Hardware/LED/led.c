#include "led.h"

void LED_Red_On(void) {
    HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_RESET);
}

void LED_Red_Off(void) {
    HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_SET);
}

void LED_Green_On(void) {
    HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_RESET);
}

void LED_Green_Off(void) {
    HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_SET);
}

void LED_Blue_On(void) {
    HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_RESET);
}

void LED_Blue_Off(void) {
    HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_SET);
}

void LED_White_On(void) {
    LED_Red_On();
    LED_Green_On();
    LED_Blue_On();
}

void LED_Yellow_On(void) {
    LED_Red_On();
    LED_Green_On();
    LED_Blue_Off();
}

void LED_Cyan_On(void) {
    LED_Red_Off();
    LED_Green_On();
    LED_Blue_On();
}

void LED_Purple_On(void) {
    LED_Red_On();
    LED_Green_Off();
    LED_Blue_On();
}

void LED_Off(void) {
    LED_Red_Off();
    LED_Green_Off();
    LED_Blue_Off();
}
