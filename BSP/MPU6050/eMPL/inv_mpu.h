/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */

/**
 *  @addtogroup  DRIVERS Sensor Driver Layer
 *  @brief       传感器驱动层
 *                用于通过 I2C 与传感器进行通信。
 *
 *  @
 *      @file       inv_mpu.h
 *      @brief      基于 I2C 的 InvenSense 陀螺仪/惯性传感器驱动接口。
 *      @details    当前该驱动支持以下设备：
 *
 *                  MPU6050
 *                  MPU6500
 *                  MPU9150
 *                  （MPU6050 + 辅助总线上的 AK8975）
 *
 *                  MPU9250
 *                  （MPU6500 + 辅助总线上的 AK8963）
 */

#ifndef _INV_MPU_H_
#define _INV_MPU_H_

#include "sys.h"


/*
 * 默认 MPU 采样频率
 *
 * 这里设置为 100Hz，
 * 即 MPU 默认的数据采样率为 100 次/秒。
 */
#define DEFAULT_MPU_HZ  (100)       // 100Hz


/*
 * 传感器类型标志
 *
 * 这些宏用于表示需要操作哪些传感器。
 *
 * MPU6050 本身主要包含：
 *      - 三轴陀螺仪
 *      - 三轴加速度计
 *
 * 某些 InvenSense 芯片还可以通过辅助 I2C 总线连接磁力计。
 */

/* X轴陀螺仪 */
#define INV_X_GYRO      (0x40)

/* Y轴陀螺仪 */
#define INV_Y_GYRO      (0x20)

/* Z轴陀螺仪 */
#define INV_Z_GYRO      (0x10)

/* 三轴陀螺仪 */
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)

/* 三轴加速度计 */
#define INV_XYZ_ACCEL   (0x08)

/* 三轴磁力计 */
#define INV_XYZ_COMPASS (0x01)


/*
 * 中断参数结构体
 *
 * 用于描述 MPU 中断相关的参数。
 *
 * 该结构体来自 InvenSense 官方驱动，
 * 原始代码主要用于不同 MCU 平台下的中断配置。
 */
struct int_param_s
{
    /*
     * 中断回调函数
     *
     * 当 MPU 产生相应中断时，
     * 可以通过该函数通知上层软件。
     */
    void (*cb)(void);

    /*
     * 中断对应的 GPIO 引脚
     */
    unsigned short pin;

    /*
     * 低功耗模式退出相关参数
     */
    unsigned char lp_exit;

    /*
     * 中断有效电平
     *
     * 0：高电平有效
     * 1：低电平有效
     */
    unsigned char active_low;
};


/*
 * MPU 中断状态标志
 *
 * 这些宏用于判断 MPU 当前产生了什么类型的中断。
 *
 * 每一个宏对应一个 bit。
 */

/* 数据准备完成中断 */
#define MPU_INT_STATUS_DATA_READY       (0x0001)

/* DMP 中断 */
#define MPU_INT_STATUS_DMP              (0x0002)

/* PLL 锁定完成中断 */
#define MPU_INT_STATUS_PLL_READY        (0x0004)

/* I2C 主机中断 */
#define MPU_INT_STATUS_I2C_MST          (0x0008)

/* FIFO 溢出中断 */
#define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)

/* 零运动检测中断 */
#define MPU_INT_STATUS_ZMOT             (0x0020)

/* 运动检测中断 */
#define MPU_INT_STATUS_MOT              (0x0040)

/* 自由落体检测中断 */
#define MPU_INT_STATUS_FREE_FALL        (0x0080)

/* DMP 中断 0 */
#define MPU_INT_STATUS_DMP_0            (0x0100)

/* DMP 中断 1 */
#define MPU_INT_STATUS_DMP_1            (0x0200)

/* DMP 中断 2 */
#define MPU_INT_STATUS_DMP_2            (0x0400)

/* DMP 中断 3 */
#define MPU_INT_STATUS_DMP_3            (0x0800)

/* DMP 中断 4 */
#define MPU_INT_STATUS_DMP_4            (0x1000)

/* DMP 中断 5 */
#define MPU_INT_STATUS_DMP_5            (0x2000)



/*========================================================
 * 初始化相关 API
 *========================================================*/

/*
 * 初始化 MPU。
 *
 * 一般在系统启动时调用，
 * 完成 MPU 基本寄存器以及工作状态的初始化。
 */
int mpu_init(void);


/*
 * 初始化 MPU 从设备。
 *
 * 用于 MPU 辅助 I2C 总线相关的从设备配置。
 */
int mpu_init_slave(void);


/*
 * 设置 MPU 的 I2C Bypass 模式。
 *
 * bypass_on：
 *      0：关闭 Bypass
 *      1：开启 Bypass
 *
 * 开启 Bypass 后，MCU 可以直接通过 I2C
 * 访问 MPU 辅助总线上的设备。
 */
int mpu_set_bypass(unsigned char bypass_on);



/*========================================================
 * MPU 配置相关 API
 *========================================================*/

/*
 * 设置低功耗加速度计模式。
 *
 * rate：
 *      低功耗模式下的采样/唤醒频率参数。
 */
int mpu_lp_accel_mode(unsigned char rate);


/*
 * 配置低功耗运动检测中断。
 *
 * thresh：
 *      运动检测阈值。
 *
 * time：
 *      运动持续时间。
 *
 * lpa_freq：
 *      低功耗加速度计工作频率。
 *
 * 这个接口与你现在做的“低功耗 + MPU6050运动唤醒”
 * 是直接相关的。
 */
int mpu_lp_motion_interrupt(unsigned short thresh,
                            unsigned char time,
                            unsigned char lpa_freq);


/*
 * 设置 MPU 中断有效电平。
 *
 * active_low：
 *      0：高电平有效
 *      1：低电平有效
 */
int mpu_set_int_level(unsigned char active_low);


/*
 * 设置 MPU 中断是否锁存。
 *
 * enable：
 *      0：不锁存
 *      1：锁存
 */
int mpu_set_int_latched(unsigned char enable);



/*
 * 设置 DMP 工作状态。
 *
 * enable：
 *      0：关闭 DMP
 *      1：开启 DMP
 */
int mpu_set_dmp_state(unsigned char enable);


/*
 * 获取当前 DMP 工作状态。
 *
 * enabled：
 *      返回 DMP 当前是否开启。
 */
int mpu_get_dmp_state(unsigned char *enabled);



/*
 * 获取低通滤波器（LPF）配置。
 */
int mpu_get_lpf(unsigned short *lpf);


/*
 * 设置低通滤波器（LPF）。
 */
int mpu_set_lpf(unsigned short lpf);



/*
 * 获取陀螺仪满量程范围（FSR）。
 */
int mpu_get_gyro_fsr(unsigned short *fsr);


/*
 * 设置陀螺仪满量程范围（FSR）。
 */
int mpu_set_gyro_fsr(unsigned short fsr);



/*
 * 获取加速度计满量程范围（FSR）。
 */
int mpu_get_accel_fsr(unsigned char *fsr);


/*
 * 设置加速度计满量程范围（FSR）。
 */
int mpu_set_accel_fsr(unsigned char fsr);



/*
 * 获取磁力计满量程范围。
 *
 * 主要用于带有辅助磁力计的 MPU 型号。
 */
int mpu_get_compass_fsr(unsigned short *fsr);



/*
 * 获取陀螺仪灵敏度。
 */
int mpu_get_gyro_sens(float *sens);


/*
 * 获取加速度计灵敏度。
 */
int mpu_get_accel_sens(unsigned short *sens);



/*
 * 获取 MPU 当前采样率。
 */
int mpu_get_sample_rate(unsigned short *rate);


/*
 * 设置 MPU 采样率。
 */
int mpu_set_sample_rate(unsigned short rate);


/*
 * 获取磁力计采样率。
 */
int mpu_get_compass_sample_rate(unsigned short *rate);


/*
 * 设置磁力计采样率。
 */
int mpu_set_compass_sample_rate(unsigned short rate);



/*
 * 获取 FIFO 当前配置。
 *
 * sensors：
 *      返回当前加入 FIFO 的传感器类型。
 */
int mpu_get_fifo_config(unsigned char *sensors);


/*
 * 配置 FIFO。
 *
 * sensors：
 *      指定哪些传感器数据进入 FIFO。
 */
int mpu_configure_fifo(unsigned char sensors);



/*
 * 获取 MPU 当前电源状态。
 *
 * power_on：
 *      返回当前 MPU 是否处于工作状态。
 */
int mpu_get_power_state(unsigned char *power_on);


/*
 * 设置需要工作的传感器。
 *
 * sensors：
 *      使用 INV_XYZ_GYRO、INV_XYZ_ACCEL
 *      等宏指定需要开启的传感器。
 */
int mpu_set_sensors(unsigned char sensors);


/*
 * 设置加速度计偏置。
 *
 * accel_bias：
 *      指向三轴加速度计偏置数据。
 */
int mpu_set_accel_bias(const long *accel_bias);



/*========================================================
 * 数据读取 / 设置 API
 *========================================================*/

/*
 * 读取陀螺仪寄存器中的原始数据。
 *
 * data：
 *      保存 X/Y/Z 三轴陀螺仪数据。
 *
 * timestamp：
 *      返回数据对应的时间戳。
 */
int mpu_get_gyro_reg(short *data,
                     unsigned long *timestamp);


/*
 * 读取加速度计寄存器中的原始数据。
 *
 * data：
 *      保存 X/Y/Z 三轴加速度数据。
 *
 * timestamp：
 *      返回数据对应的时间戳。
 */
int mpu_get_accel_reg(short *data,
                      unsigned long *timestamp);


/*
 * 读取磁力计寄存器中的原始数据。
 *
 * 主要用于带辅助磁力计的 MPU 型号。
 */
int mpu_get_compass_reg(short *data,
                        unsigned long *timestamp);


/*
 * 读取 MPU 内部温度。
 *
 * data：
 *      返回温度数据。
 *
 * timestamp：
 *      返回时间戳。
 */
int mpu_get_temperature(long *data,
                        unsigned long *timestamp);



/*
 * 获取 MPU 当前中断状态。
 *
 * status：
 *      返回当前中断状态标志。
 *
 * 可以通过：
 *
 *      MPU_INT_STATUS_DATA_READY
 *      MPU_INT_STATUS_MOT
 *      MPU_INT_STATUS_FREE_FALL
 *
 * 等宏判断具体是哪一种中断。
 */
int mpu_get_int_status(short *status);


/*
 * 从 FIFO 中读取陀螺仪和加速度计数据。
 *
 * gyro：
 *      陀螺仪数据。
 *
 * accel：
 *      加速度计数据。
 *
 * timestamp：
 *      数据时间戳。
 *
 * sensors：
 *      当前 FIFO 中包含哪些传感器数据。
 *
 * more：
 *      表示 FIFO 中是否还有更多数据。
 */
int mpu_read_fifo(short *gyro,
                  short *accel,
                  unsigned long *timestamp,
                  unsigned char *sensors,
                  unsigned char *more);


/*
 * 从 FIFO 中读取指定长度的数据流。
 *
 * length：
 *      需要读取的数据长度。
 *
 * data：
 *      数据缓存。
 *
 * more：
 *      表示 FIFO 中是否还有更多数据。
 */
int mpu_read_fifo_stream(unsigned short length,
                         unsigned char *data,
                         unsigned char *more);


/*
 * 重置 FIFO。
 *
 * 通常在 FIFO 溢出或者需要重新开始采集时调用。
 */
int mpu_reset_fifo();



/*========================================================
 * MPU 内部存储器 / 固件相关 API
 *========================================================*/

/*
 * 向 MPU 内部存储器写数据。
 *
 * mem_addr：
 *      MPU 内部存储器地址。
 *
 * length：
 *      写入数据长度。
 *
 * data：
 *      待写入的数据。
 */
int mpu_write_mem(unsigned short mem_addr,
                  unsigned short length,
                  unsigned char *data);


/*
 * 从 MPU 内部存储器读取数据。
 */
int mpu_read_mem(unsigned short mem_addr,
                 unsigned short length,
                 unsigned char *data);


/*
 * 向 MPU 加载 DMP 固件。
 *
 * length：
 *      固件长度。
 *
 * firmware：
 *      固件数据。
 *
 * start_addr：
 *      固件写入起始地址。
 *
 * sample_rate：
 *      DMP 工作采样率。
 */
int mpu_load_firmware(unsigned short length,
                      const unsigned char *firmware,
                      unsigned short start_addr,
                      unsigned short sample_rate);



/*========================================================
 * 调试 / 自检 / 中断回调 API
 *========================================================*/

/*
 * 输出 MPU 寄存器内容。
 *
 * 主要用于调试 MPU 当前寄存器配置。
 */
int mpu_reg_dump(void);


/*
 * 读取指定 MPU 寄存器。
 *
 * reg：
 *      寄存器地址。
 *
 * data：
 *      保存读取到的数据。
 */
int mpu_read_reg(unsigned char reg,
                 unsigned char *data);


/*
 * 执行 MPU 自检。
 *
 * gyro：
 *      返回陀螺仪自检结果。
 *
 * accel：
 *      返回加速度计自检结果。
 */
int mpu_run_self_test(long *gyro,
                      long *accel);


/*
 * 注册 Tap（敲击/点击）检测回调函数。
 *
 * 当检测到 Tap 事件时，
 * 驱动可以调用用户注册的回调函数。
 */
int mpu_register_tap_cb(void (*func)(unsigned char,
                                      unsigned char));



/*========================================================
 * 额外增加的辅助函数
 *========================================================*/

/*
 * 获取当前毫秒级时间。
 *
 * time：
 *      返回当前时间。
 */
void mget_ms(unsigned long *time);


/*
 * 根据方向矩阵的一行数据计算对应的缩放值。
 *
 * row：
 *      方向矩阵的一行。
 *
 * 返回：
 *      对应的缩放值。
 */
unsigned short inv_row_2_scale(const signed char *row);


/*
 * 将方向矩阵转换为 InvenSense 使用的标量表示。
 *
 * mtx：
 *      3x3 方向矩阵。
 *
 * 返回：
 *      转换后的方向标量。
 */
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx);


/*
 * 执行 MPU 自检。
 *
 * 返回值：
 *      0 / 非0，根据具体实现表示自检结果。
 */
u8 run_self_test(void);


/*
 * 初始化 MPU 的 DMP。
 *
 * 一般包括：
 *      1. MPU 基础初始化
 *      2. DMP 固件加载
 *      3. DMP 参数配置
 *      4. DMP 启动
 */
u8 mpu_dmp_init(void);


/*
 * 获取 DMP 计算出的姿态角。
 *
 * pitch：
 *      俯仰角
 *
 * roll：
 *      横滚角
 *
 * yaw：
 *      航向角
 */
u8 mpu_dmp_get_data(float *pitch,
                    float *roll,
                    float *yaw);


#endif  /* #ifndef _INV_MPU_H_ */