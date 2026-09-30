#ifndef __MAX30102_H__
#define __MAX30102_H__

#include "main.h"
#include "stdint.h"
#include "stdbool.h"
#include "my_iic_hal.h"

// MAX30102 7-bit I2C地址
#define MAX30102_ADDR 0x57

/* 寄存器地址 */
#define MAX30102_REG_INTR_STATUS_1    0x00
#define MAX30102_REG_INTR_STATUS_2    0x01

#define MAX30102_REG_INTR_ENABLE_1    0x02
#define MAX30102_REG_INTR_ENABLE_2    0x03

#define MAX30102_REG_FIFO_WR_PTR      0x04
#define MAX30102_REG_FIFO_OVF_COUNTER 0x05
#define MAX30102_REG_FIFO_RD_PTR      0x06
#define MAX30102_REG_FIFO_DATA        0x07
#define MAX30102_REG_FIFO_CONFIG      0x08

#define MAX30102_REG_MODE_CONFIG      0x09
#define MAX30102_REG_SPO2_CONFIG      0x0A

#define MAX30102_REG_LED1_PA          0x0C
#define MAX30102_REG_LED2_PA          0x0D

#define MAX30102_REG_PILOT_PA         0x10
#define MAX30102_REG_MULTI_LED_CTRL1  0x11
#define MAX30102_REG_MULTI_LED_CTRL2  0x12 
#define MAX30102_REG_TEMP_INTR        0x1F
#define MAX30102_REG_TEMP_FRAC        0x20
#define MAX30102_REG_TEMP_CONFIG      0x21
#define MAX30102_REG_PROX_INT_THRESH  0x30

#define MAX30102_REG_REV_ID           0xFE
#define MAX30102_REG_PART_ID          0xFF

/*
 * MAX30102 数据结构体
 * @param red 红色光数据
 * @param ir 红外光数据
 */
typedef struct {
    uint32_t red;
    uint32_t ir;

} MAX30102_Data_t;

/*
 * MAX30102 BSP接口
 */

bool MAX30102_Init(void);
bool MAX30102_ReadFIFO(MAX30102_Data_t *data);
bool MAX30102_ReadID(uint8_t *id);
bool MAX30102_ReadReg(uint8_t reg, uint8_t *value);
bool MAX30102_WriteReg(uint8_t reg, uint8_t value);
bool MAX30102_Reset(void);
bool MAX30102_Start(void);
bool MAX30102_Sleep(void);



#endif /*__MAX30102_H__*/
