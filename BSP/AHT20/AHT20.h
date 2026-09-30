#ifndef __AHT20_H__
#define __AHT20_H__

#include "main.h"
#include "stdbool.h"

/* AHT20 7-bit I2C 地址 */
#define AHT20_ADDR                  0x38

/* AHT20 命令 */
// 初始化
#define AHT20_CMD_INITIALIZE        0xBE
// 触发测量
#define AHT20_CMD_TRIGGER_MEASURE   0xAC
// 复位
#define AHT20_CMD_RESET             0xBA

/*
 * AHT20 数据结构体
 * @param temperature 温度数据
 * @param humidity 湿度数据
 */
typedef struct {
    float temperature;
    float humidity;
} AHT20_Data_t;

bool AHT_Read_Status(uint8_t *status);
bool AHT_Reset(void);
bool AHT20_Init(void);
bool AHT20_Read_Data(AHT20_Data_t *data);

#endif /* __AHT20_H__ */
