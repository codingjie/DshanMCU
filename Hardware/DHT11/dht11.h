#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"
#include "sys.h"
#include "delay.h"

#define DHT11_PIN_NUM 1 // PA1引脚

#define DHT11_HIGH() PAout(DHT11_PIN_NUM) = 1
#define DHT11_LOW()  PAout(DHT11_PIN_NUM) = 0
#define DHT11_READ() PAin(DHT11_PIN_NUM)

typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
} DHT11_Data;

uint8_t DHT11_ReadData(DHT11_Data *data);

#endif
