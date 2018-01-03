#include "includes.h"


CODE u8 func_code_unit[FUNC_CODE_UNIT_NUM] =    //µ¥Î»
{
    LED_V_A_Hz,                                 //0 = NOT SEL
    LED_A,                                      //1 = A
    LED_V,                                      //2 = V
    LED_Hz,                                     //3 = Hz
    LED_PER_CENT,                               //4 = %
    LED_V_A_Hz,                                 //5 = s
    LED_V_A_Hz,                                 //6 = h
    LED_SPEED,                                  //7 = rpm
};

CODE u16 APPLIC_MACRO[] = //9902
{
    1, 2, 3, 4, 5, 6, 7, 15,
};

CODE u16 RELAY_OUTPUT1[] = //1401
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 RELAY_OUTPUT2[] = //1402
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 RELAY_OUTPUT3[] = //1403
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 RELAY_OUTPUT4[] = //1410
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 RELAY_OUTPUT5[] = //1411
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 RELAY_OUTPUT6[] = //1412
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 
    31, 32, 33, 34, 35, 36, 45, 46, 47
};

CODE u16 START_FUNCTION[] = //2101
{
    1, 2, 3, 4, 5, 8,
};

CODE u16 SWITCHING_FREQ[] = //2606
{
    1, 4, 8, 12,
};

CODE u16 SIGNAL1_PARAM[] = //3401
{
    100, 101, 102, 103, 104, 105, 106, 107, 109, 110,
    111, 112, 113, 114, 115, 116, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
    142, 143, 144, 145, 149, 150, 151, 152, 158, 159,
};

CODE u16 SIGNAL2_PARAM[] = //3408
{
    100, 101, 102, 103, 104, 105, 106, 107, 109, 110,
    111, 112, 113, 114, 115, 116, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
    142, 143, 144, 145, 149, 150, 151, 152, 158, 159,
};

CODE u16 SIGNAL3_PARAM[] = //3415
{
    100, 101, 102, 103, 104, 105, 106, 107, 109, 110,
    111, 112, 113, 114, 115, 116, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
    132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
    142, 143, 144, 145, 149, 150, 151, 152, 158, 159,
};

CODE u16 EFB_BAUD_RATE[] = //5303
{
	BAUD_RATE_1200 / 100,
	BAUD_RATE_2400 / 100,
	BAUD_RATE_4800 / 100,
    BAUD_RATE_9600 / 100,
    BAUD_RATE_19200 / 100,
    BAUD_RATE_38400 / 100,
    BAUD_RATE_57600 / 100,
    BAUD_RATE_76800 / 100,
};

CODE u16 POWER[] = //10509
{
    0x0000, 0x4A62, 0x6A62, 0x7A52, 0x0122, 0x0172, 0x0242, 0x0312, 0x0462, 0x0592,
    0x0752, 0x0882, 0x1142, 0x1432, 0x1782, 0x2212, 0x2482, 0x3A34, 0x4A14, 0x5A44,
    0x5A64, 0x6A94, 0x7A24, 0x8A84, 0x9A44, 0x0124, 0x0154, 0x0174, 0x0234, 0x0254,
    0x0314, 0x0384, 0x0444, 0x0454, 0x0464, 0x0594, 0x0604, 0x0724, 0x0874, 0x0884,
    0x0964, 0x1244, 0x1254, 0x1574, 0x1804, 0x1954, 0x2464, 0x2904,
};

CODE FUNC_CODE_ENUM func_code_enum[FUNC_CODE_ENUM_NUM] = 
{
    {1401, (u16 *)RELAY_OUTPUT1, sizeof(RELAY_OUTPUT1) / sizeof(u16)}, 
    {1402, (u16 *)RELAY_OUTPUT2, sizeof(RELAY_OUTPUT2) / sizeof(u16)},
    {1403, (u16 *)RELAY_OUTPUT3, sizeof(RELAY_OUTPUT3) / sizeof(u16)},
    {1410, (u16 *)RELAY_OUTPUT4, sizeof(RELAY_OUTPUT4) / sizeof(u16)},
    {1411, (u16 *)RELAY_OUTPUT5, sizeof(RELAY_OUTPUT5) / sizeof(u16)},
    {1412, (u16 *)RELAY_OUTPUT6, sizeof(RELAY_OUTPUT6) / sizeof(u16)},
    {2101, (u16 *)START_FUNCTION, sizeof(START_FUNCTION) / sizeof(u16)},
    {2606, (u16 *)SWITCHING_FREQ, sizeof(SWITCHING_FREQ) / sizeof(u16)},
    {3401, (u16 *)SIGNAL1_PARAM, sizeof(SIGNAL1_PARAM) / sizeof(u16)},  
    {3408, (u16 *)SIGNAL2_PARAM, sizeof(SIGNAL2_PARAM) / sizeof(u16)},
    {3415, (u16 *)SIGNAL3_PARAM, sizeof(SIGNAL3_PARAM) / sizeof(u16)},    
    {5303, (u16 *)EFB_BAUD_RATE, sizeof(EFB_BAUD_RATE) / sizeof(u16)}, 
    {9902, (u16 *)APPLIC_MACRO, sizeof(APPLIC_MACRO) / sizeof(u16)},
    {10509, (u16 *)POWER, sizeof(POWER) / sizeof(u16)},
};

