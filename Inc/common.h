#ifndef __COMMON_H__
#define __COMMON_H__


typedef char s8;
typedef short s16;
typedef long s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define CODE                code
#define XDATA               xdata
#define SBIT                sbit
#define BIT                 bit
#define SFR                 sfr

#define NOP                 _nop_

typedef enum
{ 
    FALSE = 0, 
    TRUE = !FALSE,
} bool;

#define SYS_TICK_PERIOD     10u //10ms

#define START_TASK          0u
#define DISP_TASK           1u
#define KEY_TASK            2u
#define UART_TASK           3u
#define CP_TASK             4u

#define YEAR                ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \  
                             + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))  
  
#define MONTH               (__DATE__ [2] == 'n' ? 1 \  
                             : __DATE__ [2] == 'b' ? 2 \  
                             : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \  
                             : __DATE__ [2] == 'y' ? 5 \  
                             : __DATE__ [2] == 'n' ? 6 \  
                             : __DATE__ [2] == 'l' ? 7 \  
                             : __DATE__ [2] == 'g' ? 8 \  
                             : __DATE__ [2] == 'p' ? 9 \  
                             : __DATE__ [2] == 't' ? 10 \  
                             : __DATE__ [2] == 'v' ? 11 : 12)  
  
#define DAY                 ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \  
                             + (__DATE__ [5] - '0'))  
  
#define DATE_AS_INT         (((YEAR - 2000) * 12 + MONTH - 1) * 31 + DAY)


#endif