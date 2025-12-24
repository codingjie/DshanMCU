#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define DHT11_PIN_NUM 1 // PA1引脚

#define DHT11_HIGH() (DHT11_GPIO_Port -> BSRR = DHT11_Pin)
#define DHT11_LOW()  (DHT11_GPIO_Port -> BRR = DHT11_Pin)
#define DHT11_READ() (DHT11_GPIO_Port -> IDR & DHT11_Pin)

typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
} DHT11_Data;

void DHT11_Init(void);
uint8_t DHT11_ReadData(DHT11_Data *data);

#endif
