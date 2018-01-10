/** 
 * @file     main.c
 * @brief    CP(������)
 * @details  Control panel
 * @author   ����
 * @email    jianghua.wang@foxmail.com
 * @date     2017
 * @version  vX.XX
 * @par Copyright (c):  
 *           ���ֵ���
 * @par History:          
 *   version: author, date, desc\n 
 */ 

/** @addtogroup CP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
typedef unsigned long long OS_STK; //U64

/* Private macro -------------------------------------------------------------*/
#define APP_CFG_TASK_START_PRIO             1u
#define APP_CFG_TASK_DISP_PRIO              2u
#define APP_CFG_TASK_KEY_PRIO               3u
#define APP_CFG_TASK_UART_PRIO              4u
#define APP_CFG_TASK_CP_PRIO                5u

#define APP_CFG_TASK_START_STK_SIZE         512u
#define APP_CFG_TASK_DISP_STK_SIZE          512u
#define APP_CFG_TASK_KEY_STK_SIZE           1024u
#define APP_CFG_TASK_UART_STK_SIZE          512u
#define APP_CFG_TASK_CP_STK_SIZE            2048u

/* Private variables ---------------------------------------------------------*/
OS_TID TaskDisp;
OS_TID TaskKey;
OS_TID TaskUart;
OS_TID TaskCP;

static OS_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 8];
static OS_STK AppTaskDispStk[APP_CFG_TASK_DISP_STK_SIZE / 8];
static OS_STK AppTaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE / 8];
static OS_STK AppTaskUartStk[APP_CFG_TASK_UART_STK_SIZE / 8];
static OS_STK AppTaskCPStk[APP_CFG_TASK_CP_STK_SIZE / 8];

/* Private function prototypes -----------------------------------------------*/
void AppEventCreate(void);
void AppTaskCreate(void);
__task void AppTaskStart(void);

/* Private functions ---------------------------------------------------------*/
void AppEventCreate(void)
{
    os_sem_init(&key_sem, 0);
    os_sem_init(&cp_sem, 0);
    os_sem_init(&uart_sem, 0);
}

void AppTaskCreate(void)
{
    TaskDisp = os_tsk_create_user(AppTaskDisp,
                                  APP_CFG_TASK_DISP_PRIO,
                                  &AppTaskDispStk,
                                  sizeof(AppTaskDispStk));

    TaskKey = os_tsk_create_user(AppTaskKey,
                                 APP_CFG_TASK_KEY_PRIO,
                                 &AppTaskKeyStk,
                                 sizeof(AppTaskKeyStk));

    TaskUart = os_tsk_create_user(AppTaskUart,
                                  APP_CFG_TASK_UART_PRIO,
                                  &AppTaskUartStk,
                                  sizeof(AppTaskUartStk));

    TaskCP = os_tsk_create_user(AppTaskCP,
                                APP_CFG_TASK_CP_PRIO,
                                &AppTaskCPStk,
                                sizeof(AppTaskCPStk));
}

__task void AppTaskStart(void)
{
    AppEventCreate();
    AppTaskCreate();

    os_tsk_delete_self();
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int  main(void)
{
    BSP_Init();

    os_sys_init_user(AppTaskStart,
                     APP_CFG_TASK_START_PRIO,
                     &AppTaskStartStk,
                     sizeof(AppTaskStartStk));
        
    while(1);
}

/**
  * @}
  */

/******************************** END OF FILE ********************************/
