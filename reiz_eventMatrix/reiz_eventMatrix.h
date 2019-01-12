/*******************************************************************************
 *  @file       reiz_eventMatrix.h
 *  @author     jxndsfss
 *  @version    v1.0.3
 *  @date       2019-01-12
 *  @site       ShangYouSong.SZ
 *  @brief      事件矩阵实现头文件
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef REIZ_EVENT_MATRIX_H
#define REIZ_EVENT_MATRIX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Exported define -----------------------------------------------------------*/

/* 事件标志矩阵一维数组元素类型 */
#define EVENT_FLAG_MATRIX_ROW_TYPE      uint32_t

/* 事件矩阵列数 */
#define EVENT_MATRIX_COL                (sizeof( EVENT_FLAG_MATRIX_ROW_TYPE ) * 8)

/* Exported macro ------------------------------------------------------------*/

/*
    事件矩阵块宏类型定义
    使用方法：
    #define EVENT_MATRIX_ROW  2
    EVENT_PROCESS_OBJ(EVENT_MATRIX_ROW) xxxEvObj;
*/
#define EVENT_PROCESS_OBJ(row)                                  \
struct {                                                        \
    EVENT_FLAG_MATRIX_ROW_TYPE          evFlagMatrix[row];      \
    evCbMatrixRowArr_t                  evCbMatrix[row];        \
    evParaMatrixRowArr_t                evParaMatrix[row];      \
}

/* Exported types ------------------------------------------------------------*/
typedef EVENT_FLAG_MATRIX_ROW_TYPE (*pEvFlagMatrix_t)[];                        //事件标志矩阵指针类型定义
typedef void (*pEventCB_t)(void *pPara);                                        //事件处理回调函数指针类型定义
typedef pEventCB_t evCbMatrixRowArr_t[EVENT_MATRIX_COL];                        //事件回调函数矩阵行元素类型定义
typedef evCbMatrixRowArr_t (*pEvCbMatrix_t)[];                                  //事件回调函数矩阵指针类型定义
typedef void *evParaMatrixRowArr_t[EVENT_MATRIX_COL];                           //事件回调函数参数矩阵行元素类型定义
typedef evParaMatrixRowArr_t (*pEvParaMatrix_t)[];                              //事件参数矩阵指针类型定义

typedef struct eventControlBlock_ {                                             //事件控制块类型定义
    int                 matrixRow;
    pEvFlagMatrix_t     pEvFlagMatrix;                                          //事件标志矩阵指针
    pEvCbMatrix_t       pEvCbMatrix;                                            //事件处理回调函数矩阵指针
    pEvParaMatrix_t     pEvParaMatrix;                                          //事件参数矩阵指针
} ecb_t, *pEcb_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

extern bool eventMatrix_ecbInit(    pEcb_t              pEcb,                   //事件控制块初始化
                                    int                 matrixRow,
                                    pEvFlagMatrix_t     pEvFlagMatrix,
                                    pEvCbMatrix_t       pEvCbMatrix,
                                    pEvParaMatrix_t     pEvParaMatrix);

extern void eventMatrix_EventProcess(pEcb_t pEcb);                              //事件处理函数，轮询事件矩阵
extern bool eventMatrix_SetEventFlag(pEcb_t pEcb, int eventFlag);               //设置事件标志
extern bool eventMatrix_GetEventFlag(pEcb_t pEcb, int eventFlag);               //读取事件标志
extern bool eventMatrix_ClearEventFlag(pEcb_t pEcb, int eventFlag);             //清除事件标志
extern bool eventMatrix_SaveEventPara(pEcb_t pEcb, int eventFlag, void *pPara); //保存事件参数集合数据结构指针，以供事件处理回调函数使用
extern bool eventMatrix_RegistEvCB(pEcb_t pEcb, int eventFlag, pEventCB_t pCb); //注册事件处理回调函数

#ifdef __cplusplus
}
#endif

#endif /* REIZ_EVENT_MATRIX_H */

/*******************************************************************************
 *  @usecase
 *
 *  app.c
 *
 *  //事件名称标志
 *  #define EVENT_0_FLAG    0
 *  #define EVENT_1_FLAG    1
 *  //事件数量可从0开始无限增加...
 *
 *  //与事件相关的参数集合，数据类型定义
 *  typedef struct para_ {
 *      int data;
 *      //...
 *  } para_t;
 *
 *  //定义事件控制块及其指针
 *  static ecb_t ecb;
 *  extern pEcb_t pEcb = &ecb;
 *
 *  //定义事件处理回调函数
 *  static event_0_CB(void *pPara) {
 *      //...
 *      //参数使用完不释放指针pPara指向的内存，由事件处理函数内部进行释放！
 *  }
 *
 *  static event_1_CB(void *pPara) {
 *      //...
 *      //参数使用完不释放指针pPara指向的内存，由事件处理函数内部进行释放！
 *  }
 *
 *  //程序运行前初始化，注册所有事件的回调函数
 *  extern registEventCB(void) {
 *      eventMatrix_RegistEventHandleCB(pEcb, EVENT_0_FLAG, event_0_CB);
 *      eventMatrix_RegistEventHandleCB(pEcb, EVENT_1_FLAG, event_1_CB);
 *  }
 *
 *  void main(void) {
 *      para_t event0Para, event1Para;
 *
 *      registEventCB();
 *
 *      event1Para.data = 0;
 *      eventMatrix_SetEventFlag(pEcb, EVENT_0_FLAG);
 *      eventMatrix_SaveEventPara(pEcb, EVENT_0_FLAG, &event0Para);
 *
 *      event1Para.data = 1;
 *      eventMatrix_SetEventFlag(pEcb, EVENT_1_FLAG);
 *      eventMatrix_SaveEventPara(pEcb, EVENT_1_FLAG, &event1Para);
 *
 *      while(1) {
 *          eventMatrix_EventProcess(pEcb);
 *      }
 *  }
 *
 *******************************************************************************
 */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
