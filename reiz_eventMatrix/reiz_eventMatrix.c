/*******************************************************************************
 *  @file       reiz_eventMatrix.c
 *  @author     jxndsfss
 *  @version    v1.0.3
 *  @date       2019-01-12
 *  @site       ShangYouSong.SZ
 *  @brief      事件矩阵实现源文件
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_eventMatrix.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
 *  @brief  事件矩阵控制块初始化
 *  @param  pEcb          - 事件控制块指针
 *          matrixRow     - 矩阵行数
 *          pEvFlagMatrix - 标志矩阵指针
 *          pEvCbMatrix   - 事件回调函数矩阵指针
 *          pEvParaMatrix - 事件回调函数参数矩阵指针
 *  @return true          - 初始化成功
 *          false         - 初始化失败
 */
extern bool eventMatrix_ecbInit(    pEcb_t              pEcb,
                                    int                 matrixRow,
                                    pEvFlagMatrix_t     pEvFlagMatrix,
                                    pEvCbMatrix_t       pEvCbMatrix,
                                    pEvParaMatrix_t     pEvParaMatrix)
{
    if (pEcb == NULL || matrixRow == 0 || pEvFlagMatrix == NULL || 
        pEvCbMatrix == NULL || pEvParaMatrix == NULL) {
        return false;
    }

    pEcb->matrixRow         =   matrixRow;
    pEcb->pEvFlagMatrix     =   pEvFlagMatrix;
    pEcb->pEvCbMatrix       =   pEvCbMatrix;
    pEcb->pEvParaMatrix     =   pEvParaMatrix;

    return true;
}

/*******************************************************************************
 *  @brief  设置事件标志
 *  @param  pEcb      - 事件控制块指针
 *          eventName - 事件标志
 *  @return true      - 设置成功
 *          false     - 设置失败
 */
extern bool eventMatrix_SetEventFlag(pEcb_t pEcb, int eventFlag) {
    int row, col;

    if (pEcb != NULL) {
        row = eventFlag / EVENT_MATRIX_COL;
        col = eventFlag % EVENT_MATRIX_COL;
        (*pEcb->pEvFlagMatrix)[row] |= (EVENT_FLAG_MATRIX_ROW_TYPE)1 << col;
        return true;
    }
    return false;
}

/*******************************************************************************
 *  @brief  读取事件标志
 *  @param  pEcb      - 事件控制块指针
 *          eventName - 事件标志
 *  @return true      - 事件标志存在
 *          false     - 事件标志不存在
 */
extern bool eventMatrix_GetEventFlag(pEcb_t pEcb, int eventFlag) {
    int row, col;

    if (pEcb != NULL) {
        row = eventFlag / EVENT_MATRIX_COL;
        col = eventFlag % EVENT_MATRIX_COL;
        return (*pEcb->pEvFlagMatrix)[row] & ((EVENT_FLAG_MATRIX_ROW_TYPE)1 << col) ? true : false;        
    }
    return false;
}

/*******************************************************************************
 *  @brief  清除事件标志
 *  @param  pEcb      - 事件控制块指针
 *          eventName - 事件标志
 *  @return true      - 清除成功
 *          false     - 清除失败
 */
extern bool eventMatrix_ClearEventFlag(pEcb_t pEcb, int eventFlag) {
    int row, col;

    if (pEcb != NULL) {
        row = eventFlag / EVENT_MATRIX_COL;
        col = eventFlag % EVENT_MATRIX_COL;
        (*pEcb->pEvFlagMatrix)[row] &= ~((EVENT_FLAG_MATRIX_ROW_TYPE)1 << col);
        return true;
    }
    return false;
}

/*******************************************************************************
 *  @brief  事件处理函数，轮询事件矩阵
 *  @param  pEcb - 事件控制块指针
 *  @return void
 */
extern void eventMatrix_EventProcess(pEcb_t pEcb) {
    int row, col, flag;

    if (pEcb == NULL) {
        return;
    }

    for (row = 0; row < pEcb->matrixRow; row++) {
        if (!(*pEcb->pEvFlagMatrix)[row]) {                                                 //该行无事件则跳过，继续下一行
            continue;        
        }

        for (col = 0; col < EVENT_MATRIX_COL; col++) {
            flag = (*pEcb->pEvFlagMatrix)[row] & ((EVENT_FLAG_MATRIX_ROW_TYPE)1 << col);    //获得事件标志

            if (flag && (*pEcb->pEvCbMatrix)[row][col] != NULL) {                           //事件标志存在且回调函数存在则执行事件处理回调函数
                (*pEcb->pEvCbMatrix)[row][col]((*pEcb->pEvParaMatrix)[row][col]);

                if ((*pEcb->pEvParaMatrix)[row][col] != NULL) {                             //检查参数集合数据结构内存是否释放，未释放则进行释放
                    free((*pEcb->pEvParaMatrix)[row][col]);
                    (*pEcb->pEvParaMatrix)[row][col] = NULL;
                }
                (*pEcb->pEvFlagMatrix)[row] &= ~((EVENT_FLAG_MATRIX_ROW_TYPE)1 << col);     //清除事件标志
            }
        }
    }
}

/*******************************************************************************
 *  @brief  保存事件参数集合数据结构指针，以供事件处理回调函数使用
 *  @param  pEcb      - 事件控制块指针
 *          eventFlag - 事件标志
 *          pPara     - 事件参数集合数据结构指针
 *  @return true      - 保存成功
 *          false     - 保存失败
 */
extern bool eventMatrix_SaveEventPara(pEcb_t pEcb, int eventFlag, void *pPara) {
    int row, col;

    if (pEcb != NULL) {
        row = eventFlag / EVENT_MATRIX_COL;
        col = eventFlag % EVENT_MATRIX_COL;
        (*pEcb->pEvParaMatrix)[row][col] = pPara;
        return true;
    }
    return false;
}

/*******************************************************************************
 *  @brief  注册事件处理回调函数
 *  @param  pEcb      - 事件控制块指针
 *          eventFlag - 事件标志
 *          pCb       - 事件处理回调函数指针
 *  @return true      - 注册成功
 *          false     - 注册失败
 */
extern bool eventMatrix_RegistEvCB(pEcb_t pEcb, int eventFlag, pEventCB_t pCb) {
    int row, col;

    if (pEcb != NULL) {
        row = eventFlag / EVENT_MATRIX_COL;
        col = eventFlag % EVENT_MATRIX_COL;
        (*pEcb->pEvCbMatrix)[row][col] = pCb;
        return true;
    }
    return false;
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
