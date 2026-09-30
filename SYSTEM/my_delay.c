#include "my_delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief  操作系统版本的延时函数，带调度锁，防止RTOS抢占破坏延时
 * @note   必须在SysTick中断服务函数中调用，否则会导致延时函数被中断
 */
static void delay_osschedlock(void)
{
    taskENTER_CRITICAL();
}

static void delay_osschedunlock(void)
{
    taskEXIT_CRITICAL();
}

/**
 * @brief  延时初始化函数，配置SysTick定时器
 * @note   必须在系统时钟初始化完成之后调用
 */
void delay_init(void)
{
    // 选择HCLK时钟源作为SysTick定时器的时钟源
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    // 配置SysTick定时器的重装载值，使其每1毫秒产生一次中断
    HAL_SYSTICK_Config(SystemCoreClock / 1000U / uwTickFreq);
}

#if OS_SUPPORT
/**
 * @brief  微秒级延时(操作系统版本，带调度锁，防止RTOS抢占破坏延时)
 * @param  nus：要延时的微秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_us(uint32_t nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; // LOAD的值(重装载值16000000/1000=16000)
    ticks = nus * (SYS_CLK / 1000000U); // 要延时的节拍数
    delay_osschedlock();
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)   // 计数器递减，未达到0重装载
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told; // 计数器递减，已达到0重装载,(16000 - 15990 + 10)
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break; // 延时结束
            }
        }
    }
    delay_osschedunlock();
}

/**
 * @brief  毫秒级延时(操作系统版本，带调度锁，防止RTOS抢占破坏延时)
 * @param  nms：要延时的毫秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_ms(u16 nms)
{
    osDelay(nms);
}

/**
 * @brief  微秒级延时(非操作系统版本，不带调度锁，不阻塞中断)
 * @param  nus：要延时的微秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_us_noOS(uint32_t nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; // LOAD的值(重装载值16000000/1000=16000)
    ticks = nus * (SYS_CLK / 1000000U); // 要延时的节拍数
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)   // 计数器递减，未达到0重装载
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told; // 计数器递减，已达到0重装载,(16000 - 15990 + 10)
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break; // 延时结束
            }
        }
    }
}

/**
 * @brief  毫秒级延时(非操作系统版本)
 * @param  nms：要延时的毫秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_ms_noOS(u16 nms)
{
    u32 i;
    for (i = 0; i < nms; i++)
    {
        delay_us(1000);
    }
}
#else
/**
 * @brief  微秒级延时(非操作系统版本，不带调度锁，不阻塞中断)
 * @param  nus：要延时的微秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_us(uint32_t nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; // LOAD的值(重装载值16000000/1000=16000)
    ticks = nus * (SYS_CLK / 1000000U); // 要延时的节拍数
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)   // 计数器递减，未达到0重装载
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told; // 计数器递减，已达到0重装载,(16000 - 15990 + 10)
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break; // 延时结束
            }
        }
    }
}

/**
 * @brief  毫秒级延时(非操作系统版本，不带调度锁，不阻塞中断)
 * @param  nms：要延时的毫秒数
 * @note   使用SysTick递减计数器VAL寄存器做查询方式延时，不阻塞中断
 */
void delay_ms(u16 nms)
{
    u32 i;
    for (i = 0; i < nms; i++)
    {
        delay_us(1000);
    }
}
#endif
