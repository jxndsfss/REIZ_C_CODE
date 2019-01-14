/*******************************************************************************
 *  @file       reiz_ringQueue.c
 *  @author     jxndsfss
 *  @version    v1.0.3
 *  @date       2019-01-14
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
 *  @param  pRingQ      - 环形队列控制块指针
 *          bufferArray - 存储数据的数组指针
 *          arraySize   - 存储数据的数组大小
 *  @return true        - 成功
 *          false       - 失败
 */
extern bool ringQueue_Init(pRingQueue_t pRingQ, uint8_t *pBufferArray, int32_t arraySize) {
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
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
    pRingQ->LostBytesNum    =   0;
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
extern int32_t ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst) {
#else
extern void ringQueue_GetByteP(pRingQueue_t pRingQ, uint8_t *pDst) {
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
extern int32_t ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num) {
#else
extern void ringQueue_GetMult(pRingQueue_t pRingQ, uint8_t *pDst, int32_t num) {
#endif
    if(num <= pRingQ->count){
        while(num){
            pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
            *pDst++ = pRingQ->pBuffer[pRingQ->head];
            pRingQ->count--;
            num--;
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
 *  @brief  获取环形队列缓存当前存储字节数
 *  @param  pRingQ - 环形队列缓存指针
 *  @return count  - 环形队列缓存当前存储字节数
 */
extern uint32_t ringQueue_GetCount(pRingQueue_t pRingQ) {
    return pRingQ->count;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存是否已满
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 已满
 *          false  - 未满
 */
extern bool ringQueue_IsFull(pRingQueue_t pRingQ) {
    return pRingQ->count == pRingQ->size;
}

/*******************************************************************************
 *  @brief  查看环形队列缓存是否为空
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 为空
 *          false  - 不为空
 */
extern bool ringQueue_IsEmpty(pRingQueue_t pRingQ) {
    return pRingQ->count == 0;
}

/*******************************************************************************
 *  @brief  从环状队列缓存读取1个字节
 *  @param  pRingQ  - 队列指针
 *  @return retData - 读取的字节
 *          0       - 环状队列缓存为空时返回0
 */
extern uint8_t ringQueue_GetByte(pRingQueue_t pRingQ) {
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
extern uint8_t ringQueue_Peek(pRingQueue_t pRingQ) {
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
extern int32_t ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst) {
#else
extern void ringQueue_PeekP(pRingQueue_t pRingQ, uint8_t *pDst) {
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
 *  @brief  将1字节存入环形队列缓存(溢出时不存储新接收的数据)
 *  @param  pRingQ - 环形队列缓存指针
 *          data   - 存入的数据
 *  @return -1     - 溢出
 *          count  - 环形队列缓存当前存储字节数
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
extern int32_t ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data) {
#else
extern void ringQueue_PutByte(pRingQueue_t pRingQ, uint8_t data) {
#endif
    if (pRingQ->count < pRingQ->size) {
        pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
        pRingQ->pBuffer[pRingQ->tail] = data;
        pRingQ->count++;
#if MAX_COUNT_ONCE_STORED_ENABLE
        pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                            pRingQ->count :
                            pRingQ->maxCount;
#endif
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
#if OVERFLOW_TIMES_COUNT_ENABLE
        pRingQ->overflowTimes++;
#endif
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
        pRingQ->LostBytesNum++;
#endif
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
        return -1;
#endif
}

/*******************************************************************************
 *  @brief  将多个字节存入环形队列缓存(溢出时不存储新接收的数据)
 *  @param  pRingQ - 环形队列缓存指针
 *          pSrc   - 存入的数据起始指针
 *          num    - 存入的字节数
 *  @return -1     - 溢出
 *          count  - 环形队列缓存当前存储字节数
 *          -----------------------------------
 *          void
 */
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
extern int32_t ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num) {
#else
extern void ringQueue_PutMult(pRingQueue_t pRingQ, uint8_t *pSrc, int32_t num) {
#endif
    while(num){
        if (pRingQ->count < pRingQ->size) {
            pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
            pRingQ->pBuffer[pRingQ->tail] = *pSrc++;
            pRingQ->count++;
#if MAX_COUNT_ONCE_STORED_ENABLE
            pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                                pRingQ->count :
                                pRingQ->maxCount;
#endif
        } else {
#if OVERFLOW_TIMES_COUNT_ENABLE
            pRingQ->overflowTimes++;
#endif
#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
            pRingQ->LostBytesNum += num;
#endif
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
            return -1;
#else
            return;
#endif
        }
        num--;
    }
#if GET_PUT_PEEK_RETURN_COUNT_ENABLE
    return pRingQ->count;
#endif
}

/*******************************************************************************
 *  @brief  清空环形队列缓存
 *  @param  pRingQ - 环形队列缓存指针
 *  @return void
 */
extern void ringQueue_Flush(pRingQueue_t pRingQ) {
    pRingQ->head = pRingQ->tail = pRingQ->count = 0;
}

/*******************************************************************************
 *  @brief  获取环形队列缓存空余容量字节数
 *  @param  pRingQ - 环形队列缓存指针
 *  @return int    - 空余字节数
 */
extern uint32_t ringQueue_GetFree(pRingQueue_t pRingQ) {
    return pRingQ->size - pRingQ->count;
}

#if OVERFLOW_TIMES_COUNT_ENABLE
/*******************************************************************************
 *  @brief  查看环形队列缓存是否发生溢出
 *  @param  pRingQ - 环形队列缓存指针
 *  @return true   - 溢出
 *          false  - 未溢出
 */
extern bool ringQueue_IsOverflow(pRingQueue_t pRingQ) {
    return pRingQ->overflowTimes > 0 ? true : false;
}

/*******************************************************************************
 *  @brief  读取环形队列缓存溢出次数
 *  @param  pRingQ        - 环形队列缓存指针
 *  @return overflowTimes - 溢出次数
 */
extern int32_t ringQueue_GetOverflowTimes(pRingQueue_t pRingQ) {
    return pRingQ->overflowTimes;
}

#endif /* OVERFLOW_TIMES_COUNT_ENABLE */

#if OVERFLOW_LOST_BYTES_COUNT_ENABLE
/*******************************************************************************
 *  @brief  读取溢出导致的丢失数据总字节数
 *  @param  pRingQ       - 环形队列缓存指针
 *  @return LostBytesNum - 溢出导致的丢失数据总字节数
 */
extern int32_t ringQueue_GetLostBytesNum(pRingQueue_t pRingQ) {
    return pRingQ->LostBytesNum;
}

#endif /* OVERFLOW_LOST_BYTES_COUNT_ENABLE */

#if MAX_COUNT_ONCE_STORED_ENABLE
/*******************************************************************************
 *  @brief  读取环形队列缓存之前存储内容最大字节数量
 *  @param  pRingQ   - 环形队列缓存指针
 *  @return maxCount - 环形队列缓存存储过的最大字节数
 */
extern int32_t ringQueue_GetMaxCount(pRingQueue_t pRingQ) {
    return pRingQ->maxCount;
}

#endif /* MAX_COUNT_ONCE_STORED_ENABLE */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
