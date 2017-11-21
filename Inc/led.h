#ifndef __LED_H__
#define __LED_H__


SBIT RS485 = P3^2;

SBIT LEDOE = P4^6;	
SBIT LEDX0 = P1^4;    
SBIT LEDX1 = P1^3;
SBIT LEDX2 = P1^2;
SBIT LEDX3 = P1^1;
SBIT LEDX4 = P1^0;	 
SBIT LEDX5 = P1^5;

#define LED_DISP_PORT           P0

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


#endif          
