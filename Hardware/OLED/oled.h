#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "i2c.h"
#include "stdlib.h"
#include "oledfont.h"

#define OLED_ADDR 0x78 // OLED I2C地址（0X3C << 1）

void OLED_Init(void);
void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Fill(void);
void OLED_Clear(void);
void OLED_Dispay_On(void);
void OLED_Dispay_Off(void);

void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char* str, uint8_t size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t intLen, uint8_t decLen, uint8_t size);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num);

#endif
