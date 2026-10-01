#include "my_iic_hal.h"
#include "my_delay.h"

#define IIC_DELAY_US_FAST1    1
#define IIC_DELAY_US_FAST2    2

#define IIC_DELAY_US_SLOW1    15
#define IIC_DELAY_US_SLOW2    20



iic_bus_my_t SensorBus = 
{
    .IIC_SDA_PORT = GPIOC,
    .IIC_SCL_PORT = GPIOA,

    .IIC_SDA_PIN = GPIO_PIN_9,
    .IIC_SCL_PIN = GPIO_PIN_8,

    .IIC_MUTEX = NULL,
};

/**
  * @brief SDA输入模式设置
  * @param None
  * @retval None
  */
 void SDA_Input_Mode(iic_bus_my_t *bus)
 {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);
 }

/**
  * @brief SDA输出模式设置
  * @param None
  * @retval None
  */
 void SDA_Output_Mode(iic_bus_my_t *bus)
 {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);
 }

/**
  * @brief SDA输出设置
  * @param val 输出值
  * @retval None
  */
 void SDA_Output(iic_bus_my_t *bus, uint16_t val)
 {
    if (val)
    {
        bus->IIC_SDA_PORT->BSRR |= bus->IIC_SDA_PIN;
    }
    else
    {
        bus->IIC_SDA_PORT->BSRR = (uint32_t)bus->IIC_SDA_PIN << 16U;
    }
 }

 /**
  * @brief SCL输出设置
  * @param val 输出值
  * @retval None
  */
 void SCL_Output(iic_bus_my_t *bus, uint16_t val)
 {
    if (val)
    {
        bus->IIC_SCL_PORT->BSRR |= bus->IIC_SCL_PIN;
    }
    else
    {
        bus->IIC_SCL_PORT->BSRR = (uint32_t)bus->IIC_SCL_PIN << 16U;
    }
 }

 /**
  * @brief SDA输入模式设置
  * @param None
  * @retval GPIO_PinState
  */
 uint8_t SDA_Input(iic_bus_my_t *bus)
 {
    if (HAL_GPIO_ReadPin(bus->IIC_SDA_PORT, bus->IIC_SDA_PIN) == GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
 }

 /**
  * @brief IIC开始信号发送
  * @param None
  * @retval None
  */
 void IICStart(iic_bus_my_t *bus)
 {
    SDA_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW2);
    SCL_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
    SDA_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
 }

 /**
  * @brief IIC停止信号发送
  * @param None
  * @retval None
  */
 void IICStop(iic_bus_my_t *bus)
 {
    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW2);
    SDA_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
    SDA_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
 }

 /**
  * @brief IIC等待应答
  * @param None
  * @retval 状态(SUCCESS/ERROR)
  */
 uint8_t IICWaitAck(iic_bus_my_t *bus)
{
    uint16_t cErrTime = 5;

    SDA_Input_Mode(bus);

    SCL_Output(bus, 1);

    delay_us(IIC_DELAY_US_SLOW1);

    while (SDA_Input(bus))
    {
        cErrTime--;

        if (cErrTime == 0)
        {
            SCL_Output(bus, 0);
            delay_us(1);

            SDA_Output_Mode(bus);
            IICStop(bus);

            return ERROR;
        }

        delay_us(1);
    }

    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);

    SDA_Output_Mode(bus);

    return SUCCESS;
}

 /**
  * @brief IIC发送应答
  * @param None
  * @retval None
  */
 void IICSendAck(iic_bus_my_t *bus)
 {
    SDA_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
 }

 /**
  * @brief IIC发送非应答
  * @param None
  * @retval None
  */
 void IICSendNotAck(iic_bus_my_t *bus)
 {
    SDA_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 1);
    delay_us(IIC_DELAY_US_SLOW1);
    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW1);
 }

 /**
  * @brief IIC发送字节
  * @param SendByte 字节数据
  * @retval None
  */
 void IICSendByte(iic_bus_my_t *bus, uint8_t SendByte)
 {
    unsigned char i = 8;
    while (i--)
    {
        SCL_Output(bus, 0);
        delay_us(IIC_DELAY_US_SLOW2);
        SDA_Output(bus, SendByte & 0x80);
        delay_us(IIC_DELAY_US_SLOW1);
        SendByte <<= 1;
        delay_us(IIC_DELAY_US_SLOW1);
        SCL_Output(bus, 1);
        delay_us(IIC_DELAY_US_SLOW1);
    }
    SCL_Output(bus, 0);
    delay_us(IIC_DELAY_US_SLOW2);
 }

 /**
  * @brief IIC接收字节
  * @param None
  * @retval 字节数据
  */
 uint8_t IICReceiveByte(iic_bus_my_t *bus)
 {
    uint8_t i = 8;
    uint8_t RecByte = 0;
    SDA_Input_Mode(bus);
    while (i--)
    {
        RecByte <<= 1;
        SCL_Output(bus, 0);
        delay_us(IIC_DELAY_US_SLOW2);
        SCL_Output(bus, 1);
        delay_us(IIC_DELAY_US_SLOW1);
        RecByte |= SDA_Input(bus);
    }
    SCL_Output(bus, 0);
    SDA_Output_Mode(bus);
    return RecByte;
 }

 /**
  * @brief IIC写入一个字节
  * @param daddr 设备地址
  * @param reg 寄存器地址
  * @param data 字节数据
  * @retval 状态(SUCCESS/ERROR)
  */
 uint8_t IIC_Write_One_Byte(iic_bus_my_t *bus, uint8_t daddr, uint8_t reg, uint8_t data)
 {
    if (IIC_Lock(bus, osWaitForever) == ERROR)
    {
        return ERROR;
    }

    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus) == ERROR) //等待应答
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICSendByte(bus, reg);

    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICSendByte(bus, data);

    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICStop(bus);

    delay_us(IIC_DELAY_US_SLOW1);
    
    IIC_Unlock(bus);

    return SUCCESS;
 }

 /**
  * @brief IIC写入多个字节
  * @param daddr 设备地址
  * @param reg 寄存器地址
  * @param length 字节数
  * @param buff[length] 数据缓冲区
  * @retval 状态(SUCCESS/ERROR)
  */
 uint8_t IIC_Write_Multi_Byte(iic_bus_my_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[])
 {
    if (IIC_Lock(bus, osWaitForever) == ERROR)
    {
        return ERROR;
    }

    uint8_t i;
    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus) == ERROR) //等待应答
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICSendByte(bus, reg);
    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }
    for (i = 0; i < length; i++)
    {
        IICSendByte(bus, buff[i]);
        if (IICWaitAck(bus) == ERROR)
        {
            IICStop(bus);
            IIC_Unlock(bus);
            return ERROR;
        }
    }

    IICStop(bus);
    delay_us(1);
    IIC_Unlock(bus);

    return SUCCESS;
 }

 /**
  * @brief IIC读取一个字节
  * @param daddr 设备地址
  * @param reg 寄存器地址
  * @retval 字节数据
  */
 uint8_t IIC_Read_One_Byte(iic_bus_my_t *bus, uint8_t daddr, uint8_t reg, uint8_t *dat)
 {
    if (dat == NULL)
    {
        return ERROR;
    }

    if (IIC_Lock(bus, osWaitForever) == ERROR)
    {
        return ERROR;
    }
    
    IICStart(bus);

    IICSendByte(bus, daddr << 1);

    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICSendByte(bus, reg);
    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICStart(bus);
    IICSendByte(bus, (daddr << 1) | 0x01);

    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }
    
    *dat = IICReceiveByte(bus);
    IICSendNotAck(bus);
    IICStop(bus);
    IIC_Unlock(bus);
    return SUCCESS;
 }

 /**
  * @brief IIC读取多个字节
  * @param daddr 设备地址
  * @param reg 寄存器地址
  * @param length 字节数
  * @param buff[length] 数据缓冲区
  * @retval 状态(SUCCESS/ERROR)
  */
 uint8_t IIC_Read_Multi_Byte(
    iic_bus_my_t *bus,
    uint8_t daddr,
    uint8_t reg,
    uint8_t length,
    uint8_t buff[])
{
    uint8_t i;

    if (IIC_Lock(bus, osWaitForever) != SUCCESS)
    {
        return ERROR;
    }

    IICStart(bus);

    IICSendByte(bus, daddr << 1);
    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    IICSendByte(bus, reg);
    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    /* repeated START */
    IICStart(bus);

    IICSendByte(bus, (daddr << 1) | 0x01);
    if (IICWaitAck(bus) == ERROR)
    {
        IICStop(bus);
        IIC_Unlock(bus);
        return ERROR;
    }

    for (i = 0; i < length; i++)
    {
        buff[i] = IICReceiveByte(bus);

        if (i < length - 1)
        {
            IICSendAck(bus);
        }
    }

    IICSendNotAck(bus);
    IICStop(bus);

    IIC_Unlock(bus);

    return SUCCESS;
}

 /**
  * @brief IIC初始化
  * @param bus IIC总线结构体指针
  * @param clk_enable 时钟使能
  * @retval None
  */
void IICInit(iic_bus_my_t *bus, uint8_t clk_enable)
{
    if (bus == NULL)
    {
        return;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (clk_enable & IIC_CLK_GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }

    if (clk_enable & IIC_CLK_GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    if (clk_enable & IIC_CLK_GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // SDA
    GPIO_InitStruct.Pin = bus->IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);

    // SCL
    GPIO_InitStruct.Pin = bus->IIC_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(bus->IIC_SCL_PORT, &GPIO_InitStruct);

    // 初始化总线
    SDA_Output(bus, 1);
    SCL_Output(bus, 1);

    // 初始化互斥锁
    bus->IIC_MUTEX = osMutexNew(NULL);
}

/**
  * @brief IIC上锁
  * @param bus IIC总线结构体指针
  * @param timeout 超时时间
  * @retval 状态(SUCCESS/ERROR)
  */
uint8_t IIC_Lock(iic_bus_my_t *bus, uint32_t timeout)
{
    if (bus == NULL || bus->IIC_MUTEX == NULL)
    {
        return ERROR;
    }
    
    if (osMutexAcquire(bus->IIC_MUTEX, timeout) == osOK)
    {
        return SUCCESS;
    }

    return ERROR;
}

/**
  * @brief IIC解锁
  * @param bus IIC总线结构体指针
  * @retval None
  */
void IIC_Unlock(iic_bus_my_t *bus)
{
    if (bus != NULL && bus->IIC_MUTEX != NULL)
    {
        osMutexRelease(bus->IIC_MUTEX);
    } 
}

