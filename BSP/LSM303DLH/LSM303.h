#ifndef _LSM303_H_ 
#define _LSM303_H_ 

#include "my_iic_hal.h" 
#include "stdbool.h"

/**
 * @brief LSM303DLH 三轴加速度 + 三轴磁力计（电子罗盘）驱动头文件
 * @note 芯片内部分为加速度计(A)与磁力计(M)两个I2C从设备，拥有独立地址
 */

//===================== 磁力计（罗盘）寄存器定义 =====================

//RW 配置寄存器A：输出速率、温度传感器使能
#define	LSM303_CRA_REG_M					0x00	
#define	LSM303_CRB_REG_M					0x01	//RW 配置寄存器B：磁力计增益量程设置
#define	LSM303_MR_REG_M						0x02	//RW 模式寄存器：连续转换/单次/休眠模式
#define	LSM303_OUT_X_H_M					0x03	//R 磁力计X轴高字节
#define	LSM303_OUT_X_L_M					0x04	//R 磁力计X轴低字节
#define	LSM303_OUT_Z_H_M					0x05	//R 磁力计Z轴高字节
#define	LSM303_OUT_Z_L_M					0x06	//R 磁力计Z轴低字节
#define	LSM303_OUT_Y_H_M					0x07	//R 磁力计Y轴高字节
#define	LSM303_OUT_Y_L_M					0x08	//R 磁力计Y轴低字节
#define	LSM303_SR_REG_M						0x09	//R 状态寄存器：数据就绪标志
#define	LSM303_IRA_REG_M					0x0A	//R 识别寄存器A：芯片ID
#define	LSM303_IRB_REG_M					0x0B	//R 识别寄存器B：芯片ID
#define	LSM303_IRC_REG_M					0x0C	//R 识别寄存器C：芯片ID

//===================== 加速度计寄存器定义 =====================

//RW 控制寄存器1：三轴使能、输出数据速率
#define	LSM303_CTRL_REG1_A					0x20	
#define	LSM303_CTRL_REG2_A					0x21	//RW 控制寄存器2：高通滤波配置
#define	LSM303_CTRL_REG3_A					0x22	//RW 控制寄存器3：中断信号映射
#define	LSM303_CTRL_REG4_A					0x23	//RW 控制寄存器4：量程、大端小端、自测试
#define	LSM303_CTRL_REG5_A					0x24	//RW 控制寄存器5：FIFO、中断使能
#define	LSM303_HP_FILTER_RESET_A		    0x25	//R 高通滤波复位寄存器，读操作清滤波
#define	LSM303_REFERENCE_A					0x26	//RW 参考值寄存器，高通滤波参考基准
#define	LSM303_STATUS_REG_A					0x27	//R 加速度状态寄存器：数据就绪、溢出标志
#define	LSM303_OUT_X_L_A					0x28	//R 加速度X轴低字节
#define	LSM303_OUT_X_H_A					0x29	//R 加速度X轴高字节
#define	LSM303_OUT_Y_L_A					0x2A	//R 加速度Y轴低字节
#define	LSM303_OUT_Y_H_A					0x2B	//R 加速度Y轴高字节
#define	LSM303_OUT_Z_L_A					0x2C	//R 加速度Z轴低字节
#define	LSM303_OUT_Z_H_A					0x2D	//R 加速度Z轴高字节
#define	LSM303_INT1_CFG_A					0x30	//RW INT1中断配置：触发条件
#define	LSM303_INT1_SOURCE_A				0x31	//R INT1中断源寄存器：中断触发标志
#define	LSM303_INT1_THS_A					0x32	//RW INT1中断阈值
#define	LSM303_INT1_DURATION_A				0x33	//RW INT1中断持续时间
#define	LSM303_INT2_CFG_A					0x34	//RW INT2中断配置：触发条件
#define	LSM303_INT2_SOURCE_A				0x35	//R INT2中断源寄存器：中断触发标志
#define	LSM303_INT2_THS_A					0x36	//RW INT2中断阈值
#define	LSM303_INT2_DURATION_A				0x37	//RW INT2中断持续时间

//===================== 温度传感器寄存器（集成在磁力计域） =====================

//R 温度高字节
#define TEMP_OUT_H_M					    0x31	
#define TEMP_OUT_L_M					    0x32	//R 温度低字节

//===================== I2C 从机地址 =====================

//加速度计I2C从地址
#define LSM303_SlaveAddr_A					0x18	
#define LSM303_SlaveAddr_M					0x1E	//磁力计+温度传感器I2C从地址			

//===================== 函数声明 =====================
/**
 * @brief 读取单个寄存器
 * @param RegAddr 寄存器地址
 * @retval 读到的寄存器8bit数据
 */
unsigned char LSM303_ReadOneReg(unsigned char RegAddr);

/**
 * @brief 连续读取多个寄存器
 * @param RegAddr 起始寄存器地址
 * @param RegNum 读取寄存器个数
 * @param DataBuff 数据接收缓冲区
 */
void LSM303_ReadMultiReg(unsigned char RegAddr, unsigned char RegNum, unsigned char DataBuff[]);

/**
 * @brief 温度相关单寄存器读取（磁力计地址域）
 * @param RegAddr 寄存器地址
 * @retval 读到的寄存器8bit数据
 */
unsigned char LSM303_Temp_ReadOneReg(unsigned char RegAddr);

/**
 * @brief 写单个寄存器
 * @param RegAddr 寄存器地址
 * @param dat 需要写入的8bit数据
 * @retval true成功，false失败
 */
bool LSM303_WriteOneReg(unsigned char RegAddr, unsigned char dat);

/**
 * @brief LSM303DLH初始化：加速度计、磁力计配置
 * @retval 0成功，非0失败
 */
unsigned char LSM303DLH_Init(void);

/**
 * @brief 芯片进入休眠模式，降低功耗
 * @retval true成功，false失败
 */
bool LSM303DLH_Sleep(void);

/**
 * @brief 唤醒芯片，恢复测量
 * @retval true成功，false失败
 */
bool LSM303DLH_Wakeup(void);

/**
 * @brief 读取三轴加速度原始数据
 * @param Xa X轴加速度原始值输出指针
 * @param Ya Y轴加速度原始值输出指针
 * @param Za Z轴加速度原始值输出指针
 */
void LSM303_ReadAcceleration(int16_t *Xa, int16_t *Ya, int16_t *Za);

/**
 * @brief 读取三轴磁力计原始数据
 * @param Xm X轴磁力原始值输出指针
 * @param Ym Y轴磁力原始值输出指针
 * @param Zm Z轴磁力原始值输出指针
 */
void LSM303_ReadMagnetic(int16_t *Xm, int16_t *Ym, int16_t *Zm);

/**
 * @brief 根据加速度计算Z轴倾角（俯仰/横滚之一）
 * @param Xa Ya Za 三轴加速度原始值
 * @retval 角度值，单位：度
 */
int LSM303DLH_CalculationZAxisAngle(int16_t Xa, int16_t Ya, int16_t Za);

/**
 * @brief 根据加速度计算X轴倾角（俯仰/横滚之一）
 * @param Xa Ya Za 三轴加速度原始值
 * @retval 角度值，单位：度
 */
int LSM303DLH_CalculationXAxisAngle(int16_t Xa, int16_t Ya, int16_t Za);

/**
 * @brief 读取片上温度原始值
 * @param Temp 温度原始数据输出指针
 */
void LSM303_ReadTemperature(int16_t *Temp);

/**
 * @brief 计算航向角（方位角Azimuth，电子罗盘角度）
 * @param Xa,Ya,Za 加速度原始值
 * @param Xm,Ym,Zm 磁力计原始值
 * @retval 航向角，单位：度 0~360
 */
float Azimuth_Calculate(int16_t Xa, int16_t Ya, int16_t Za, int16_t Xm, int16_t Ym, int16_t Zm);

#endif /*_LSM303_H_*/
