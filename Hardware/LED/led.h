#ifndef __LED_H
#define __LED_H

#include "main.h"

// 单色控制
void LED_Red_On(void);
void LED_Red_Off(void);
void LED_Green_On(void);
void LED_Green_Off(void);
void LED_Blue_On(void);
void LED_Blue_Off(void);

// 组合颜色
void LED_White_On(void);   // 白色：RGB全亮
void LED_Yellow_On(void);  // 黄色：RG亮
void LED_Cyan_On(void);    // 青色：GB亮
void LED_Purple_On(void);  // 紫色：RB亮
void LED_Off(void);        // 全灭

#endif
