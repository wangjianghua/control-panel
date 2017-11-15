#ifndef __LED_H__
#define __LED_H__


#define DisKeepTime     2

#define LED_A           0xf7	  
#define LED_RUN         0xfb
#define LED_MODE        0xfd
#define LED_HZ          0xef
#define LED_V           0xdf
#define LED_FR          0x7f

#define FLASH_LEDA      0x08  
#define FLASH_LEDRUN    0x04
#define FLASH_LEDMODE   0x02
#define FLASH_LEDHZ     0x10
#define FLASH_LEDV      0x20
#define FLASH_LEDFR     0x80

sbit RT485 = P3^2;
sbit LEDOE = P4^6;	

sbit LEDX0 = P1^4;    
sbit LEDX1 = P1^3;
sbit LEDX2 = P1^2;
sbit LEDX3 = P1^1;
sbit LEDX4 = P1^0;	 
sbit LEDX5 = P1^5;

extern code u8 led_tab[];
extern  u8  LED_BUFF[6];

extern u8          FlashLed;
extern u8          FlashPosition;

extern u8 FlashTime;


#endif          