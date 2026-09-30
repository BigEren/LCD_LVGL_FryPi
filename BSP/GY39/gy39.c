#include "gy39.h"
#include "my_iic_hal.h"
#include "i2c.h"

static iic_bus_my_t *GY39_bus = &SensorBus;

/**
 * @brief 读取 GY39 连续寄存器
 */
static bool GY39_ReadRegs(uint8_t reg,
                          uint8_t *data,
                          uint8_t len)
{
    // return IIC_Read_Multi_Byte(
    //     GY39_bus,
    //     GY39_ADDR,
    //     reg,
    //     len,
    //     data) == SUCCESS;
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(
        &hi2c3,
        GY39_ADDR << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        len,
        1000);
    return ret == HAL_OK;
}

bool GY39_ReadID(uint8_t *id)
{
    if (id == NULL)
    {
        return false;
    }

    // return IIC_Read_One_Byte(
    //     GY39_bus,
    //     GY39_ADDR,
    //     GY39_DEVICE_ID_REG,
    //     id
    // ) == SUCCESS;
    // HAL_I2C_Mem_Read(
    //     &hi2c3,
    //     GY39_ADDR << 1,
    //     GY39_DEVICE_ID_REG,
    //     I2C_MEMADD_SIZE_8BIT,
    //     id,
    //     1,
    //     1000);
    return true;
}

/**
 * @brief 初始化 GY39
 *
 * GY39 本身不需要像 MPU6050 那样写初始化寄存器。
 * 这里只保留统一的设备初始化接口。
 */
bool GY39_Init(void)
{
    uint8_t id = 0;


    // if (!GY39_ReadID(&id))
    // {
    //     printf("GY39_ReadID failed %d \n", id);
    //     return false;
    // }
    // if (id != GY39_DEVICE_ID_REG)
    // {
    //     printf("ID not match %d & %d\n", id, GY39_DEVICE_ID_REG);
    //     return false;
    // }

    return true;
}

/**
 * @brief 读取 GY39 全部数据
 *
 * 0x00 ~ 0x0D 共14字节
 *
 * 0x00~0x03 : 光照
 * 0x04~0x05 : 温度
 * 0x06~0x09 : 气压
 * 0x0A~0x0B : 湿度
 * 0x0C~0x0D : 海拔
 */
bool GY39_ReadData(GY39_Data_t *data)
{
    uint8_t buf[14];
    if (data == NULL || !GY39_ReadRegs(0x00, buf, 14))
    {
        printf("[GY39] ReadRegs failed\n");
        return false;
    }

    /*================ 光照 =================*/
    uint32_t lux_raw = 
            ((uint32_t)buf[0] << 24) |
            ((uint32_t)buf[1] << 16) |
            ((uint32_t)buf[2] << 8)  |
            ((uint32_t)buf[3]);
    
    /*
     * 手册：
     * Lux = raw / 100
     */
    data->lux = (float)lux_raw / 100.0f;

    /*================ 温度 =================*/
    uint16_t temp_raw = 
            ((uint16_t)buf[4] << 8) |
            ((uint16_t)buf[5]);
    
    /*
     * 手册：
     * Temp = raw / 100
     */
    data->temp = (float)temp_raw / 100.0f;

    /*================ 气压 =================*/
    uint32_t press_raw = 
            ((uint32_t)buf[6] << 24) |
            ((uint32_t)buf[7] << 16) |
            ((uint32_t)buf[8] << 8)  |
            ((uint32_t)buf[9]);
    
    /*
     * 手册：
     * Press = raw / 100
     */
    data->press = (float)press_raw / 100.0f;

    /*================ 湿度 =================*/
    uint16_t hum_raw = 
            ((uint16_t)buf[10] << 8) |
            ((uint16_t)buf[11]);
    
    /*
     * 手册：
     * Hum = raw / 100
     */
    data->hum = (float)hum_raw / 100.0f;

    /*================ 海拔 =================*/
    data->alt = 
            ((int16_t)buf[12] << 8) |
            ((int16_t)buf[13]);


    return true;
}