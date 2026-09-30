#include "health_sensor.h"
#include "max30102.h"
#include "my_delay.h"
#include "algorithm.h"
#include "HWDataAccess.h"


#define HEALTH_SAMPLE_PERIOD  (1000 / FS)

static uint32_t red_buffer[BUFFER_SIZE];
static uint32_t ir_buffer[BUFFER_SIZE];

bool HealthSensor_Init(void)
{
    return MAX30102_Init();
}

/**
 * @brief 从 MAX30102 读取健康数据
 * 
 * @param result 存储健康数据的结构体指针
 * @return true 读取成功
 * @return false 读取失败
 */
bool HealthSensor_Read(HealthData_t *result)
{
    if (result == NULL)
    {
        return false;
    }

    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        if (!HWInterface.MAX30102.GetI_RData())
        {
            return false;
        }
        red_buffer[i] = HWInterface.MAX30102.data.red;
        ir_buffer[i] = HWInterface.MAX30102.data.ir;
        delay_ms(HEALTH_SAMPLE_PERIOD);
    }

    return HealthAlgorithm_Process(
        red_buffer,
        ir_buffer,
        BUFFER_SIZE,
        result
    );
}