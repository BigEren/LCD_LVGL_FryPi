#include "user_KeyTask.h"
#include "main.h"
#include "key.h"
#include "user_MessageQueue.h"

void KeyTask(void* argument)
{
    uint8_t keystr = 0;
    uint8_t Stopstr = 0;
    uint8_t IdleBreadstr = 0;

    for(;;)
    {
        switch (Key_GetValue())
        {
            case KEY_NONE:
                keystr = 0;
                break;
            case KEY_SHORT_PRESS:
                keystr = 1;
                osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
                osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreadstr, 0, 1);
                break;
            case KEY_LONG_PRESS:
                keystr = 2;
                osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
                break;
            default:
                break;
        }
        osDelay(1);
    }
}
