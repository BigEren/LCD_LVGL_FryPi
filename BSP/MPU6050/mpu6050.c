#include "mpu6050.h"
#include "my_delay.h"

#define MPU_INT_PORT GPIOB
#define MPU_INT_PIN  GPIO_PIN_12

#define MPU6050_CLOCK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

/************************************************************************/
/**
 * @brief  初始化 MPU6050 INT 引脚
 */
/************************************************************************/
void MPU_INT_Pin_Init(void)
{
    printf("MPU_INT_Pin_Init in cubemx\n");
}

/************************************************************************/
/**
 * @brief  MPU6050 软件 I2C 总线对象
 * @note
 * 这里将 MPU6050 使用的 SDA、SCL 引脚绑定到软件 I2C 总线。
 *
 * SDA -> GPIOB PIN13
 * SCL -> GPIOB PIN14
 */
/************************************************************************/
static iic_bus_my_t *MPU6050_bus = &SensorBus;

/**
 * @brief  初始化 MPU6050 运动检测功能
 */
void MPU_Motion_Init(void)			
{
    MPU6050_WriteReg(MPU_MOTION_DET_REG,0x01);    // 设置运动检测阈值为 2mg
    MPU6050_WriteReg(MPU_MOTION_DUR_REG,0x01);    // 设置运动检测持续时间为 1ms
    MPU6050_WriteReg(MPU_INTBP_CFG_REG,0X90);     // 设置 INT 引脚为低电平，持续时间为 50us
    MPU6050_WriteReg(MPU_INT_EN_REG,0x40);        // 使能 INT 中断   
}

/************************************************************************/
/**
 * @brief  写 MPU6050 寄存器
 * @param reg 寄存器地址
 * @param value 寄存器值
 * @retval 状态
 */
/************************************************************************/
bool MPU6050_WriteReg(uint8_t reg, uint8_t value)
{
    return IIC_Write_One_Byte(
        MPU6050_bus,
        MPU_ADDR,
        reg,
        value
    ) == SUCCESS;
}

/************************************************************************/
/**
 * @brief  写 MPU6050 寄存器
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 寄存器值长度
 * @param buff 寄存器值指针
 * @retval 状态
 */
/************************************************************************/
uint8_t MPU6050_WriteRegs(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buff)
{
    if (buff == NULL || len == 0)
    {
        return 1;
    }

    return (uint8_t)IIC_Write_Multi_Byte(
        MPU6050_bus,
        addr,
        reg,
        len,
        buff
    );
}

/************************************************************************/
/**
 * @brief  读 MPU6050 寄存器
 * @param reg 寄存器地址
 * @return uint8_t 寄存器值
 */
/************************************************************************/
uint8_t MPU6050_ReadReg(uint8_t reg)
{
    uint8_t value = 0;

    if (IIC_Read_One_Byte(
        MPU6050_bus,
        MPU_ADDR,
        reg,
        &value
    ) == SUCCESS)
    {
        return value;
    }

    return 0;
}

/************************************************************************/
/**
 * @brief  读 MPU6050 寄存器
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 寄存器值长度
 * @param buff 寄存器值指针
 * @retval 状态
 */
/************************************************************************/
uint8_t MPU6050_ReadRegs(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buff)
{
    if (buff == NULL || len == 0)
    {
        return false;
    }

    return IIC_Read_Multi_Byte(
        MPU6050_bus,
        addr,
        reg,
        len,
        buff
    );
}

/************************************************************************/
/**
 * @brief  读 MPU6050 ID 寄存器
 * @param id 寄存器值指针
 * @retval 状态
 */
/************************************************************************/
uint8_t MPU6050_ReadID(void)
{
    return MPU6050_ReadReg(
        MPU_ID
    );
}


uint8_t MPU_Write_Multi_Byte(uint8_t addr,uint8_t length,uint8_t buff[])
{
	if(IIC_Write_Multi_Byte(MPU6050_bus, MPU_ADDR << 1, addr,length, buff))
	{
		return 1;
	}
	return 0;
}

uint8_t MPU_Read_Multi_Byte(uint8_t addr, uint8_t length, uint8_t buff[])
{
	if(IIC_Read_Multi_Byte(MPU6050_bus, MPU_ADDR << 1, addr, length, buff))
	{
		return 1;
	}
	return 0;
}

/************************************************************************/
/**
 * @brief  初始化 MPU6050
 * @retval 状态
 */
/************************************************************************/
bool MPU_Init(void)
{
    uint8_t id;
    
    // 复位MPU6050
    if (!MPU6050_WriteReg(
        MPU_PWR_MGMT1_REG,
        0x80
    )) 
    {
        printf("MPU6050 复位失败\n");
        return false;
    }

    delay_ms(100);

    // 唤醒MPU6050
    if (!MPU6050_WriteReg(
        MPU_PWR_MGMT1_REG,
        0x00
    )) 
    {
        printf("MPU6050 唤醒失败\n");
        return false;
    }

    //G传感器, 2000dps
    if (
        !MPU_Set_Gyro_Fsr(3)
    )
    {
        printf("MPU6050 设置陀螺仪满量程失败\n");
        return false;
    }
    
    //A传感器, 2g
    if (
        !MPU_Set_Accel_Fsr(2)
    )
    {
        printf("MPU6050 设置加速度计满量程失败\n");
        return false;
    }

    // 采样率 50HZ
    if (
        !MPU_Set_Rate(50)
    )
    {
        printf("MPU6050 首次设置采样率: 失败\n");
        return false;
    }

    // 关闭所有中断
    if (!MPU6050_WriteReg(
        MPU_INT_EN_REG,
        0x00
    )) 
    {
        printf("MPU6050 关闭所有中断失败\n");
        return false;
    }

    // IIC主从模式关闭
    if (!MPU6050_WriteReg(
        MPU_USER_CTRL_REG,
        0x00
    )) 
    {
        printf("MPU6050 关闭IIC主从模式失败\n");
        return false;
    }

    // disable FIFO
    if (!MPU6050_WriteReg(
        MPU_FIFO_EN_REG,
        0x00
    )) 
    {
        printf("MPU6050 关闭FIFO失败\n");
        return false;
    }

    // INT active low
    if (!MPU6050_WriteReg(
        MPU_INTBP_CFG_REG,
        0x80
    )) 
    {
        printf("MPU6050 设置INT active low失败\n");
        return false;
    }

    // 读取 MPU6050 ID 寄存器
    if ((id = MPU6050_ReadID()) == 0)
    {
        printf("MPU6050 读取ID失败\n");
        return false;
    }

    // 检查 ID 是否正确
    if (id != MPU_ID)
    {
        printf("MPU6050 ID 错误\n");
        return false;
    }
    printf("ID: 0x%02X\n", id);

    //SET the internal 8MHz, sleep=0, cycle=1, TEMP_DIS=1 // low power modes
    if (!MPU6050_WriteReg(
        MPU_PWR_MGMT1_REG,
        0x28
    )) 
    {
        printf("MPU6050 设置低功耗模式失败\n");
        return false;
    }

    //enable accelerometer, disable gyroscope, set the wake up frequence=20Hz
    if (!MPU6050_WriteReg(
        MPU_PWR_MGMT2_REG,
        0x87
    )) 
    {
        printf("MPU6050 设置低功耗模式失败\n");
        return false;
    }

    // 采样率 50HZ
    if (
        !MPU_Set_Rate(50)
    )
    {
        printf("MPU6050 再次设置采样率: 失败\n");
        return false;
    }

    MPU_Motion_Init();
}

/********************************************************************/
/**
 * @brief  使 MPU6050 进入睡眠模式
 * @retval 状态
 */
/********************************************************************/
bool MPU_Sleep(void)
{
    return MPU6050_WriteReg(
        MPU_PWR_MGMT1_REG,
        0x48
    );  // sleep=1,cycle=0,temp_dis=1,internal 8MHz
}

/********************************************************************/
/**
 * @brief  使 MPU6050 从睡眠模式唤醒
 * @retval 状态
 */
/********************************************************************/
bool MPU_Wakeup(void)
{
    return MPU6050_WriteReg(
        MPU_PWR_MGMT1_REG,
        0x28
    );  // sleep=0, cycle=1, temp_dis=1, internal 8MHz
}

/********************************************************************/
/**
 * @brief  读取 MPU6050 状态寄存器
 * @retval 状态
 */
/********************************************************************/
uint8_t MPU_Read_Status(void)
{
    return MPU6050_ReadReg(
        MPU_INT_STA_REG
    );
}

/********************************************************************/
/**
 * @brief  设置 MPU6050 陀螺仪量程
 * @param fsr 陀螺仪量程
 * @retval 状态
 */
/********************************************************************/
bool MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(
        MPU_GYRO_CFG_REG,
        fsr << 3
    );
}

/********************************************************************/
/**
 * @brief  设置 MPU6050 加速度计量程
 * @param fsr 加速度计量程
 * @retval 状态
 */
/********************************************************************/
bool MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU6050_WriteReg(
        MPU_ACCEL_CFG_REG,
        fsr << 3
    );
}

/********************************************************************/
/**
 * @brief  设置 MPU6050 低通滤波器
 * @param lpf 低通滤波器
 * @retval 状态
 */
/********************************************************************/
bool MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data = 0;
    if (lpf >= 188)
    {
        data = 1;
    }
    else if (lpf >= 98)
    {
        data = 2;
    }
    else if (lpf >= 42)
    {
        data = 3;
    }
    else if (lpf >= 21)
    {
        data = 4;
    }
    else if (lpf >= 10)
    {
        data = 5;
    }
    else
    {
        data = 6;
    }

    return MPU6050_WriteReg(
        MPU_CFG_REG,
        data
    );
}

/********************************************************************/
/**
 * @brief  设置 MPU6050 采样率
 * @param rate 采样率
 * @retval 状态
 */
/********************************************************************/
bool MPU_Set_Rate(uint16_t rate)
{
    uint8_t data = 0;
    if (rate >= 1000) rate = 1000;
    if (rate < 4) rate = 4;
    data = 1000 / rate - 1;
    data = MPU6050_WriteReg(
        MPU_SAMPLE_RATE_REG,
        data
    );
    return MPU_Set_LPF(rate / 2);
}

/********************************************************************/
/**
 * @brief  读取 MPU6050 温度
 * @retval 温度
 */
/********************************************************************/
short MPU_Get_Temperature(void)
{
    uint8_t buff[2];
    short raw;
    float temp;
    return MPU6050_ReadRegs(
        MPU_ADDR,
        MPU_TEMP_OUTH_REG,
        2,
        buff
    );
    raw = (short)(buff[0] << 8 | buff[1]);
    temp=36.53 + ((double)raw) / 340;
    return temp * 100;
}

/********************************************************************/
/**
 * @brief  读取 MPU6050 陀螺仪数据
 * @param gx 陀螺仪 X 轴数据
 * @param gy 陀螺仪 Y 轴数据
 * @param gz 陀螺仪 Z 轴数据
 * @retval 状态
 */
/********************************************************************/
bool MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    uint8_t buff[6];
    bool res;
    res = MPU6050_ReadRegs(
        MPU_ADDR,
        MPU_GYRO_XOUTH_REG,
        6,
        buff
    );
    if (res)
    {
        *gx = ((short)buff[0] << 8) | buff[1];
        *gy = ((short)buff[2] << 8) | buff[3];
        *gz = ((short)buff[4] << 8) | buff[5];
    }
    return res;
}

/********************************************************************/
/**
 * @brief  读取 MPU6050 加速度数据
 * @param ax 加速度 X 轴数据
 * @param ay 加速度 Y 轴数据
 * @param az 加速度 Z 轴数据
 * @retval 状态
 */
/********************************************************************/
bool MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    uint8_t buff[6];
    bool res;
    res = MPU6050_ReadRegs(
        MPU_ADDR,
        MPU_ACCEL_XOUTH_REG,
        6,
        buff
    );
    if (res)
    {
        *ax = ((short)buff[0] << 8) | buff[1];
        *ay = ((short)buff[2] << 8) | buff[3];
        *az = ((short)buff[4] << 8) | buff[5];
    }
    return res;
}

/********************************************************************/
/**
 * @brief  计算 MPU6050 角度
 * @param roll 角度
 * @param pitch 角度
 */
/********************************************************************/
void MPU_Get_Angles(float *roll, float *pitch)
{
    short ax, ay, az;
    MPU_Get_Accelerometer(&ax, &ay, &az);
    *roll = atanf ((float)ay / (float)az);              // 计算 roll 角度
    *pitch = -atanf (ax / sqrtf(ay * ay + az * az));    // 计算 pitch 角度
}

bool MPU_isHorizontal(void)
{
    float roll, pitch;
    MPU_Get_Angles(&roll, &pitch);
    if (roll <= 0.50 && roll >= -0.50 && pitch <= 0.50 && pitch >= -0.50)
    {
        return true;
    }
    return false;
}

