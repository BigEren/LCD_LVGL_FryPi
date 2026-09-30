#ifndef __USER_QUEUE_H__
#define __USER_QUEUE_H__

#define USER_QUEUE_SIZE 7

#include "main.h"
#include "stdbool.h"

typedef struct
{
    int8_t front;
    int8_t rear;
    int8_t size;
    uint32_t data[USER_QUEUE_SIZE];
} My_Queue_t;

// 初始化队列
void My_Queue_Init(My_Queue_t *queue);

// 判断队列是否为空
bool My_Queue_IsEmpty(My_Queue_t *queue);

// 判断队列是否已满
bool My_Queue_IsFull(My_Queue_t *queue);

// 入队
void My_EnQueue(My_Queue_t *queue, unsigned long item);

// 出队
uint32_t My_DeQueue(My_Queue_t *queue);



#endif /* __USER_QUEUE_H__ */