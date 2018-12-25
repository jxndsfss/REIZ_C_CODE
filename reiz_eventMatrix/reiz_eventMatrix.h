/*
    @file       reiz_eventMatrix.h
    @author     jxndsfss
    @version    v1.0.0
    @date       2018-12-25
    @site       ShangYouSong.SZ
    @brief      事件矩阵实现头文件

    @usecase    宏作为函数的参数，宏中包含矩阵位置的行与列参数
                getEventFlag(EVENT_1_FLAG);
                setEventFlag(EVENT_4_FLAG);
                clearEventFlag(EVENT_31_FLAG);
*/

/*----- define to prevent recursive inclusion --------------------------------*/
#ifndef REIZ_EVENT_MATRIX_H
#define REIZ_EVENT_MATRIX_H

#ifdef __cplusplus
extern "C"
{
#endif

/*----- includes -------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*----- exported define ------------------------------------------------------*/

/* 事件矩阵行数，所能表示总事件数为 MATRIX_ROW*32，事件矩阵用于事件较多的场合 */
#define MATRIX_ROW          3                   

/* 注：事件矩阵列数COL最大为31 */
#define EVENT_0_FLAG        0, 0
#define EVENT_1_FLAG        0, 1
#define EVENT_2_FLAG        0, 2
#define EVENT_3_FLAG        1, 3
#define EVENT_4_FLAG        1, 4
#define EVENT_5_FLAG        1, 5
#define EVENT_29_FLAG       2, 29
#define EVENT_30_FLAG       2, 30
#define EVENT_31_FLAG       2, 31

/*----- exported macro -------------------------------------------------------*/
/*----- exported types -------------------------------------------------------*/
/*----- exported variables ---------------------------------------------------*/

/*----- exported functions prototypes ----------------------------------------*/
extern bool getEventFlag(int row, int col);                 //读取事件标志
extern void setEventFlag(int row, int col);                 //设置事件标志
extern void clearEventFlag(int row, int col);               //清除事件标志
extern bool eventMatrixIsEmpty(void);                       //事件矩阵是否为空

#ifdef __cplusplus
}
#endif

#endif /* REIZ_EVENT_MATRIX_H */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2018 REIZ ***************************/
