/********************************************************************************************************
 *  @file       module_Queue_8.c
 *  @author     chenLei
 *  @version    v1.0.0
 *  @date       2019-06-06
 *  @site       SYS.SZ
 *  @brief      8位元素存储队列源文件
 ********************************************************************************************************
 */

/* Includes ------------------------------------------------------------------------------------------ */

#include "module_Queue_8.h"

/* Private define ------------------------------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------------------------------- */
/* Private typedef ----------------------------------------------------------------------------------- */
/* Private variables --------------------------------------------------------------------------------- */
/* Exported variables -------------------------------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------------------------------- */

/********************************************************************************************************
 *  @brief  初始化静态元素存储环形队列
 *  @param  pRingQ      - 环形队列控制块指针
 *          bufferArray - 存储数据的数组指针
 *          arraySize   - 存储数据的数组大小
 *  @return true        - 成功
 *          false       - 失败
 */
bool queue8_Init ( pQueue8_t pRingQ, uint8_t *pBufferArray, uint32_t arraySize ) {
    if (pRingQ == NULL || pBufferArray == NULL || arraySize == 0) {
        return false;
    }

    pRingQ->pBuffer         =   pBufferArray;
    pRingQ->size            =   arraySize;
    pRingQ->count           =   0;
    pRingQ->head            =   0;
    pRingQ->tail            =   0;
#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
    pRingQ->maxCount        =   0;
#endif
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
    pRingQ->overflowTimes   =   0;
#endif

    return true;
}

/********************************************************************************************************
 *  @brief  动态创建元素存储环形队列并进行初始化
 *  @param  queueSize - 队列大小
 *  @return NULL      - 创建失败
 *          !NULL     - 创建成功，返回环形队列控制块指针
 */
pQueue8_t queue8_Create ( uint32_t queueSize ) {
    pQueue8_t pRingQ;
    uint8_t *pBuffer;
        
    if (queueSize == 0) {
        return NULL;
    }
    
    pRingQ = (pQueue8_t)malloc(sizeof(queue8_t));
    if (pRingQ != NULL) {
        pBuffer = malloc(queueSize);
        if (pBuffer != NULL) {
            pRingQ->pBuffer         =   pBuffer;
            pRingQ->size            =   queueSize;
            pRingQ->count           =   0;
            pRingQ->head            =   0;
            pRingQ->tail            =   0;
#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
            pRingQ->maxCount        =   0;
#endif
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
            pRingQ->overflowTimes   =   0;
#endif
            return pRingQ;
        } else {
            free(pRingQ);
        }
    }
    return NULL;
}

/********************************************************************************************************
 *  @brief  删除动态创建的元素存储环形队列，不可用于静态环形队列！
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return void
 */
void queue8_Delete ( pQueue8_t pRingQ ) {
    if (pRingQ != NULL) {
        if (pRingQ->pBuffer != NULL) {
            free(pRingQ->pBuffer);
        }
        free(pRingQ);
    }
}

/********************************************************************************************************
 *  @brief  清空环形队列
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return void
 */
void queue8_Flush ( pQueue8_t pRingQ ) {
    pRingQ->head = pRingQ->tail = pRingQ->count = 0;

#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
    pRingQ->maxCount = 0;
#endif

#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
    pRingQ->overflowTimes = 0;
#endif
}

/********************************************************************************************************
 *  @brief  查看环形队列是否已满
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return true   - 已满
 *          false  - 未满
 */
bool queue8_IsFull ( pQueue8_t pRingQ ) {
    return pRingQ->count == pRingQ->size;
}

/********************************************************************************************************
 *  @brief  查看环形队列是否为空
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return true   - 为空
 *          false  - 不为空
 */
bool queue8_IsEmpty ( pQueue8_t pRingQ ) {
    return pRingQ->count == 0;
}

/********************************************************************************************************
 *  @brief  从环形队列取出1个元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *          pDst   - 元素存储地址
 *  @return false  - 读取失败，无数据
 *          true   - 读取成功
 */
bool queue8_GetElement ( pQueue8_t pRingQ, uint8_t *pDst ) {
    if (pRingQ->count > 0) {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        *pDst =  pRingQ->pBuffer[pRingQ->head];
        pRingQ->count--;
        return true;
    } else {
        return false;
    }
}

/********************************************************************************************************
 *  @brief  从环形队列取出多个元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *          pDst   - 元素存储起始地址
 *          num    - 读取数目
 *  @return false  - 读取失败，数据数量不足
 *          true   - 读取成功
 */
bool queue8_GetMult ( pQueue8_t pRingQ, uint8_t *pDst, int32_t num ) {
    if(num <= pRingQ->count){
        while(num--){
            pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
            *pDst++ = pRingQ->pBuffer[pRingQ->head];
            pRingQ->count--;
        }
        return true;
    }
    return false;
}

/********************************************************************************************************
 *  @brief  获取环形队列空余元素容量数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return int    - 队列中剩余可存储元素个数
 */
uint32_t queue8_GetFree ( pQueue8_t pRingQ ) {
    return pRingQ->size - pRingQ->count;
}

/********************************************************************************************************
 *  @brief  获取环形队列当前存储元素个数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return count  - 环形队列当前存储元素个数
 */
uint32_t queue8_GetCount ( pQueue8_t pRingQ ) {
    return pRingQ->count;
}

/********************************************************************************************************
 *  @brief  将元素存入环形队列
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return void
 */
void queue8_PutElement ( pQueue8_t pRingQ, uint8_t data ) {
    pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
    pRingQ->pBuffer[pRingQ->tail] = data;

    if (pRingQ->count < pRingQ->size) {
        pRingQ->count++;
#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
        pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                            pRingQ->count :
                            pRingQ->maxCount;
#endif
    }
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
    else {
        pRingQ->overflowTimes++;
    }
#endif
}

/********************************************************************************************************
 *  @brief  查看环形队列是否发生溢出
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 
 */
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
bool queue8_IsOverflow ( pQueue8_t pRingQ ) {
    return pRingQ->overflowTimes > 0;
}
#endif

/********************************************************************************************************
 *  @brief  读取环形队列溢出次数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 溢出次数
 */
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
uint32_t queue8_GetOverflowTimes ( pQueue8_t pRingQ ) {
    return pRingQ->overflowTimes;
}
#endif

/********************************************************************************************************
 *  @brief  读取环形队列之前存储过的最大元素数量
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 之前存储过的最大元素数量
 */
#if QUEUE_8_MAX_COUNT_ONCE_STORED_ENABLE
uint32_t queue8_GetMaxCount ( pQueue8_t pRingQ ) {
    return pRingQ->maxCount;
}
#endif

/********************************************************************************************************
 *  @brief  获取环形队列当前尾位置，即最后一个数据的位置
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 末元素的位置
 */
uint32_t queue8_GetTailLoc ( pQueue8_t pRingQ ) {
    return pRingQ->tail;
}

/********************************************************************************************************
 *  @brief  给定数据终止位置，读取该位置前的所有数据
 *          [pDst缓存的大小必须 >= pRingQ的环形队列大小，否则当环形队列溢出时读取，pDst缓存访问会越界!]
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *          pDst   - 数据存储起始地址
 *          loc    - 读取数据末位置
 *  @return 
 */
void queue8_GetElementViaLoc ( pQueue8_t pRingQ, uint8_t *pDst, uint32_t loc ) {
    if ((pRingQ->count == pRingQ->size)
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
        && (pRingQ->overflowTimes > 0)
#endif
       ){
        pRingQ->head = pRingQ->tail;
#if QUEUE_8_OVERFLOW_TIMES_COUNT_ENABLE
        pRingQ->overflowTimes = 0;
#endif
    }

    do {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        *pDst++ = pRingQ->pBuffer[pRingQ->head];
        pRingQ->count--;
    } while (pRingQ->head != loc && pRingQ->count > 0);
}

/******************************************* END OF FILE ************************************************
************************************** (C) Copyright 2019 REIZ *****************************************/