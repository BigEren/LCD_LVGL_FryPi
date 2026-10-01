#include "user_SensUpdateTask.h"
#include "HWDataAccess.h"
#include "user_MessageQueue.h"
#include "mpu6050.h"
#include "LSM303.h"

#include "cmsis_os2.h"

uint8_t IdleBreakstr=0;


/**
 * @brief 手表水平状态检查任务
 * 
 * @param argument 任务参数
 */
void MPUCheckTask(void *argument)
{
    for (;;)
    {
        if (HWInterface.IMU.wrist_is_enabled)
        {
            if (MPU_isHorizontal())
            {
                HWInterface.IMU.wrist_state = WRIST_UP;
            }
            else
            {
                HWInterface.IMU.wrist_state = WRIST_DOWN;
            }
        }
        osDelay(300);
    }
}

void HealthDataUpdateTask(void *argument)
{
    if (false == HWInterface.MAX30102.ConnectionError)
    {
        HWInterface.MAX30102.Start();
    }
    for (;;)
    {
        if (false == HWInterface.MAX30102.ConnectionError)
        {
            if (false == HWInterface.MAX30102.ReadHealthData())
            {
                printf("HealthDataUpdateTask: ReadHealthData failed\n");
            }
        }
        osDelay(3000);
    }
}

void AHT20DataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.AHT20.ConnectionError)
        {
            if (false == HWInterface.AHT20.GetData())
            {
                printf("AHT20DataUpdateTask: GetData failed\n");
            }
        }
        osDelay(1000);
    }
}

void StepsDataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.IMU.ConnectionError)
        {
            if (false == HWInterface.IMU.GetSteps())
            {
                printf("StepsDataUpdateTask: GetSteps failed\n");
            }
        }
        osDelay(500);
    }
}

void EcompassDataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.Ecompass.ConnectionError)
        {
            if (false == HWInterface.Ecompass.WakeUp())
            {
                printf("EcompassDataUpdateTask: WakeUp failed\n");
                goto delay;
            }
            int16_t Xa, Ya, Za, Xm, Ym, Zm;
            LSM303_ReadAcceleration(&Xa, &Ya, &Za);
            LSM303_ReadMagnetic(&Xm, &Ym, &Zm);
            float tmp = Azimuth_Calculate(Xa, Ya, Za, Xm, Ym, Zm) + 0;      //0 offset
            if (tmp < 0)
            {
                tmp += 360;
            }
            if (tmp >= 0 && tmp <= 360)
            {
                HWInterface.Ecompass.direction = (uint16_t)tmp;
            }
        }
        delay:
        osDelay(500);
    }
}

void GY39DataUpdateTask(void *argument)
{
    for (;;)
    {
        if (false == HWInterface.GY39.ConnectionError)
        {
            if (false == HWInterface.GY39.GetData())
            {
                printf("GY39DataUpdateTask: GetData failed\n");
            }
        }
        osDelay(1000);
    }
}

