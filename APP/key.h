#ifndef __KEY_H__
#define __KEY_H__


#define ShiftKey                    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define RunKey                      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define StopKey                     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
#define EnterKey                    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define DownKey                     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define JogKey                      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
#define PrgKey                      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define UpKey                       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)

#define ERRORCOUNT                  20

#define KEY_VAL_NONE                0	

#define ShiftValue                  1
#define RunValue                    2
#define StopValue                   3
#define EnterValue                  4
#define DownValue                   5
#define JogValue                    6
#define PrgValue                    7
#define UpKeyValue                  8	
#define FreeRunKey		            9

#define ShiftValue_L1		        10
#define RunValue_L1                 11
#define StopValue_L1                12
#define EnterValue_L1               13
#define DownValue_L1                14
#define JogValue_L1                 15
#define PrgValue_L1                 16
#define UpKeyValue_L1               17
#define FreeRunKey_L1		        18

#define ShiftValue_L2		        19
#define RunValue_L2                 20
#define StopValue_L2                21
#define EnterValue_L2               22
#define DownValue_L2                23
#define JogValue_L2                 24
#define PrgValue_L2                 25
#define UpKeyValue_L2               26
#define FreeRunKey_L2		        27

#define ShiftValue_L3		        28
#define RunValue_L3                 29
#define StopValue_L3                30
#define EnterValue_L3               31
#define DownValue_L3                32
#define JogValue_L3                 33
#define PrgValue_L3                 34
#define UpKeyValue_L3               35
#define FreeRunKey_L3		        36

#define ShiftValue_L4		        37
#define RunValue_L4                 38
#define StopValue_L4                39
#define EnterValue_L4               40
#define DownValue_L4                41
#define JogValue_L4                 42
#define PrgValue_L4                 43
#define UpKeyValue_L4               44
#define FreeRunKey_L4		        45

#define ShiftValueUp		        46
#define RunValueUp                  47
#define StopValueUp                 48
#define EnterValueUp                49
#define DownValueUp                 50
#define JogValueUp                  51
#define PrgValueUp                  52
#define UpKeyValueUp                53
#define FreeRunKeyUp		        54

#define NOCOM_TASKAND               0

#define LINKCOM_TASKAND             1
#define RUNCOM_TASKAND              2
#define STOPCOM_TASKAND	            3
#define RESETCOM_TASKAND            4
#define JOGRUNCOM_TASKAND           5
#define JOGSTOPCOM_TASKAND          6
#define READALARMCOM_TASKAND        7
#define SHIFTRUNCOM_TASKAND	        8 
#define SHIFTSTOPCOM_TASKAND        9
#define RUNMONICOM_TASKAND          10
#define STOPMONICOM_TASKAND         11
#define READSTATUSCOM_TASKAND       12

#define ENTERZUNOCOM_TASKAND        13
#define ZUNOFLASHCOM_TASKAND        14
#define ZUNODOWNCOM_TASKAND	        15
#define ZUNOUPCOM_TASKAND           16

#define ENTERFUNOCOM_TASKAND        17
#define FUNNOFLASHCOM_TASKAND       18
#define FUNNODOWNCOM_TASKAND        19
#define FUNNOUPCOM_TASKAND          20

#define ENTERPARACOM_TASKAND        21
#define READPARACOM_TASKAND         22
#define PARAFLASHCOM_TASKAND        23
#define PARADOWNCOM_TASKAND         24
#define PARAUPCOM_TASKAND           25
#define WRITEPARACOM_TASKAND        26

#define READFREQCOM_TASKAND         27
#define FREQDOWNCOM_TASKAND         28
#define FREQUPCOM_TASKAND           29

#define SECRETCOM_TASKAND           30

#define UPLOADCOM_TASKAND           31
#define DOWNLOADCOM_TASKAND         32

#define IDENTIFYCOM_TASKAND         33
#define CLEARIDENTIFYCOM_TASKAND    34

#define CLEARUPLOADCOM_TASKAND      35
#define CLEARDOWNLOADCOM_TASKAND    36

#define FREERUNCOM_TASKAND          37

#define FACTSECRETCOM_TASKAND       38
#define READZUNOCOM_TASKAND         39
#define READFUNNOCOM_TASKAND        40

#define MAINMENU                    1
#define ZUNOMENU                    2
#define FUNNOMENU                   3
#define PARAMENU                    4
#define PASSWORDMENU                5
#define UPDOWNMENU                  6
#define ALARMMENU                   7
#define IDENTIFYMENU                8
#define FACTMENU                    9

extern OS_SEM key_sem;
extern XDATA u8 key_value;
extern XDATA u8 key_fsm; 

BIT CheckKeyPress(void);
u8 KeyScan(void);
BIT ReadKeyPress(u8 key);
u8 ReadKeyVal(void);
__task void AppTaskKey(void);


#endif
