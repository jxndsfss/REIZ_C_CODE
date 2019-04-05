/*******************************************************************************
 *  @file       reiz_strCmdParseUseCase.c
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-12
 *  @site       ShangYouSong.SZ
 *  @brief      字符串协议解析模块使用例程
 *******************************************************************************
 *  使用方法：
 *  1.根据MCU位数修改 reiz_strCmdParse.h 中的宏 FLAG_MATRIX_ROW_TYPE，使其为unsigned int类型
 *      例：#define FLAG_MATRIX_ROW_TYPE          uint32_t
 *  2.根据命令类型数量 n 修改宏值 MATRIX_ROW，使 MATRIX_ROW * MATRIX_COL >= n
 *      例：#define MATRIX_ROW                    1
 *  3.完善命令类型宏，宏值从1开始递增
 *      例：#define CMD_TYPE_GET_BATTERY_LEVEL    1
 *  4.建立和完善命令类型元素数组 cmdTypeElement_t，保证最后一行为{0, 0}
 *      例：static cmdTypeElement_t cmdTypeEleArr[] = {}
 *  5.定义 cpcb_t 命令解析控制块变量和 PROTOCOL_PARSE_OJB(MATRIX_ROW) 协议对象变量
 *      例：static cpcb_t cpcb;                     
 *      例：static STR_CMD_PARSE_OJB(MATRIX_ROW) cmdParseObj;
 *  6.注册各命令回调函数 registerCmdCB();
 *  7.初始化命令解析控制块 cpcbInit();
 *  8.APP中执行命令解析和命令处理
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_strCmdParse.h"
#include "reiz_strCmdParseUseCase.h"
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

static cpcb_t cpcb;                                                             //协议解析控制块变量定义
static STR_CMD_PARSE_OJB(MATRIX_ROW) cmdParseObj;                               //协议命令解析对象变量定义

//定义命令类型及命令类型字符串
static cmdTypeElement_t cmdTypeEleArr[] = {
    { CMD_TYPE_GET_BATTERY_LEVEL    ,   "GetBatteryLevel"   },
    { CMD_TYPE_GET_SIGNAL_STRENGTH  ,   "GetSignalStrength" },
    { CMD_TYPE_SET_DEVICE_NAME      ,   "SetDeviceName"     },
    { CMD_TYPE_GET_TEMPERATURE      ,   "GetTemperature"    },
    { 0                             ,   0                   }                   //此行必须有，作为数组结束行！
};

/* Exported variables --------------------------------------------------------*/

extern pCpcb_t pCpcb = &cpcb;

/* Private function prototypes -----------------------------------------------*/
static void registerAllCmdCB(void);
static void GetBatteryLevel_ProcesCB(void *pPara);
static void GetSignalStrength_ProcesCB(void *pPara);
static void SetDeviceName_ProcesCB(void *pPara);
static void GetTemperature_ProcesCB(void *pPara);

/*******************************************************************************
*  @brief  注册协议命令回调函数
*  @param  void
*  @return void
*/
static void registerAllCmdCB(void) {
    strCmdParse_registerCmdCB(pCpcb, CMD_TYPE_GET_BATTERY_LEVEL, GetBatteryLevel_ProcesCB);
    strCmdParse_registerCmdCB(pCpcb, CMD_TYPE_GET_SIGNAL_STRENGTH, GetSignalStrength_ProcesCB);
    strCmdParse_registerCmdCB(pCpcb, CMD_TYPE_SET_DEVICE_NAME, SetDeviceName_ProcesCB);
    strCmdParse_registerCmdCB(pCpcb, CMD_TYPE_GET_TEMPERATURE, GetTemperature_ProcesCB);
}

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
*  @brief   命令解析初始化
*  @param   void
*  @return  void
*/
extern void cmdParseInit(void) {
    strCmdParse_cpcbInit(   pCpcb,
                            MATRIX_ROW, 
                            (pCmdTypeEleArr_t)cmdTypeEleArr,
                            (pFlagMatrix_t)cmdParseObj.flagMatrix,
                            (pCbMatrix_t)cmdParseObj.cbMatrix,
                            (pParaMatrix_t)cmdParseObj.paraMatrix);
    registerAllCmdCB();
}

/*******************************************************************************
 *  @brief  字符串协议命令解析测试函数
 *  @param  void
 *  @return void
 */
extern void strCmdParseTest(void) {

    //命令解析初始化
    cmdParseInit();

    //测试命令字符串
    char *pTestStr1 = "CMD GetBatteryLevel\r\n";
    char *pTestStr2 = "CMD GetSignalStrength\r\n";
    char *pTestStr3 = "CMD SetDeviceName OnePlus 3T\r\n";
    char *pTestStr4 = "CMD GetTemperature\r\n";

    //解析命令类型
    strCmdParse_cmdTypeParse(pCpcb, pTestStr1);

    //解析命令类型
    strCmdParse_cmdTypeParse(pCpcb, pTestStr2);

    //解析命令类型
    strCmdParse_cmdTypeParse(pCpcb, pTestStr3);

    //解析命令类型
    strCmdParse_cmdTypeParse(pCpcb, pTestStr4);

    //命令处理
    strCmdParse_cmdProcess(pCpcb);
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
