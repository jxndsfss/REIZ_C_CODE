/*
    @file     reiz_string.c
    @author   jxndsfss
    @version  v1.0.1
    @date     2018-12-20
    @site     ShangYouSong.SZ
    @brief    字符串相关函数实现源文件
*/

/* includes ------------------------------------------------------------------*/
#include "reiz_string.h"

/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private typedef -----------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
/* private function prototypes -----------------------------------------------*/

/*
    @brief  删除字符串中所有出现的某个字符
    @param  pDes - 被处理的字符串
    @param  c    - 需要删除的字符
    @return void
*/
void deleteCharInStr_rz(char *pDes, char c){
    char *pStore = pDes;

    while (*pDes != '\0') {
        if (*pDes != c) {
            *pStore++ = *pDes;
        }
        pDes++;
    }
    *pStore = '\0';
}

/*
    @brief  删除字符串pDes中所有在字符串pSearch中出现的字符
    @param  pDes    - 被处理的字符串
    @param  pSearch - 包含所有需要删除的字符的字符串，供搜索比较
    @return void
*/
void deleteSomeCharInStr_rz(char *pDes, char *pSearch){
    char *pStore = pDes, *pModify;

    while (*pDes != '\0') {
        pModify = pSearch;

        while (*pModify != '\0') {
            if (*pDes == *pModify) {
                break;
            }
            pModify++;
        }

        if (*pModify != '\0') {
            pDes++;
            continue;
        }
        *pStore++ = *pDes++;
    }
    *pStore = '\0';
}

/*
    @brief  将字符串中出现的小写字母转成大写字母
    @param  pStr - 字符串指针
    @return void
*/
void strToUpper_rz(char *pStr){
    while (*pStr != '\0') {
        if (*pStr >= 'a' && *pStr <= 'z') {
            *pStr -= 'a' - 'A';
        }
        pStr++;
    }
}

/*
    @brief  将字符串中出现的大写字母转成小写字母
    @param  pStr - 字符串指针
    @return void
*/
void strToLower_rz(char *pStr){
    while (*pStr != '\0') {
        if (*pStr >= 'A' && *pStr <= 'Z') {
            *pStr += 'a' - 'A';
        }
        pStr++;
    }
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2018 REIZ ***************************/
