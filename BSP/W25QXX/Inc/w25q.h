#ifndef __W25Q_H__
#define __W25Q_H__

#include "main.h"
#include "spi.h"

#define W25Q_CS_Pin GPIO_PIN_0
#define W25Q_CS_GPIO_Port GPIOC

#define W25Q_CS_LOW() HAL_GPIO_WritePin(W25Q_CS_GPIO_Port, W25Q_CS_Pin, GPIO_PIN_RESET)
#define W25Q_CS_HIGH() HAL_GPIO_WritePin(W25Q_CS_GPIO_Port, W25Q_CS_Pin, GPIO_PIN_SET)

#define W25Q_READ_ID_CMD 0x9F
#define W25Q_SECTOR_ERASE_CMD 0x20
#define W25Q_WRITE_CMD 0x02
#define W25Q_READ_CMD 0x03

#define W25Q_PAGE_SIZE 256

void W25Q_Init(void);
uint8_t W25Q_SPI_RWByte(uint8_t byte);
uint32_t W25Q_Read_ID(void);
void W25Q_WaitForIdle(void);
void W25Q_WriteEnable(void);
void W25Q_SectorErase(uint32_t addr);
void W25Q_PageWrite(uint32_t addr, uint8_t *data, uint32_t len);
void W25Q_WriteData(uint32_t addr, uint8_t *data, uint32_t len);
void W25Q_ReadData(uint32_t addr, uint8_t *data, uint32_t len);



#endif /* __W25Q_H__ */