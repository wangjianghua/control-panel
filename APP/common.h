#ifndef __COMMON_H__
#define __COMMON_H__


typedef u8 bool;
typedef float fp32;

typedef enum
{ 
    FALSE = 0, 
    TRUE = !FALSE,
} BOOLEAN;

#define CODE                                const
#define XDATA               
#define SBIT                
#define BIT                                 u8
#define SFR                 

#define NOP                                 __NOP

#define OffsetOf(struct_t, member)          ((size_t)((char *)&((struct_t *)0)->member - (char *)(struct_t *)0))
#define SizeOf(struct_t, member)            (sizeof(((struct_t *)0)->member))

#define YEAR                                ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
                                             + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))  
  
#define MONTH                               (__DATE__ [2] == 'n' ? 1 \
                                             : __DATE__ [2] == 'b' ? 2 \
                                             : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                                             : __DATE__ [2] == 'y' ? 5 \
                                             : __DATE__ [2] == 'n' ? 6 \
                                             : __DATE__ [2] == 'l' ? 7 \
                                             : __DATE__ [2] == 'g' ? 8 \
                                             : __DATE__ [2] == 'p' ? 9 \
                                             : __DATE__ [2] == 't' ? 10 \
                                             : __DATE__ [2] == 'v' ? 11 : 12)  
  
#define DAY                                 ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                                             + (__DATE__ [5] - '0'))  
  
#define DATE_AS_INT                         (((YEAR - 2000) * 12 + MONTH - 1) * 31 + DAY)

u8 get_data_length(u32 data);


#endif
