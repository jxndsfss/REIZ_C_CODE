/*
    @file       reiz_eventMatrix.c
    @author     jxndsfss
    @version    v1.0.0
    @date       2018-12-25
    @site       ShangYouSong.SZ
    @brief      事件矩阵实现源文件
*/

/*----- includes -------------------------------------------------------------*/
#include "reiz_eventMatrix.h"

/*----- private define -------------------------------------------------------*/
/*----- private macro --------------------------------------------------------*/
/*----- private typedef ------------------------------------------------------*/

/*----- private variables ----------------------------------------------------*/
uint32_t eventMatrix[MATRIX_ROW] = {0};

/*----- exported variables ---------------------------------------------------*/
/*----- private function prototypes ------------------------------------------*/

/*
    @brief  读取事件标志
    @param  row - 事件所在矩阵行数
    @param  col - 事件所在矩阵列数
    @return true  - 事件存在
    @return false - 事件不存在
*/
extern bool getEventFlag(int row, int col) {
    return eventMatrix[row] & (1 << col);
}

/*
    @brief  设置事件标志
    @param  row - 事件所在矩阵行数
    @param  col - 事件所在矩阵列数
    @return void
*/
extern void setEventFlag(int row, int col) {
    eventMatrix[row] |= (1 << col);
}

/*
    @brief  清除事件标志
    @param  row - 事件所在矩阵行数
    @param  col - 事件所在矩阵列数
    @return void
*/
extern void clearEventFlag(int row, int col) {
    eventMatrix[row] &= ~(1 << col);
}

/*
    @brief  事件矩阵是否为空，即有无事件存在
    @param  void
    @return true  - 矩阵为空，无事件存在
    @return false - 矩阵不为空，有事件存在
*/
extern bool eventMatrixIsEmpty(void) {
    for (int i = 0; i < MATRIX_ROW; i++) {
        if (eventMatrix[i])
            return false;
    }
    return true;
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2018 REIZ ***************************/
