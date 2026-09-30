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


#include "HWDataAccess.h"

/*==================== RTC Functions ====================*/

/*******************************************************************************/
/**
 * @brief 获取 RTC 时间日期
 * 
 * @param nowdatetime 时间日期结构体指针
 * @return true 获取成功
 * @return false 获取失败
 */
/*******************************************************************************/
void HW_RTC_Get_TimeDate(HW_DateTime_Interface_t *nowdatetime)
{
    #if HW_USE_RTC
        if (nowdatetime != NULL)
        {
            RTC_DateTypeDef nowdate;
            RTC_TimeTypeDef nowtime;
            HAL_RTC_GetTime(&hrtc, &nowtime, RTC_FORMAT_BIN);
            HAL_RTC_GetDate(&hrtc, &nowdate, RTC_FORMAT_BIN);
            nowdatetime->WeekDay = weekday_calculate(nowdate.Year, nowdate.Month, nowdate.Date, 20);
            nowdatetime->Month = nowdate.Month;
            nowdatetime->Date = nowdate.Date;
            nowdatetime->Year = nowdate.Year;
            nowdatetime->Hour = nowtime.Hours;
            nowdatetime->Minute = nowtime.Minutes;
            nowdatetime->Second = nowtime.Seconds;
        }
    #else
        nowdatetime->WeekDay = 7;
        nowdatetime->Month = 1;
        nowdatetime->Date = 1;
        nowdatetime->Year = 23;
        nowdatetime->Hour = 5;
        nowdatetime->Minute = 20;
        nowdatetime->Second = 21;
    #endif
}

/*******************************************************************************/
/**
 * @brief 设置 RTC 日期
 * 
 * @param year 年
 * @param month 月
 * @param date 日
 */
/*******************************************************************************/
void HW_RTC_Set_Date(uint8_t year, uint8_t month, uint8_t date)
{
    #if HW_USE_RTC
        RTC_SetDate(year, month, date);
    #endif
}

/*******************************************************************************/
/**
 * @brief 设置 RTC 时间
 * 
 * @param hour 小时
 * @param minute 分
 * @param second 秒
 */
/*******************************************************************************/
void HW_RTC_Set_Time(uint8_t hour, uint8_t minute, uint8_t second)
{
    #if HW_USE_RTC
        RTC_SetTime(hour, minute, second);
    #endif
}

/*******************************************************************************/
/**
 * @brief 计算日期是星期几
 * 
 * @param setyear 年
 * @param setmonth 月
 * @param setday 日
 * @param century 世纪
 * @return uint8_t 星期几
 */
/*******************************************************************************/
uint8_t HW_WeekDay_Calclate(uint8_t setyear, uint8_t setmonth, uint8_t setday, uint8_t century)
{
    int w;
    if (setmonth == 1 || setmonth == 2)
    {
        setyear--;
        setmonth += 12;
    }
    w = setyear + setyear / 4 + 26*(setmonth + 1)/10 + setday - 1 - 2 * century;
    while(w<0)
		w+=7;
	w%=7;
	w=(w==0)?7:w;
	return w;
}

/*==================== LCD Functions ====================*/

/*******************************************************************************/
/**
 * @brief 设置 LCD 亮度
 * 
 * @param light 亮度
 */
/*******************************************************************************/
void HW_LCD_Set_Light(uint8_t light)
{
    #if HW_USE_LCD
        LCD_Set_Light(light);
    #endif
}

/*==================== AHT20 Functions =====================*/

/*******************************************************************************/
/**
 * @brief 初始化 AHT20
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
/*******************************************************************************/
static bool HW_AHT20_Init(void)
{
    #if HW_USE_AHT20
        return AHT20_Init();
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 从 AHT20 读取数据
 * 
 * @param humi 湿度指针
 * @param temp 温度指针
 * @return true 读取成功
 * @return false 读取失败
 */
/*******************************************************************************/
static bool HW_AHT20_GetData(void)
{
    #if HW_USE_AHT20
        if (!HWInterface.AHT20.ConnectionError)
        {
            return AHT20_Read_Data(&HWInterface.AHT20.data);
        }
    #endif
    return false;
}

/*==================== IMU Functions ====================*/

/*******************************************************************************/
/**
 * @brief 初始化 MPU6050
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
/*******************************************************************************/
static bool HW_MPU6050_Init(void)
{
    #if HW_USE_IMU
        return mpu_dmp_init() == 0;
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 使能 MPU6050 手势检测
 */
/*******************************************************************************/
void HW_MPU6050_Wrist_Enable(void)
{
    #if HW_USE_IMU
        HWInterface.IMU.wrist_is_enabled = 1;
    #endif
}

/*******************************************************************************/
/**
 * @brief 失能 MPU6050 手势检测
 */
/*******************************************************************************/
void HW_MPU6050_Wrist_Disable(void)
{
    #if HW_USE_IMU
        HWInterface.IMU.wrist_is_enabled = 0;
    #endif
}

/*******************************************************************************/
/**
 * @brief 获取 MPU6050 步数
 * 
 * @return uint16_t 步数
 */
/*******************************************************************************/
uint16_t HW_MPU6050_Get_Steps(void)
{
    #if HW_USE_IMU
        unsigned long STEPS = 0;
        if (!HWInterface.IMU.ConnectionError)
        {
            dmp_get_pedometer_step_count(&STEPS);
            return STEPS;
        }
    #endif
        return 0;
}

/*******************************************************************************/
/**
 * @brief 设置 MPU6050 步数
 * 
 * @param steps 步数
 */
/*******************************************************************************/
int HW_MPU6050_Set_Steps(uint16_t steps)
{
    #if HW_USE_IMU
        dmp_set_pedometer_step_count(steps);
    #endif
}

/*******************************************************************************/
/**
 * @brief 初始化 MAX30102
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
/*******************************************************************************/
static bool HW_MAX30102_Init(void)
{
    #if HW_USE_MAX30102
        return MAX30102_Init();
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 从 MAX30102 读取数据
 * 
 * @return true 读取成功
 * @return false 读取失败
 */
/*******************************************************************************/
static bool HW_MAX30102_GetI_RData(void)
{
    #if HW_USE_MAX30102
        return MAX30102_ReadFIFO(&HWInterface.MAX30102.data);
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 从 MAX30102 读取健康数据
 * 
 * @return true 读取成功
 * @return false 读取失败
 */
/*******************************************************************************/
static bool HW_MAX30102_ReadHealthData(void)
{
    #if HW_USE_MAX30102
        return HealthSensor_Read(
            &HWInterface.MAX30102.health_data
        );
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 启动 MAX30102
 * 
 * @return true 启动成功
 * @return false 启动失败
 */
/*******************************************************************************/
static bool HW_MAX30102_Start(void)
{
    #if HW_USE_MAX30102
        return MAX30102_Start();
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 进入 MAX30102 休眠模式
 * 
 * @return true 休眠成功
 * @return false 休眠失败
 */
/*******************************************************************************/
static bool HW_MAX30102_Sleep(void)
{
    #if HW_USE_MAX30102
        return MAX30102_Sleep();
    #endif
    return false;
}


/*=================== GY39 =====================*/
/*******************************************************************************/
/**
 * @brief 初始化 GY39
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
/*******************************************************************************/
static bool HW_GY39_Init(void)
{
    #if HW_USE_GY39
        return GY39_Init();
    #endif
    return false;
}

/*******************************************************************************/
/**
 * @brief 从 GY39 读取数据
 * 
 * @return true 读取成功
 * @return false 读取失败
 */
/*******************************************************************************/
static bool HW_GY39_GetData(void)
{
    #if HW_USE_GY39
        return GY39_ReadData(&HWInterface.GY39.data);
    #endif
    return false;
}

/*=================== LSM303 =====================*/
/*******************************************************************************/
static bool HW_Ecompass_Init(void)
{
    #if HW_USE_Ecompass
        return LSM303DLH_Init() == 0;
    #endif
    return false;
}

static bool HW_Ecompass_Sleep(void)
{
    #if HW_USE_Ecompass
        return LSM303DLH_Sleep();
    #endif
    return false;
}

static bool HW_Ecompass_WakeUp(void)
{
    #if HW_USE_Ecompass
        return LSM303DLH_Wakeup();
    #endif
    return false;
}



/**
 * @brief 初始化硬件数据访问接口
 * 
 */
HW_Interface_t HWInterface = {
    .RealTimeClock = {
        .CalculateWeekDay = HW_WeekDay_Calclate,
        .GetTimeDate = HW_RTC_Get_TimeDate,
        .SetDate = HW_RTC_Set_Date,
        .SetTime = HW_RTC_Set_Time
    },
    .LCD = {
        .SetLight = HW_LCD_Set_Light
    },
    .IMU = {
        .ConnectionError = true,
        .Steps = 0,
        .wrist_is_enabled = 0,
        .wrist_state = WRIST_UP,
        .Init = HW_MPU6050_Init,
        .GetSteps = HW_MPU6050_Get_Steps,
        .SetSteps = HW_MPU6050_Set_Steps,
        .WristEnable = HW_MPU6050_Wrist_Enable,
        .WristDisable = HW_MPU6050_Wrist_Disable
    },
    .AHT20 = {
        .ConnectionError = true,
        .data.temperature = 52.0f,
        .data.humidity = 13.14f,
        .Init = HW_AHT20_Init,
        .GetData = HW_AHT20_GetData
    },
    .MAX30102 = {
        .ConnectionError = true,
        .data = {0},
        .health_data = {0},
        .Init = HW_MAX30102_Init,
        .GetI_RData = HW_MAX30102_GetI_RData,
        .ReadHealthData = HW_MAX30102_ReadHealthData,
        .Start = HW_MAX30102_Start,
        .Sleep = HW_MAX30102_Sleep
    },
    .Ecompass = {
        .ConnectionError = true,
        .direction = 45,
        .Init = HW_Ecompass_Init,
        .WakeUp = HW_Ecompass_WakeUp,
        .Sleep = HW_Ecompass_Sleep
    }
};
