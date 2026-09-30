#include "em70x8.h"
#include "my_iic_hal.h"

/*GPIOB Clock Enable*/
#define EM7028_CLK_ENABLE_B() __HAL_RCC_GPIOB_CLK_ENABLE()

/* my iic bus */
iic_bus_my_t EM7028_bus = {
    .IIC_SDA_PORT = GPIOB,
    .IIC_SCL_PORT = GPIOB,

    .IIC_SDA_PIN = GPIO_PIN_13,
    .IIC_SCL_PIN = GPIO_PIN_14,
};

/**
 * @brief 读取EM7028指定寄存器的一个字节数据
 * @param reg_addr 寄存器地址
 * @retval 读取到的字节数据
 */
uint8_t EM7028_Read_OneReg(unsigned char RegAddr)
{
    unsigned char dat;

    dat = IIC_Read_One_Byte(&EM7028_bus, EM7028_ADDR, RegAddr);

    return dat;
}

/**
 * @brief 写入EM7028指定寄存器的一个字节数据
 * @param reg_addr 寄存器地址
 * @param dat 要写入的字节数据
 * @retval 无
 */
void EM7028_Write_OneReg(unsigned char RegAddr, unsigned char dat)
{
    IIC_Write_One_Byte(&EM7028_bus, EM7028_ADDR, RegAddr, dat);
}

/**
 * @brief 获取EM7028芯片ID
 * @param None
 * @retval 芯片ID， 0x36 表示成功
 */
uint8_t EM7028_Read_ID(void)
{
    return EM7028_Read_OneReg(ID_REG);
}

/**
 * @brief 初始化EM7028芯片
 * @param None
 * @retval 0 成功， 1 芯片：ID检测失败
 */
uint8_t EM7028_hrs_Init(void)
{
    uint8_t i = 5;

    /* 初始化IIC总线 */
    IIC_Init(&EM7028_bus);

    /* 检查芯片ID */
    while (EM7028_Read_ID() != EM7028_ID && i)
    {
        HAL_Delay(100);
        i--;
    }

    /* 检查是否检测到芯片 */
    if (0 == i)
    {
        return 1;
    }

    /* 关闭心率检测，进入配置状态 */
    EM7028_Write_OneReg(HRS_CFG, 0x00);

    /*
     * HRS1 使能，HRS2 关闭
     * 心率测量使用 LED1，开启红光和红外传感器
     * LED1 打开时，测量结果存储到 HRS_DATA0
     */

     /* 设置HRS2数据偏移为0 */
     EM7028_Write_OneReg(HRS2_DATA_OFFSET, 0x00);
     
     /* 设置HRS2增益为1 */
     EM7028_Write_OneReg(HRS2_GAIN_CTRL, 0x01);

     /*
     * 配置 HRS1 控制寄存器：
     * HRS1 GAIN = 1
     * HRS1 RANGE = 8
     * HRS1 FREQ = 2.62144MHz，采样时间约 1.5625ms
     * HRS1 RES = 16 bits
     * HRS1 mode
     */
     EM7028_Write_OneReg(HRS1_CTRL, 0x47);

     /* 配置中断控制寄存器，LED 编程电流为 2.5mA */
     EM7028_Write_OneReg(INT_CTRL, 0x00);

     return 0;
}


/**
 * @brief  使能 EM7028 心率检测功能
 * @param  无
 * @return 0：使能成功；1：芯片 ID 检测失败
 */
uint8_t EM7028_hrs_Enable(void)
{
     uint8_t i = 5;

    /* 检查芯片ID */
    while (EM7028_Read_ID() != EM7028_ID && i)
    {
        HAL_Delay(100);
        i--;
    }

    /* 检查是否检测到芯片 */
    if (0 == i)
    {
        return 1;
    }

    /* 使能心率检测 */
    EM7028_Write_OneReg(HRS_CFG, 0x08);
    
    return 0;
}

/**
 * @brief  关闭 EM7028 心率检测功能
 * @param  无
 * @return 0：关闭成功；1：芯片 ID 检测失败
 */
uint8_t EM7028_hrs_Disable(void)
{
     uint8_t i = 5;

    /* 检查芯片ID */
    while (EM7028_Read_ID() != EM7028_ID && i)
    {
        HAL_Delay(100);
        i--;
    }

    /* 检查是否检测到芯片 */
    if (0 == i)
    {
        return 1;
    }

    /* 关闭心率检测 */
    EM7028_Write_OneReg(HRS_CFG, 0x00);
    
    return 0;
}

/**
 * @brief  获取 EM7028 HRS1 的 16 位心率原始数据
 * @param  无
 * @return HRS1_DATA0_H 和 HRS1_DATA0_L 组成的 16 位数据
 */
uint16_t EM7028_hrs_Read_Data(void)
{
    uint16_t dat;
    dat = EM7028_Read_OneReg(HRS1_DATA0_H) << 8;
    dat |= EM7028_Read_OneReg(HRS1_DATA0_L);
    return dat;
}