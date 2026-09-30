#ifndef __EM70X8_H__
#define __EM70X8_H__

/* EM7028 芯片 ID */
#define EM7028_ID               0x36

/* EM7028 IIC 从设备地址 */
#define EM7028_ADDR             0x24

/* EM7028 寄存器地址定义 */
#define ID_REG                  0x00    /* 芯片 ID 寄存器 */
#define HRS_CFG                 0x01    /* 心率检测配置寄存器 */
#define HRS_INT_CTRL            0x02    /* 心率中断控制寄存器 */
#define HRS_LT_L                0x03    /* 心率低阈值低字节 */
#define HRS_LT_H                0x04    /* 心率低阈值高字节 */
#define HRS_HT_L                0x05    /* 心率高阈值低字节 */
#define HRS_HT_H                0x06    /* 心率高阈值高字节 */
#define LED_CRT                 0x07    /* LED 驱动电流控制寄存器 */

#define HRS2_DATA_OFFSET        0x08    /* HRS2 数据偏移设置 */
#define HRS2_CTRL               0x09    /* HRS2 控制寄存器 */
#define HRS2_GAIN_CTRL          0x0A    /* HRS2 增益控制寄存器 */
#define HRS1_CTRL               0x0D    /* HRS1 控制寄存器 */
#define INT_CTRL                0x0E    /* 中断及 LED 电流控制寄存器 */
#define SOFT_RESET              0x0F    /* 软件复位寄存器 */

/* HRS2 数据寄存器 */
#define HRS2_DATA0_L            0x20    /* HRS2 数据 0 低字节 */
#define HRS2_DATA0_H            0x21    /* HRS2 数据 0 高字节 */
#define HRS2_DATA1_L            0x22    /* HRS2 数据 1 低字节 */
#define HRS2_DATA1_H            0x23    /* HRS2 数据 1 高字节 */
#define HRS2_DATA2_L            0x24    /* HRS2 数据 2 低字节 */
#define HRS2_DATA2_H            0x25    /* HRS2 数据 2 高字节 */
#define HRS2_DATA3_L            0x26    /* HRS2 数据 3 低字节 */
#define HRS2_DATA3_H            0x27    /* HRS2 数据 3 高字节 */

/* HRS1 数据寄存器 */
#define HRS1_DATA0_L            0x28    /* HRS1 数据 0 低字节 */
#define HRS1_DATA0_H            0x29    /* HRS1 数据 0 高字节 */
#define HRS1_DATA1_L            0x2A    /* HRS1 数据 1 低字节 */
#define HRS1_DATA1_H            0x2B    /* HRS1 数据 1 高字节 */
#define HRS1_DATA2_L            0x2C    /* HRS1 数据 2 低字节 */
#define HRS1_DATA2_H            0x2D    /* HRS1 数据 2 高字节 */
#define HRS1_DATA3_L            0x2E    /* HRS1 数据 3 低字节 */
#define HRS1_DATA3_H            0x2F    /* HRS1 数据 3 高字节 */

uint8_t EM7028_Read_OneReg(uint8_t reg);
uint8_t EM7028_Write_OneReg(uint8_t reg, uint8_t data);
uint8_t EM7028_Read_ID(void);
uint8_t EM7028_hrs_Init(void);
uint8_t EM7028_hrs_Enable(void);
uint8_t EM7028_hrs_Disable(void);
uint16_t EM7028_hrs_Read_Data(void);



#endif /* __EM70X8_H__ */
