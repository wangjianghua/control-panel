#ifndef __LED_H__
#define __LED_H__


#define LEDOE_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LEDOE_DISALBE()         GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define LEDX0_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define LEDX0_DISALBE()         GPIO_SetBits(GPIOB, GPIO_Pin_10)

#define LEDX1_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define LEDX1_DISALBE()         GPIO_SetBits(GPIOA, GPIO_Pin_15)

#define LEDX2_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define LEDX2_DISALBE()         GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define LEDX3_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define LEDX3_DISALBE()         GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define LEDX4_ENABLE()          GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LEDX4_DISALBE()         GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define LEDX5_ENABLE()          GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define LEDX5_DISALBE()         GPIO_SetBits(GPIOB, GPIO_Pin_11)

#define LED_DISP_PORT           GPIOA

#define LED_DISP_TIME           2u
#define LED_BLINK_TIME          80u

#define LED_DISP_BUF_SIZE       6u

#define LED_A                   0xf7	  
#define LED_RUN                 0xfb
#define LED_MODE                0xfd
#define LED_Hz                  0xef
#define LED_V                   0xdf
#define LED_FR                  0x7f

#define BLINK_LEDA              0x08  
#define BLINK_LEDRUN            0x04
#define BLINK_LEDMODE           0x02
#define BLINK_LEDHz             0x10
#define BLINK_LEDV              0x20
#define BLINK_LEDFR             0x80

extern CODE u8 led_table[];
extern XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE];
extern XDATA u8 led_blink_pos;
extern XDATA u8 blink_led;

__task void AppTaskDisp(void);


#endif          
