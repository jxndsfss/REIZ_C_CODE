/*******************************************************************************
 *  @file       reiz_ringQueue.h
 *  @author     jxndsfss
 *  @version    v1.0.2
 *  @date       2019-01-06
 *  @site       ShangYouSong.SZ
 *  @brief      环形队列缓存头文件
 *******************************************************************************
 * 
 *  使用方法：
 *  
 *  //定义缓存数组大小
 *  #define BUFFER_SIZE 512
 *  
 *  //创建缓存数组、环形缓队列、环形队列指针，初始化环形队列指针
 *  uint8_t uartBuffer[BUFFER_SIZE] = {0};
 *  ringQueue_t uartRingQ;
 *  pRingQueue_t pUartRingQ = &uartRingQ;
 *  
 *  //构建环形队列，进行初始化
 *  ringQueue_Construct(pUartRingQ, uartBuffer, BUFFER_SIZE);
 *  
 *  //往环形队列中存储一字节数据
 *  ringQueue_PutByte(pUartRingQ, 0x12);
 *  ...
 * 
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
#define OVERFLOW_TIMES_COUNT_ENABLE         1                                   //溢出次数统计功能
#define OVERFLOW_LOST_BYTES_COUNT_ENABLE    1                                   //溢出字节数统计功能
#define MAX_COUNT_ONCE_STORED_ENABLE        1                                   //统计缓存之前存储过的最大字节数
#define GET_PUT_PEEK_RETURN_COUNT_ENABLE    1                                   //存取函数是否返回缓存当前存储字节数

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t     *pBuffer;                                                       //实际数据存储数组地址
    int32_t     length;                                                         //实际数据存储数组长度
    int32_t     count;                                                          //当前环形存储区存储字节数
    int32_t     head;                                                           //头索引，数据插入
    int32_t     tail;                                                           //尾索引，数据取出
#if MAX_COUNT_ONCE_STORED_ENABLE
    int32_t     maxCount;                                                       //环形存储区内存储过的最大字节数
#endif
#if OVERFLOW_TIMES_COUNT_ENABLE
    int32_t     overflowTimes;                                                  //溢出次数
#endif
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
    int32_t     LostBytesNum;                                                   //溢出后丢失的数据字节数量
#endif
} ringQueue_t, *pRingQueue_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

extern void ringQueue_Construct(pRingQueue_t pRingQ, uint8_t *bufferArray, int32_t arraySize);  //创建环形队列缓存并进行初始化
extern void ringQueue_Flush(pRingQueue_t pRingQ);                                               //清空环形队列缓存
extern bool ringQueue_IsFull(pRingQueue_t pRingQ);                                              //查看环形队列缓存是否已满
extern bool ringQueue_IsEmpty(pRingQueue_t pRingQ);                                             //查看环形队列缓存是否为空
extern uint8_t ringQueue_GetByte(pRingQueue_t pRingQ);                                          //从环状队列缓存读取1个字节
extern uint8_t ringQueue_Peek(pRingQueue_t pRingQ);                                             //查看环形队列缓存尾端1字节内容，但不取出该字节
extern uint32_t ringQueue_GetFree(pRingQueue_t pRingQ);                                         //获取环形队列缓存空余容量字节数
extern uint32_t ringQueue_GetCount(pRingQueue_t pRingQ);                                        //获取环形队列缓存当前存储字节数

#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
extern int32_t ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst);                          //从环状队列缓存读取1个字节
extern int32_t ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num);              //从环形队列缓存读取多个字节
extern int32_t ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data);                            //将1字节存入环形队列缓存
extern int32_t ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num);              //将多个字节存入环形队列缓存
extern int32_t ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst);                             //查看环形队列缓存尾端1字节内容，但不取出该字节
#else
extern void ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst);                             //从环状队列缓存读取1个字节
extern void ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num);                 //从环形队列缓存读取多个字节
extern void ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data);                               //将1字节存入环形队列缓存
extern void ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num);                 //将多个字节存入环形队列缓存
extern void ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst);                                //查看环形队列缓存尾端1字节内容，但不取出该字节
#endif

#if OVERFLOW_TIMES_COUNT_ENABLE 
extern bool ringQueue_IsOverflow(pRingQueue_t pRingQ);                                          //查看环形队列缓存是否发生溢出
extern int32_t ringQueue_GetOverflowTimes(pRingQueue_t pRingQ);                                 //读取环形队列缓存溢出次数
#endif      

#if OVERFLOW_LOST_BYTES_COUNT_ENABLE    
extern int32_t ringQueue_GetLostBytesNum(pRingQueue_t pRingQ);                                  //读取溢出导致的丢失数据总字节数
#endif      

#if MAX_COUNT_ENABLE    
extern int32_t ringQueue_GetMaxCount(pRingQueue_t pRingQ);                                      //读取环形队列缓存之前存储内容最大字节数量
#endif

#ifdef __cplusplus
}
#endif

#endif /* REIZ_RING_QUEUE_H */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
