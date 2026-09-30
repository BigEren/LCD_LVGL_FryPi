#include "user_MAX30102Task.h"
#include "max30102.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "HWDataAccess.h"

void user_MAX30102Task(void *argument)
{
    uint8_t print_count = 0;

    printf("MAX30102Task Start\r\n");

    if (HWInterface.MAX30102.Init())
    {
        HWInterface.MAX30102.ConnectionError = false;
        printf("MAX30102 Init Success\r\n");
    }
    else
    {
        HWInterface.MAX30102.ConnectionError = true;
        printf("MAX30102 Init Failed\r\n");
    }

    for (;;)
    {
        if (!HWInterface.MAX30102.ConnectionError)
        {
            if (HWInterface.MAX30102.ReadHealthData())
            {
                printf("HR:%ld  SpO2:%ld  HRValid:%d  SpO2Valid:%d\r\n",
                       HWInterface.MAX30102.health_data.heart_rate,
                       HWInterface.MAX30102.health_data.spo2,
                       HWInterface.MAX30102.health_data.heart_rate_valid,
                       HWInterface.MAX30102.health_data.spo2_valid);
            }
            else
            {
                printf("Health Read Failed\r\n");
            }
        }

        /*
         * HealthSensor_Read() 本身已经完成一整个采样窗口，
         * 所以这里不需要20ms循环。
         */
        osDelay(100);

    }
}
