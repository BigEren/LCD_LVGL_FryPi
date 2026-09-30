#ifndef __HEALTH_ALGORITHM_H__
#define __HEALTH_ALGORITHM_H__

#include "stdint.h"
#include "stdbool.h"

/**
 * @brief 健康数据结构体
 * @param heart_rate 心率
 * @param spo2 血氧值
 * @param heart_rate_valid 心率是否有效
 * @param spo2_valid 血氧值是否有效
 */
typedef struct
{
    int32_t heart_rate;
    int8_t heart_rate_valid;

    int32_t spo2;
    int8_t spo2_valid;

} HealthData_t;

bool HealthAlgorithm_Process(uint32_t *red_buffer,
                               uint32_t *ir_buffer,
                               uint16_t sample_count,
                               HealthData_t *result);

#endif /*__HEALTH_ALGORITHM_H__*/
