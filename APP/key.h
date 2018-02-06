#ifndef __KEY_H__
#define __KEY_H__


#define EnterKey                    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define RunKey                      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define StopKey                     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
#define FwdRevKey                   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define DownKey                     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define LocRemKey                   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
#define ExitKey                     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define UpKey                       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define FuncCodeKey                 (GPIO_ReadInputData(GPIOB) & (GPIO_Pin_1 | GPIO_Pin_5))

#define KEY_VAL_NONE                0u

#define KEY_MSG_NONE                KEY_VAL_NONE
#define KEY_MSG                     key_value

#define KEY_MSG_ENTER               0x01
#define KEY_MSG_RUN                 0x02
#define KEY_MSG_STOP                0x03
#define KEY_MSG_FWD_REV             0x04
#define KEY_MSG_SHIFT               0x04
#define KEY_MSG_DOWN                0x05
#define KEY_MSG_LOC_REM             0x06
#define KEY_MSG_EXIT                0x07
#define KEY_MSG_UP                  0x08
#define KEY_MSG_FUNC_CODE           0x09

#define KEY_MSG_DOWN_LONG           (KEY_MSG_DOWN + 36u)
#define KEY_MSG_LOC_REM_LONG        (KEY_MSG_LOC_REM + 36u)
#define KEY_MSG_UP_LONG             (KEY_MSG_UP + 36u)
#define KEY_MSG_FUNC_CODE_LONG      (KEY_MSG_FUNC_CODE + 36u)

extern OS_SEM key_sem;
extern XDATA u8 key_value;

__task void AppTaskKey(void);


#endif
