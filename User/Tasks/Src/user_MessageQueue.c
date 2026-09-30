#include "user_MessageQueue.h"

osMessageQueueId_t Key_MessageQueue;
osMessageQueueId_t Idle_MessageQueue;
osMessageQueueId_t Stop_MessageQueue;
osMessageQueueId_t IdleBreak_MessageQueue;
osMessageQueueId_t HomeUpdata_MessageQueue;
osMessageQueueId_t DataSave_MessageQueue;

/**
 * @brief 初始化消息队列
 * 
 */
void User_MessageQueueInit(void)
{
    Key_MessageQueue =
        osMessageQueueNew(1, 1, NULL);

    Idle_MessageQueue =
        osMessageQueueNew(1, 1, NULL);

    Stop_MessageQueue =
        osMessageQueueNew(1, 1, NULL);

    HomeUpdata_MessageQueue =
        osMessageQueueNew(1, 1, NULL);

    DataSave_MessageQueue =
        osMessageQueueNew(2, 1, NULL); 

    IdleBreak_MessageQueue =
        osMessageQueueNew(1, 1, NULL);
}
