#include "step_conter.h"
#include "math.h"

#define ACCEL_1G 16384.0f

/* 运动幅度阈值 */
#define STEP_HIGH_THRESHOLD 1800.0f
#define STEP_LOW_THRESHOLD  1200.0f


/* 两步之间最小间隔：50Hz下，25个采样点=500ms */
#define STEP_MIN_INTERVAL       25

/**
 * @brief 初始化步数计数器
 * @param counter 步数计数器指针
 */
bool StepCounter_Init(StepCounter_t *counter)
{
    if (counter == NULL)
    {
        return false;
    }
    counter->step_count = 0;
    counter->last_magnitude = 0.0f;
    counter->above_threshold = 0;
    counter->interval_count = STEP_MIN_INTERVAL;
    return true;
}

/**
 * @brief 更新步数计数器
 * @param counter 步数计数器指针
 * @param mpu MPU6050 数据指针
 */
bool StepCounter_Update(StepCounter_t *counter, MPU6050_Data_t *mpu)
{
    if (counter == NULL || mpu == NULL)
    {
        return false;
    }

    float magnitude;
    float dynamic;

    magnitude = sqrtf(
        (float)mpu->ax * mpu->ax
        + (float)mpu->ay * mpu->ay
        + (float)mpu->az * mpu->az
    );

    dynamic = fabsf(magnitude - ACCEL_1G);

    /*
     * 每一个采样点都计算时间间隔
     */
    if (counter->interval_count < STEP_MIN_INTERVAL)
    {
        counter->interval_count++;
    }

    /*
     * 当前没有处于一次步伐触发状态
     */
    if (counter->above_threshold == 0)
    {
        /*
         * 超过高阈值，并且距离上一步已经超过最小间隔
         */
        if (dynamic > STEP_HIGH_THRESHOLD &&
            counter->interval_count >= STEP_MIN_INTERVAL)
        {
            counter->step_count++;

            counter->above_threshold = 1;

            /*
             * 开始重新计算两步之间的时间
             */
            counter->interval_count = 0;
        }
    }
    else
    {
        /*
         * 必须明显回落后，才允许下一步
         */
        if (dynamic < STEP_LOW_THRESHOLD)
        {
            counter->above_threshold = 0;
        }
    }

    counter->last_magnitude = dynamic;

    return true;
}

/**
 * @brief 获取步数计数器步数
 * @param counter 步数计数器指针
 * @return 步数
 */
uint32_t StepCounter_GetSteps(StepCounter_t *counter)
{
    if (counter == NULL)
    {
        return 0;
    }
    return counter->step_count;
}