#ifndef __CP_H__
#define __CP_H__


#define KEY_MSG_NONE            KEY_VAL_NONE
#define KEY_MSG                 key_value

#define KEY_MSG_UP              0x08
#define KEY_MSG_DOWN            0x05
#define KEY_MSG_EXIT            0x07
#define KEY_MSG_RUN             0x02
#define KEY_MSG_STOP            0x03
#define KEY_MSG_FWD_REV         0x04
#define KEY_MSG_LOC_REM         0x06
#define KEY_MSG_ENTER           0x01
#define KEY_MSG_FUNC_CODE       0x09

#define LED_DP_MASK             0x01
#define LED_LOC_REM_MASK        0x10
#define LED_RUN_MASK            0x20
#define LED_V_MASK              0x02
#define LED_A_MASK              0x04
#define LED_Hz_MASK             0x08
#define LED_FWD_REV_MASK        0x80
#define LED_SPEED_MASK          (LED_A_MASK | LED_Hz_MASK)
#define LED_TORQUE_MASK         (LED_V_MASK | LED_A_MASK)
#define LED_V_A_Hz_MASK         (LED_V_MASK | LED_A_MASK | LED_Hz_MASK)

#define MAX_REF_VAL             (500u * 100u)

#define VFD_LOC                 TRUE  //表示变频器处于本地控制，即控制命令来自控制盘
#define VFD_REM                 FALSE //表示变频器处于远程控制，例如I/O (X1)或现场总线

#define VFD_REV                 TRUE  //反转
#define VFD_FWD                 FALSE //正转

#define VFD_REPLY_TIMEOUT       1000u //1s

#define COPY_BAUDRATE		    19200u
#define OTHER_BAUDRATE		    9600u

#define VFD_PARA_FLAG           ('H')                       //变频器参数标志

#define CP_PARA_ADDR            0u                          //控制盘参数存储地址

#define VFD_PARA_FLAG_ADDR      (AT24CXX - (3u * 1024u))    //1B, 变频器参数标志存储地址
#define VFD_PARA_LEN_ADDR       (VFD_PARA_FLAG_ADDR + 1u)   //2B, 变频器参数长度存储地址
#define VFD_PARA_ADDR           (VFD_PARA_LEN_ADDR + 2u)    //nB, 变频器参数存储地址

typedef enum
{
    OUTPUT_FREQ = 0x103,
    TORQUE = 0x105,    
    DC_BUS_VOLTAGE = 0x107,
} ENUM_RUN_DATA;

typedef int (*FORM)(unsigned int key_msg, unsigned int form_msg);

typedef enum
{
    FORM_ID_ERR = 0,
    FORM_ID_HOME = 1,
    FORM_ID_HOME1 = 1,
    FORM_ID_HOME2,
    FORM_ID_HOME3,
    FORM_ID_REF,
    FORM_ID_REF_VAL,
    FORM_ID_PARA,
    FORM_ID_PARA_GROUP,
    FORM_ID_PARA_GRADE,
    FORM_ID_PARA_VAL,
    FORM_ID_COPY,
    FORM_ID_COPY_UPLOAD,
    FORM_ID_COPY_DOWNLOAD_ALL,
    FORM_ID_COPY_DOWNLOAD_PART,
    FORM_ID_COPY_UPLOAD_RATE,
    FORM_ID_COPY_DOWNLOAD_ALL_RATE,
    FORM_ID_COPY_DOWNLOAD_PART_RATE,
    MAX_FORM_NUM,
} ENUM_FORM_ID;

typedef enum
{
    FORM_MSG_NONE = 0,
    FORM_MSG_REFRESH,
    FORM_MSG_KEY,
    FORM_MSG_DATA,
} ENUM_FORM_MSG;

typedef enum
{
    FORM_ERR_SET_CMD = 0,
    FORM_ERR_FAULT_CMD,
    FORM_ERR_ALARM_CMD,
    MAX_FORM_ERR_CMD,
} ENUM_FORM_ERR_CMD;

typedef enum
{
    FORM_HOME_SET_CMD = 0,
    FORM_HOME_READ_CMD,
    MAX_FORM_HOME_CMD,
} ENUM_FORM_HOME_CMD;

typedef enum
{
    FORM_REF_SET_CMD = 0,
    MAX_FORM_REF_CMD,
} ENUM_FORM_REF_CMD;

typedef enum
{
    FORM_REF_VAL_SET_CMD = 0,
    MAX_FORM_REF_VAL_CMD,
} ENUM_FORM_REF_VAL_CMD;

typedef enum
{
    FORM_PARA_SET_CMD = 0,
    MAX_FORM_PARA_CMD,
} ENUM_FORM_PARA_CMD;

typedef enum
{
    FORM_PARA_GROUP_SET_CMD = 0,
    MAX_FORM_PARA_GROUP_CMD,
} ENUM_FORM_PARA_GROUP_CMD;

typedef enum
{
    FORM_PARA_GRADE_SET_CMD = 0,
    MAX_FORM_PARA_GRADE_CMD,
} ENUM_FORM_PARA_GRADE_CMD;

typedef enum
{
    FORM_PARA_VAL_SET_CMD = 0,
    MAX_FORM_PARA_VAL_CMD,
} ENUM_FORM_PARA_VAL_CMD;

typedef enum
{
    FORM_COPY_SET_CMD = 0,
    MAX_FORM_COPY_CMD,
} ENUM_FORM_COPY_CMD;

typedef enum
{
    FORM_COPY_UPLOAD_SET_CMD = 0,
    MAX_FORM_COPY_UPLOAD_CMD,
} ENUM_FORM_COPY_UPLOAD_CMD;

typedef enum
{
    FORM_COPY_DOWNLOAD_ALL_SET_CMD = 0,
    MAX_FORM_COPY_DOWNLOAD_ALL_CMD,
} ENUM_FORM_COPY_DOWNLOAD_ALL_CMD;

typedef enum
{
    FORM_COPY_DOWNLOAD_PART_SET_CMD = 0,
    MAX_FORM_COPY_DOWNLOAD_PART_CMD,
} ENUM_FORM_COPY_DOWNLOAD_PART_CMD;

typedef enum
{
    COPY_UPLOAD_RATE_BAUDRATE = 0,
    MAX_COPY_UPLOAD_RATE_BAUDRATE_CMD,
} ENUM_COPY_UPLOAD_RATE_BAUDRATE_CMD;

typedef enum
{
    COPY_UPLOAD_RATE_SET_CMD = 0,
    COPY_UPLOAD_RATE_CMD,
    COPY_UPLOAD_RATE_TAIL_CMD,
    MAX_COPY_UPLOAD_RATE_CMD,
} ENUM_COPY_UPLOAD_RATE_CMD;

typedef enum
{
    COPY_DOWNLOAD_ALL_RATE_SET_CMD = 0,
    COPY_DOWNLOAD_ALL_RATE_CMD,
    COPY_DOWNLOAD_ALL_RATE_TAIL_CMD,
    MAX_COPY_DOWNLOAD_ALL_RATE_CMD,
} ENUM_COPY_DOWNLOAD_ALL_RATE_CMD;

typedef enum
{
    KEEP_VFD_CONNECT_CMD = 0,
    MAX_KEEP_VFD_CONNECT_CMD,
} ENUM_KEEP_VFD_CONNECT_CMD;

typedef struct _cp_para_rom
{
	u16 ref; //给定值
} CP_PARA_ROM, *P_CP_PARA_ROM;

typedef struct _cp_para_ram 
{
    bool clr_err; //清除错误
    bool lr; //本地远程操作方式，本地LOC、远程REM
    bool run; //运行
    bool stop; //停止
    bool ref_chang; //给定更改标志
    bool fr; //旋转状态，正转FWD、反转REV  
    bool func_code_visible; //99组以后功能码可视标志
    u8 cmd; //命令字
	u8 group; //变频器分组
	u8 grade; //变频器分级
	u8 rate; //进度
	u8 vfd_para_crc; //变频器参数存取校验码
    u8 group_nearby[4]; //当前附近组号
    u8 grade_nearby[8]; //当前附近级号
    u16 count; //计数器
    u16 ref; //给定值
    u16 disp_para1; //主界面显示参数1
    u16 disp_para2; //主界面显示参数2
    u16 disp_para3; //主界面显示参数3
    u16 disp_para_type[3]; //主界面显示参数类型
    u16 vfd_para; //变频器参数
    u16 vfd_para_count; //变频器参数当前数目
    u16 vfd_para_total; //变频器参数总数目
    u16 vfd_para_index; //变频器参数存取索引
    u16 fault_code; //故障码
    u16 alarm_code; //报警码
} CP_PARA_RAM, *P_CP_PARA_RAM;

extern OS_SEM cp_sem;
extern XDATA CP_PARA_ROM cp_para_rom;
extern XDATA CP_PARA_RAM cp_para_ram;

__task void AppTaskCP(void);


#endif
