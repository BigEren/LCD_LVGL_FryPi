#include "LSM303.h"
#include "math.h"

#define PI 3.1415926
#define Delayms(X) HAL_Delay(X)

iic_bus_my_t *LSM303_bus = &SensorBus;

/**
 * @brief 读取单个寄存器
 * @param RegAddr 寄存器地址
 * @retval 读到的寄存器8bit数据
 */
unsigned char LSM303_ReadOneReg(unsigned char RegAddr)
{
    unsigned char dat;
    unsigned char SlaveAddr = (RegAddr > 0x19) ? LSM303_SlaveAddr_A : LSM303_SlaveAddr_M;
    IIC_Read_One_Byte(LSM303_bus, SlaveAddr, RegAddr, &dat);
    return dat;
}

/**
 * @brief 连续读取多个寄存器
 * @param RegAddr 起始寄存器地址
 * @param RegNum 读取寄存器个数
 * @param DataBuff 数据接收缓冲区
 */
void LSM303_ReadMultiReg(unsigned char RegAddr, unsigned char RegNum, unsigned char DataBuff[])
{
    unsigned char i;
    for (i = 0; i < RegNum; i++)
    {
        DataBuff[i] = LSM303_ReadOneReg(RegAddr + i);
    }
}

/**
 * @brief 温度相关单寄存器读取（磁力计地址域）
 * @param RegAddr 寄存器地址
 * @retval 读到的寄存器8bit数据
 */
unsigned char LSM303_Temp_ReadOneReg(unsigned char RegAddr)
{
    unsigned char dat;
	IIC_Read_One_Byte(LSM303_bus, LSM303_SlaveAddr_M, RegAddr, &dat);
	return dat;
}

/**
 * @brief 写单个寄存器
 * @param RegAddr 寄存器地址
 * @param dat 需要写入的8bit数据
 * @retval true成功，false失败
 */
bool LSM303_WriteOneReg(unsigned char RegAddr, unsigned char dat)
{
    unsigned char SlaveAddr = (RegAddr > LSM303_SlaveAddr_A) ? LSM303_SlaveAddr_A : LSM303_SlaveAddr_M;
    return IIC_Write_One_Byte(LSM303_bus, SlaveAddr, RegAddr, dat) == SUCCESS;
}

/**
 * @brief LSM303DLH初始化：加速度计、磁力计配置
 * @retval 0成功，非0失败
 */
unsigned char LSM303DLH_Init(void)
{
    unsigned char temp;
    unsigned char retry = 0;

    for (retry = 0; retry < 3; retry++)
    {
        LSM303_WriteOneReg(LSM303_CTRL_REG4_A, 0x10);           // CTRL_REG4: FS=01，量程 ±4g
        Delayms(1);
        LSM303_WriteOneReg(LSM303_CTRL_REG1_A, 0x2F);           // CTRL_REG1: 低功耗模式，10Hz输出，XYZ三轴使能
        Delayms(1);
        temp = LSM303_ReadOneReg(LSM303_CTRL_REG1_A);           // 回读寄存器校验写入是否成功
        if (temp != 0x2F)
        {
            Delayms(10);                                        // 写入失败，延时后重试
        }
        else break;
    }
    if (temp != 0x2F)
    {
        return 1;                                               // 尝试三次仍失败
    }
    Delayms(1);

    // ========== 磁力计初始化，最多重试3次 ==========
    for(retry = 0;retry < 3;retry ++)
    {
        LSM303_WriteOneReg(LSM303_CRA_REG_M, 0x10);             // CRA_REG_M: 磁力计输出速率15Hz，关闭温度传感器
        Delayms(1); 
        LSM303_WriteOneReg(LSM303_CRB_REG_M, 0x80);             // CRB_REG_M: 增益配置 ±4.0Gauss，XY轴450 LSB/Gs，Z轴400 LSB/Gs
        Delayms(1); 
        LSM303_WriteOneReg(LSM303_MR_REG_M, 0x00);              // MR_REG_M: 连续转换模式，持续采集磁场数据
        Delayms(1);                                                       
        
        temp = LSM303_ReadOneReg(LSM303_MR_REG_M);      
        if(temp != 0)                                    
        {
            Delayms(10);                                        // 写入失败，延时重试
        }
        else break;                                             // 写入成功，退出重试循环
    }
    if(temp != 0)                                        
    {
        return 1;                                               // 磁力计初始化失败返回1
    }
    return 0;                                                   // 全部初始化成功返回0
}

/**
 * @brief 芯片进入休眠模式，降低功耗
 * @retval true成功，false失败
 */
bool LSM303DLH_Sleep(void)
{
    return LSM303_WriteOneReg(LSM303_MR_REG_M, 0x03)                 // MR_REG_M: 低功耗模式，10Hz输出，关闭三轴使能
    && LSM303_WriteOneReg(LSM303_CTRL_REG1_A, 0x0F);               // CTRL_REG1: 低功耗模式，10Hz输出，关闭三轴使能
}

/**
 * @brief 唤醒芯片，恢复测量
 * @retval true成功，false失败
 */
bool LSM303DLH_Wakeup(void)
{
    return LSM303_WriteOneReg(LSM303_MR_REG_M, 0x00)                  // MR_REG_M: 连续转换模式，持续采集磁场数据，XYZ三轴使能
    && LSM303_WriteOneReg(LSM303_CTRL_REG1_A, 0x2F);               // CTRL_REG1: 低功耗模式，10Hz输出，XYZ三轴使能
}

/**
 * @brief 读取三轴加速度原始数据
 * @param Xa X轴加速度原始值输出指针
 * @param Ya Y轴加速度原始值输出指针
 * @param Za Z轴加速度原始值输出指针
 */
void LSM303_ReadAcceleration(int16_t *Xa, int16_t *Ya, int16_t *Za)
{
    uint8_t buff[6];
    int16_t temp;
    LSM303_ReadMultiReg(LSM303_OUT_X_L_A, 6, buff);

    temp = buff[1];
    temp <<= 8;
    temp |= buff[0];
    *Xa = temp;

    temp = buff[3];
    temp <<= 8;
    temp |= buff[2];
    *Ya = temp;

    temp = buff[5];
    temp <<= 8;
    temp |= buff[4];
    *Za = temp;
}
/**
 * @brief 读取三轴磁力计原始数据
 * @param Xm X轴磁力原始值输出指针
 * @param Ym Y轴磁力原始值输出指针
 * @param Zm Z轴磁力原始值输出指针
 */
void LSM303_ReadMagnetic(int16_t *Xm, int16_t *Ym, int16_t *Zm)
{
    uint8_t buff[6];
    int16_t temp;
    LSM303_ReadMultiReg(LSM303_OUT_X_L_M, 6, buff);

    temp = buff[0];
    temp <<= 8;
    temp |= buff[1];
    *Xm = temp;

    temp = buff[2];
    temp <<= 8;
    temp |= buff[3];
    *Zm = temp;

    temp = buff[4];
    temp <<= 8;
    temp |= buff[5];
    *Ym = temp;
}

/**
 * @brief 读取片上温度原始值
 * @param Temp 温度原始数据输出指针
 */
void LSM303_ReadTemperature(int16_t *Temp)
{
	uint8_t buff[2];
	int16_t temp;
	IIC_Read_Multi_Byte(LSM303_bus, LSM303_SlaveAddr_M, TEMP_OUT_H_M, 2, buff);
	temp = buff[0];
	temp <<= 8;
	temp |= buff[1];
	temp >>= 4;
	*Temp = temp / 3 + 0.5;//8 LSB/deg
}

/**
 * @brief 根据加速度计算Z轴倾角（俯仰/横滚之一）
 * @param Xa Ya Za 三轴加速度原始值
 * @retval 角度值，单位：度
 */
int LSM303DLH_CalculationZAxisAngle(int16_t Xa, int16_t Ya, int16_t Za)
{
    double A;
    float fx, fy, fz;

    A = sqrt((int)Xa * Xa + (int)Ya * Ya + (int)Za * Za);
    fx = Xa / A;
    fy = Ya / A;
    fz = Za / A;

    // 计算Z轴倾角（俯仰/横滚之一）
    A = fx *fx + fy *fy;
    A = sqrt(A);
    A = (double)A / fz;
    A = atan(A) * 180 / PI;

    if(A >= 0)
    {
        A = 90  - A;
    }
    else
    {
        A += 90;
        A = -A;
    }

    return A * 100;
}

/**
 * @brief 根据加速度计算X轴倾角（俯仰/横滚之一）
 * @param Xa Ya Za 三轴加速度原始值
 * @retval 角度值，单位：度
 */
int LSM303DLH_CalculationXAxisAngle(int16_t Xa, int16_t Ya, int16_t Za)
{
    double A;
	float fx,fy,fz;
	
	A = sqrt((int)Xa*Xa + (int)Ya*Ya + (int)Za*Za);	//计算角加速度的矢量模长 |A|=根号下(X*X+Y*Y+Z*Z)
	fx = Xa/A;
	fy = Ya/A;
	fz = Za/A;
	
	
	//X方向
	A = fz*fz+fy*fy;
	A = sqrt(A);
	
	A = (double)A/fx;
	A = atan(A); 
	A = A*180/PI;
	if(A < 0)
	{
		A += 90; //向上为正
	}
	else
	{
		A = 90-A;
		A = 0-A; //向下为负
	}
	return A*100;
}

/**
 * @brief 计算航向角（方位角Azimuth，电子罗盘角度）
 * @param Xa,Ya,Za 加速度原始值
 * @param Xm,Ym,Zm 磁力计原始值
 * @retval 航向角，单位：度 0~360
 */
float Azimuth_Calculate(int16_t Xa, int16_t Ya, int16_t Za, int16_t Xm, int16_t Ym, int16_t Zm)
{
    float pitch, roll, Hy, Hx, Azimuth; 
	pitch   = atan2f(Xa, sqrtf(Ya * Ya + Za * Za));
	roll    = atan2f(Ya, sqrtf(Xa * Xa + Za * Za));
	Hy      = Ym * cosf(roll) + Xm * sinf(roll) * sinf(pitch) - Zm * cosf(pitch) * sinf(roll);
	Hx      = Xm * cosf(pitch) + Zm * sinf(pitch);
	Azimuth = atan2f(Hy,Hx)*180.0/PI;
	return Azimuth;
}
