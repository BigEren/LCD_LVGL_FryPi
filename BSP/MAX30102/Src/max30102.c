#include "max30102.h"
#include "my_delay.h"

/*
 * MAX30102 软件 I2C 引脚配置
 *
 * MAX30102:
 *   SDA -> GPIOC PIN13
 *   SCL -> GPIOA PIN14
 *
 * 注意：
 * AHT20 的 SDA / SCL 通常需要配置为开漏输出，并配合上拉电阻使用。
 */

#define MAX30102_CLK_ENABLE_B() __HAL_RCC_GPIOB_CLK_ENABLE()
/*******************************************************************************/
/**
 * @brief  MAX30102 软件 I2C 总线对象
 * @note
 * 这里将 MAX30102 使用的 SDA、SCL 引脚绑定到软件 I2C 总线。
 *
 * SDA -> GPIOB PIN13
 * SCL -> GPIOB PIN14
 */
/*******************************************************************************/
iic_bus_my_t *MAX30102_bus = &SensorBus;

/**
 * @brief  写 MAX30102 寄存器
 * @param reg 寄存器地址
 * @param value 寄存器值
 * @retval 状态
 */
bool MAX30102_WriteReg(uint8_t reg, uint8_t value)
{
    return IIC_Write_One_Byte(
        MAX30102_bus,
        MAX30102_ADDR,
        reg,
        value
    ) == SUCCESS;
}

/*******************************************************************************/
/**
 * @brief  读 MAX30102 寄存器
 * @param reg 寄存器地址
 * @param value 寄存器值指针
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_ReadReg(uint8_t reg, uint8_t *value)
{
    if (value == NULL)
    {
        return false;
    }

    if (IIC_Read_One_Byte(
        MAX30102_bus,
        MAX30102_ADDR,
        reg,
        value
    ) == SUCCESS)
    {
        return true;
    }

    return false;
}

bool MAX30102_Reset(void)
{
    if (!MAX30102_WriteReg(
            MAX30102_REG_MODE_CONFIG,
            0x40))
    {
        return false;
    }

    delay_ms(10);

    return true;
}

/*******************************************************************************/
/**
 * @brief  初始化 MAX30102
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_Init(void)
{

    // 重置 MAX30102
    if (!MAX30102_Reset())
    {
        return false;
    }

    delay_ms_noOS(10);

    /*
     * 清 FIFO 指针
     */
    if (!MAX30102_WriteReg(MAX30102_REG_FIFO_CONFIG, 0x00))
        return false;
    if (!MAX30102_WriteReg(MAX30102_REG_FIFO_OVF_COUNTER, 0x00))
        return false;
    if (!MAX30102_WriteReg(MAX30102_REG_FIFO_RD_PTR, 0x00))
        return false;
    
    /*
     * FIFO 配置
     */
    if (!MAX30102_WriteReg(MAX30102_REG_FIFO_CONFIG, 0x6F))
        return false;

    /*
     * SpO2模式
     */
    if (!MAX30102_WriteReg(MAX30102_REG_MODE_CONFIG, 0x03))
        return false;
    
    /*
     * SpO2配置
     */
    if (!MAX30102_WriteReg(MAX30102_REG_SPO2_CONFIG, 0x2F))
        return false;

    /*
     * LED 电流
     */
    if (!MAX30102_WriteReg(MAX30102_REG_LED1_PA, 0x17))
        return false;
    if (!MAX30102_WriteReg(MAX30102_REG_LED2_PA, 0x17))
        return false;

    // if(!MAX30102_WriteReg(MAX30102_REG_PILOT_PA, 0x7f))
    //     return false;

    return true;
}

/*******************************************************************************/
/**
 * @brief  读 MAX30102 FIFO
 * @param red RED 通道值指针
 * @param ir IR 通道值指针
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_ReadFIFO(MAX30102_Data_t *data)
{
    uint8_t buff[6];

    if (data == NULL)
    {
        return false;
    }

    if (IIC_Read_Multi_Byte(
        MAX30102_bus,
        MAX30102_ADDR,
        MAX30102_REG_FIFO_DATA,
        6,
        buff
    ) != SUCCESS)
    {
        return false;
    }

    /*
     * RED 18bit
     */
    data->red =
        ((uint32_t)(buff[0] & 0x03) << 16) |
        ((uint32_t)(buff[1]) << 8) |
        buff[2];

    /*
     * IR 18bit
     */
    data->ir =
        ((uint32_t)(buff[3] & 0x03) << 16) |
        ((uint32_t)(buff[4]) << 8) |
        buff[5];

    return true;
}

/*******************************************************************************/
/**
 * @brief  读 MAX30102 ID
 * @param id ID指针
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_ReadID(uint8_t *id)
{
    if (id == NULL)
    {
        return false;
    }

    return MAX30102_ReadReg(MAX30102_REG_PART_ID, id);
}

/*******************************************************************************/
/**
 * @brief  MAX30102 进入睡眠模式
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_Sleep(void)
{
    return MAX30102_WriteReg(
        MAX30102_REG_MODE_CONFIG,
        0x00
    );
}

/*******************************************************************************/
/**
 * @brief  MAX30102 从睡眠模式唤醒
 * @retval 状态
 */
/*******************************************************************************/
bool MAX30102_Start(void)
{
    /*
     * 重新进入 SpO2 模式
     */
    if (!MAX30102_WriteReg(
            MAX30102_REG_MODE_CONFIG,
            0x03))
    {
        return false;
    }
    osDelay(1);
    return true;
}
