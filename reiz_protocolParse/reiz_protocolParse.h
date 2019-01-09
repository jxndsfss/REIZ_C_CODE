/*******************************************************************************
 *  @file       reiz_protocolParse.h
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-10
 *  @site       ShangYouSong.SZ
 *  @brief      字符串协议解析模块头文件
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
    命令类型矩阵列数，设计成矩阵的目的是为了能够无限增加指令的数量，解析的指令类型再多也不怕！ 
*/
#define MATRIX_COL                  (sizeof( FLAG_MATRIX_ROW_TYPE ) * 8)

/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

typedef void (*pCmdCB_t)(void *pPara);                                          //协议命令处理回调函数指针类型定义
typedef void *pPara_t;                                                          //协议命令处理函数参数指针类型定义

typedef struct cmdTypeElement_ {                                                //协议命令类型元素数据类型定义
    int     cmdType;                                                            //协议命令类型
    char    *pCmdTypeStr;                                                       //协议命令类型字符串指针
} cmdTypeElement_t;

typedef cmdTypeElement_t (*pCmdTypeEleArr_t)[];                                 //协议命令类型字符串指针数组指针类型定义（一维数组指针）
typedef FLAG_MATRIX_ROW_TYPE (*pFlagMatrix_t)[];                                //协议指令收到标志矩阵数组指针类型定义（一维数组指针）
typedef pCmdCB_t cbMatrixRowArray_t[MATRIX_COL];                                //协议指令回调函数指针矩阵行元素数组类型定义（大小为sizeof(FLAG_MATRIX_ROW_TYPE)*8的函数指针数组）
typedef cbMatrixRowArray_t (*pCbMatrix_t)[];                                    //协议指令回调函数指针矩阵数组指针类型定义（二维数组指针）
typedef pPara_t paraMatrixRowArray_t[MATRIX_COL];                               //协议指令参数指针矩阵行元素数组类型定义（大小为sizeof(FLAG_MATRIX_ROW_TYPE)*8的指针数组）
typedef paraMatrixRowArray_t (*pParaMatrix_t)[];                                //协议指令参数指针矩阵数组指针类型定义（二维数组指针）

typedef struct cmdParseControlBlock_ {                                          //协议命令解析控制块数据类型定义
    int                     matrixRow;                                          //协议命令类型矩阵行数
    pFlagMatrix_t           pFlagMatrix;                                        //协议命令标志矩阵数组指针
    pCmdTypeEleArr_t        pCmdTypeEleArr;                                     //协议命令类型字符串指针数组指针
    pCbMatrix_t             pCbMatrix;                                          //协议命令处理回调函数指针矩阵指针
    pParaMatrix_t           pParaMatrix;                                        //协议命令参数集合数据结构指针矩阵
} cpcb_t, *pCpcb_t;

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

extern void cmdParseControlBlockInit(   pCpcb_t             pCpcb,              //协议命令解析控制块初始化
                                        int                 matrixRow,
                                        pFlagMatrix_t       pFlagMatrix,
                                        pCmdTypeEleArr_t    pCmdTypeEleArr,
                                        pCbMatrix_t         pCbMatrix,
                                        pParaMatrix_t       pParaMatrix);

extern bool registerCmdCB(pCpcb_t pCpcb, int cmdType, pCmdCB_t pCmdCb);         //注册协议命令回调函数
extern int cmdTypeParse(pCpcb_t pCpcb, char *pCmdStr);                          //协议命令类型解析函数，返回命令类型
extern bool setCmdFlag(pCpcb_t pCpcb, int cmdType, pPara_t pCmdData);           //设置协议命令类型标志，命令收到时设置，传递命令数据包（或命令参数集合数据类型）指针，以供命令解析函数使用
extern void cmdProcess(pCpcb_t pCpcb);                                          //命令回调函数执行函数(事件处理函数)

#ifdef __cplusplus
}
#endif

#endif /* REIZ_PROTOCOL_PARSE_H */

/*******************************************************************************
@usecase
--------------------------------------------------------------------------------
@parseModule

//命令类型矩阵行数，所能表示总协议命令数为 MATRIX_ROW * sizeof(FLAG_MATRIX_ROW_TYPE) * 8，
//命令类型矩阵矩阵用于命令类型较多的场合，举例若 FLAG_MATRIX_ROW_TYPE 为 uint32_t，
//则命令类型总数为MATRIX_ROW * 32，若MATRIX_ROW为2，则命令类型总数为64
#define MATRIX_ROW      2

//命令类型从1开始，CMD_TYPE_NULL = 0，根据此处命令数量定义符号常量 MATRIX_ROW
#define CMD_TYPE_1      1
#define CMD_TYPE_2      2
#define CMD_TYPE_3      3
#define CMD_TYPE_4      4
//...

static cpcb_t cmdParseCtrBlk;
extern pCpcb_t pCpcb = &cmdParseCtrBlk;
static FLAG_MATRIX_ROW_TYPE flagArr[MATRIX_ROW] = {0};
static cbMatrixRowArray_t cbArr[MATRIX_ROW] = {0};
static paraMatrixRowArray_t paraArr[MATRIX_ROW] = {0};

//定义命令类型及命令类型字符串
static cmdTypeElement_t cmdTypeEleArr[] = {
    {   CMD_TYPE_1      ,   "GetBatteryLevel"   },
    {   CMD_TYPE_2      ,   "GetSignalStrength" },
    {   CMD_TYPE_3      ,   "SetDeviceName"     },
    {   CMD_TYPE_4      ,   "GetTemperature"    },
    {   0               ,   0                   }
}

//APP编写的函数
//命令解析处理回调函数
static void cmd_1_ParseProcessCB(char *pCmdData) {
    //...
}

static void cmd_2_ParseProcessCB(char *pCmdData) {
    //...
}

static void cmd_3_ParseProcessCB(char *pCmdData) {
    //...
}

static void cmd_4_ParseProcessCB(char *pCmdData) {
    //...
}

static void registerAllCmdCB(void) {
    registerCmdCB(pCpcb, CMD_TYPE_1, cmd_1_ParseProcessCB);
    registerCmdCB(pCpcb, CMD_TYPE_2, cmd_2_ParseProcessCB);
    registerCmdCB(pCpcb, CMD_TYPE_3, cmd_3_ParseProcessCB);
    registerCmdCB(pCpcb, CMD_TYPE_4, cmd_4_ParseProcessCB);
}
--------------------------------------------------------------------------------
@parseModule API

extern void cpcbInit(void) {
    cmdParseControlBlockInit( pCpcb, MATRIX_ROW, cmdTypeEleArr, flagArr, cbArr, paraArr);
    registerAllCmdCB();
}
--------------------------------------------------------------------------------
//app task1 - parseCmd

cpcbInit();

char *pTestStr = "CMD GetBatteryLevel\r\n";
int cmdType = cmdTypeParse(pCpcb, pTestStr);
char *pPara = malloc(strlen(pTestStr)+1);

strcpy(pPara, pTestStr);
setCmdFlag(pCpcb, cmdType, pPara);
--------------------------------------------------------------------------------
//app task2 - ProcessCmd

cmdProcess(pCpcb);
--------------------------------------------------------------------------------

*/

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
