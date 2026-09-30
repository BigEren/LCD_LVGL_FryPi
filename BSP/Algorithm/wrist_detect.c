#include "wrist_detect.h"
#include "math.h"

#define ACCEL_1G                16384.0f

/* 水平方向判定阈值 */
#define HORIZONTAL_THRESHOLD    0.80f

#define WRIST_CONFIRM_COUNT     3

/**
 * @brief 初始化手腕检测
 * 
 * @param wrist_detect 指向手腕检测结构体的指针
 */
void WristDetector_Init(WristDetector_t *detector)
{
    if (detector == NULL)
    {
        return;
    }
    detector->state = WRIST_DOWN;
    detector->up_count = 0;
    detector->down_count = 0;
}


/**
 * @brief 更新手腕检测状态
 * 
 * @param detector 指向手腕检测结构体的指针
 * @param data MPU6050 数据指针
 * @return WristState_t 手腕状态
 */
WristState_t WristDetector_Update(WristDetector_t *detector,
     MPU6050_Data_t *data)
{
    if (detector == NULL || data == NULL)
    {
        return WRIST_DOWN;
    }

    float ax, ay, az;
    float magnitude;

    ax = (float)data->ax;
    ay = (float)data->ay;
    az = (float)data->az;

    /*
     * 三轴加速度合成大小
     */
    magnitude = sqrtf(ax * ax + ay * ay + az * az);

    if (magnitude < 1.0f)
    {
        return detector->state;
    }

    /*
     * 计算 Z 轴在总加速度中的占比
     *
     * 手表水平放置时：
     *
     * |AZ| ≈ 1g
     * magnitude ≈ 1g
     *
     * 因此：
     *
     * |AZ| / magnitude ≈ 1
     */
    float z_ratio = fabs(az) / magnitude;

    /*
     * Z轴占比足够大：
     * 认为手表处于水平状态
     */
    if (z_ratio >= HORIZONTAL_THRESHOLD)
    {
        detector->down_count = 0;

        if (detector->state == WRIST_DOWN)
        {
            detector->up_count++;
            if (detector->up_count >= WRIST_CONFIRM_COUNT)
            {
                detector->state = WRIST_UP;
                detector->up_count = 0;
            }
        }
        else
        {
            detector->up_count = 0;
        }
    }
    else
    {
        detector->up_count = 0;
        if (detector->state == WRIST_UP)
        {
            detector->down_count++;
            if (detector->down_count >= WRIST_CONFIRM_COUNT)
            {
                detector->state = WRIST_DOWN;
                detector->down_count = 0;
            }
        }
        else
        {
            detector->down_count = 0;
        }
    }
    return detector->state;
}
