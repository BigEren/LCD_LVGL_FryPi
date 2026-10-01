#ifndef __GY39_H__
#define __GY39_H__

#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "my_iic_hal.h"

/*==================== GY39地址 ====================*/

#define GY39_ADDR                 0x5B


/*==================== 数据结构 ===================*/

/**
 * @brief GY39 数据结构
 * 
 * @param lux 光强luxv 
 * @param temp 温度°C 
 * @param hum 湿度%RH 
 * @param press 压力hPa 
 * @param alt 海拔高度m 
 */
typedef struct
{
    float lux;      // 光强lux
    float temp;     // 温度°C
    float hum;      // 湿度%RH
    float press;    // 压力hPa
    int16_t alt;    // 海拔高度m

} GY39_Data_t;

/*==================== BSP函数接口 ====================*/

bool GY39_Init(void);
bool GY39_ReadData(GY39_Data_t *data);


#endif /*__GY39_H__*/
