/*******************************************************************************
 *  @file       reiz_ringQueue.c
 *  @author     jxndsfss
 *  @version    v1.0.5
 *  @date       2019-04-04
 *  @site       ShangYouSong.SZ
 *  @brief      环形队列缓存源文件
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_ringQueue.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
 *  @brief  环形队列缓存并进行初始化
 *  @param  pRingQ       - 环形队列控制块指针
 *          pBufferArray - 存储数据的数组指针
 *          arraySize    - 存储数据的数组大小
 *  @return true         - 成功
 *          false        - 失败
 */
bool ringQueue_Init(pRingQueue_t pRingQ, uint8_t *pBufferArray, int32_t arraySize) {
    if (pRingQ == NULL || pBufferArray == NULL || arraySize == 0) {
        return false;
    }
    
    pRingQ->pBuffer         =   pBufferArray;
    pRingQ->size            =   arraySize;
    pRingQ->count           =   0;
    pRingQ->head            =   0;
    pRingQ->tail            =   0;
#if MAX_COUNT_ONCE_STORED_ENABLE
    pRingQ->maxCount        =   0;
#endif
#if OVERFLOW_TIMES_COUNT_ENABLE
    pRingQ->overflowTimes   =   0;
#endif
    
    return true;
}

/*******************************************************************************
 *  @brief  从环状队列缓存读取1个字节
 *  @param  pRingQ - 队列指针
 *          pData  - 数据存储指针
 *  @return -1     - 读取失败
 *          count  - 读取后队列中当前存储字节数
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst) {
#else
void ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst) {
#endif
    if (pRingQ->count > 0) {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        *pDst = pRingQ->pBuffer[pRingQ->head];
        pRingQ->count--;
        
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
    
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
    return -1;
#endif
}

/*******************************************************************************
 *  @brief  从环形队列缓存读取多个字节
 *  @param  pRingQ - 环形队列缓存指针
 *          pDst   - 数据存储指针
 *          num    - 读取字节数
 *  @return -1     - 读取失败
 *          count  - 读取后环形队列缓存的当前存储字节数
 *          -------------------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num) {
#else
void ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num) {
#endif
    if(num <= pRingQ->count){
        while(num--){
            pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
            *pDst++ = pRingQ->pBuffer[pRingQ->head];
            pRingQ->count--;
        }
        
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
    
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return -1;
#endif
}

/*******************************************************************************
 *  @brief  通过尾位置读取缓存内容，即读出缓存内该位置前所有内容
 *  @param  pRingQ - 环形队列缓存指针
 *  @return -1     - 读取失败
 *          count  - 读取后环形队列缓存的当前存储字节数
 *          -------------------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_GetDataViaLocation(pRingQueue_t pRingQ, uint8_t *pDst, uint32_t loc) {
#else
void ringQueue_GetDataViaLocation(pRingQueue_t pRingQ, uint8_t *pDst, uint32_t loc) {
#endif
    if(pRingQ->count > 0){
        while(pRingQ->head != loc){
            pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
            *pDst++ = pRingQ->pBuffer[pRingQ->head];
            pRingQ->count--;
        }
        
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
    
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return -1;
#endif
}

/*******************************************************************************
 *  @brief  当环形队列存满且溢出时，指定location作为环形队列缓存最后接收位置，获取该位置前的所有数据.
 *          这种情况的溢出，是考虑到缓存若接收到一些无效数据字节导致缓存存满而溢出的情况！
 *  @param  pRingQ   - 环形队列缓存指针
 *  @return pDst     - 环形队列缓存当前存储字节数
 *          location - 指定作为缓存最终接收位置
 */
void ringQueue_GetAllDataViaLocationOverflow(pRingQueue_t pRingQ, uint8_t *pDst, uint32_t location) {
    if (ringQueue_IsFull(pRingQ) && ringQueue_IsOverflow(pRingQ)) {
        pRingQ->head = pRingQ->tail = location;
        ringQueue_GetMult(pRingQ, pDst, pRingQ->size);
        ringQueue_Flush(pRingQ);
    }
}

/*******************************************************************************
 *  @brief  获取环形队列缓存当前存储字节数
 *  @param  pRingQ - 环形队列缓存指针
 *  @return count  - 环形队列缓存当前存储字节数
 */
uint32_t ringQueue_GetCount(pRingQueue_t pRingQ) {
    return pRingQ->count;
}

/*******************************************************************************
 *  @brief  获取环形队列尾位置，即最后存储字节在缓存中的存储位置
 *  @param  pRingQ   - 环形队列缓存指针
 *  @return uint32_t - 环形队列缓存尾位置，即最后存储字节在缓存中的位置
 */
uint32_t ringQueue_GetTailLocation(pRingQueue_t pRingQ) {
    return pRingQ->tail;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存是否已满
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 已满
 *          false  - 未满
 */
bool ringQueue_IsFull(pRingQueue_t pRingQ) {
    return pRingQ->count == pRingQ->size;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存是否为空
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 为空
 *          false  - 不为空
 */
bool ringQueue_IsEmpty(pRingQueue_t pRingQ) {
    return pRingQ->count == 0;
}

/*******************************************************************************
 *  @brief  从环状队列缓存读取1个字节
 *  @param  pRingQ  - 队列指针
 *  @return retData - 读取的字节
 *          0       - 环状队列缓存为空时返回0
 */
uint8_t ringQueue_GetByte(pRingQueue_t pRingQ) {
    uint8_t retData = 0;
    
    if (pRingQ->count > 0) {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        retData = pRingQ->pBuffer[pRingQ->head];
        pRingQ->count--;
    }
    return retData;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存尾端1字节内容，但不取出该字节
 *  @param  pRingQ  - 队列指针
 *  @return retData - 查看的字节
 *          0       - 环状队列缓存为空时返回0
 */
uint8_t ringQueue_Peek(pRingQueue_t pRingQ) {
    return pRingQ->count > 0 ? pRingQ->pBuffer[pRingQ->tail] : 0;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存尾端1字节内容，但不取出该字节
 *  @param  pRingQ - 环形队列缓存指针
 *          pDst   - 存放数据的变量指针
 *  @return count  - 环形队列缓存当前存储字节数
 *          -1     - 查看失败，缓存为空
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst) {
#else
void ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst) {
#endif
    if (pRingQ->count > 0) {
        *pDst = pRingQ->pBuffer[pRingQ->tail];
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return -1;
#endif
}

/*******************************************************************************
 *  @brief  将1字节存入环形队列缓存(溢出时循环覆盖之前的数据)
 *  @param  pRingQ - 环形队列缓存指针
 *          data   - 存入的数据
 *  @return count  - 环形队列缓存当前存储字节数
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data) {
#else
void ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data) {
#endif
    pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
    pRingQ->pBuffer[pRingQ->tail] = data;
    
    if (pRingQ->count < pRingQ->size) {
        pRingQ->count++;
#if MAX_COUNT_ONCE_STORED_ENABLE
        pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                            pRingQ->count :
                            pRingQ->maxCount;
#endif
    }
#if OVERFLOW_TIMES_COUNT_ENABLE
    else {
        pRingQ->overflowTimes++;
    }
#endif
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
    return pRingQ->count;
#endif
}

/*******************************************************************************
 *  @brief  将多个字节存入环形队列缓存(溢出时循环覆盖之前的数据)
 *  @param  pRingQ - 环形队列缓存指针
 *          pSrc   - 存入的数据起始指针
 *          num    - 存入的字节数
 *  @return count  - 环形队列缓存当前存储字节数
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
int32_t ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num) {
#else
void ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num) {
#endif
    while(num--){
        pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
        pRingQ->pBuffer[pRingQ->tail] = *pSrc++;
            
        if (pRingQ->count < pRingQ->size) {
            pRingQ->count++;
#if MAX_COUNT_ONCE_STORED_ENABLE
            pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                                pRingQ->count :
                                pRingQ->maxCount;
#endif
        }
#if OVERFLOW_TIMES_COUNT_ENABLE
        else {
            pRingQ->overflowTimes++;
        }
#endif
    }
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
    return pRingQ->count;
#endif
}

/*******************************************************************************
 *  @brief  清空环形队列缓存，计数清零，溢出计数清零
 *  @param  pRingQ - 环形队列缓存指针
 *  @return void
 */
void ringQueue_Flush(pRingQueue_t pRingQ) {
    pRingQ->head = pRingQ->tail = pRingQ->count = pRingQ->overflowTimes = 0;
}

/*******************************************************************************
 *  @brief  获取环形队列缓存空余容量字节数
 *  @param  pRingQ - 环形队列缓存指针
 *  @return int    - 空余字节数
 */
uint32_t ringQueue_GetFree(pRingQueue_t pRingQ) {
    return pRingQ->size - pRingQ->count;
}

#if OVERFLOW_TIMES_COUNT_ENABLE
/*******************************************************************************
 *  @brief  查看环形队列缓存是否发生溢出
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 溢出
 *          false  - 未溢出
 */
bool ringQueue_IsOverflow(pRingQueue_t pRingQ) {
    return pRingQ->overflowTimes > 0 ? true : false;
}

/*******************************************************************************
 *  @brief  读取环形队列缓存溢出次数
 *  @param  pRingQ        - 环形队列缓存指针
 *  @return overflowTimes - 溢出次数
 */
int32_t ringQueue_GetOverflowTimes(pRingQueue_t pRingQ) {
    return pRingQ->overflowTimes;
}

#endif /* OVERFLOW_TIMES_COUNT_ENABLE */

#if MAX_COUNT_ONCE_STORED_ENABLE
/*******************************************************************************
 *  @brief  读取环形队列缓存之前存储内容最大字节数量
 *  @param  pRingQ   - 环形队列缓存指针
 *  @return maxCount - 环形队列缓存存储过的最大字节数
 */
int32_t ringQueue_GetMaxCount(pRingQueue_t pRingQ) {
    return pRingQ->maxCount;
}

#endif /* MAX_COUNT_ONCE_STORED_ENABLE */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/