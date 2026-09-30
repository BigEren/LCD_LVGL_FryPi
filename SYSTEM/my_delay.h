#ifndef __MY_DELAY_H__
#define __MY_DELAY_H__
#include "sys.h"

void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(u16 nms);
void delay_us_noOS(uint32_t nus);
void delay_ms_noOS(u16 nms);



#endif /* __MY_DELAY_H__ */
