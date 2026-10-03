#include "key.h"
#include "my_delay.h"

void Key_Port_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA0 */
	GPIO_InitStruct.Pin = KEY1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
  	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

uint8_t KeyScan(uint8_t mode)
{
	static uint8_t key_up = 1;
	uint8_t keyvalue=0;
	if(mode) key_up = 1;
	if( key_up && (!KEY1))
	{
		delay_ms(3);//ensure the key is down
		if(!KEY1) keyvalue = 1;
		if(keyvalue) key_up = 0;
	}
	else
	{
		delay_ms(3);//ensure the key is up
		if(KEY1)
			key_up = 1;
	}
	return keyvalue;
}

volatile uint8_t key_ignore_next = 0;   // 唤醒后忽略一次按键
/**
 * @brief  我的按键扫描
 * @return 0:无动作, 1:短按, 2:长按
 * @note   必须在 5~10ms 周期的任务中调用
 */
KeyValue_t Key_GetValue(void)
{
	static KeyState_t key_state = KEY_IDLE;
	static uint32_t tick = 0;
	KeyValue_t key_value = KEY_NONE;

	if (key_ignore_next)
	{
		if (KEY1)
		{
			key_ignore_next = 0;
			key_state = KEY_IDLE;
		}
		return KEY_NONE;
	}

	switch (key_state)
	{
		case KEY_IDLE:
			if (!KEY1)
			{
				key_state = KEY_PRESS_DEBOUNCE;
				tick = HAL_GetTick();
			}
			break;
		
		case KEY_PRESS_DEBOUNCE:
			if (HAL_GetTick() - tick >= 10)
			{
				if (!KEY1)
				{
					key_state = KEY_PRESSED;
					tick = HAL_GetTick();
				}
				else
				{
					key_state = KEY_IDLE;
				}
			}
			break;

		case KEY_PRESSED:
			if (KEY1)
			{
				key_state = KEY_RELEASE_DEBOUNCE;
				tick = HAL_GetTick();
			}
			else if (HAL_GetTick() - tick >= 1000)
			{
				key_state = KEY_LONG_TRIGGERED;
				key_value = KEY_LONG_PRESS;
			}
			break;

		case KEY_LONG_TRIGGERED:
			if (KEY1)
			{
				key_state = KEY_IDLE;
			}
			break;

		case KEY_RELEASE_DEBOUNCE:
			if (HAL_GetTick() - tick >= 10)
			{
				if (KEY1)
				{
					key_state = KEY_IDLE;
					key_value = KEY_SHORT_PRESS;
				}
				else
				{
					key_state = KEY_PRESSED;
				}
			}
			break;
		default:
			key_state = KEY_IDLE;
			break;
	}

	return key_value;
}

volatile uint8_t key_wakeup_flag = 0;
volatile uint8_t system_in_sleep = 0;   // 0=正常运行，1=睡眠中

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY1_PIN)
    {
		if (system_in_sleep)
		{
			key_wakeup_flag = 1;
		}
    }
}

