
#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "my_iic_hal.h"
#include "sys.h"
#include "math.h"
#include "my_delay.h"
#include "stdbool.h"

/*============================================================
 *                    MPU6050 寄存器定义
 *============================================================*/

/* 
 * 自检相关寄存器
 * 用于 MPU6050 加速度计和陀螺仪的自检功能
 */
// 陀螺仪 X 轴自检寄存器
#define MPU_SELF_TESTX_REG      0x0D    // 陀螺仪 X 轴自检寄存器
#define MPU_SELF_TESTY_REG      0x0E    // 陀螺仪 Y 轴自检寄存器
#define MPU_SELF_TESTZ_REG      0x0F    // 陀螺仪 Z 轴自检寄存器
#define MPU_SELF_TESTA_REG      0x10    // 加速度计自检寄存器


/* 
 * 基本采样与传感器配置
 */
// 采样率分频寄存器
#define MPU_SAMPLE_RATE_REG     0x19    // 采样率分频寄存器
#define MPU_CFG_REG             0x1A    // 数字低通滤波器配置寄存器
#define MPU_GYRO_CFG_REG        0x1B    // 陀螺仪量程配置寄存器
#define MPU_ACCEL_CFG_REG       0x1C    // 加速度计量程配置寄存器


/* 
 * 运动检测相关寄存器
 */
// 运动检测阈值寄存器
#define MPU_MOTION_DET_REG      0x1F    // 运动检测阈值寄存器
#define MPU_MOTION_DUR_REG      0x20    // 运动检测持续时间寄存器


/* 
 * FIFO 相关寄存器
 */
// FIFO 功能使能寄存器
#define MPU_FIFO_EN_REG         0x23    // FIFO 功能使能寄存器


/*============================================================
 *                    MPU6050 I2C 主机控制
 *============================================================*/

/*
 * MPU6050 内部 I2C 主机功能相关寄存器
 * MPU6050 内部可以通过辅助 I2C 总线连接其他传感器
 */

// I2C 主机控制寄存器
#define MPU_I2CMST_CTRL_REG     0x24    // I2C 主机控制寄存器

#define MPU_I2CSLV0_ADDR_REG    0x25    // I2C 从设备 0 地址寄存器
#define MPU_I2CSLV0_REG         0x26    // I2C 从设备 0 数据寄存器
#define MPU_I2CSLV0_CTRL_REG    0x27    // I2C 从设备 0 控制寄存器

#define MPU_I2CSLV1_ADDR_REG    0x28    // I2C 从设备 1 地址寄存器
#define MPU_I2CSLV1_REG         0x29    // I2C 从设备 1 数据寄存器
#define MPU_I2CSLV1_CTRL_REG    0x2A    // I2C 从设备 1 控制寄存器

#define MPU_I2CSLV2_ADDR_REG    0x2B    // I2C 从设备 2 地址寄存器
#define MPU_I2CSLV2_REG         0x2C    // I2C 从设备 2 数据寄存器
#define MPU_I2CSLV2_CTRL_REG    0x2D    // I2C 从设备 2 控制寄存器

#define MPU_I2CSLV3_ADDR_REG    0x2E    // I2C 从设备 3 地址寄存器
#define MPU_I2CSLV3_REG         0x2F    // I2C 从设备 3 数据寄存器
#define MPU_I2CSLV3_CTRL_REG    0x30    // I2C 从设备 3 控制寄存器

#define MPU_I2CSLV4_ADDR_REG    0x31    // I2C 从设备 4 地址寄存器
#define MPU_I2CSLV4_REG         0x32    // I2C 从设备 4 数据寄存器
#define MPU_I2CSLV4_DO_REG      0x33    // I2C 从设备 4 写数据寄存器
#define MPU_I2CSLV4_CTRL_REG    0x34    // I2C 从设备 4 控制寄存器
#define MPU_I2CSLV4_DI_REG      0x35    // I2C 从设备 4 读数据寄存器

#define MPU_I2CMST_STA_REG      0x36    // I2C 主机状态寄存器


/*============================================================
 *                    中断相关寄存器
 *============================================================*/
// 中断/旁路配置寄存器
#define MPU_INTBP_CFG_REG       0x37    // 中断/旁路配置寄存器
#define MPU_INT_EN_REG          0x38    // 中断使能寄存器
#define MPU_INT_STA_REG         0x3A    // 中断状态寄存器


/*============================================================
 *                    加速度计数据寄存器
 *============================================================*/
// 加速度 X 轴高 8 位
#define MPU_ACCEL_XOUTH_REG     0x3B    // 加速度 X 轴高 8 位
#define MPU_ACCEL_XOUTL_REG     0x3C    // 加速度 X 轴低 8 位

#define MPU_ACCEL_YOUTH_REG     0x3D    // 加速度 Y 轴高 8 位
#define MPU_ACCEL_YOUTL_REG     0x3E    // 加速度 Y 轴低 8 位

#define MPU_ACCEL_ZOUTH_REG     0x3F    // 加速度 Z 轴高 8 位
#define MPU_ACCEL_ZOUTL_REG     0x40    // 加速度 Z 轴低 8 位


/*============================================================
 *                    温度数据寄存器
 *============================================================*/
// 温度数据高 8 位
#define MPU_TEMP_OUTH_REG       0x41    // 温度数据高 8 位
#define MPU_TEMP_OUTL_REG       0x42    // 温度数据低 8 位


/*============================================================
 *                    陀螺仪数据寄存器
 *============================================================*/
// 陀螺仪 X 轴高 8 位
#define MPU_GYRO_XOUTH_REG      0x43    // 陀螺仪 X 轴高 8 位
#define MPU_GYRO_XOUTL_REG      0x44    // 陀螺仪 X 轴低 8 位

#define MPU_GYRO_YOUTH_REG      0x45    // 陀螺仪 Y 轴高 8 位
#define MPU_GYRO_YOUTL_REG      0x46    // 陀螺仪 Y 轴低 8 位

#define MPU_GYRO_ZOUTH_REG      0x47    // 陀螺仪 Z 轴高 8 位
#define MPU_GYRO_ZOUTL_REG      0x48    // 陀螺仪 Z 轴低 8 位


/*============================================================
 *                    运动检测状态
 *============================================================*/
// 运动检测状态寄存器
#define MPU_MOT_DET_STA_REG     0x61    // 运动检测状态寄存器


/*============================================================
 *                    I2C 从设备数据寄存器
 *============================================================*/
// I2C 从设备 0 输出数据寄存器
#define MPU_I2CSLV0_DO_REG      0x63    // I2C 从设备 0 输出数据寄存器
#define MPU_I2CSLV1_DO_REG      0x64    // I2C 从设备 1 输出数据寄存器
#define MPU_I2CSLV2_DO_REG      0x65    // I2C 从设备 2 输出数据寄存器
#define MPU_I2CSLV3_DO_REG      0x66    // I2C 从设备 3 输出数据寄存器


/*============================================================
 *                    其他控制寄存器
 *============================================================*/
// I2C 主机延时控制寄存器
#define MPU_I2CMST_DELAY_REG    0x67    // I2C 主机延时控制寄存器

#define MPU_SIGPATH_RST_REG     0x68    // 信号通道复位寄存器

#define MPU_MDETECT_CTRL_REG    0x69    // 运动检测控制寄存器

#define MPU_USER_CTRL_REG       0x6A    // 用户控制寄存器

#define MPU_PWR_MGMT1_REG       0x6B    // 电源管理寄存器 1
#define MPU_PWR_MGMT2_REG       0x6C    // 电源管理寄存器 2


/*============================================================
 *                    FIFO 数据寄存器
 *============================================================*/
// FIFO 计数器高 8 位
#define MPU_FIFO_CNTH_REG       0x72    // FIFO 计数器高 8 位
#define MPU_FIFO_CNTL_REG       0x73    // FIFO 计数器低 8 位
#define MPU_FIFO_RW_REG         0x74    // FIFO 读写寄存器


/*============================================================
 *                    设备 ID
 *============================================================*/
// MPU6050 设备 ID 寄存器
#define MPU_DEVICE_ID_REG       0x75    // MPU6050 设备 ID 寄存器


/*============================================================
 *                    MPU6050 I2C 地址
 *============================================================*/

/*
 * AD0 引脚决定 MPU6050 的 I2C 地址：
 *
 * AD0 = 0 -> 0x68
 * AD0 = 1 -> 0x69
 *
 * 注意：
 * 这里使用的是 7 位 I2C 从设备地址。
 * 实际进行 I2C 通信时，读写位由 I2C 驱动自动处理。
 */
#define MPU_ADDR                0x68

/*
 * MPU6050 默认设备 ID
 * WHO_AM_I 寄存器正常情况下返回 0x70
 */
#define MPU_ID                  0x70


/*============================================================
 *                    初始化相关函数
 *============================================================*/

/*
 * MPU6050 外部中断引脚初始化
 *
 * 用于配置 MPU6050 INT 引脚。
 * 如果使用运动检测唤醒功能，
 * 可以通过该引脚产生外部中断。
 * 这里使用了cubemx生成的代码。
 */

void MPU_INT_Pin_Init(void);


/*
 * MPU6050 运动检测功能初始化
 *
 * 配置运动检测阈值、持续时间以及运动中断。
 */

void MPU_Motion_Init(void);

/*
 * MPU6050 完整初始化
 *
 * 返回值：
 * false   初始化失败
 * true    初始化成功
 */

bool MPU_Init(void);


/*============================================================
 *                    I2C 读写函数
 *============================================================*/

/*
 * 连续写多个字节
 *
 * addr：设备地址
 * reg ：起始寄存器地址
 * len ：写入数据长度
 * buf ：待写入的数据
 */
uint8_t MPU6050_WriteRegs(
    uint8_t addr, 
    uint8_t reg, 
    uint8_t len, 
    uint8_t *buff
);


/*
 * 连续读取多个字节
 *
 * addr：设备地址
 * reg ：起始寄存器地址
 * len ：读取数据长度
 * buf ：接收数据缓冲区
 */

uint8_t MPU6050_ReadRegs(
    uint8_t addr,
    uint8_t reg,
    uint8_t len,
    uint8_t *buff
);


/*
 * 写一个字节到 MPU6050 寄存器
 */

 bool MPU6050_WriteReg(
    uint8_t reg, 
    uint8_t value
);


/*
 * 从 MPU6050 寄存器读取一个字节
 */

uint8_t MPU6050_ReadReg(uint8_t reg);


/*
 * 连续读取多个字节
 *
 * addr：起始寄存器地址
 * length：读取长度
 * buff：接收缓冲区
 */
uint8_t MPU_Read_Multi_Byte(
    uint8_t addr,
    uint8_t length,
    uint8_t buff[]
);


/*
 * 连续写入多个字节
 *
 * addr：起始寄存器地址
 * length：写入长度
 * buff：待写入数据
 */
uint8_t MPU_Write_Multi_Byte(
    uint8_t addr,
    uint8_t length,
    uint8_t buff[]
);


/*============================================================
 *                    MPU6050 参数配置
 *============================================================*/

/*
 * 设置陀螺仪满量程
 *
 * fsr：
 * 0 -> ±250 °/s
 * 1 -> ±500 °/s
 * 2 -> ±1000 °/s
 * 3 -> ±2000 °/s
 */
bool MPU_Set_Gyro_Fsr(uint8_t fsr);


/*
 * 设置加速度计满量程
 *
 * fsr：
 * 0 -> ±2g
 * 1 -> ±4g
 * 2 -> ±8g
 * 3 -> ±16g
 */

bool MPU_Set_Accel_Fsr(uint8_t fsr);


/*
 * 设置数字低通滤波器
 *
 * lpf：低通滤波器截止频率参数
 */

bool MPU_Set_LPF(uint16_t lpf);


/*
 * 设置采样率
 *
 * rate：目标采样率，单位 Hz
 */

bool MPU_Set_Rate(uint16_t rate);


/*
 * 设置 FIFO 功能
 *
 * sens：需要进入 FIFO 的传感器数据类型
 */
bool MPU_Set_Fifo(uint8_t sens);


/*============================================================
 *                    低功耗 / 唤醒
 *============================================================*/

/*
 * MPU6050 进入休眠模式
 *
 * 通过电源管理寄存器关闭传感器工作。
 * 适用于系统进入低功耗状态。
 */
bool MPU_Sleep(void);


/*
 * MPU6050 唤醒
 *
 * 退出休眠状态，恢复传感器工作。
 */
bool MPU_Wakeup(void);


/*
 * 读取 MPU6050 状态
 *
 * 用于读取当前传感器状态。
 */
uint8_t MPU_Read_Status(void);


/*============================================================
 *                    传感器数据读取
 *============================================================*/

/*
 * 获取 MPU6050 内部温度
 *
 * 返回值：温度原始数据
 */
short MPU_Get_Temperature(void);


/*
 * 获取三轴陀螺仪数据
 *
 * gx：X 轴角速度
 * gy：Y 轴角速度
 * gz：Z 轴角速度
 */
bool MPU_Get_Gyroscope(
    short *gx,
    short *gy,
    short *gz
);


/*
 * 获取三轴加速度数据
 *
 * ax：X 轴加速度
 * ay：Y 轴加速度
 * az：Z 轴加速度
 */
bool MPU_Get_Accelerometer(
    short *ax,
    short *ay,
    short *az
);


/*============================================================
 *                    姿态 / 手腕状态
 *============================================================*/

/*
 * 根据加速度数据计算姿态角
 *
 * roll ：横滚角
 * pitch：俯仰角
 */
void MPU_Get_Angles(
    float *roll,
    float *pitch
);


/*
 * 判断 MPU6050 当前是否处于水平状态
 *
 * 返回值：
 * 1 -> 水平
 * 0 -> 非水平
 */
bool MPU_isHorizontal(void);


#endif /* __MPU6050_H__ */
