#include "user_AHT20Task.h"
#include "AHT20.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "HWDataAccess.h"

void user_AHT20Task(void *argument)
{
    /*
     * AHT20 初始化
     */
    if (!HWInterface.AHT20.Init())
    {
        HWInterface.AHT20.ConnectionError = true;
        printf("AHT20 Init Failed\r\n");
    }
    else
    {
        HWInterface.AHT20.ConnectionError = false;
        printf("AHT20 Init Success\r\n");
    }

    for (;;)
    {
        if (!HWInterface.AHT20.ConnectionError)
        {
            if (HWInterface.AHT20.GetData())
            {
                printf("AHT20Task Temp = %.2f, Humidity = %.2f\n",
                    HWInterface.AHT20.data.temperature, HWInterface.AHT20.data.humidity);
            }
            else
            {
                printf("AHT20_Read_Data Failed\n");
            }
        }
        osDelay(1000);
    }
}
