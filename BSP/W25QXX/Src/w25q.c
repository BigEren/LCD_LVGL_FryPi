#include "w25q.h"

#define W25Q_SPI hspi2

// 初始化W25QXX, cube中已初始化, 无需重复初始化
void W25Q_Init(void)
{
    printf("W25Q_Init\n");
}

/*
 * @brief SPI读写一个字节
 * @param byte 要读写的字节
 * @return 读取到的字节
 */
uint8_t W25Q_SPI_RWByte(uint8_t byte)
{
    uint8_t res;
    HAL_SPI_TransmitReceive(&W25Q_SPI, &byte, &res, 1, 100);
    return res;
}

/*
 * @brief 读取JEDEC ID
 * @return 读取到的JEDEC ID
 */
uint32_t W25Q_Read_ID(void)
{
    uint8_t id1, id2, id3;
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(W25Q_READ_ID_CMD);  // 发送读取JEDEC ID命令
    id1 = W25Q_SPI_RWByte(0xFF);    // 读取ID第一个字节
    id2 = W25Q_SPI_RWByte(0xFF);    // 读取ID第二个字节
    id3 = W25Q_SPI_RWByte(0xFF);    // 读取ID第三个字节
    return (id1 << 16) | (id2 << 8) | id3;
}

/*
 * @brief 等待空闲
 */
void W25Q_WaitForIdle(void)
{
    uint8_t status = 0;
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(0x05);  // 发送读取状态寄存器命令
    do {
        status = W25Q_SPI_RWByte(0xFF); // 读取状态寄存器
    } while (status & 0x01);    // 检查忙标志位（第0位），如果为1则继续等待
    W25Q_CS_HIGH();
}

/*
 * @brief 使能写入
 */
void W25Q_WriteEnable(void)
{
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(0x06);  // 发送使能写入命令
    W25Q_CS_HIGH();
}

/*
 * @brief 扇区擦除
 * @param addr 扇区地址始地址(4K对齐)
 * @return 无
 */
void W25Q_SectorErase(uint32_t addr)
{
    W25Q_WriteEnable();
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(W25Q_SECTOR_ERASE_CMD);  // 发送扇区擦除命令
    W25Q_SPI_RWByte((addr >> 16) & 0xFF);  // 发送扇区地址高字节
    W25Q_SPI_RWByte((addr >> 8) & 0xFF);  // 发送扇区地址中字节
    W25Q_SPI_RWByte(addr & 0xFF);  // 发送扇区地址低字节
    W25Q_CS_HIGH();
    W25Q_WaitForIdle();  // 等待擦除完成
}

/*
 * @brief 写入数据
 * @param addr 写入地址
 * @param data 要写入的数据
 * @param len 数据长度
 * @return 无
 */
void W25Q_PageWrite(uint32_t addr, uint8_t *data, uint32_t len)
{
    W25Q_WriteEnable();
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(W25Q_WRITE_CMD);  // 发送写入命令
    W25Q_SPI_RWByte((addr >> 16) & 0xFF);  // 发送写入地址高字节
    W25Q_SPI_RWByte((addr >> 8) & 0xFF);  // 发送写入地址中字节
    W25Q_SPI_RWByte(addr & 0xFF);  // 发送写入地址低字节
    for (uint16_t i = 0; i < len; i++)
    {
        W25Q_SPI_RWByte(data[i]);    // 发送数据字节
    }
    W25Q_CS_HIGH();
    W25Q_WaitForIdle();  // 等待写入完成
}

/*
 * @brief 跨页写入数据
 * @param addr 写入地址
 * @param data 要写入的数据
 * @param len 数据长度
 * @return 无
 */
void W25Q_WriteData(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint16_t page_remain;

    while (len)
    {
        // 计算当前页剩余字节数
        page_remain = W25Q_PAGE_SIZE - (addr % W25Q_PAGE_SIZE);

        // 如果remain不足一页
        if (len < page_remain)
        {
            page_remain = len;
        }
        // 写入数据
        W25Q_PageWrite(addr, data, page_remain);
        // 更新地址和数据指针
        addr += page_remain;
        data += page_remain;
        // 更新数据长度
        len -= page_remain;
    }
}

/*
 * @brief 读取数据
 * @param addr 读取地址
 * @param data 读取到的数据
 * @param len 数据长度
 * @return 无
 */
void W25Q_ReadData(uint32_t addr, uint8_t *data, uint32_t len)
{
    W25Q_CS_LOW();
    W25Q_SPI_RWByte(W25Q_READ_CMD);  // 发送读取命令
    W25Q_SPI_RWByte((addr >> 16) & 0xFF);  // 发送写入地址高字节
    W25Q_SPI_RWByte((addr >> 8) & 0xFF);  // 发送写入地址中字节
    W25Q_SPI_RWByte(addr & 0xFF);  // 发送写入地址低字节
    for (uint32_t i = 0; i < len; i++)
    {
        data[i] = W25Q_SPI_RWByte(0xFF);    // 读取数据字节
    }
    W25Q_CS_HIGH();
    //W25Q_WaitForIdle();  // 等待读取完成
}
