/* Private includes -----------------------------------------------------------*/
// includes
#include "user_LVGLTask.h"
#include "user_TasksInit.h"
#include "user_MessageQueue.h"

// sys includes

// bsp includes

// ui includes
#include "lvgl.h"
#include "ui.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS Tick Hook, to increase the LVGL tick
  * @param  None
  * @retval None
  */
 void TaskTickHook(void)
 {
    //to increase the LVGL tick
    lv_tick_inc(1);
 }

/**
  * @brief  LVGL Handler task, to run the lvgl
  * @param  argument: Not used
  * @retval None
  */
void user_LVGLTask(void *argument)
{
    uint8_t IdleBreakstr=0;
    while(1)
    {
        if(lv_disp_get_inactive_time(NULL) < 1000)
        {
            //Idle time break, set to 0
            osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, NULL, 0);
        }
        uint32_t wait = lv_task_handler();
        if(wait < 1U) 
        {
            wait = 1U;
        }
        if(wait > 30U)
        {
            wait = 30U;
        }
        osDelay(wait);
    }
}
