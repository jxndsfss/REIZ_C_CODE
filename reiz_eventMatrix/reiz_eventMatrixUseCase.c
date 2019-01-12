/*******************************************************************************
 *  @file       reiz_eventMatrixUseCase.c
 *  @author     jxndsfss
 *  @version    v1.0.0
 *  @date       2019-01-12
 *  @site       ShangYouSong.SZ
 *  @brief      事件矩阵模块使用例程源文件
 *******************************************************************************
 *  使用方法：
 *  1.根据MCU位数修改 reiz_eventMatrix.h 中的宏 EVENT_FLAG_MATRIX_ROW_TYPE，使其为unsigned int类型
 *      例：#define EVENT_FLAG_MATRIX_ROW_TYPE    uint32_t
 *  2.根据事件数量 n 定义宏值 EVENT_MATRIX_ROW，使 EVENT_MATRIX_ROW * MATRIX_COL >= n
 *      例：#define EVENT_MATRIX_ROW      1
 *  3.完善事件类型宏，宏值从0开始递增
 *      例：#define PRINT_0_EVENT_FLAG    1
 *  4.定义 ecb_t 事件控制块变量和 EVENT_PROCESS_OBJ(EVENT_MATRIX_ROW) 事件处理对象变量
 *      例：static ecb_t ecb_t ecb;                    
 *      例：static EVENT_PROCESS_OBJ(MATRIX_ROW) evProcessObj;
 *  5.注册各事件回调函数 eventMatrix_RegistEvCB();
 *  6.初始化事件控制块 eventMatrix_ecbInit();
 *  7.APP中执行设置事件标志和事件处理
 */

/* Includes ------------------------------------------------------------------*/
#include "reiz_eventMatrix.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/

/* 
 * 事件矩阵行数，所能表示总事件数为 EVENT_MATRIX_ROW * sizeof(EVENT_FLAG_MATRIX_ROW_TYPE) * 8，
 * 事件矩阵用于事件较多的场合，举例若 EVENT_FLAG_MATRIX_ROW_TYPE 为 uint32_t，
 * 则事件总数为 EVENT_MATRIX_ROW * 32，若 EVENT_MATRIX_ROW 为2，则事件总数为64
 */
#define EVENT_MATRIX_ROW        1

//事件标志符号常量定义
#define PRINT_0_EVENT_FLAG      0
#define PRINT_1_EVENT_FLAG      1
#define PRINT_2_EVENT_FLAG      2
#define PRINT_3_EVENT_FLAG      3
#define PRINT_4_EVENT_FLAG      4
#define PRINT_5_EVENT_FLAG      5
#define PRINT_6_EVENT_FLAG      6
#define PRINT_7_EVENT_FLAG      7
#define PRINT_8_EVENT_FLAG      8
#define PRINT_9_EVENT_FLAG      9
#define PRINT_10_EVENT_FLAG     10
#define PRINT_11_EVENT_FLAG     11
#define PRINT_12_EVENT_FLAG     12
#define PRINT_13_EVENT_FLAG     13
#define PRINT_14_EVENT_FLAG     14
#define PRINT_15_EVENT_FLAG     15
#define PRINT_16_EVENT_FLAG     16
#define PRINT_17_EVENT_FLAG     17
#define PRINT_18_EVENT_FLAG     18
#define PRINT_19_EVENT_FLAG     19

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static ecb_t ecb;                                                               //事件控制块变量定义
static EVENT_PROCESS_OBJ(EVENT_MATRIX_ROW) evProcessObj;                        //事件处理对象变量定义

/* Exported variables --------------------------------------------------------*/

extern pEcb_t  pEcb = &ecb;                                                     //事件控制块指针变量定义

/* Private function prototypes -----------------------------------------------*/

//事件回调函数声明
static void registAllEventHandleCB(void);
static void event_0_handleCb(void *pPara);
static void event_1_handleCb(void *pPara);
static void event_2_handleCb(void *pPara);
static void event_3_handleCb(void *pPara);
static void event_4_handleCb(void *pPara);
static void event_5_handleCb(void *pPara);
static void event_6_handleCb(void *pPara);
static void event_7_handleCb(void *pPara);
static void event_8_handleCb(void *pPara);
static void event_9_handleCb(void *pPara);
static void event_10_handleCb(void *pPara);
static void event_11_handleCb(void *pPara);
static void event_12_handleCb(void *pPara);
static void event_13_handleCb(void *pPara);
static void event_14_handleCb(void *pPara);
static void event_15_handleCb(void *pPara);
static void event_16_handleCb(void *pPara);
static void event_17_handleCb(void *pPara);
static void event_18_handleCb(void *pPara);
static void event_19_handleCb(void *pPara);


static void registAllEventHandleCB(void) {
    eventMatrix_RegistEvCB(pEcb, PRINT_0_EVENT_FLAG, event_0_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_1_EVENT_FLAG, event_1_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_2_EVENT_FLAG, event_2_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_3_EVENT_FLAG, event_3_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_4_EVENT_FLAG, event_4_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_5_EVENT_FLAG, event_5_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_6_EVENT_FLAG, event_6_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_7_EVENT_FLAG, event_7_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_8_EVENT_FLAG, event_8_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_9_EVENT_FLAG, event_9_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_10_EVENT_FLAG, event_10_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_11_EVENT_FLAG, event_11_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_12_EVENT_FLAG, event_12_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_13_EVENT_FLAG, event_13_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_14_EVENT_FLAG, event_14_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_15_EVENT_FLAG, event_15_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_16_EVENT_FLAG, event_16_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_17_EVENT_FLAG, event_17_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_18_EVENT_FLAG, event_18_handleCb);
    eventMatrix_RegistEvCB(pEcb, PRINT_19_EVENT_FLAG, event_19_handleCb);
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_0_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
	}
    printf("event 0 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_1_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 1 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_2_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 2 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_3_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 3 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_4_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 4 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_5_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 5 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_6_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 6 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_7_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 7 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_8_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 8 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_9_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 9 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_10_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 10 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_11_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 11 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_12_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 12 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_13_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 13 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_14_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 14 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_15_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 15 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_16_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 16 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_17_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 17 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_18_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 18 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  xxx事件回调函函数
 *  @param  pPara - 参数指针
 *  @return void
 */
static void event_19_handleCb(void *pPara) {
    if (pPara != NULL) {
        printf("event %d happened!\n", *(int*)pPara);
    }
    printf("event 19 callback function executed!\n\n");
}

/*******************************************************************************
 *  @brief  设置事件标志，APP中根据实际情况设置不同事件标志位
 *  @param  void
 *  @return void
 */
extern void setAllEventFlag(void) {
    int *p = NULL;

    eventMatrix_SetEventFlag(pEcb, PRINT_0_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 0;
    eventMatrix_SaveEventPara(pEcb, PRINT_0_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_1_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 1;
    eventMatrix_SaveEventPara(pEcb, PRINT_1_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_2_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 2;
    eventMatrix_SaveEventPara(pEcb, PRINT_2_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_3_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 3;
    eventMatrix_SaveEventPara(pEcb, PRINT_3_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_4_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 4;
    eventMatrix_SaveEventPara(pEcb, PRINT_4_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_5_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 5;
    eventMatrix_SaveEventPara(pEcb, PRINT_5_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_6_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 6;
    eventMatrix_SaveEventPara(pEcb, PRINT_6_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_7_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 7;
    eventMatrix_SaveEventPara(pEcb, PRINT_7_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_8_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 8;
    eventMatrix_SaveEventPara(pEcb, PRINT_8_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_9_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 9;
    eventMatrix_SaveEventPara(pEcb, PRINT_9_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_10_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 10;
    eventMatrix_SaveEventPara(pEcb, PRINT_10_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_11_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 11;
    eventMatrix_SaveEventPara(pEcb, PRINT_11_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_12_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 12;
    eventMatrix_SaveEventPara(pEcb, PRINT_12_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_13_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 13;
    eventMatrix_SaveEventPara(pEcb, PRINT_13_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_14_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 14;
    eventMatrix_SaveEventPara(pEcb, PRINT_14_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_15_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 15;
    eventMatrix_SaveEventPara(pEcb, PRINT_15_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_16_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 16;
    eventMatrix_SaveEventPara(pEcb, PRINT_16_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_17_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 17;
    eventMatrix_SaveEventPara(pEcb, PRINT_17_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_18_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 18;
    eventMatrix_SaveEventPara(pEcb, PRINT_18_EVENT_FLAG, p);

    eventMatrix_SetEventFlag(pEcb, PRINT_19_EVENT_FLAG);
    p = malloc(sizeof(int));
    *p = 19;
    eventMatrix_SaveEventPara(pEcb, PRINT_19_EVENT_FLAG, p);
}

/*******************************************************************************
 *  @brief  事件处理初始化
 *  @param  void
 *  @return void
 */
extern void evProcessInit(void) {
    eventMatrix_ecbInit(pEcb,
                        EVENT_MATRIX_ROW,
                        (pEvFlagMatrix_t)evProcessObj.evFlagMatrix,
                        (pEvCbMatrix_t)evProcessObj.evCbMatrix,
                        (pEvParaMatrix_t)evProcessObj.evParaMatrix);
    registAllEventHandleCB();
}

/*******************************************************************************
 *  @brief  事件处理测试函数
 *  @param  void
 *  @return void
 */
extern void eventProcessTest(void) {
    evProcessInit();
    setAllEventFlag();
    eventMatrix_EventProcess(pEcb);
}

/******************************** END OF FILE **********************************
*************************** (C) Copyright 2019 REIZ ***************************/
