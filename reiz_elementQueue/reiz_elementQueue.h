/********************************************************************************************************
 *  @file       reiz_elementQueue.h
 *  @author     chenLei
 *  @version    v1.0.1
 *  @date       2019-04-04
 *  @site       xinMeiYuan.A41
 *  @brief      元素存储队列头文件
 ********************************************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------------------------------- */
#ifndef REIZ_ELEMENT_QUEUE_H
#define REIZ_ELEMENT_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Extern define ------------------------------------------------------------------------------------- */
#ifdef REIZ_ELEMENT_QUEUE_GLOBALS
#define REIZ_ELEMENT_QUEUE_EXT
#else
#define REIZ_ELEMENT_QUEUE_EXT extern
#endif

/* Includes ------------------------------------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Exported define ----------------------------------------------------------------------------------- */

/* 宏值：1为打开，0为关闭 */
#define ELE_OVERFLOW_TIMES_COUNT_ENABLE         0                               //溢出覆盖次数统计功能
#define ELE_MAX_COUNT_ONCE_STORED_ENABLE        0                               //统计队列之前存储过的最大字节数
#define ELE_GET_PUT_RETURN_COUNT_ENABLE         0                               //存取函数是否返回队列当前存储元素个数

#define ELEMENT_TYPE                            uint32_t                        //元素数据类型定义

/* Exported macro ------------------------------------------------------------------------------------ */

/* Exported types ------------------------------------------------------------------------------------ */
typedef struct eleQueue_ {
    ELEMENT_TYPE    *pBuffer;                                                   //实际数据存储数组地址
    uint32_t        size;                                                       //队列所能存储元素数目
    uint32_t        count;                                                      //队列当前存储元素数目
    uint32_t        head;                                                       //头索引，元素插入
    uint32_t        tail;                                                       //尾索引，元素取出
#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
    uint32_t        maxCount;                                                   //环形存储区内存储过的最大元素个数
#endif
#if ELE_OVERFLOW_TIMES_COUNT_ENABLE
    uint32_t        overflowTimes;                                              //溢出次数
#endif
} eleQueue_t, *pEleQueue_t;

/* 元素存储环形队列缓存对象宏类型定义 */
#define ELEMENT_QUEUE_OBJ(bufferSize)   struct {                                    \
                                            eleQueue_t      ringQ;                  \
                                            ELEMENT_TYPE    buffer[bufferSize];     \
                                        }

/* Exported variables -------------------------------------------------------------------------------- */

/* Exported functions prototypes --------------------------------------------------------------------- */

extern bool         eleQueue_Init               ( pEleQueue_t   pRingQ,                         //创建环形队列缓存并进行初始化
                                                  ELEMENT_TYPE *pBufferArray,
                                                  uint32_t      arraySize   );
extern void         eleQueue_Flush              ( pEleQueue_t pRingQ );                         //清空环形队列
extern bool         eleQueue_IsFull             ( pEleQueue_t pRingQ );                         //查看环形队列是否已满
extern bool         eleQueue_IsEmpty            ( pEleQueue_t pRingQ );                         //查看环形队列是否为空
extern bool         eleQueue_GetElement         ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //从环形队列取出1个元素
extern bool         eleQueue_Peek               ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //查看环形队列尾端1个元素内容，但不取出该元素
extern uint32_t     eleQueue_GetFree            ( pEleQueue_t pRingQ );                         //获取环形队列空余元素容量数
extern uint32_t     eleQueue_GetCount           ( pEleQueue_t pRingQ );                         //获取环形队列当前存储元素个数

#if ELE_GET_PUT_RETURN_COUNT_ENABLE
extern int32_t      eleQueue_GetElementP        ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //从环状队列读取1个元素
extern uint32_t     eleQueue_PutElement         ( pEleQueue_t pRingQ, ELEMENT_TYPE  data );     //将元素存入环形队列
extern int32_t      eleQueue_PeekP              ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //查看环形队列存尾端元素内容，但不取出该元素
#else
extern void         eleQueue_GetElementP        ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //从环状队列读取1个元素
extern void         eleQueue_PutElement         ( pEleQueue_t pRingQ, ELEMENT_TYPE  data );     //将元素存入环形队列
extern void         eleQueue_PeekP              ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst );     //查看环形队列存尾端元素内容，但不取出该元素
#endif      

#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
extern bool         eleQueue_IsOverflow         ( pEleQueue_t pRingQ );                         //查看环形队列是否发生溢出
extern uint32_t     eleQueue_GetOverflowTimes   ( pEleQueue_t pRingQ );                         //读取环形队列溢出次数
#endif

#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
extern uint32_t     eleQueue_GetMaxCount        ( pEleQueue_t pRingQ );                         //读取环形队列之前存储过的最大元素数量
#endif

#ifdef __cplusplus
}
#endif

#endif /* REIZ_ELEMENT_QUEUE_H */

/****************************************** END OF FILE *************************************************
*********************************** (C) Copyright 2019 Medlinket ***************************************/
