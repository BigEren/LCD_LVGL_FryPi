#include "user_Queue.h"

// 初始化队列
void My_Queue_Init(My_Queue_t *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// 判断队列是否为空
bool My_Queue_IsEmpty(My_Queue_t *queue)
{
    return queue->size == 0;
}

// 判断队列是否已满
bool My_Queue_IsFull(My_Queue_t *queue)
{
    return queue->size == USER_QUEUE_SIZE;
}

// 入队
void My_EnQueue(My_Queue_t *queue, unsigned long item)
{
    if (My_Queue_IsFull(queue))
    {
        printf("队列已满\n");
        return;
    }
    queue->rear = (queue->rear + 1) % USER_QUEUE_SIZE;
    queue->data[queue->rear] = item;
    queue->size++;
}

// 出队
uint32_t My_DeQueue(My_Queue_t *queue)
{
    if (My_Queue_IsEmpty(queue))
    {
        printf("队列为空\n");
        return 0;
    }
    uint32_t item = queue->data[queue->front];
    queue->front = (queue->front + 1) % USER_QUEUE_SIZE;
    queue->size--;
    return item;
}
