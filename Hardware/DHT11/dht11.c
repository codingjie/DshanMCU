#include "dht11.h"

// DWT初始化
static void DWT_Init(void) {
    CoreDebug -> DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable DWT
    DWT -> CYCCNT = 0; // Reset the cycle counter
    DWT -> CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Enable the cycle counter
}

// 微妙延时
static void Delay_us(uint32_t us) {
    uint32_t start = DWT -> CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while((DWT -> CYCCNT - start) < ticks);
}

// 设置为输出模式
static void DHT11_SetOutput(void) {
    // CNF=00(推挽), MODE=11(50MHz输出)
    DHT11_GPIO_Port->CRL &= ~(0xF << (DHT11_PIN_NUM * 4));
    DHT11_GPIO_Port->CRL |= (0x3 << (DHT11_PIN_NUM * 4));
}

// 设置为输入模式
static void DHT11_SetInput(void) {
    // CNF=10(上拉/下拉输入), MODE=00(输入)
    DHT11_GPIO_Port->CRL &= ~(0xF << (DHT11_PIN_NUM * 4));
    DHT11_GPIO_Port->CRL |= (0x8 << (DHT11_PIN_NUM * 4));
}

// 读取一个字节
static uint8_t DHT11_ReadByte(void) {
    uint8_t data = 0;
    uint8_t retry; // 重复次数
    for (uint8_t i = 0; i < 8; i++) {
        retry = 0;
        while(!DHT11_READ() && retry < 100) { // 等待低电平结束
            Delay_us(1);
            retry++;
        }
        Delay_us(40); // 延时40us
        data <<= 1;
        if(DHT11_READ()) { // 如果是高电平
            data |= 1; // 记录为1
        }
        retry = 0;
        while(DHT11_READ() && retry < 100) { // 等待高电平结束
            Delay_us(1);
            retry++;    
        }
    }
    return data;
}

void DHT11_Init(void) {
    DWT_Init();
    DHT11_HIGH();
    HAL_Delay(1000); // 等待传感器稳定
}

// 读取数据
uint8_t DHT11_ReadData(DHT11_Data *data) {
    uint8_t buf[5];
    uint8_t retry = 0;
    // 主机起始信号
    DHT11_SetOutput();
    DHT11_LOW();
    HAL_Delay(20); // 拉低至少18ms
    DHT11_HIGH();
    Delay_us(30); // 拉高20-40us
    // 切换输入模式
    DHT11_SetInput();
    // 检测DHT11响应
    if (DHT11_READ()) {
        return 1; // 响应失败
    }
    // 等待80us低电平
    retry = 0;
    while(!DHT11_READ() && retry < 100) { // 等待DHT11拉低
        Delay_us(1);
        retry++;
    }
    if (retry >= 100) {
        return 2; // 响应超时
    }
    // 等待80us高电平
    retry = 0;
    while(DHT11_READ() && retry < 100) { // 等待DHT11拉高
        Delay_us(1);
        retry++;
    }
    if (retry >= 100) {
        return 3; // 响应超时
    }
    // 读取5个字节数据
    for (uint8_t i = 0; i < 5; i++) {
        buf[i] = DHT11_ReadByte();
    }
    // 恢复输出高电平
    DHT11_SetOutput();
    DHT11_HIGH();
    // 校验和验证
    if (buf[4] == (buf[0] + buf[1] + buf[2] + buf[3])) {
        data -> humidity_int = buf[0];
        data -> humidity_dec = buf[1];
        data -> temperature_int = buf[2];
        data -> temperature_dec = buf[3];
        return 0; // 读取成功
    }
    return 4; // 校验和错误
}
