#include "user_GY39Task.h"
#include "gy39.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "HWDataAccess.h"

void user_GY39Task(void *argument)
{
    uint8_t print_count = 0;

    printf("GY39Task Start\r\n");

    if (HWInterface.GY39.Init())
    {
        HWInterface.GY39.ConnectionError = false;
        printf("GY39 Init Success\r\n");
    }
    else
    {
        HWInterface.GY39.ConnectionError = true;
        printf("GY39 Init Failed\r\n");
    }

    for (;;)
    {
        if (!HWInterface.GY39.ConnectionError)
        {
            if (HWInterface.GY39.GetData())
            {
                print_count++;

                /* 5Hz读取，4秒打印一次 */
                if (print_count >= 20)
                {
                    printf(
                        "GY39 Lux:%lu T:%.2f H:%.2f P:%.2f Alt:%d\r\n",
                        HWInterface.GY39.data.lux,
                        HWInterface.GY39.data.temp,
                        HWInterface.GY39.data.hum,
                        HWInterface.GY39.data.press,
                        HWInterface.GY39.data.alt
                    );

                    print_count = 0;
                }
            }
            else
            {
                printf("GY39 Read FAIL\r\n");
            }
        }
        osDelay(1000);
    }
}