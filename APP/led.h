#ifndef __LED_H__
#define __LED_H__


#define LED_DISP_PORT           GPIOA

#define LEDOE_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LEDOE_DISABLE()         GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define LEDX0_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define LEDX0_DISABLE()         GPIO_SetBits(GPIOB, GPIO_Pin_10)

#define LEDX1_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define LEDX1_DISABLE()         GPIO_SetBits(GPIOA, GPIO_Pin_15)

#define LEDX2_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define LEDX2_DISABLE()         GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define LEDX3_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define LEDX3_DISABLE()         GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define LEDX4_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LEDX4_DISABLE()         GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define LEDX5_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define LEDX5_DISABLE()         GPIO_SetBits(GPIOB, GPIO_Pin_11)

#define LED_DISP_TIME           2u
#define LED_BLINK_TIME          30u

#define LED_DISP_BUF_SIZE       6u

#define LED_DP_MASK             0x01
#define LED_LOC_REM_MASK        0x10
#define LED_RUN_MASK            0x20
#define LED_V_MASK              0x02
#define LED_A_MASK              0x04
#define LED_Hz_MASK             0x08
#define LED_FWD_REV_MASK        0x80
#define LED_SPEED_MASK          (LED_A_MASK | LED_Hz_MASK)
#define LED_TORQUE_MASK         (LED_V_MASK | LED_A_MASK)
#define LED_V_A_Hz_MASK         (LED_V_MASK | LED_A_MASK | LED_Hz_MASK)

extern CODE u8 led_table[];
extern XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE];
extern XDATA u8 led_blink_pos;
extern XDATA u8 blink_led;

__task void AppTaskDisp(void);


#endif          
