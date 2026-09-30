#include "user_PrintSensDataTask.h"
#include "HWDataAccess.h"

void user_PrintSensDataTask(void *argument)
{
    uint8_t print_count = 0;
    for (;;)
    {
        print_count++;
        if (!HWInterface.AHT20.ConnectionError && print_count == 1)
        {
            printf("AHT20Task Temp = %.2f, Humidity = %.2f\n",
                    HWInterface.AHT20.data.temperature, HWInterface.AHT20.data.humidity);
        }
        else if (!HWInterface.IMU.ConnectionError && print_count == 2)
        {
            printf("step: %d\n", HWInterface.IMU.Steps);
        }
        else if (!HWInterface.MAX30102.ConnectionError && print_count == 3)
        {
            printf("HR:%ld  SpO2:%ld  HRValid:%d  SpO2Valid:%d\r\n",
                       HWInterface.MAX30102.health_data.heart_rate,
                       HWInterface.MAX30102.health_data.spo2,
                       HWInterface.MAX30102.health_data.heart_rate_valid,
                       HWInterface.MAX30102.health_data.spo2_valid);
        }
        else if (!HWInterface.Ecompass.ConnectionError && print_count == 4)
        {
            printf("Ecompass: %d\n", HWInterface.Ecompass.direction);
        }
        print_count %= 4;
        HWInterface.RealTimeClock.GetTimeDate(&HWInterface.RealTimeClock.nowdateTime);
        printf("%d年%d月%d日 %02d:%02d:%02d 第%d周 \n", 
            HWInterface.RealTimeClock.nowdateTime.Year + 2000, HWInterface.RealTimeClock.nowdateTime.Month, 
            HWInterface.RealTimeClock.nowdateTime.Date, HWInterface.RealTimeClock.nowdateTime.Hour, 
            HWInterface.RealTimeClock.nowdateTime.Minute, HWInterface.RealTimeClock.nowdateTime.Second,
            HWInterface.RealTimeClock.nowdateTime.WeekDay
        );
        
        osDelay(1000);
    }
}
