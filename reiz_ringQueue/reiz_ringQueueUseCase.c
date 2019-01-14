/*******************************************************************************
 *  @file       reiz_ringQueueUseCase.c
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-14
 *  @site       ShangYouSong.SZ
 *  @brief      环形队列缓存用例源文件
 *******************************************************************************
 */ 

/* Includes ------------------------------------------------------------------*/
#include "reiz_ringQueueUseCase.h"
#include "reiz_ringQueue.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
#define RING_QUEUE_BUFFER_SIZE  64

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static RING_QUEUE_OBJ(RING_QUEUE_BUFFER_SIZE) xxxRingQueueObj;                  //定义环形队列对象变量

/* Exported variables --------------------------------------------------------*/
extern pRingQueue_t pRingQ = &xxxRingQueueObj.ringQ;                            //定义环形队列变量指针并赋值

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
 *  @brief  xxx环形队列缓存初始化，xxx可为项目中不同的模块，比如BLE模块、GPRS模块等
 *  @param  void
 *  @return void
 */
extern void xxxRingQueueInit(void) {
    ringQueue_Init(pRingQ, xxxRingQueueObj.buffer, RING_QUEUE_BUFFER_SIZE);     //xxx环形队列初始化
}

/*******************************************************************************
 *  @brief  环形队列缓存测试函数
 *  @param  void
 *  @return void
 */
extern void ringQueueTest(void) {
    xxxRingQueueInit();

    printf("\n\n\n------------------------------------------------\n");
    printf("reiz_ringQueueUseCase\n");
    printf("------------------------------------------------\n");

    if (ringQueue_IsEmpty(pRingQ)) {
        printf("ringQueue buffer is empty!\n");
    }
    else {
        printf("ringQueue buffer is not empty!\n");
    }

    //往环形队列中存储数据
    for (uint8_t i = 0; i < 70; i++) {
        ringQueue_PutByte(pRingQ, i);
        printf("put a byte:%d\n", i);
    }

    int num;
    uint8_t rdByte;

    if (ringQueue_IsFull(pRingQ)) {
        printf("ringQueue is full!\n");
    }
    else {
        printf("ringQueue is not full!\n");
    }

    if (ringQueue_IsEmpty(pRingQ)) {
        printf("ringQueue is empty!\n");
    }
    else {
        printf("ringQueue is not empty!\n");
    }
    
    num = ringQueue_GetFree(pRingQ);
    printf("the free size is:%d\n", num);

    num = ringQueue_GetCount(pRingQ);
    printf("the stored size is:%d\n", num);

    rdByte = ringQueue_Peek(pRingQ);
    printf("the last byte is:%d\n", rdByte);

#if OVERFLOW_TIMES_COUNT_ENABLE
    if (ringQueue_IsOverflow(pRingQ)) {
        printf("ringQueue in the history is overflow!\n");
        int times = ringQueue_GetOverflowTimes(pRingQ);
        printf("in the history overflow times is:%d\n", times);
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
        int nums = ringQueue_GetLostBytesNum(pRingQ);
        printf("in the history overflow lost bytes is:%d\n", nums);
#endif
    } else {
        printf("ringQueue in the history is not overflow!\n");
    }
#endif

    do {
        num = ringQueue_GetByteP(pRingQ, &rdByte);
        printf("                  get a byte:%d\n", rdByte);
    } while (num);

    uint8_t data[] = { 11, 22, 33, 44, 55, 66, 77, 88, 99};
    ringQueue_PutMult(pRingQ, data, sizeof(data));

    if (ringQueue_IsFull(pRingQ)) {
        printf("ringQueue is full!\n");
    }
    else {
        printf("ringQueue is not full!\n");
    }

    if (ringQueue_IsEmpty(pRingQ)) {
        printf("ringQueue is empty!\n");
    }
    else {
        printf("ringQueue is not empty!\n");
    }

    num = ringQueue_GetFree(pRingQ);
    printf("the free size is:%d\n", num);

    num = ringQueue_GetCount(pRingQ);
    printf("the stored size is:%d\n", num);

    rdByte = ringQueue_Peek(pRingQ);
    printf("the last byte is:%d\n", rdByte);

#if OVERFLOW_TIMES_COUNT_ENABLE
    if (ringQueue_IsOverflow(pRingQ)) {
        printf("ringQueue in the history is overflow!\n");
        int times = ringQueue_GetOverflowTimes(pRingQ);
        printf("in the history overflow times is:%d\n", times);
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
        int nums = ringQueue_GetLostBytesNum(pRingQ);
        printf("in the history overflow lost bytes is:%d\n", nums);
#endif
    }
    else {
        printf("ringQueue in the history is not overflow!\n");
    }
#endif

    do {
        num = ringQueue_GetByteP(pRingQ, &rdByte);
        printf("                  get a byte:%d\n", rdByte);
    } while (num);

    uint8_t temp;
    while (!ringQueue_IsEmpty(pRingQ)) {
        ringQueue_GetByteP(pRingQ, &temp);
        printf("get a byte is:%d\n", temp);
    }

    if (ringQueue_IsFull(pRingQ)) {
        printf("ringQueue is full!\n");
    }
    else {
        printf("ringQueue is not full!\n");
    }

    if (ringQueue_IsEmpty(pRingQ)) {
        printf("ringQueue is empty!\n");
    }
    else {
        printf("ringQueue is not empty!\n");
    }

    num = ringQueue_GetFree(pRingQ);
    printf("the free size is:%d\n", num);

    num = ringQueue_GetCount(pRingQ);
    printf("the stored size is:%d\n", num);

    rdByte = ringQueue_Peek(pRingQ);
    printf("the last byte is:%d\n", rdByte);

#if OVERFLOW_TIMES_COUNT_ENABLE
    if (ringQueue_IsOverflow(pRingQ)) {
        printf("ringQueue is  in the history overflow!\n");
        int times = ringQueue_GetOverflowTimes(pRingQ);
        printf("in the history overflow times is:%d\n", times);
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
        int nums = ringQueue_GetLostBytesNum(pRingQ);
        printf("in the history overflow lost bytes is:%d\n", nums);
#endif
    }
    else {
        printf("ringQueue is not in the history overflow!\n");
    }
#endif

    printf("------------------------------------------------\n\n\n");
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
