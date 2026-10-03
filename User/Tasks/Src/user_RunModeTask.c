#include "user_RunModeTask.h"

#include "main.h"
#include "cmsis_os2.h"

#include "HWDataAccess.h"
#include "user_MessageQueue.h"
#include "usart.h"
#include "key.h"

/* Idle Time Count */
uint16_t IdleTimeCount = 0;

/**
 * @brief 系统进入空闲模式任务
 * @param argument 无
 * @return 无
 */
void IdleEnterTask(void *argument)
{
    uint8_t Idlestr = 0;
    uint8_t IdleBreak = 0;

    for (;;)
    {
        // 等待空闲事件, 有则关闭背光
        if (osMessageQueueGet(
            Idle_MessageQueue,
            &Idlestr,
            NULL,
            1) == osOK)
        {
            LCD_Set_Light(5);
        }

        if (osMessageQueueGet(
            IdleBreak_MessageQueue,
            &IdleBreak,
            NULL,
            1) == osOK)
        {
            IdleTimeCount = 0;
            LCD_Set_Light(5);
            LCD_Set_Light(100);
        }

        osDelay(10);
    }
}

/**
 * @brief 系统进入睡眠模式任务
 * @param argument 无
 * @return 无
 */
void StopEnterTask(void *argument)
{
    uint8_t Stopstr = 0;
    uint8_t Wrist_Flag = 0;
    for (;;)
    {
        /* 等待消息 */
        if(osMessageQueueGet(
            Stop_MessageQueue,
            &Stopstr,
            NULL,
            0) == osOK)
        {
            /*************************** 系统进入睡眠模式前处理 ***************************/
            sleep:
            IdleTimeCount = 0;

            /* 睡眠前处理 */
            printf("Enter Sleep\r\n");

            // 关闭 UART
            //HAL_UART_MspDeInit(&huart1);

            // lcd
            //LCD_RES_Clr();
            LCD_Close_Light();

            // 关闭 Touch
            CST816_Sleep();

            /***********************************************************************************/

			/********************************** 系统睡眠处理 ************************************/

            printf("A\r\n");                          // 测试1
            vTaskSuspendAll();
            printf("B\r\n");                          // 测试2
            HAL_GPIO_WritePin(LED_T_GPIO_Port, LED_T_Pin, GPIO_PIN_SET);   // 测试3
            CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
            HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
            //systick int
            CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
            //enter stop mode
			HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

            /***********************************************************************************/

			/********************************** 系统唤醒处理 ************************************/

            //resume run mode and reset the sysclk
			SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
            HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
            SystemClock_Config();
            xTaskResumeAll();

            /***********************************************************************************/

			/********************************** 系统唤醒处理 ************************************/

            // MPU Check
            if (HWInterface.IMU.wrist_is_enabled == 1)
            {
                uint8_t horizontal = 0;
                horizontal = MPU_isHorizontal();
                if (horizontal && HWInterface.IMU.wrist_is_enabled == WRIST_DOWN)
                {
                    HWInterface.IMU.wrist_state = WRIST_UP;
                    Wrist_Flag = 1;
                }
                else if (!horizontal && HWInterface.IMU.wrist_state == WRIST_UP)
                {
                    HWInterface.IMU.wrist_state = WRIST_DOWN;
                    IdleTimeCount = 0;
                    goto sleep;
                }
            }

            if (!KEY1 || Wrist_Flag)
            {
                Wrist_Flag = 0;     // 恢复
            }
            else
            {
                IdleTimeCount = 0;
                goto sleep;         // 继续睡眠
            }

            //usart
			//HAL_UART_MspInit(&huart1);
			//lcd
			LCD_Init();
            LCD_Open_Light();
			LCD_Set_Light(100);
			//touch
			CST816_Wakeup();
            printf("System Wakeup\r\n");

        }
        osDelay(100);
    }
}

/**
 * @brief 系统空闲定时器回调函数
 * @param argument 无
 * @return 无
 */
void IdleTimerCallback(void *argument)
{
    IdleTimeCount++;
    if (IdleTimeCount == 100)
    {
        uint8_t Idlestr = 0;
        osMessageQueuePut(Idle_MessageQueue, &Idlestr, NULL, 1);
    }
    if (IdleTimeCount == 150)
    {
        uint8_t Stopstr = 1;
        IdleTimeCount = 0;
        osMessageQueuePut(Stop_MessageQueue, &Stopstr, NULL, 1);
    }
}
