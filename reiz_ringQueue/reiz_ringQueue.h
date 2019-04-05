/*******************************************************************************
 *  @file       reiz_ringQueue.h
 *  @author     jxndsfss
 *  @version    v1.0.5
 *  @date       2019-04-04
 *  @site       ShangYouSong.SZ
 *  @brief      环形队列缓存头文件
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef REIZ_RING_QUEUE_H
#define REIZ_RING_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Exported define -----------------------------------------------------------*/

/* 宏值：1为打开，0为关闭 */
#define OVERFLOW_TIMES_COUNT_ENABLE         1                                   //溢出覆盖次数统计功能
#define MAX_COUNT_ONCE_STORED_ENABLE        0                                   //统计缓存之前存储过的最大字节数
#define GET_PUT_PEEK_RETURN_COUNT_ENABLE    0                                   //存取函数是否返回缓存当前存储字节数

/* Exported types ------------------------------------------------------------*/
typedef struct ringQueueControlBlock_ {
    uint8_t     *pBuffer;                                                       //实际数据存储数组地址
    int32_t     size;                                                           //实际数据存储数组长度
    int32_t     count;                                                          //当前环形存储区存储字节数
    int32_t     head;                                                           //头索引，数据插入
    int32_t     tail;                                                           //尾索引，数据取出
#if MAX_COUNT_ONCE_STORED_ENABLE
    int32_t     maxCount;                                                       //环形存储区内存储过的最大字节数
#endif
#if OVERFLOW_TIMES_COUNT_ENABLE
    int32_t     overflowTimes;                                                  //溢出次数
#endif
} ringQueue_t, *pRingQueue_t;

/* Exported macro ------------------------------------------------------------*/

/*
    环形队列缓存对象宏类型定义
*/
#define RING_QUEUE_OBJ(bufferSize)      struct {                                \
                                            ringQueue_t ringQ;                  \
                                            uint8_t buffer[bufferSize];         \
                                        }

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

extern bool ringQueue_Init(pRingQueue_t pRingQ, uint8_t *pBufferArray, int32_t arraySize);          //创建环形队列缓存并进行初始化
extern void ringQueue_Flush(pRingQueue_t pRingQ);                                                   //清空环形队列缓存
extern bool ringQueue_IsFull(pRingQueue_t pRingQ);                                                  //查看环形队列缓存是否已满
extern bool ringQueue_IsEmpty(pRingQueue_t pRingQ);                                                 //查看环形队列缓存是否为空
extern uint8_t  ringQueue_GetByte(pRingQueue_t pRingQ);                                              //从环状队列缓存读取1个字节
extern uint8_t  ringQueue_Peek(pRingQueue_t pRingQ);                                                 //查看环形队列缓存尾端1字节内容，但不取出该字节
extern uint32_t ringQueue_GetFree(pRingQueue_t pRingQ);                                             //获取环形队列缓存空余容量字节数
extern uint32_t ringQueue_GetCount(pRingQueue_t pRingQ);                                            //获取环形队列缓存当前存储字节数
extern uint32_t ringQueue_GetTailLocation(pRingQueue_t pRingQ);                                     //获取环形队列尾位置，即最后存储字节在缓存中的存储位置
extern void ringQueue_GetAllDataViaLocationOverflow(    pRingQueue_t pRingQ,                        //溢出时，读取所有已接收数据
                                                        uint8_t *pDst,
                                                        uint32_t location);

#if GET_PUT_PEEK_RETURN_COUNT_ENABLE    
extern int32_t ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst);                              //从环状队列缓存读取1个字节
extern int32_t ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num);                  //从环形队列缓存读取多个字节
extern int32_t ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data);                                //将1字节存入环形队列缓存
extern int32_t ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num);                  //将多个字节存入环形队列缓存
extern int32_t ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst);                                 //查看环形队列缓存尾端1字节内容，但不取出该字节
extern int32_t ringQueue_GetDataViaLocation(pRingQueue_t pRingQ, uint8_t *pDst, uint32_t loc);      //通过尾位置读取缓存内容，即读出缓存内该位置前所有内容
#else
extern void ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst);                                 //从环状队列缓存读取1个字节
extern void ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num);                     //从环形队列缓存读取多个字节
extern void ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data);                                   //将1字节存入环形队列缓存
extern void ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num);                     //将多个字节存入环形队列缓存
extern void ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst);                                    //查看环形队列缓存尾端1字节内容，但不取出该字节
extern void ringQueue_GetDataViaLocation(pRingQueue_t pRingQ, uint8_t *pDst, uint32_t loc);         //通过尾位置读取缓存内容，即读出缓存内该位置前所有内容
#endif

#if OVERFLOW_TIMES_COUNT_ENABLE
extern bool ringQueue_IsOverflow(pRingQueue_t pRingQ);                                              //查看环形队列缓存是否发生溢出
extern int32_t ringQueue_GetOverflowTimes(pRingQueue_t pRingQ);                                     //读取环形队列缓存溢出次数
#endif  

#if MAX_COUNT_ONCE_STORED_ENABLE
extern int32_t ringQueue_GetMaxCount(pRingQueue_t pRingQ);                                          //读取环形队列缓存之前存储内容最大字节数量
#endif

#ifdef __cplusplus
}
#endif

#endif /* REIZ_RING_QUEUE_H */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/