#ifndef __MY_IIC_HAL_H__
#define __MY_IIC_HAL_H__

#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os2.h"
#include "stdint.h"

#define IIC_CLK_GPIOA    (1U << 0)
#define IIC_CLK_GPIOB    (1U << 1)
#define IIC_CLK_GPIOC    (1U << 2)

typedef struct
{
    GPIO_TypeDef *IIC_SDA_PORT;  // SDA端口
    uint16_t IIC_SDA_PIN;         // SDA引脚

    GPIO_TypeDef *IIC_SCL_PORT;  // SCL端口
    uint16_t IIC_SCL_PIN;         // SCL引脚

    osMutexId_t IIC_MUTEX;        // IIC互斥锁
} iic_bus_my_t;

/* 全局唯一的软件I2C总线 */
extern iic_bus_my_t SensorBus;

/* GPIO */

void SDA_Input_Mode(iic_bus_my_t *bus);
void SDA_Output_Mode(iic_bus_my_t *bus);

void SDA_Output(iic_bus_my_t *bus, uint16_t val);
void SCL_Output(iic_bus_my_t *bus, uint16_t val);

uint8_t SDA_Input(iic_bus_my_t *bus);

/* I2C时序 */

void IICStart(iic_bus_my_t *bus);
void IICStop(iic_bus_my_t *bus);

uint8_t IICWaitAck(iic_bus_my_t *bus);
void IICSendAck(iic_bus_my_t *bus);
void IICSendNotAck(iic_bus_my_t *bus);

void IICSendByte(iic_bus_my_t *bus, uint8_t cSendByte);
uint8_t IICReceiveByte(iic_bus_my_t *bus);

/* 总线初始化 */
void IICInit(iic_bus_my_t *bus, uint8_t clk_enable );

/* 带总线锁的设备访问接口 */

uint8_t IIC_Write_One_Byte(iic_bus_my_t *bus, uint8_t daddr,uint8_t reg,uint8_t data);
uint8_t IIC_Write_Multi_Byte(iic_bus_my_t *bus, uint8_t daddr,uint8_t reg,uint8_t length,uint8_t buff[]);
uint8_t IIC_Read_One_Byte(iic_bus_my_t *bus, uint8_t daddr,uint8_t reg, uint8_t *dat);
uint8_t IIC_Read_Multi_Byte(iic_bus_my_t *bus, uint8_t daddr, uint8_t reg, uint8_t length, uint8_t buff[]);

/* 互斥锁 */

uint8_t IIC_Lock(
    iic_bus_my_t *bus,
    uint32_t timeout);

void IIC_Unlock(
    iic_bus_my_t *bus);

#endif /* __MY_IIC_HAL_H__ */
