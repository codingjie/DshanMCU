#include "delay.h"

/**
 * @brief  初始化 DWT CYCCNT
 * @note   在 SystemClock_Config() 之后调用
 */
void delay_init(void) {
    /* 使能 DWT 外设 */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    /* 复位计数器 */
    DWT->CYCCNT = 0;
    /* 使能 CYCCNT */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
 * @brief  微秒延时（基于 CPU 主频）
 */
void delay_us(uint32_t us) {
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000U);

    while ((DWT->CYCCNT - start) < ticks);
}

/**
 * @brief  毫秒延时
 */
void delay_ms(uint32_t ms) {
    while (ms--) {
        delay_us(1000);
    }
}
