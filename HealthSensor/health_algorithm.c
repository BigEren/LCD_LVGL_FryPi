#include "health_algorithm.h"
#include "algorithm.h"
#include <stdio.h>

/**
 * @brief  处理 MAX30102 数据
 * @param red_buffer 红光数据缓冲区
 * @param ir_buffer 红外光数据缓冲区
 * @param sample_count 样本数量
 * @param result 结果指针
 * @retval 状态
 */
bool HealthAlgorithm_Process(uint32_t *red_buffer,
                             uint32_t *ir_buffer,
                             uint16_t sample_count,
                             HealthData_t *result)
{
    if (red_buffer == NULL ||
        ir_buffer == NULL ||
        result == NULL)
    {
        return false;
    }

    /*
     * Maxim算法内部数组大小由 BUFFER_SIZE 决定，
     * 当前为 150。
     */
    if (sample_count != BUFFER_SIZE)
    {
        return false;
    }

    /*
     * 调用 Maxim 官方心率 + 血氧算法
     */
    maxim_heart_rate_and_oxygen_saturation(
        ir_buffer,
        sample_count,
        red_buffer,
        &(result->spo2),
        &(result->spo2_valid),
        &(result->heart_rate),
        &(result->heart_rate_valid)
    );
    
    return true;
}
