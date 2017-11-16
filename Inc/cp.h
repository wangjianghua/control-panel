#ifndef __CP_H__
#define __CP_H__


#define KEY_MSG_NONE            NoKeyV
#define KEY_MSG                 KeyV1

#define KEY_MSG_UP              0x05
#define KEY_MSG_DOWN            0x08
#define KEY_MSG_EXIT            0x07
#define KEY_MSG_START           0x02
#define KEY_MSG_STOP            0x03
#define KEY_MSG_RF              0x04
#define KEY_MSG_LOC_REM         0x06
#define KEY_MSG_ENTER           0x01

#define LED_DP_MASK             0x01
#define LED_LOC_REM_MASK        0x02
#define LED_RUN_MASK            0x04
#define LED_V_MASK              0x20
#define LED_A_MASK              0x08
#define LED_Hz_MASK             0x10
#define LED_FR_MASK             0x80
#define LED_SPEED_MASK          (LED_A_MASK | LED_Hz_MASK)
#define LED_TORQUE_MASK         (LED_V_MASK | LED_A_MASK)
#define LED_V_A_Hz_MASK         (LED_V_MASK | LED_A_MASK | LED_Hz_MASK)

#define MAX_REF_VAL             (500u * 1000u)

#define OPER_LOC                TRUE

typedef int (*FORM)(unsigned int key_msg, unsigned int form_msg);

typedef enum
{
    FORM_ID_NONE = 0,
    FORM_ID_HOME1 = 0,
    FORM_ID_HOME2,
    FORM_ID_HOME3,
    FORM_ID_REF,
    FORM_ID_REF_VAL,
    MAX_FORM_NUM,
} FORM_ID;

typedef enum
{
    FORM_MSG_NONE = 0,
    FORM_MSG_REFRESH,
    FORM_MSG_KEY,
    FORM_MSG_DATA,
} FORM_MSG;

typedef enum
{
    FORM_HOME_SET_CMD = 0,
    FORM_HOME_READ_CMD,
    FORM_HOME_ALARM_CMD,
    FORM_HOME_FAULT_CMD,
    MAX_FORM_HOME_CMD,
} FORM_HOME_CMD;

typedef enum
{
    FORM_REF_SET_CMD = 0,
    FORM_REF_ALARM_CMD,
    FORM_REF_FAULT_CMD,
    MAX_FORM_REF_CMD,
} FORM_REF_CMD;

typedef enum
{
    FORM_REF_VAL_SET_CMD = 0,
    FORM_REF_VAL_ALARM_CMD,
    FORM_REF_VAL_FAULT_CMD,
    MAX_FORM_REF_VAL_CMD,
} FORM_REF_VAL_CMD;

typedef struct _cp 
{
    u8 cmd;
    bool reset;
    bool oper;
    bool start;
    bool stop;
    bool ref_chang;
    u16 ref_temp;
    u16 count;
    u16 ref;
    u16 disp_para1;
    u16 disp_para2;
    u16 disp_para3;
} CP, *PCP;

extern CP g_cp_para;
extern bit runstatus;


#endif
