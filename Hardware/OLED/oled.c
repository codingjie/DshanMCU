#include "oled.h"

// 初始化OLED屏幕
void OLED_Init(void) {
    uint8_t CMD_Data[] = { // 设备初始化命令数据
        0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
        0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
        0xD8, 0x30, 0x8D, 0x14, 0xAF
    };

    for(uint8_t i = 0; i < 27; i++) { // 将初始化数据写入OLED
        HAL_I2C_Mem_Write(&hi2c1 , 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, CMD_Data + i, 1, 0x100);
    }
}

// 写命令
void OLED_WriteCmd(uint8_t cmd) {
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

// 写数据
void OLED_WriteData(uint8_t data) {
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

// 在OLED上设置显示位置
void OLED_SetPos(uint8_t x, uint8_t y) {
    OLED_WriteCmd(0xB0 + y);                 // 设置页地址（0~7）
    OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10); // 设置列地址高4位（0x10~0x1F）
    OLED_WriteCmd(x & 0x0F);                 // 设置列地址低4位（0x00~0x0F）
}

// 屏幕全亮
void OLED_Fill(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_WriteCmd(0xB0 + page); // 设置页地址
        OLED_WriteCmd(0x00);        // 设置列地址低4位
        OLED_WriteCmd(0x10);        // 设置列地址高4位
        for (uint8_t col = 0; col < 128; col++) {
            OLED_WriteData(0xFF);    // 写入数据
        }
    }
}

// 清屏函数
void OLED_Clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_WriteCmd(0xB0 + page); // 设置页地址
        OLED_WriteCmd(0x00);        // 设置列地址低4位
        OLED_WriteCmd(0x10);        // 设置列地址高4位
        for (uint8_t col = 0; col < 128; col++) {
            OLED_WriteData(0x00);    // 写入数据
        }
    }
}

// 打开显示
void OLED_Dispay_On(void) {
    OLED_WriteCmd(0x8D); // 设置电荷泵
    OLED_WriteCmd(0x14); // 使能电荷泵
    OLED_WriteCmd(0xAF); // 打开显示
}

// 关闭显示
void OLED_Dispay_Off(void) {
    OLED_WriteCmd(0x8D); // 设置电荷泵
    OLED_WriteCmd(0x10); // 禁用电荷泵
    OLED_WriteCmd(0xAE); // 关闭显示
}

/**
* @brief 在指定位置显示一个字符,包括部分字符
* @param x: x轴起点坐标 0~127
* @param y: y轴起点坐标 0~7页面坐标
* @param chr: 字符
* @param size: 选择字体
* @retval None
*/
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size) {
    uint8_t c = chr - ' ';
    uint8_t i = 0;
    if (size == 16) { // 8x16
        OLED_SetPos(x, y);
        for (i = 0; i < 8; i++) {
            OLED_WriteData(asc2_1608[c][i]);
        }
        OLED_SetPos(x, y + 1);
        for (i = 0; i < 8; i++) {
            OLED_WriteData(asc2_1608[c][i + 8]);
        }
    }
    else {
        // 其他字体大小可以在此添加
    }
}

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, char* str, uint8_t size) {
    while(*str != '\0') {
        OLED_ShowChar(x, y, *str, size);
        x += 8;
        if (x > 120) { // 换行处理
            x = 0;
            y += 2;
        }
        str++;
    }
}

/**
* @brief 显示2个数字
* @param x: x轴起点坐标
* @param y: y轴起点坐标
* @param num: 数值(0~4294967295)
* @param len: 数字的位数
* @param size: 字体大小
* @retval None
*/
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size) {
    uint8_t k; // 用于遍历每一位
    uint8_t temp; // 存储当前处理的数字
    uint8_t enshow = 0; // 用于控制前导零的显示
    uint8_t pow = 1; // 存储最高位的权值
    for (k = 0; k < len - 1; k++) {
        pow *= 10;
    }
    for (k = 0; k < len; k++) {
        temp = (num / pow) % 10; // 数字依次从高位取到低位
        pow /= 10;
        if (enshow == 0 && k < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size / 2) * k, y, ' ', size); // 显示空格
                continue;
            }
            else {
                enshow = 1; // 遇到第一个非零数字，开始显示后续数字
            }
        }
        OLED_ShowChar(x + (size / 2) * k, y, temp + '0', size); // 显示数字字符
    }
}

/**
* @brief 显示浮点数
* @param x: x轴起点坐标
* @param y: y轴起点坐标
* @param num: 浮点数
* @param intLen: 整数部分位数
* @param decLen: 小数部分位数
* @param size: 字体大小
* @retval None
*/
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t intLen, uint8_t decLen, uint8_t size) {
    uint32_t intPart, decPart;
    uint8_t width = size / 2;
    // 处理负数
    if (num < 0) {
        OLED_ShowChar(x, y, '-', size);
        x += width;
        num = -num;
    }
    intPart = (uint32_t) num; // 获取整数部分
    // 计算小数部分
    float decTemp = num - intPart;
    for (uint8_t i = 0; i < decLen; i++) {
        decTemp *= 10;
    }
    decPart = (uint32_t)(decTemp + 0.5f); // 四舍五入获取小数部分
    // 显示整数部分
    OLED_ShowNum(x, y, intPart, intLen, size);
    x += width * intLen;
    // 显示小数点
    OLED_ShowChar(x, y, '.', size);
    x += width;
    // 显示小数部分（含前导零）
    uint32_t pow = 1;
    for (uint8_t i = 0; i < decLen - 1; i++) {
        pow *= 10;
    }
    for (uint8_t i = 0; i < decLen; i++) {
        OLED_ShowChar(x + width * i, y, (decPart / pow) % 10 + '0', size);
        pow /= 10;
    }
}

/**
* @brief 显示汉字
* @param x: x轴起点坐标
* @param y: y轴起点坐标
* @param num: 汉字索引
* @retval None
*/
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num) {
    uint8_t i = 0;
    OLED_SetPos(x, y);
    for (i = 0; i < 16; i++) {
        OLED_WriteData(Hzk[num * 2][i]);
    }
    OLED_SetPos(x, y + 1);
    for (i = 0; i < 16; i++) {
        OLED_WriteData(Hzk[num * 2 + 1][i]);
    }
}
