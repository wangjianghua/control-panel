#ifndef __LED_H__
#define __LED_H__


sbit RS485 = P3^2;

sbit LEDOE = P4^6;	
sbit LEDX0 = P1^4;    
sbit LEDX1 = P1^3;
sbit LEDX2 = P1^2;
sbit LEDX3 = P1^1;
sbit LEDX4 = P1^0;	 
sbit LEDX5 = P1^5;

#define LED_DISP_PORT           P0

#define DISP_KEEP_TIME          2u
#define LED_DISP_BUF_SIZE       6u

#define LED_A                   0xf7	  
#define LED_RUN                 0xfb
#define LED_MODE                0xfd
#define LED_HZ                  0xef
#define LED_V                   0xdf
#define LED_FR                  0x7f

#define FLASH_LEDA              0x08  
#define FLASH_LEDRUN            0x04
#define FLASH_LEDMODE           0x02
#define FLASH_LEDHZ             0x10
#define FLASH_LEDV              0x20
#define FLASH_LEDFR             0x80

extern code u8 led_table[];
extern xdata u8 led_disp_buf[LED_DISP_BUF_SIZE];
extern xdata u8 FlashTime; 
extern xdata u8 FlashPosition;
extern xdata u8 FlashLed;


#endif          