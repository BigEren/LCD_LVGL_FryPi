// #ifndef __STEP_COUNTER_H__
// #define __STEP_COUNTER_H__

// #include "stdint.h"
// #include "stdbool.h"
// #include "string.h"
// #include "mpu6050.h"

// /**
//  * @brief 步数计数器结构体
//  * @param step_count 步数计数
//  * @param last_magnitude 上一次加速度合成量
//  * @param above_threshold 是否超过阈值
//  */
// typedef struct
// {
//     uint32_t step_count;

//     float last_magnitude;
    
//     uint8_t above_threshold;

//     uint8_t interval_count;
// } StepCounter_t;

// bool StepCounter_Init(StepCounter_t *counter);
// bool StepCounter_Update(StepCounter_t *counter, MPU6050_Data_t *mpu);
// uint32_t StepCounter_GetSteps(StepCounter_t *counter);


// #endif /* __STEP_COUNTER_H__ */
