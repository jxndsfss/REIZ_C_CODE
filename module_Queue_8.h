/********************************************************************************************************
 *  @file       module_Queue_8.h
 *  @author     chenLei
 *  @version    v1.0.0
 *  @date       2019-06-06
 *  @site       SYS.SZ
 *  @brief      8位元素存储队列头文件
 *
 *  使用方法 --------------------------------------------------------------------------------------------
 *  
 *  [1]静态创建队列
 *  1.定义队列大小
 *  #define QUEUE_1_SIZE      20
 *  #define QUEUE_2_SIZE      20
 *  #define QUEUE_3_SIZE      20
 *  
 *  2.定义队列对象
 *  static QUEUE_8_OBJ(QUEUE_1_SIZE) queueObj_1;
 *  static QUEUE_8_OBJ(QUEUE_2_SIZE) queueObj_2;
 *  static QUEUE_8_OBJ(QUEUE_3_SIZE) queueObj_3;
 *  
 *  3.定义队列指针变量
 *  pQueue8_t pQ_1 = &queueObj_1.ringQ;
 *  pQueue8_t pQ_2 = &queueObj_2.ringQ;
 *  pQueue8_t pQ_3 = &queueObj_3.ringQ;
 *  
 *  4.调用初始化函数进行队列数据结构初始化
 *  queue8_Init(pQ_1, queueObj_1.buffer, QUEUE_1_SIZE);
 *  queue8_Init(pQ_2, queueObj_2.buffer, QUEUE_2_SIZE);
 *  queue8_Init(pQ_3, queueObj_3.buffer, QUEUE_3_SIZE);
 *  
 *  5.调用其他api函数使用队列
 *
 *  [2]动态创建队列
 *  1.定义队列控制块指针变量保存动态创建的队列控制块指针
 *  pQueue8_t pQ = queue8_Create(10);
 *  
 *  2.调用其他api函数使用队列
 *  
 *  3.删除动态创建的队列，释放内存
 *  queue8_Delete(pQ);
 *              
 ********************************************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------------------------------- */
#ifndef MODULE_QUEUE_8_H
#define MODULE_QUEUE_8_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Extern define ------------------------------------------------------------------------------------- */
#ifdef MODULE_QUEUE_8_GLOBALS
#define MODULE_QUEUE_8_EXT
#else
#define MODULE_QUEUE_8_EXT extern
#endif

/* Includes ------------------------------------------------------------------------------------------ */
    
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Exported define ----------------------------------------------------------------------------------- */

/* 宏值：1为打开，0为关闭 */
#define QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE         1                               //溢出覆盖次数统计功能
#define QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE        0                               //统计队列之前存储过的最大字节数

/* Exported macro ------------------------------------------------------------------------------------ */

/* Exported types ------------------------------------------------------------------------------------ */
    
typedef struct queue8_ {
    uint8_t    *pBuffer;                                                            //实际数据存储数组地址
    uint32_t    size;                                                               //队列所能存储元素数目
    uint32_t    count;                                                              //队列当前存储元素数目
    uint32_t    head;                                                               //头索引，元素插入
    uint32_t    tail;                                                               //尾索引，元素取出
#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
    uint32_t    maxCount;                                                           //环形存储区内存储过的最大元素个数
#endif
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
    uint32_t    overflowTimes;                                                      //溢出次数
#endif
} queue8_t, *pQueue8_t;

/* 元素存储环形队列缓存对象宏类型定义 */
#define QUEUE_8_OBJ(bufferSize)     struct {                                        \
                                        queue8_t   ringQ;                           \
                                        uint8_t    buffer[bufferSize];              \
                                    }

/* Exported variables -------------------------------------------------------------------------------- */

/* Exported functions prototypes --------------------------------------------------------------------- */

extern bool      queue8_Init                ( pQueue8_t pRingQ,                     //创建环形队列缓存并进行初始化
                                              uint8_t  *pBufferArray,
                                              uint32_t  arraySize );
extern pQueue8_t queue8_Create              ( uint32_t  queueSize );                //动态创建环形队列
extern void      queue8_Delete              ( pQueue8_t pRingQ );                   //删除动态创建的环形队列，释放内存
extern void      queue8_Flush               ( pQueue8_t pRingQ );                   //清空环形队列
extern bool      queue8_IsFull              ( pQueue8_t pRingQ );                   //查看环形队列是否已满
extern bool      queue8_IsEmpty             ( pQueue8_t pRingQ );                   //查看环形队列是否为空
extern bool      queue8_GetElement          ( pQueue8_t pRingQ, uint8_t *pDst );    //从环形队列取出1个元素
extern bool      queue8_GetMult             ( pQueue8_t pRingQ,                     //从环形队列取出多个元素
                                              uint8_t  *pDst,
                                              int32_t   num );
extern void      queue8_PutElement          ( pQueue8_t pRingQ, uint8_t  data );    //将元素存入环形队列
extern bool      queue8_Peek                ( pQueue8_t pRingQ, uint8_t *pDst );    //查看环形队列尾端1个元素内容，但不取出该元素
extern uint32_t  queue8_GetFree             ( pQueue8_t pRingQ );                   //获取环形队列空余元素容量数
extern uint32_t  queue8_GetCount            ( pQueue8_t pRingQ );                   //获取环形队列当前存储元素个数
extern uint32_t  queue8_GetTailLoc          ( pQueue8_t pRingQ );                   //获取环形队列当前尾位置，即最后一个数据的位置
extern void      queue8_GetElementViaLoc    ( pQueue8_t pRingQ,                     //给定数据终止位置，读取该位置前的所有数据
                                              uint8_t  *pDst,
                                              uint32_t  loc );
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
extern bool      queue8_IsOverflow          ( pQueue8_t pRingQ );                   //查看环形队列是否发生溢出
extern uint32_t  queue8_GetOverflowTimes    ( pQueue8_t pRingQ );                   //读取环形队列溢出次数
#endif

#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
extern uint32_t  queue8_GetMaxCount         ( pQueue8_t pRingQ );                   //读取环形队列之前存储过的最大元素数量
#endif

#ifdef __cplusplus
}
#endif

#endif /* MODULE_QUEUE_8_H */

/******************************************* END OF FILE ************************************************
************************************** (C) Copyright 2019 REIZ *****************************************/