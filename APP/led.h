#ifndef __LED_H__
#define __LED_H__


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

#define LED_DISP_PORT           GPIOA

#define LED_DISP_TIME           2u
#define LED_BLINK_TIME          50u

#define LED_DISP_BUF_SIZE       6u

extern CODE u8 led_table[];
extern XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE];
extern XDATA u8 led_blink_pos;
extern XDATA u8 blink_led;

__task void AppTaskDisp(void);


#endif          
