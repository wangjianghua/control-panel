#ifndef __COMMON_H__
#define __COMMON_H__


typedef char s8;
typedef short s16;
typedef long s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef enum
{ 
    FALSE = 0, 
    TRUE 
} bool;

#define SYS_TICK_PERIOD     10u //10ms

#define START_TASK          0u
#define LED_TASK            1u
#define KEY_TASK            2u
#define RX_TASK             3u
#define CP_TASK             4u


#endif