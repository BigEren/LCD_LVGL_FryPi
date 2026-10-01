/* Private includes -----------------------------------------------------------*/
// includes
#include "user_TasksInit.h"
#include "user_HardwareInitTask.h"
#include "task.h"
#include "tim.h"
#include "usart.h"
#include "stm32f4xx_it.h"
#include "data_save.h"
#include "lcd.h"
#include "version.h"

// sys includes
#include "sys.h"
#include "my_delay.h"

// bsp includes
#include "my_iic_hal.h"
#include "HWDataAccess.h"
#include "key.h"


// ui includes
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  hardwares init task
  * @param  argument: Not used
  * @retval None
  */
void user_HardwareInitTask(void *argument)
{
    while (1)
    {
        vTaskSuspendAll();

        // RTC Wake
        if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2000, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
        {
            Error_Handler();
        }
        // uart start
        HAL_UART_Receive_DMA(&huart1, (uint8_t *)HardInt_receive_str, 25);
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

        // PWM start
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

        // delay init
        delay_init();

        // key init
        Key_Port_Init();

        // storage init
        Storage_Init();
        if (!Storage_Check())
        {
            // uint8_t recbuf[3];
            // SettingGet(recbuf, 0x10, 2);
            // if ((recbuf[0] != 0 && recbuf[0] != 1) || (recbuf[1] != 0 && recbuf[1] != 1))
            // {
            //     HWInterface.IMU.wrist_is_enabled = 0;
            // }
            // else
            // {
            //     HWInterface.IMU.wrist_is_enabled = recbuf[0];
            // }

            // RTC_DateTypeDef nowdate;
            // HAL_RTC_GetDate(&hrtc, &nowdate, RTC_FORMAT_BIN);

            // SettingGet(recbuf, 0x20, 3);
            // if (recbuf[0] == nowdate.Date)
            // {
            //     uint16_t steps = 0;
            //     steps = recbuf[1] & 0x00FF;
            //     steps = steps << 8 | recbuf[2];
            //     if (!HWInterface.IMU.ConnectionError)
            //     {
            //         dmp_set_pedometer_step_count((unsigned long)steps);
            //     }
            // }
        }
        HWInterface.IMU.WristEnable();

        // touch init
        CST816_GPIO_Init();
        CST816_RESET();

        // lcd
        LCD_Init();
        LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
        delay_ms_noOS(10);
        LCD_Open_Light();
        LCD_Set_Light(50);
        LCD_ShowString(72, LCD_H/2, (uint8_t*)"Welcome!", LIGHTBLUE, BLACK, 24, 0);   //12*6,16*8,24*12,32*16
        uint8_t lcd_buf_str[17];
        sprintf(lcd_buf_str, "OV-Watch V%d.%d.%d", watch_version_major(), watch_version_minor(), watch_version_patch());
        LCD_ShowString(34, LCD_H/2+48, (uint8_t*)lcd_buf_str, WHITE, BLACK, 24, 0);
        delay_ms_noOS(1000);
        LCD_Fill(0, LCD_H/2-24, LCD_W, LCD_H/2+49, BLACK);

        // ui LVGL disp init
        lv_init();
        lv_port_disp_init();
        lv_port_indev_init();
        ui_init();

        // bsp init
        IICInit(&SensorBus,
             IIC_CLK_GPIOA | 
             IIC_CLK_GPIOB);

        xTaskResumeAll();

        // sensor init
        uint8_t num = 3;
        // while (num && HWInterface.AHT20.ConnectionError == true)
        // {
        //     num--;
        //     HWInterface.AHT20.ConnectionError = !HWInterface.AHT20.Init();
        // }

        num = 3;
        while (num && HWInterface.IMU.ConnectionError == true)
        {
            num--;
            HWInterface.IMU.ConnectionError = !HWInterface.IMU.Init();
        }

        num = 3;
        while (num && HWInterface.MAX30102.ConnectionError == true)
        {
            num--;
            HWInterface.MAX30102.ConnectionError = !HWInterface.MAX30102.Init();
        }
        if (!HWInterface.MAX30102.ConnectionError)
        {
            HWInterface.MAX30102.Sleep();
        }

        // num = 3;
        // while (num && HWInterface.Ecompass.ConnectionError == true)
        // {
        //     num--;
        //     HWInterface.Ecompass.ConnectionError = !HWInterface.Ecompass.Init();
        // }
        // if (!HWInterface.Ecompass.ConnectionError)
        // {
        //     HWInterface.Ecompass.Sleep();
        // }

        num = 3;
        while (num && HWInterface.GY39.ConnectionError == true)
        {
            num--;
            HWInterface.GY39.ConnectionError = !HWInterface.GY39.Init();
        }

        vTaskDelete(NULL);
    }
}
