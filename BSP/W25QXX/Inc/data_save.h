#ifndef __DATA_SAVE_H__
#define __DATA_SAVE_H__

#include "w25q.h"

#define STORAGE_MAGIC1_ADDR     0x000000
#define STORAGE_MAGIC2_ADDR     0x000001


#define USER_WRIST_ADDR         0x000010
#define UI_APP_SY_EN_ADDR       0x000011


#define LAST_SAVE_DAY_ADDR      0x000020
#define DAY_STEPS_ADDR          0x000021

uint32_t Storage_Init(void);
uint8_t Storage_Check(void);
uint8_t Storage_Write(uint32_t addr, uint8_t *data, uint32_t len);

uint8_t SettingSave(uint8_t *buf, uint32_t addr, uint16_t length);
uint8_t SettingGet(uint8_t *buf, uint32_t addr, uint16_t length);

#endif /* __DATA_SAVE_H__ */
