/*******************************************************************************
 *  @file       reiz_protocolParse.h
 *  @author     jxndsfss
 *  @version    v1.0.1
 *  @date       2019-01-11
 *  @site       ShangYouSong.SZ
 *  @brief      字符串协议解析模块头文件
 *******************************************************************************
 */

/*******************************************************************************
 *  @algorithm  cmdType--> flagMatrix--> cbMatrix--> paraMatrix
 *              命令类型--> 标志矩阵--> 回调函数矩阵--> 参数矩阵
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef REIZ_PROTOCOL_PARSE_H
#define REIZ_PROTOCOL_PARSE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported define -----------------------------------------------------------*/

/*
    命令标志矩阵行（一维数组）元素类型，一行有 MATRIX_COL 个标志
    在APP中定义 MATRIX_ROW，根据指令类型数量定义矩阵行数
    总指令类型数量为 MATRIX_ROW * MATRIX_COL，可根据应用需求无限扩增！
*/
#define FLAG_MATRIX_ROW_TYPE        uint32_t

/*
    命令类型矩阵列数，设计成矩阵的目的是为了能够无限增加指令的数量！
*/
#define MATRIX_COL                  (sizeof( FLAG_MATRIX_ROW_TYPE ) * 8)

/* Exported macro ------------------------------------------------------------*/

/*
    协议解析对象宏类型定义
    使用方法：
    #define MATRIX_ROW  2
    PROTOCOL_PARSE_OJB(MATRIX_ROW) xxxProtocolParseOjb;
*/
#define PROTOCOL_PARSE_OJB(row)     struct {                                        \
                                        FLAG_MATRIX_ROW_TYPE    flagMatrix[row];    \
                                        cbMatrixRowArray_t      cbMatrix[row];      \
                                        paraMatrixRowArray_t    paraMatrix[row];    \
                                    }

/* Exported types ------------------------------------------------------------*/

typedef void (*pCmdCB_t)(void *pPara);                                          //协议命令处理回调函数指针类型定义
typedef void *pPara_t;                                                          //协议命令处理函数参数指针类型定义

typedef struct cmdTypeElement_ {                                                //协议命令类型元素数据类型定义
    int     cmdType;                                                            //协议命令类型
    char    *pCmdTypeStr;                                                       //协议命令类型字符串指针
} cmdTypeElement_t;

typedef cmdTypeElement_t (*pCmdTypeEleArr_t)[];                                 //协议命令类型字符串指针数组指针类型定义（一维数组指针）
typedef FLAG_MATRIX_ROW_TYPE (*pFlagMatrix_t)[];                                //协议命令收到标志矩阵数组指针类型定义（一维数组指针）
typedef pCmdCB_t cbMatrixRowArray_t[MATRIX_COL];                                //协议命令回调函数指针矩阵行元素数组类型定义（大小为sizeof(FLAG_MATRIX_ROW_TYPE)*8的函数指针数组）
typedef cbMatrixRowArray_t (*pCbMatrix_t)[];                                    //协议命令回调函数指针矩阵数组指针类型定义（二维数组指针）
typedef pPara_t paraMatrixRowArray_t[MATRIX_COL];                               //协议命令参数指针矩阵行元素数组类型定义（大小为sizeof(FLAG_MATRIX_ROW_TYPE)*8的指针数组）
typedef paraMatrixRowArray_t (*pParaMatrix_t)[];                                //协议命令参数指针矩阵数组指针类型定义（二维数组指针）

typedef struct cmdParseControlBlock_ {                                          //协议命令解析控制块数据类型定义
    int                     matrixRow;                                          //协议命令类型矩阵行数
    pCmdTypeEleArr_t        pCmdTypeEleArr;                                     //协议命令类型字符串指针数组指针
    pFlagMatrix_t           pFlagMatrix;                                        //协议命令标志矩阵数组指针
    pCbMatrix_t             pCbMatrix;                                          //协议命令处理回调函数指针矩阵指针
    pParaMatrix_t           pParaMatrix;                                        //协议命令参数集合数据结构指针矩阵
} cpcb_t, *pCpcb_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

extern void cpcbInit(   pCpcb_t             pCpcb,                              //协议命令解析控制块初始化
                        int                 matrixRow,
                        pCmdTypeEleArr_t    pCmdTypeEleArr,
                        pFlagMatrix_t       pFlagMatrix,
                        pCbMatrix_t         pCbMatrix,
                        pParaMatrix_t       pParaMatrix);

extern bool registerCmdCB(pCpcb_t pCpcb, int cmdType, pCmdCB_t pCmdCb);         //注册协议命令回调函数
extern bool cmdTypeParse(pCpcb_t pCpcb, char *pCmdStr);                         //协议命令类型解析函数
extern void cmdProcess(pCpcb_t pCpcb);                                          //命令回调函数执行函数(事件处理函数)

#ifdef __cplusplus
}
#endif

#endif /* REIZ_PROTOCOL_PARSE_H */

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
