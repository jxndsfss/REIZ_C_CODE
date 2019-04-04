/********************************************************************************************************
 *  @file       reiz_elementQueue.c
 *  @author     chenLei
 *  @version    v1.0.0
 *  @date       2019-03-30
 *  @site       xinMeiYuan.A41
 *  @brief      FIFO先进先出元素存储环形队列源文件
 ********************************************************************************************************
 */

/* Includes ------------------------------------------------------------------------------------------ */
#include "reiz_elementQueue.h"

/* Private define ------------------------------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------------------------------- */
/* Private typedef ----------------------------------------------------------------------------------- */
/* Private variables --------------------------------------------------------------------------------- */
/* Exported variables -------------------------------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------------------------------- */

/********************************************************************************************************
 *  @brief  创建元素存储环形队列并进行初始化
 *  @param  pRingQ       - 环形队列控制块指针
 *          pBufferArray - 存储数据的数组指针
 *          arraySize    - 存储数据的数组大小
 *  @return true         - 成功
 *          false        - 失败
 */
bool eleQueue_Init ( pEleQueue_t pRingQ, ELEMENT_TYPE *pBufferArray, uint32_t arraySize ) {
    if (pRingQ == NULL || pBufferArray == NULL || arraySize == 0) {
        return false;
    }
    
    pRingQ->pBuffer         =   pBufferArray;
    pRingQ->size            =   arraySize;
    pRingQ->count           =   0;
    pRingQ->head            =   0;
    pRingQ->tail            =   0;
#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
    pRingQ->maxCount        =   0;
#endif
#if ELE_OVERFLOW_TIMES_COUNT_ENABLE
    pRingQ->overflowTimes   =   0;
#endif
    
    return true;
}

/********************************************************************************************************
 *  @brief  清空环形队列
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return void
 */
void eleQueue_Flush ( pEleQueue_t pRingQ ) {
    pRingQ->head = pRingQ->tail = pRingQ->count = 0;
}

/********************************************************************************************************
 *  @brief  查看环形队列是否已满
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return true   - 已满
 *          false  - 未满
 */
bool eleQueue_IsFull ( pEleQueue_t pRingQ ) {
    return pRingQ->count == pRingQ->size;
}

/********************************************************************************************************
 *  @brief  查看环形队列是否为空
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return true   - 为空
 *          false  - 不为空
 */
bool eleQueue_IsEmpty ( pEleQueue_t pRingQ ) {
    return pRingQ->count == 0;
}

/********************************************************************************************************
 *  @brief  从环形队列取出1个元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 0    - 无数据
 *          其他 - 元素内容
 */
ELEMENT_TYPE eleQueue_GetElement ( pEleQueue_t pRingQ ) {
    if (pRingQ->count > 0) {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        pRingQ->count--;
        return pRingQ->pBuffer[pRingQ->head];
    } else {
        return 0;
    }
}

/********************************************************************************************************
 *  @brief  查看环形队列尾端1个元素内容，但不取出该元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 0    - 无数据
 *          其他 - 元素内容
 */
ELEMENT_TYPE eleQueue_Peek ( pEleQueue_t pRingQ ) {
    return pRingQ->count > 0 ? pRingQ->pBuffer[pRingQ->tail] : 0;
}

/********************************************************************************************************
 *  @brief  获取环形队列空余元素容量数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return int    - 队列中剩余可存储元素个数
 */
uint32_t eleQueue_GetFree ( pEleQueue_t pRingQ ) {
    return pRingQ->size - pRingQ->count;
}

/********************************************************************************************************
 *  @brief  获取环形队列当前存储元素个数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return count  - 环形队列当前存储元素个数
 */
uint32_t eleQueue_GetCount ( pEleQueue_t pRingQ ) {
    return pRingQ->count;
}

/********************************************************************************************************
 *  @brief  从环状队列读取1个元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return -1     - 读取失败
 *          count  - 读取后队列中当前存储元素个数
 *          -------------------------------------
 *          void
 */
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
int32_t eleQueue_GetElementP ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst ) {
#else
void eleQueue_GetElementP ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst ) {
#endif
    if (pRingQ->count > 0) {
        pRingQ->head = (pRingQ->head + 1) % pRingQ->size;
        *pDst = pRingQ->pBuffer[pRingQ->head];
        pRingQ->count--;
        
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
    
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
    return -1;
#endif
}

/********************************************************************************************************
 *  @brief  查看环形队列存尾端元素内容，但不取出该元素
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return count  - 环形队列当前存储元素个数
 *          -1     - 查看失败，队列为空
 *          -----------------------------------
 *          void
 */
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
int32_t eleQueue_PeekP ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst ) {
#else
void eleQueue_PeekP ( pEleQueue_t pRingQ, ELEMENT_TYPE *pDst ) {
#endif
    if (pRingQ->count > 0) {
        *pDst = pRingQ->pBuffer[pRingQ->tail];
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
        return pRingQ->count;
#else
        return;
#endif
    }
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
        return -1;
#endif
}

/********************************************************************************************************
 *  @brief  将元素存入环形队列
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return count  - 环形队列当前存储元素个数
 *          -----------------------------------
 *          void
 */
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
uint32_t eleQueue_PutElement ( pEleQueue_t pRingQ, ELEMENT_TYPE data ) {
#else
void eleQueue_PutElement ( pEleQueue_t pRingQ, ELEMENT_TYPE data ) {
#endif
    pRingQ->tail = (pRingQ->tail + 1) % pRingQ->size;
    pRingQ->pBuffer[pRingQ->tail] = data;
    
    if (pRingQ->count < pRingQ->size) {
        pRingQ->count++;
#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
        pRingQ->maxCount = (pRingQ->count > pRingQ->maxCount) ?
                            pRingQ->count :
                            pRingQ->maxCount;
#endif
    }
#if ELE_OVERFLOW_TIMES_COUNT_ENABLE
    else {
        pRingQ->overflowTimes++;
    }
#endif
#if ELE_GET_PUT_RETURN_COUNT_ENABLE
    return pRingQ->count;
#endif
}

/********************************************************************************************************
 *  @brief  查看环形队列是否发生溢出
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 
 */
#if ELE_OVERFLOW_TIMES_COUNT_ENABLE
bool eleQueue_IsOverflow ( pEleQueue_t pRingQ ) {
    return pRingQ->overflowTimes > 0 ? true : false;
}
#endif

/********************************************************************************************************
 *  @brief  读取环形队列溢出次数
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 
 */
#if ELE_OVERFLOW_TIMES_COUNT_ENABLE
uint32_t eleQueue_GetOverflowTimes ( pEleQueue_t pRingQ ) {
    return pRingQ->overflowTimes;
}
#endif

/********************************************************************************************************
 *  @brief  读取环形队列之前存储过的最大元素数量
 *  @param  pRingQ - 元素存储环形队列控制块指针
 *  @return 
 */
#if ELE_MAX_COUNT_ONCE_STORED_ENABLE
uint32_t eleQueue_GetMaxCount ( pEleQueue_t pRingQ ) {
    return pRingQ->maxCount;
}
#endif

/****************************************** END OF FILE *************************************************
*********************************** (C) Copyright 2019 Medlinket ***************************************/
