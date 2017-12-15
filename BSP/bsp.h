/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the 
*                                 IAR STM32F103ZE-SK Evaluation Board
*
* Filename      : bsp.h
* Version       : V2.00
* Programmer(s) : FT
*                 ЛЊаж
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stm32f10x.h>

/*
*********************************************************************************************************
*                                               OTHER'S
*********************************************************************************************************
*/

#define DEBUG_INFO_PRINT_EN          0u

#if (DEBUG_INFO_PRINT_EN > 0u)
#define DEBUG_PRINT(s)         printf s
#else
#define DEBUG_PRINT(s)             
#endif

#define WDT_EN                       0u

#if (WDT_EN > 0u)
#define clr_wdt()         { IWDG_ReloadCounter(); }
#else
#define clr_wdt()
#endif

#define USART1_PREEMPT_PRIO          1u
#define USART1_SUB_PRIO              0u

void STM32_SoftReset(void);
void GPIO_OutputByte(GPIO_TypeDef *GPIOx, uint8_t Data);
void GPIO_OutputHalfWord(GPIO_TypeDef *GPIOx, uint16_t Data);
void USART_BaudRate(USART_TypeDef *USARTx, uint32_t BaudRate);
void BSP_Init(void);

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
