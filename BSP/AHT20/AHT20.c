#include "AHT20.h"
#include "my_iic_hal.h"
#include "cmsis_os2.h"
#include "my_delay.h"

#define AHT20_IIC_ACK_CHECK(bus)            \
    do                                      \
    {                                       \
        if (IICWaitAck(bus) == ERROR)       \
        {                                   \
            IICStop(bus);                   \
            IIC_Unlock(bus);                \
            return false;                   \
        }                                   \
    } while (0)

/*
 * AHT20 使用公共 SensorBus
 */
static iic_bus_my_t *AHT20_bus = &SensorBus;

/**
 * @brief 发送 AHT20 三字节命令
 *
 * 例如：
 *   0xBE 0x08 0x00 -> 初始化
 *   0xAC 0x33 0x00 -> 测量
 */
static bool AHT_SendCommand(uint8_t cmd1, uint8_t param1, uint8_t param2)
{
	if (IIC_Lock(AHT20_bus, osWaitForever) != SUCCESS)
	{
		return false;
	}

	IICStart(AHT20_bus);

	// 发送设备地址（写操作0x70）
	IICSendByte(AHT20_bus, AHT20_ADDR << 1);
	AHT20_IIC_ACK_CHECK(AHT20_bus);

	// 发送命令字节 1
	IICSendByte(AHT20_bus, cmd1);
	AHT20_IIC_ACK_CHECK(AHT20_bus);

	// 发送参数字节 1
	IICSendByte(AHT20_bus, param1);
	AHT20_IIC_ACK_CHECK(AHT20_bus);

	// 发送参数字节 2
	IICSendByte(AHT20_bus, param2);
	AHT20_IIC_ACK_CHECK(AHT20_bus);

	IICStop(AHT20_bus);
	// 解锁总线
	IIC_Unlock(AHT20_bus);

	return true;
}

/**
 * @brief  读取 AHT20 状态寄存器
 *
 * @note
 * AHT20 的设备地址：
 *
 * 写地址：0x70
 * 读地址：0x71
 *
 * 读取状态时：
 *
 * START
 *   -> 发送 0x71（设备读地址）
 *   -> 接收 1 个字节状态数据
 * STOP
 *
 * @return AHT20 状态寄存器的值
 */
bool AHT_Read_Status(uint8_t *status)
{
	if (IIC_Lock(AHT20_bus, osWaitForever) != SUCCESS || status == NULL)
	{
		return false;
	}

    IICStart(AHT20_bus);

	// 发送设备地址（读操作0x71）
    IICSendByte(AHT20_bus, (AHT20_ADDR << 1) | 0x01);
    AHT20_IIC_ACK_CHECK(AHT20_bus);

    *status = IICReceiveByte(AHT20_bus);

    IICSendNotAck(AHT20_bus);
    IICStop(AHT20_bus);

	// 解锁总线
	IIC_Unlock(AHT20_bus);

    return true;
}


/**
 * @brief  检查 AHT20 校准状态
 *
 * @note
 * AHT20 状态寄存器中：
 *
 * Bit[3] = CAL
 *   0 -> 未校准
 *   1 -> 已校准
 *
 * 同时这里通过 0x68 检查相关状态位，
 * 判断 AHT20 是否处于正常工作状态。
 *
 * @return
 * 1 -> 校准正常
 * 0 -> 校准异常
 */
uint8_t AHT_Read_Cal_Enable(void)
{
    uint8_t val = 0;

    AHT_Read_Status(&val);

    if((val & 0x68) == 0x08) // 检查 Bit[3] = CAL, 并且 Bit[5]/Bit[6] = 0
    {
        return 1; // 校准正常
    }
    else
    {
        return 0; // 校准异常
    }
}


/**
 * @brief  AHT20 软件复位
 *
 * @note
 * AHT20 软复位命令：
 *
 * 设备写地址：0x70
 * 复位命令：  0xBA
 *
 * 通信过程：
 *
 * START
 *   -> 0x70
 *   -> 0xBA
 * STOP
 */
bool AHT_Reset(void)
{
    if (!AHT_SendCommand(AHT20_CMD_RESET, 0x00, 0x00))
    {
        return false;
    }

	// 等待 AHT20 复位完成（建议等待 20ms）
    delay_ms(20);
    return true;
}


/**
 * @brief  AHT20 初始化
 *
 * @note
 * 初始化流程：
 *
 * 1. 开启 GPIOA、GPIOC 时钟
 * 2. 初始化软件 I2C
 * 3. 等待 AHT20 上电稳定
 * 4. 读取状态寄存器
 * 5. 如果未校准，则发送初始化命令
 *
 * AHT20 初始化命令：
 *
 * 写地址：0x70
 * 命令：  0xBE
 * 参数：  0x08
 * 参数：  0x00
 *
 * @return
 * 0 -> 初始化完成
 * 1 -> 初始化失败（未收到应答）
 */
bool AHT20_Init(void)
{
    // ret
    uint8_t status;


    // 等待 AHT20 上电稳定（建议等待 40ms）
    delay_ms_noOS(40);

    // 读取状态寄存器

	if (!AHT_Read_Status(&status))
	{
		return false;
	}

	if ((status & 0x08) == 0)
	{
		/*
         * 初始化：
         * BE 08 00
         */
		if (!AHT_SendCommand(AHT20_CMD_INITIALIZE, 0x08, 0x00))
		{
			return false;
		}
		// 等待 AHT20 校准完成（建议等待 10ms）
    	delay_ms_noOS(10);
	}

	return true;
}


/**
 * @brief  读取 AHT20 温湿度数据
 *
 * @param  humi  用于保存湿度数据的指针
 * @param  temp  用于保存温度数据的指针
 *
 * @note
 * AHT20 一次测量返回 6 个字节：
 *
 * Byte1：状态
 * Byte2：湿度数据[19:12]
 * Byte3：湿度数据[11:4]
 * Byte4：湿度数据[3:0] + 温度数据[19:16]
 * Byte5：温度数据[15:8]
 * Byte6：温度数据[7:0]
 *
 * 湿度原始数据：
 *
 *     SRH = Byte2 << 12
 *         | Byte3 << 4
 *         | Byte4 >> 4
 *
 * 温度原始数据：
 *
 *     ST = (Byte4 & 0x0F) << 16
 *        | Byte5 << 8
 *        | Byte6
 *
 * @return
 * 0 -> 读取成功
 * 1 -> 读取超时
 */
bool AHT20_Read_Data(AHT20_Data_t *data)
{
	if (data == NULL)
	{
		return false;
	}

	uint8_t status;
	uint8_t cnt;

	/* 保存 AHT21 返回的 6 个数据字节 */
	uint8_t buff[6];

	/*
	 * 用于保存拼接后的 20bit 原始数据。
	 */
	uint32_t RetuData = 0;

    /*
	 *==========================================================
	 * 第一阶段：发送测量命令
	 *==========================================================
	 *
	 * AHT20 测量命令：
	 *
	 * 0xAC -> 触发测量
	 * 0x33 -> 参数
	 * 0x00 -> 参数
	 *
	 * 通信过程：
	 *
	 * START
	 *   -> 0x70
	 *   -> 0xAC
	 *   -> 0x33
	 *   -> 0x00
	 * STOP
	 */
	if (!AHT_SendCommand(AHT20_CMD_TRIGGER_MEASURE, 0x33, 0x00))
	{
		return false;
	}

    /*
	 * AHT20 开始测量后需要一定时间完成转换。
	 *
	 * 这里先等待 80ms。
	 */
	delay_ms(80);

    /*
	 *==========================================================
	 * 第二阶段：检查测量是否完成
	 *==========================================================
	 *
	 * 状态寄存器：
	 *
	 * Bit7 = BUSY
	 *
	 * BUSY = 1：
	 *     测量正在进行
	 *
	 * BUSY = 0：
	 *     测量完成，可以读取数据
	 */
    for (cnt = 0; cnt < 5; cnt++)
	{
		if (!AHT_Read_Status(&status))
		{
			return false;
		}
		if ((status & 0x80) == 0x00)
		{
			break;
		}
		if (cnt == 4)
		{
			return false;
		}
		delay_ms(5);
	}

    /*
	 *==========================================================
	 * 第三阶段：读取测量结果
	 *==========================================================
	 *
	 * 使用 AHT20 读地址 0x71。
	 *
	 * 连续读取 6 个字节：
	 *
	 * Byte1 -> 状态
	 * Byte2 -> 湿度
	 * Byte3 -> 湿度
	 * Byte4 -> 湿度 + 温度
	 * Byte5 -> 温度
	 * Byte6 -> 温度
	 */
	if (IIC_Lock(AHT20_bus, osWaitForever) != SUCCESS)
	{
		return false;
	}

    IICStart(AHT20_bus);

    // 发送设备地址（读操作0x71）
    IICSendByte(AHT20_bus, (AHT20_ADDR << 1) | 0x01);
	AHT20_IIC_ACK_CHECK(AHT20_bus);

    // 读取 6 个字节数据
    for (uint8_t i = 0; i < 6; i++)
    {
        buff[i] = IICReceiveByte(AHT20_bus);
        if (i < 5)
        {
            IICSendAck(AHT20_bus);
        }
        else
        {
            IICSendNotAck(AHT20_bus);
        }
    }

    // 发送停止信号
    IICStop(AHT20_bus);

	// 解锁总线
	IIC_Unlock(AHT20_bus);

    /*
	 *==========================================================
	 * 第四阶段：计算湿度
	 *==========================================================
	 *
	 * 湿度 20bit 原始数据：
	 *
	 *     Byte2[7:0]
	 *     Byte3[7:0]
	 *     Byte4[7:4]
	 *
	 * 拼接：
	 *
	 *     Byte2 << 12
	 *     Byte3 << 4
	 *     Byte4 >> 4
	 *
	 * 最终得到：
	 *
	 *     RetuData = 湿度原始值
	 */
    RetuData = (RetuData | buff[1]) << 8;
    RetuData = (RetuData | buff[2]) << 8;
    RetuData = (RetuData | buff[3]);
    RetuData >>= 4;

    /*
	 *==========================================================
	 * 湿度转换
	 *==========================================================
	 *
	 * AHT20 湿度计算公式：
	 *
	 *     RH = SRH / 2^20 * 100%
	 *
	 * 这里使用整数运算：
	 *
	 *     SRH * 1000 >> 20
	 *
	 * 然后再除以 10。
	 *
	 * 最终得到浮点数湿度。
	 */
    data->humidity = (float)(RetuData * 1000.0f / (1 << 20)) / 10.0f;

    /*
	 *==========================================================
	 * 第五阶段：计算温度
	 *==========================================================
	 *
	 * 温度 20bit 原始数据：
	 *
	 *     Byte4[3:0]
	 *     Byte5
	 *     Byte6
	 *
	 * 拼接：
	 *
	 *     (Byte4 & 0x0F) << 16
	 *     Byte5 << 8
	 *     Byte6
	 */
    RetuData = 0;
    RetuData = (RetuData | (buff[3] & 0x0F)) << 8;
    RetuData = (RetuData | buff[4]) << 8;
    RetuData = (RetuData | buff[5]);

    /*
	 * 温度数据只有 20bit，
	 * 因此通过 0xFFFFF 保留低 20 位。
	 */
	RetuData = RetuData & 0xFFFFF;

    /*
	 *==========================================================
	 * 温度转换
	 *==========================================================
	 *
	 * AHT20 温度计算公式：
	 *
	 *     T = ST / 2^20 * 200 - 50
	 *
	 * 这里采用整数运算：
	 *
	 *     ST * 2000 >> 20
	 *
	 * 然后减去 500，
	 * 最后除以 10，
	 * 得到实际温度。
	 */
    data->temperature = (float)(RetuData * 2000.0f / (1 << 20) - 500.0f) / 10.0f;

    return true;

}