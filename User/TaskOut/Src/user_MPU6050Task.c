#include "user_MPU6050Task.h"
#include "mpu6050.h"
#include "cmsis_os2.h"
#include "step_conter.h"
#include <stdio.h>
#include "HWDataAccess.h"

void user_MPU6050Task(void *argument)
{
    uint8_t print_count = 0;

    printf("MPU6050Task Start\r\n");

    if (!HWInterface.MPU6050.Init())
    {
        HWInterface.MPU6050.ConnectionError = true;
        printf("MPU6050 Init Failed\r\n");
    }
    else
    {
        HWInterface.MPU6050.ConnectionError = false;
        printf("MPU6050 Init Success\r\n");
    }

    for (;;)
    {
        if (!HWInterface.MPU6050.ConnectionError)
        {
            if (HWInterface.MPU6050.GetData())
            {
                HWInterface.MPU6050.Process();
                HWInterface.MPU6050.WristProcess();
                if (print_count % 20 == 0)
                {
                    if (HWInterface.MPU6050.wrist_state == WRIST_UP)
                    printf("Up\r\n");
                    else
                    printf("Down\r\n");

                    printf("ax:%d ay:%d az:%d\r\n",
                        HWInterface.MPU6050.data.ax,
                        HWInterface.MPU6050.data.ay,
                        HWInterface.MPU6050.data.az);
                }

                print_count++;
                if (print_count >= 50)
                {
                    printf("MPUTask Steps:%lu\r\n",
                        HWInterface.MPU6050.GetSteps());
                    print_count = 0;
                }
            }
        }

        osDelay(20);
    }
}
