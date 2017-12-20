#ifndef __CP_H__
#define __CP_H__


#define MAX_REF_VAL             (500u * 100u)

#define VFD_LOC                 TRUE  //表示变频器处于本地控制，即控制命令来自控制盘
#define VFD_REM                 FALSE //表示变频器处于远程控制，例如I/O (X1)或现场总线

#define VFD_REV                 TRUE  //反转
#define VFD_FWD                 FALSE //正转

#define VFD_REPLY_TIMEOUT       1000u //1s

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
    bool func_code_visible; //隐藏功能码可视标志
    bool disp_para_sign[3]; //主界面显示参数符号位
    bool disp_para_enum[3]; //主界面显示参数枚举位
    u8 cmd; //命令字
	u8 group; //变频器分组
	u8 grade; //变频器分级
	u8 rate; //进度
	u8 vfd_para_crc; //变频器参数存取校验码
    u8 group_nearby[4]; //当前附近组号
    u8 grade_nearby[8]; //当前附近级号
    u8 vfd_para_unit; //变频器参数单位
    u8 disp_para_unit[3]; //主界面显示参数单位
    u8 disp_para_dcl[3]; //主界面显示参数小数位
    u8 vfd_para_shift; //变频器参数位切换
    u16 count; //计数器
    u16 ref; //变频器给定值
    u16 ref_mod; //给定更改值
    u16 disp_para_val[3]; //主界面显示参数值
    u16 disp_para_type[3]; //主界面显示参数类型
    u16 vfd_para_val; //变频器参数值
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
