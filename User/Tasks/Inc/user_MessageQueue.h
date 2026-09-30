#ifndef __USER_MESSAGE_QUEUE_H__
#define __USER_MESSAGE_QUEUE_H__

#include <stdint.h>
#include "cmsis_os2.h"

/* 睡眠 */
extern osMessageQueueId_t Stop_MessageQueue;

/* 首页数据更新 */
extern osMessageQueueId_t HomeUpdata_MessageQueue;

/* 传感器数据保存 */
extern osMessageQueueId_t DataSave_MessageQueue;

/* 防止进入休眠 */
extern osMessageQueueId_t IdleBreak_MessageQueue;

/* key事件 */
extern osMessageQueueId_t Key_MessageQueue;

/* 空闲事件 */
extern osMessageQueueId_t Idle_MessageQueue;

void User_MessageQueueInit(void);

#endif /*__USER_MESSAGE_QUEUE_H__*/