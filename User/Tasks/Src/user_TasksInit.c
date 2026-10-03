/* Private includes -----------------------------------------------------------*/
// includes
#include "user_TasksInit.h"

// sys includes
#include "sys.h"
#include "stdio.h"

// bsp includes
#include "key.h"

// gui includes
#include "lvgl.h"

// task includes
#include "user_HardwareInitTask.h"
#include "user_LVGLTask.h"
#include "user_SensUpdateTask.h"
#include "user_MessageQueue.h"
#include "user_PrintSensDataTask.h"
#include "user_RunModeTask.h"
#include "user_ScrRenewTask.h"
#include "user_KeyTask.h"



/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
osTimerId_t IdleTimerHandle = NULL;


/* Timers --------------------------------------------------------------------*/


/* Tasks ---------------------------------------------------------------------*/
// HardwareInitTask Init
osThreadId_t user_HardwareInitTaskHandle = NULL;
const osThreadAttr_t user_HardwareInitTask_attributes = {
    .name = "user_HardwareInitTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t)osPriorityHigh,
};

// LVGLTask Init
osThreadId_t user_LVGLTaskHandle = NULL;
const osThreadAttr_t user_LVGLTask_attributes = {
    .name = "user_LVGLTask",
    .stack_size = 128 * 24,
    .priority = (osPriority_t)osPriorityNormal,
};

// // AHT20Task Init
// osThreadId_t user_AHT20TaskHandle = NULL;
// const osThreadAttr_t user_AHT20Task_attributes = {
//     .name = "user_AHT20Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// // MPU6050Task Init
// osThreadId_t user_MPU6050TaskHandle = NULL;
// const osThreadAttr_t user_MPU6050Task_attributes = {
//     .name = "user_MPU6050Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// // MAX30102Task Init
// osThreadId_t user_MAX30102TaskHandle = NULL;
// const osThreadAttr_t user_MAX30102Task_attributes = {
//     .name = "user_MAX30102Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// GY39Task Init
// osThreadId_t user_GY39TaskHandle = NULL;
// const osThreadAttr_t user_GY39Task_attributes = {
//     .name = "user_GY39Task",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// MPUCheckTask Init
osThreadId_t MPUCheckTaskHandle = NULL;
const osThreadAttr_t MPUCheckTask_attributes = {
    .name = "MPUCheckTask",
    .stack_size = 128 * 3,
    .priority = (osPriority_t)osPriorityLow1,
};

// AHT20DataUpdateTask Init
// osThreadId_t AHT20DataUpdateTaskHandle = NULL;
// const osThreadAttr_t AHT20DataUpdateTask_attributes = {
//     .name = "AHT20DataUpdateTask",
//     .stack_size = 128 * 10,
//     .priority = (osPriority_t)osPriorityLow1,
// };

// HealthDataUpdateTask Init
osThreadId_t HealthDataUpdateTaskHandle = NULL;
const osThreadAttr_t HealthDataUpdateTask_attributes = {
    .name = "HealthDataUpdateTask",
    .stack_size = 128 * 6,
    .priority = (osPriority_t)osPriorityLow1,
};

// StepsDataUpdateTask Init
osThreadId_t StepsDataUpdateTaskHandle = NULL;
const osThreadAttr_t StepsDataUpdateTask_attributes = {
    .name = "StepsDataUpdateTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t)osPriorityLow1,
};

// StopEnterTask Init
osThreadId_t StopEnterTaskHandle = NULL;
const osThreadAttr_t StopEnterTask_attributes = {
    .name = "StopEnterTask",
    .stack_size = 128 * 16,
    .priority = (osPriority_t)osPriorityLow1,
};

// PrintSensDataTask Init
osThreadId_t PrintSensDataTaskHandle = NULL;
const osThreadAttr_t PrintSensDataTask_attributes = {
    .name = "PrintSensDataTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t)osPriorityLow1,
};

// IdleEnterTask Init
osThreadId_t IdleEnterTaskHandle = NULL;
const osThreadAttr_t IdleEnterTask_attributes = {
    .name = "IdleEnterTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t)osPriorityLow1,
};

// EcompassDataUpdateTask Init
osThreadId_t EcompassDataUpdateTaskHandle = NULL;
const osThreadAttr_t EcompassDataUpdateTask_attributes = {
    .name = "EcompassDataUpdateTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t)osPriorityLow1,
};

// GY39DataUpdateTask Init  
osThreadId_t GY39DataUpdateTaskHandle = NULL;
const osThreadAttr_t GY39DataUpdateTask_attributes = {
    .name = "GY39DataUpdateTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t)osPriorityLow1,
};

// KeyTask Init
osThreadId_t KeyTaskHandle = NULL;
const osThreadAttr_t KeyTask_attributes = {
    .name = "KeyTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t)osPriorityLow1,
};

// ScrRenewTask Init
osThreadId_t ScrRenewTaskHandle = NULL;
const osThreadAttr_t ScrRenewTask_attributes = {
    .name = "ScrRenewTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t)osPriorityLow1,
};

/* Message queues ------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void user_Tasks_Init(void)
{
    /* add mutexes, ... */

    /* add semaphores, ... */

    /* start timers, add new ones, ... */
    IdleTimerHandle = osTimerNew(IdleTimerCallback, osTimerPeriodic, NULL, NULL);
	osTimerStart(IdleTimerHandle,100);  //100ms

    /* add queues, ... */
    User_MessageQueueInit();
	/* add threads, ... */
    user_HardwareInitTaskHandle = osThreadNew(user_HardwareInitTask, NULL, &user_HardwareInitTask_attributes);
    user_LVGLTaskHandle = osThreadNew(user_LVGLTask, NULL, &user_LVGLTask_attributes);
    StepsDataUpdateTaskHandle = osThreadNew(StepsDataUpdateTask, NULL, &StepsDataUpdateTask_attributes);
    HealthDataUpdateTaskHandle = osThreadNew(HealthDataUpdateTask, NULL, &HealthDataUpdateTask_attributes);
    // AHT20DataUpdateTaskHandle = osThreadNew(AHT20DataUpdateTask, NULL, &AHT20DataUpdateTask_attributes);
    MPUCheckTaskHandle = osThreadNew(MPUCheckTask, NULL, &MPUCheckTask_attributes);
    StopEnterTaskHandle = osThreadNew(StopEnterTask, NULL, &StopEnterTask_attributes);
    IdleEnterTaskHandle = osThreadNew(IdleEnterTask, NULL, &IdleEnterTask_attributes);
    PrintSensDataTaskHandle = osThreadNew(user_PrintSensDataTask, NULL, &PrintSensDataTask_attributes);
    //EcompassDataUpdateTaskHandle = osThreadNew(EcompassDataUpdateTask, NULL, &EcompassDataUpdateTask_attributes);   
    GY39DataUpdateTaskHandle = osThreadNew(GY39DataUpdateTask, NULL, &GY39DataUpdateTask_attributes);
    KeyTaskHandle = osThreadNew(KeyTask, NULL, &KeyTask_attributes);
    ScrRenewTaskHandle = osThreadNew(ScrRenewTask, NULL, &ScrRenewTask_attributes);

    /* add events, ... */

	/* add  others ... */
}
