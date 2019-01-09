/*******************************************************************************
 *  @file       reiz_protocolParse.c
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-10
 *  @site       ShangYouSong.SZ
 *  @brief      字符串协议解析模块源文件
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_protocolParse.h"
#include <string.h>
#include <stdlib.h>

/* Private define ------------------------------------------------------------*/

#define CMD_TYPE_NULL                   0                                       //无效空命令类型

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
 *  @brief  协议命令解析控制块初始化
 *  @param  pCpcb            - 命令解析控制块指针
 *          matrixRow        - 命令类型矩阵行数
 *          pFlagMatrix      - 命令收到标志矩阵数组指针
 *          pCmdTypeEleArr   - 命令类型元素数组指针
 *          pCbMatrix        - 命令处理回调函数指针矩阵数组指针
 *          pParaMatrix      - 命令处理回调函数参数指针矩阵数组指针
 *  @return void
 */
extern void cmdParseControlBlockInit(   pCpcb_t             pCpcb,
                                        int                 matrixRow,
                                        pFlagMatrix_t       pFlagMatrix,
                                        pCmdTypeEleArr_t    pCmdTypeEleArr,
                                        pCbMatrix_t         pCbMatrix,
                                        pParaMatrix_t       pParaMatrix)
{
    pCpcb->matrixRow        =   matrixRow;
    pCpcb->pFlagMatrix      =   pFlagMatrix;
    pCpcb->pCmdTypeEleArr   =   pCmdTypeEleArr;
    pCpcb->pCbMatrix        =   pCbMatrix;
    pCpcb->pParaMatrix      =   pParaMatrix;
}

/*******************************************************************************
 *  @brief  注册协议命令回调函数
 *  @param  pCpcb   - 命令解析控制块指针
 *          cmdType - 命令类型
 *          pCmdCb  - 命令解析处理回调函数
 *  @return true    - 注册成功
 *          false   - 注册失败
 */
extern bool registerCmdCB(pCpcb_t pCpcb, int cmdType, pCmdCB_t pCmdCb) {
    int row, col;

    if (pCpcb != NULL && cmdType != CMD_TYPE_NULL) {
        row = cmdType / MATRIX_COL;
        col = cmdType % MATRIX_COL;
        (*pCpcb->pCbMatrix)[row][col] = pCmdCb;
        return true;
    }
    return false;
}

/*******************************************************************************
 *  @brief  协议命令类型解析函数，返回命令类型
 *  @param  pCpcb         - 命令解析控制块指针
 *          pCmdStr       - 命令数据包字符串指针
 *  @return CMD_TYPE_NULL - 无匹配命令类型
 *          cmdType       - 匹配到的命令类型
 */
extern int cmdTypeParse(pCpcb_t pCpcb, char *pCmdStr) {
    int i = 0;

    if (pCpcb != NULL && pCmdStr != NULL) {
        while ((*pCpcb->pCmdTypeEleArr)[i].cmdType != CMD_TYPE_NULL) {
            if (strstr(pCmdStr, (*pCpcb->pCmdTypeEleArr)[i].pCmdTypeStr)) {
                return (*pCpcb->pCmdTypeEleArr)[i].cmdType;                     //返回匹配成功的指令类型
            }
            i++;
        }
    }
    return CMD_TYPE_NULL;
}

/*******************************************************************************
 *  @brief  设置协议命令类型标志，命令收到时设置，传递命令数据包（或命令参数集合数据类型）指针，以供命令解析函数使用
 *  @param  pCpcb    - 命令解析控制块指针
 *          cmdType  - 协议命令类型
 *          pCmdData - 协议命令参数指针
 *  @return true     - 设置成功
 *          false    - 设置失败
 */
extern bool setCmdFlag(pCpcb_t pCpcb, int cmdType, pPara_t pCmdData) {
    int row, col;

    if (pCpcb != NULL && cmdType != CMD_TYPE_NULL) {
        row = cmdType / MATRIX_COL;
        col = cmdType % MATRIX_COL;
        (*pCpcb->pParaMatrix)[row][col] = pCmdData;                             //存储参数指针
        (*pCpcb->pFlagMatrix)[row] |= ((FLAG_MATRIX_ROW_TYPE)1 << col);         //设置命令类型标志，表示收到该条命令
        return true;
    }
    return false;
}

/*******************************************************************************
 *  @brief  命令回调函数执行函数(事件处理函数)
 *  @param  pCpcb - 命令解析控制块指针
 *  @return void
 */
extern void cmdProcess(pCpcb_t pCpcb) {
    int         row, col, set;
    pCmdCB_t    pCb;
    pPara_t     pPara;

    if (pCpcb == NULL) {
        return;
    }

    for (row = 0; row < pCpcb->matrixRow; row++) {
        if ((*pCpcb->pFlagMatrix)[row] != 0) {                                          //命令标志矩阵行不为0
            for (col = 0; col < MATRIX_COL; col++) {
                set = (*pCpcb->pFlagMatrix)[row] & ((FLAG_MATRIX_ROW_TYPE)1 << col);    //获取命令类型标志位
                if (set) {                                                              //命令类型标志位存在
                    pCb = (pCmdCB_t)(*pCpcb->pCbMatrix)[row][col];
                    pPara = (*pCpcb->pParaMatrix)[row][col];

                    pCb(pPara);                                                         //执行命令回调函数

                    if (pPara != NULL) {                                                //参数内存未释放则释放参数内存
                        free(pPara);
                        (*pCpcb->pParaMatrix)[row][col] = NULL;                         //清零参数指针
                    }
                    (*pCpcb->pFlagMatrix)[row] &= ~((FLAG_MATRIX_ROW_TYPE)1 << col);    //删除命令类型标志位
                }
            }
        }
    }
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
