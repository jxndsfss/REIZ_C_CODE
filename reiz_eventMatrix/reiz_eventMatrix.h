/*******************************************************************************
 *  @file       reiz_eventMatrix.h
 *  @author     jxndsfss
 *  @version    v1.0.1
 *  @date       2019-01-06
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

/* 
 * 事件矩阵行数，所能表示总事件数为 MATRIX_ROW * sizeof(FLAG_MATRIX_ROW_TYPE) * 8，
 * 事件矩阵用于事件较多的场合，举例若 FLAG_MATRIX_ROW_TYPE 为 uint32_t，
 * 则事件总数为MATRIX_ROW * 32，若MATRIX_ROW为2，则事件总数为64
 */
#define MATRIX_ROW                  1

/* 事件标志矩阵一维数组元素类型 */
#define FLAG_MATRIX_ROW_TYPE        uint32_t

/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef void (*pEventCB_t)(void *pPara);                                                        //事件处理回调函数指针类型定义
typedef void *pPara_t;                                                                          //事件处理函数参数构造数据类型指针

typedef struct eventControlBlock_ {
    FLAG_MATRIX_ROW_TYPE    flagMatrix[ MATRIX_ROW ];                                           //事件标志矩阵
    pEventCB_t              CbMatrix  [ MATRIX_ROW ][ sizeof(FLAG_MATRIX_ROW_TYPE) * 8 ];       //事件处理回调函数指针矩阵
    pPara_t                 paraMatrix[ MATRIX_ROW ][ sizeof(FLAG_MATRIX_ROW_TYPE) * 8 ];       //事件参数集合数据结构指针矩阵
} ecb_t, *pEcb_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
extern void eventMatrix_EventProcess(pEcb_t pEcb);                                              //事件处理函数，轮询事件矩阵
extern bool eventMatrix_SetEventFlag(pEcb_t pEcb, int eventFlag);                               //设置事件标志
extern bool eventMatrix_GetEventFlag(pEcb_t pEcb, int eventFlag);                               //读取事件标志
extern bool eventMatrix_ClearEventFlag(pEcb_t pEcb, int eventFlag);                             //清除事件标志
extern bool eventMatrix_SaveEventPara(pEcb_t pEcb, int eventFlag, pPara_t pPara);               //保存事件参数集合数据结构指针，以供事件处理回调函数使用
extern bool eventMatrix_RegistEventHandleCB(pEcb_t pEcb, int eventFlag, pEventCB_t pCb);        //注册事件处理回调函数

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
