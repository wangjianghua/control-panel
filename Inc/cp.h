#ifndef __CP_H__
#define __CP_H__


#define KEY_MSG_NONE            NoKeyV
#define KEY_MSG                 KeyV1

#define KEY_MSG_UP              0x05
#define KEY_MSG_DOWN            0x08
#define KEY_MSG_EXIT            0x07
#define KEY_MSG_RUN             0x02
#define KEY_MSG_STOP            0x03
#define KEY_MSG_FWD_REV         0x04
#define KEY_MSG_LOC_REM         0x06
#define KEY_MSG_ENTER           0x01

#define LED_DP_MASK             0x01
#define LED_LOC_REM_MASK        0x02
#define LED_RUN_MASK            0x04
#define LED_V_MASK              0x20
#define LED_A_MASK              0x08
#define LED_Hz_MASK             0x10
#define LED_FWD_REV_MASK        0x80
#define LED_SPEED_MASK          (LED_A_MASK | LED_Hz_MASK)
#define LED_TORQUE_MASK         (LED_V_MASK | LED_A_MASK)
#define LED_V_A_Hz_MASK         (LED_V_MASK | LED_A_MASK | LED_Hz_MASK)

#define MAX_REF_VAL             (500u * 1000u)

#define VFD_LOC                 TRUE  //表示变频器处于本地控制，即控制命令来自控制盘
#define VFD_REM                 FALSE //表示变频器处于远程控制，例如I/O (X1)或现场总线

#define VFD_REV                 TRUE  //反转
#define VFD_FWD                 FALSE //正转

#define VFD_REPLY_TIMEOUT       100u //25 * 100 = 1s

typedef int (*FORM)(unsigned int key_msg, unsigned int form_msg);

typedef enum
{
    FORM_ID_HOME = 0,
    FORM_ID_HOME1 = 0,
    FORM_ID_HOME2,
    FORM_ID_HOME3,
    FORM_ID_REF,
    FORM_ID_REF_VAL,
    FORM_ID_PARA,
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

typedef enum
{
    FORM_PARA_SET_CMD = 0,
    FORM_PARA_ALARM_CMD,
    FORM_PARA_FAULT_CMD,
    MAX_FORM_PARA_CMD,
} FORM_PARA_CMD;

typedef struct _cp 
{
    u8 cmd; //命令字
    bool reset; //复位
    bool lr; //本地远程操作方式，本地LOC、远程REM
    bool run; //运行
    bool stop; //停止
    bool ref_chang; //给定更改标志
    bool fr; //旋转状态，正转FWD、反转REV
    u16 count; //计数器
    u16 ref_temp; //给定临时值
    u16 ref; //给定值
    u16 disp_para1; //主界面显示参数1
    u16 disp_para2; //主界面显示参数2
    u16 disp_para3; //主界面显示参数3
} CP, *PCP;

extern bool runstatus;
extern CP g_cp_para;


#endif
