#ifndef __LED_H__
#define __LED_H__


#define LED_DISP_PORT           GPIOA

#define LED_OE_ENABLE()         GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LED_OE_DISABLE()        GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define LED_X0_ENABLE()         GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define LED_X0_DISABLE()        GPIO_SetBits(GPIOB, GPIO_Pin_10)

#define LED_X1_ENABLE()         GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define LED_X1_DISABLE()        GPIO_SetBits(GPIOA, GPIO_Pin_15)

#define LED_X2_ENABLE()         GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define LED_X2_DISABLE()        GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define LED_X3_ENABLE()         GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define LED_X3_DISABLE()        GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define LED_X4_ENABLE()         GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED_X4_DISABLE()        GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define LED_X5_ENABLE()         GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define LED_X5_DISABLE()        GPIO_SetBits(GPIOB, GPIO_Pin_11)

#define LED_DISP_TIME           1u
#define LED_BLINK_TIME          50u

#define LED_DISP_BUF_SIZE       6u

#define LED_DP_MASK             0x01
#define LED_LOC_REM_MASK        0x10
#define LED_RUN_MASK            0x20
#define LED_V_MASK              0x02
#define LED_A_MASK              0x04
#define LED_Hz_MASK             0x08
#define LED_FWD_REV_MASK        0x80
#define LED_SPEED_MASK          (LED_A_MASK | LED_Hz_MASK)
#define LED_PER_CENT_MASK       (LED_V_MASK | LED_A_MASK)
#define LED_V_A_Hz_MASK         (LED_V_MASK | LED_A_MASK | LED_Hz_MASK)

#define LED_DP                  (~LED_DP_MASK)
#define LED_LOC_REM             (~LED_LOC_REM_MASK)
#define LED_RUN                 (~LED_RUN_MASK)
#define LED_V                   (~LED_V_MASK)
#define LED_A                   (~LED_A_MASK)
#define LED_Hz                  (~LED_Hz_MASK)
#define LED_FWD_REV             (~LED_FWD_REV_MASK)
#define LED_SPEED               (~LED_SPEED_MASK)
#define LED_PER_CENT            (~LED_PER_CENT_MASK)
#define LED_V_A_Hz              (~LED_V_A_Hz_MASK)

extern CODE u8 led_table[];
extern XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE];
extern XDATA u8 led_blink_pos;
extern XDATA u8 blink_led;

__task void AppTaskDisp(void);


#endif          
