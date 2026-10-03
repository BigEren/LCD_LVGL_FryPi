#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

//KEY1
#define KEY1_PORT	GPIOA
#define KEY1_PIN	GPIO_PIN_0
#define KEY1 HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN)

// Key State
/**
 * @brief  按键状态类型
 * @note   空闲, 按下去抖, 按下去, 长按触发, 抬起去抖
 */
typedef enum
{
	KEY_IDLE = 0,			// 空闲
	KEY_PRESS_DEBOUNCE,		// 按下去抖
	KEY_PRESSED,			// 按下去
	KEY_LONG_TRIGGERED,		// 长按触发
	KEY_RELEASE_DEBOUNCE,	// 抬起去抖
} KeyState_t;

// Key Value Tyepe
/**
 * @brief  按键值类型
 * @note   无按键, 短按, 长按
 */
typedef enum
{
	KEY_NONE = 0,			// 无按键
	KEY_SHORT_PRESS,		// 短按
	KEY_LONG_PRESS,			// 长按
} KeyValue_t;
	
void Key_Port_Init(void);
void Key_Interrupt_Callback(void);
uint8_t KeyScan(uint8_t mode);
KeyValue_t Key_GetValue(void);

extern volatile uint8_t key_ignore_next;   // 唤醒后忽略一次按键
extern volatile uint8_t key_wakeup_flag;     // 唤醒标志
extern volatile uint8_t system_in_sleep;   // 0=正常运行，1=睡眠中


#ifdef __cplusplus
}
#endif
#endif

