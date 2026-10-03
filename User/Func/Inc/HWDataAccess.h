/**
 *  @addtogroup  Hareware Middle Layer
 *  @brief       Hardware Middle Layer, to access data from BSP and STM32 HAL Library
 *
 *  @{
 *      @file       HWDataAccess.c
 *      @brief      middleware, for UI and APP Layer to get the hardware data
 *      @details    you can enable or disable in .h file.
 * 					加这个文件是为了：
 *                    1.方便UI移植, 你要把工程移植到PC仿真,直接把MidFunc中的文件和UI文件都复制过去,然后直接把.h文件中的HW_USE_HARDWARE变成0就行了.
 *   				  2.方便控制硬件，自由地把.h文件中的HW_USE_xxx变成0就可以失能不想用的硬件
 * 					
 */


#ifndef __HW_DATA_ACCESS_H__
#define __HW_DATA_ACCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==================== Hardware Define ====================*/
/**
 *  if not use, just set 0
 *
 *
 *  if just test ui, no hardware, just set HW_USE_HARDWARE 0
 *
 */
#define HW_USE_HARDWARE 1

#ifdef HW_USE_HARDWARE

    #define HW_USE_RTC 1
    #define HW_USE_LCD 1
    #define HW_USE_AHT20 1
    #define HW_USE_MAX30102 1
    #define HW_USE_IMU 1
    #define HW_USE_GY39 1
    #define HW_USE_Ecompass  1

#endif



/*==================== Include ====================*/

#include <stdint.h>
#include <stdbool.h>

#if HW_USE_RTC
    #include "rtc.h"
#endif

#if HW_USE_LCD
    #include "lcd_init.h"
#endif

#if HW_USE_AHT20
    #include "AHT20.h"
#endif

#if HW_USE_MAX30102
    #include "max30102.h"
    #include "health_sensor.h"
#endif

#if HW_USE_IMU
    #include "mpu6050.h"
    #include "inv_mpu.h"
    #include "inv_mpu_dmp_motion_driver.h"
#endif

#if HW_USE_GY39
    #include "gy39.h"
#endif

#if HW_USE_Ecompass
    #include "LSM303.h"
#endif

#include "step_conter.h"
#include "health_sensor.h"
#include "wrist_detect.h"



/*==================== RTC ====================*/

/**
 *  @brief       RTC Data Time Interface
 */
typedef struct 
{
    uint8_t WeekDay;

    uint8_t Month;

    uint8_t Date;

    uint8_t Year;

    uint8_t Hour;

    uint8_t Minute;

    uint8_t Second;

} HW_DateTime_Interface_t;

/**
 *  @brief       RTC Settime Interface
 */
typedef struct
{
    HW_DateTime_Interface_t nowdateTime;
    void (*GetTimeDate)(HW_DateTime_Interface_t *nowdateTime);
    void (*SetDate)(uint8_t year, uint8_t month, uint8_t date);
    void (*SetTime)(uint8_t hour, uint8_t minute, uint8_t second);
    uint8_t (*CalculateWeekDay)(uint8_t setyear, uint8_t setmonth, uint8_t setdate, uint8_t century);

} HW_RTC_Interface_t;

/*==================== LCD ====================*/

/**
 *  @brief       LCD Interface
 */
typedef struct
{
    void (*SetLight)(uint8_t light);

} HW_LCD_Interface_t;



/*==================== AHT20 ====================*/
typedef struct 
{
    bool ConnectionError;

    AHT20_Data_t data;

    bool (*Init)(void);
    bool (*GetData)(void);

} HW_AHT20_Interface_t;

/*==================== MAX30102 ====================*/
typedef struct
{
    bool ConnectionError;

    MAX30102_Data_t data;
    HealthData_t health_data;

    bool (*Init)(void);
    bool (*GetI_RData)(void);
    bool (*ReadHealthData)(void);
    void (*Sleep)(void);
    void (*Start)(void);

} HW_MAX30102_Interface_t;

/*==================== MPU6050 ====================*/

/**
  * @brief  HW IMU wrist state defines
  */
#define WRIST_UP 1
#define WRIST_DOWN 0

/**
 *  @brief       MPU6050 Interface
 */
typedef struct 
{
    bool ConnectionError;

    uint16_t Steps;
    uint8_t wrist_state;
    uint8_t wrist_is_enabled;

    bool (*Init)(void);
    uint16_t (*GetSteps)(void);
    void (*WristEnable)(void);
    void (*WristDisable)(void);
    int (*SetSteps)(unsigned long count);

} HW_IMU_Interface_t;

/*==================== GY39 ====================*/
typedef struct 
{
    bool ConnectionError;

    GY39_Data_t data;

    bool (*Init)(void);
    bool (*GetData)(void);

} HW_GY39_Interface_t;

/*==================== LSM303 ====================*/
/**
 *  @brief       LSM_303 Ecompass Interface
 */
typedef struct 
{   
    bool ConnectionError;
    uint16_t direction;
    bool (*Init)(void);
    bool (*WakeUp)(void);
    void (*Sleep)(void);

} HW_Ecompass_Interface_t;

/*==================== Hardware Interface ====================*/
typedef struct 
{
    HW_RTC_Interface_t RealTimeClock;
    HW_LCD_Interface_t LCD;
    HW_AHT20_Interface_t AHT20;
    HW_MAX30102_Interface_t MAX30102;
    HW_IMU_Interface_t IMU;
    HW_GY39_Interface_t GY39;
    HW_Ecompass_Interface_t Ecompass;
} HW_Interface_t;

/* 全局唯一的硬件接口实例 */
extern HW_Interface_t HWInterface;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* HW_USE_HARDWARE */
