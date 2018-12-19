/*
    @file     reiz_string.h
    @author   jxndsfss
    @version  v1.0.1
    @date     2018-12-20
    @site     ShangYouSong.SZ
    @brief    字符串相关函数实现头文件
*/

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef REIZ_STRING_H
#define REIZ_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

/* includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* exported define -----------------------------------------------------------*/
/* exported macro ------------------------------------------------------------*/
/* exported types ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/

/* exported functions prototypes ---------------------------------------------*/
extern void deleteCharInStr_rz(char *pDes, char c);
extern void deleteSomeCharInStr_rz(char *pDes, char *pSearch);
extern void strToUpper_rz(char *pStr);
extern void strToLower_rz(char *pStr);

#ifdef __cplusplus
}
#endif

#endif /* REIZ_STRING_H */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2018 REIZ ***************************/
