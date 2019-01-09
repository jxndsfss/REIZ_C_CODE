/*******************************************************************************
 *  @file       reiz_protocolParseUseCase.c
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-10
 *  @site       ShangYouSong.SZ
 *  @brief      字符串协议解析模块使用例程
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_protocolParse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private define ------------------------------------------------------------*/

//命令类型矩阵行数，所能表示总协议命令数为 MATRIX_ROW * sizeof(FLAG_MATRIX_ROW_TYPE) * 8，
//命令类型矩阵矩阵用于命令类型较多的场合，举例若 FLAG_MATRIX_ROW_TYPE 为 uint32_t，
//则命令类型总数为MATRIX_ROW * 32，若MATRIX_ROW为2，则命令类型总数为64
#define MATRIX_ROW                      1

//命令类型从1开始，CMD_TYPE_NULL = 0，根据此处命令数量定义符号常量 MATRIX_ROW
#define CMD_TYPE_GET_BATTERY_LEVEL      1           //获取电量
#define CMD_TYPE_GET_SIGNAL_STRENGTH    2           //获取信号强度
#define CMD_TYPE_SET_DEVICE_NAME        3           //设置设备名
#define CMD_TYPE_GET_TEMPERATURE        4           //获取温度值

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//定义命令类型及命令类型字符串
static cmdTypeElement_t cmdTypeEleArr[] = {
    { CMD_TYPE_GET_BATTERY_LEVEL    ,   "GetBatteryLevel"   },
    { CMD_TYPE_GET_SIGNAL_STRENGTH  ,   "GetSignalStrength" },
    { CMD_TYPE_SET_DEVICE_NAME      ,   "SetDeviceName"     },
    { CMD_TYPE_GET_TEMPERATURE      ,   "GetTemperature"    },
    { 0                             ,   0                   }
};

static cpcb_t cmdParseCtrBlk;
static FLAG_MATRIX_ROW_TYPE flagArr[MATRIX_ROW];
static cbMatrixRowArray_t cbArr[MATRIX_ROW];
static paraMatrixRowArray_t paraArr[MATRIX_ROW];

/* Exported variables --------------------------------------------------------*/

extern pCpcb_t pCpcb = &cmdParseCtrBlk;

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
 *  @brief  获取电量回调函数
 *  @param  pPara - 命令参数数据类型指针
 *  @return void
 */
static void GetBatteryLevel_ProcesCB(void *pPara) {
    int bat = 93;
    printf("battery level is: %d \n", bat);
}

/*******************************************************************************
 *  @brief  获取信号强度回调函数
 *  @param  pPara - 命令参数数据类型指针
 *  @return void
 */
static void GetSignalStrength_ProcesCB(void *pPara) {
    int signal = 98;
    printf("signal strength is: %d \n", signal);
}

/*******************************************************************************
 *  @brief  设置设备名回调函数
 *  @param  pPara - 命令参数数据类型指针
 *  @return void
 */
static void SetDeviceName_ProcesCB(void *pPara) {
    char *p1, *p2;

    p1 = strstr((char const*)pPara, cmdTypeEleArr[CMD_TYPE_SET_DEVICE_NAME - 1].pCmdTypeStr);
    p1 = strstr(p1, " ");
    p1++;

    p2 = strstr((char const*)pPara, "\r\n");
    *p2 = '\0';

    printf("Device Name is: %s \n", p1);
}

/*******************************************************************************
 *  @brief  获取温度值回调函数
 *  @param  pPara - 命令参数数据类型指针
 *  @return void
 */
static void GetTemperature_ProcesCB(void *pPara) {
    float temp = 36.5;
    printf("Temperature is: %.1f \n", temp);
}

/*******************************************************************************
 *  @brief  注册协议命令回调函数
 *  @param  void
 *  @return void
 */
static void registerAllCmdCB(void) {
    registerCmdCB(pCpcb, CMD_TYPE_GET_BATTERY_LEVEL, GetBatteryLevel_ProcesCB);
    registerCmdCB(pCpcb, CMD_TYPE_GET_SIGNAL_STRENGTH, GetSignalStrength_ProcesCB);
    registerCmdCB(pCpcb, CMD_TYPE_SET_DEVICE_NAME, SetDeviceName_ProcesCB);
    registerCmdCB(pCpcb, CMD_TYPE_GET_TEMPERATURE, GetTemperature_ProcesCB);
}

/*******************************************************************************
*  @brief   命令解析控制块初始化
*  @param   void
*  @return  void
*/
extern void cpcbInit(void) {
    cmdParseControlBlockInit(   pCpcb, 
                                MATRIX_ROW, 
                                (pFlagMatrix_t)flagArr,
                                (pCmdTypeEleArr_t)cmdTypeEleArr,
                                (pCbMatrix_t)cbArr,
                                (pParaMatrix_t)paraArr);
    registerAllCmdCB();
}

/*******************************************************************************
 *  @brief  字符串协议解析测试函数
 *  @param  void
 *  @return void
 */
extern void protocolParseTest(void) {
    int cmdType;
    char *pPara;

    //命令解析控制块初始化
    cpcbInit();

    //测试命令字符串
    char *pTestStr1 = "CMD GetBatteryLevel\r\n";
    char *pTestStr2 = "CMD GetSignalStrength\r\n";
    char *pTestStr3 = "CMD SetDeviceName OnePlus 3T\r\n";
    char *pTestStr4 = "CMD GetTemperature\r\n";

    //解析命令类型
    cmdType = cmdTypeParse(pCpcb, pTestStr1);
    setCmdFlag(pCpcb, cmdType, NULL);

    //解析命令类型
    cmdType = cmdTypeParse(pCpcb, pTestStr2);
    setCmdFlag(pCpcb, cmdType, NULL);

    //解析命令类型，传递参数
    cmdType = cmdTypeParse(pCpcb, pTestStr3);
    pPara = malloc(strlen(pTestStr3) + 1);
    if (pPara) {
        memcpy(pPara, pTestStr3, strlen(pTestStr3) + 1);
    }
    setCmdFlag(pCpcb, cmdType, pPara);

    //解析命令类型
    cmdType = cmdTypeParse(pCpcb, pTestStr4);
    setCmdFlag(pCpcb, cmdType, NULL);

    //命令处理
    cmdProcess(pCpcb);
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
