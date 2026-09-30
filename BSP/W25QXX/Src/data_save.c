#include "data_save.h"
#include "string.h"
#include "my_delay.h"

/******************************************
W25Q 数据描述:

0x000000 : 0x55 check
0x000001 : 0xAA check


0x000010 : user wrist setting
0x000011 : user ui_APPSy_EN setting


0x000020 : Last Save Day
0x000021 : Day Steps

*******************************************/

#define W25Q_SECTOR_SIZE 4096

/*
 * @brief 初始化数据保存
 */
uint32_t Storage_Init(void)
{
    W25Q_Init();
    return W25Q_Read_ID();
}

/*
 * @brief 检查数据保存是否初始化
 */
uint8_t Storage_Check(void)
{
    uint8_t check_buff[2];
    // 读取标志
    W25Q_ReadData(STORAGE_MAGIC1_ADDR, check_buff, 2);

    // 检查标志是否正确
    if(check_buff[0] == 0x55 && check_buff[1] == 0xAA)
    {
        return 0;   // check ok
    }
    else
    {
        check_buff[0] = 0x55;
        check_buff[1] = 0xAA;
        Storage_Write(STORAGE_MAGIC1_ADDR, check_buff, 2);

        memset(check_buff, 0, 2);
        W25Q_ReadData(STORAGE_MAGIC1_ADDR, check_buff, 2);

        if(check_buff[0] == 0x55 && check_buff[1] == 0xAA)
        {
            printf("Storage_Check: check ok %02X %02X\n", check_buff[0], check_buff[1]);
            return 0;
        }
    }
    return 1;   // check error

}

uint8_t Storage_Write(uint32_t addr, uint8_t *data, uint32_t len)
{
    static uint8_t sector_buf[W25Q_SECTOR_SIZE];
    uint8_t verify_buf[32];
    uint32_t sector_addr;
    uint32_t offset;

    // 计算Sector起始地址
    sector_addr = addr & 0xFFFFF000;
    // 计算偏移量
    offset = addr & 0x00000FFF;

    // 检查数据是否超出Sector范围
    if(offset + len > W25Q_SECTOR_SIZE)
    {
        return 1;
    }

    // 读取Sector数据
    W25Q_ReadData(sector_addr, sector_buf, W25Q_SECTOR_SIZE);

    // 复制数据到Sector缓冲区
    memcpy(&sector_buf[offset], data, len);

    // 擦除Sector
    W25Q_SectorErase(sector_addr);

    // 写入Sector数据
    W25Q_WriteData(sector_addr, sector_buf, W25Q_SECTOR_SIZE);

    // 验证写入数据
    W25Q_ReadData(addr, verify_buf, len);
    if(memcmp(data, verify_buf, len) != 0)
    {
        return 2;   // verify error
    }

    return 0;   // write ok
}

/**
 * @brief 读取数据
 * @param addr 读取地址
 * @param data 读取到的数据
 * @param len 读取数据长度
 * @return 无
 */
uint8_t Storage_Read(uint32_t addr, uint8_t *data, uint16_t len)
{

    W25Q_ReadData(addr, data, len);

    return 0;
}


/*
 * @brief 保存设置
 * @param buf 要保存的设置数据
 * @param addr 保存地址
 * @param length 设置数据长度
 * @return 无
 */
uint8_t SettingSave(uint8_t *buf, uint32_t addr, uint16_t length)
{
    // 检查数据保存是否初始化
    if (addr > 1 && Storage_Check() != 0)
    {
        return 1;   // check error
    }

    // 写入数据
   return Storage_Write(addr, buf, length); // save ok
}

/*
 * @brief 获取读取设置
 * @param buf 读取到的设置数据
 * @param addr 读取地址
 * @param length 设置数据长度
 * @return 无
 */
uint8_t SettingGet(uint8_t *buf, uint32_t addr, uint16_t length)
{
    // 检查数据保存是否初始化
    if (addr > 1 && Storage_Check() != 0)
    {
        return 1;   // check error
    }
    // 读取数据
    W25Q_ReadData(addr, buf, length);
    return 0;   // get ok
}