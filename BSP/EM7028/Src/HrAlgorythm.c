#include "HrAlgorythm.h"

/* Queue1: 用于缓存最近 7 次采样数据*/
My_Queue_t Qdata;

/* Queue2: 用于缓存最近 7 次采样时间*/
My_Queue_t Qtime;

/* Queue3: 用于缓存最近 7 次心率结果*/
My_Queue_t QHR_List;


/**
 * @brief 心率算法初始化
 *
 * 初始化三个队列：
 * 1. datas    保存传感器采样值
 * 2. times    保存采样时间
 * 3. HR_List  保存计算得到的心率
 */
void HR_Algorythm_Init(void)
{
    My_Queue_Init(&Qdata);
    My_Queue_Init(&Qtime);
    My_Queue_Init(&QHR_List);
}


/**
 * @brief 对心率数据进行平均滤波
 *
 * @param HrList  心率数据数组
 * @param length  参与平均计算的数据个数
 *
 * @return 平均心率
 */
uint16_t HR_Ave_Filter(uint32_t *HRList, uint8_t length)
{
    uint32_t ave = 0;
    uint8_t i = 0;

    /* 累加所有心率数据 */
    for (i = 0; i < length; i++)
    {
        ave += HRList[i];
    }

    /* 计算平均值 */
    ave /= length;

    return (uint16_t)ave;
}


/**
 * @brief 根据传感器采样数据计算心率
 *
 * @param present_dat   当前采样值
 * @param present_time  当前采样时间，单位通常为 ms
 *
 * @return 当前计算出的心率，单位：次/分钟 BPM
 */
uint8_t HR_Calculate(uint16_t present_dat, uint32_t present_time)
{
    /*
     * peaks_time[0]: 最近一次检测到峰值的时间
     * peaks_time[1]: 上一次检测到谷值的时间
     */
    static uint32_t peaks_time[2] = {0, 0};

    /* 保存当前心率结果*/
    static uint16_t HR = 0;

    /*
     * 如果采样数据队列满了，删除最旧数据
     */
    if (My_Queue_Is_Full(&Qdata))
    {
        My_Queue_Dequeue(&Qdata);
    }

    /*
     * 如果采样时间队列满了，删除最旧数据
     */
    if (My_Queue_Is_Full(&Qtime))
    {
        My_Queue_Dequeue(&Qtime);
    }

    /*
     * 如果心率队列满了，删除最旧数据
     */
    if (My_Queue_Is_Full(&QHR_List))
    {
        My_Queue_Dequeue(&QHR_List);
    }

    /* 保存当前采样数据*/
    My_EnQueue(&Qdata, present_dat);

    /* 保存当前采样时间*/
    My_EnQueue(&Qtime, present_time);

    /*
     * 判断 datas.data[3] 是否为局部最大值。
     *
     * 当前队列中大约保存 7 个采样点：
     *
     * data[0] data[1] data[2] data[3]
     * data[4] data[5] data[6]
     *
     * 如果 data[3] 比周围数据都大，
     * 就认为它可能是一次心跳峰值。
     */
    if ((Qdata.data[3] >= Qdata.data[2]) && 
        (Qdata.data[3] >= Qdata.data[1]) && 
        (Qdata.data[3] >= Qdata.data[4]) && 
        (Qdata.data[3] >= Qdata.data[5]) && 
        (Qdata.data[3] >  Qdata.data[6]) && 
        (Qdata.data[3] >  Qdata.data[0]))
    {
        /*
         * 两次心跳峰值之间至少间隔 425 ms，
         * 用于避免同一次心跳被重复检测。
         *
         * 425 ms 对应最大心率约为：
         * 60000 / 425 ≈ 141 BPM
         */
        if (Qtime.data[3] - peaks_time[0] >= 425)
        {
            /* 保存上一次检测到峰值的时间*/
            peaks_time[0] = peaks_time[1];

            /* 保存当前采样时间*/
            peaks_time[1] = Qtime.data[3];

            /*
             * 根据两次心跳间隔计算心率：
             *
             * 心率 = 60000 / 两次心跳间隔毫秒数
             */
            if (peaks_time[1] != 0)
            {
                My_EnQueue(&QHR_List, 
                60000 / (peaks_time[0] - peaks_time[1]));
            }
            
            /*
             * 当已经收集到 7 个有效心率值后，
             * 对这 7 个结果求平均，降低抖动。
             */
            if (My_Queue_Is_Full(&QHR_List))
            {
                HR = HR_Ave_Filter(QHR_List.data, HR_FILTER_SIZE);
            }
        }
    }

    return HR;
}
