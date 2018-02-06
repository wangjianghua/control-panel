#ifndef __FUNC_CODE_H__
#define __FUNC_CODE_H__


#define FUNC_CODE_UNIT_NUM                      8u
#define FUNC_CODE_ENUM_NUM                      20u
#define FUNC_CODE_LIMIT_NUM                     293u

typedef enum
{
	BAUD_RATE_1200 = 1200u,
	BAUD_RATE_2400 = 2400u,
	BAUD_RATE_4800 = 4800u,
    BAUD_RATE_9600 = 9600u,
    BAUD_RATE_19200 = 19200u,
    BAUD_RATE_38400 = 38400u,
    BAUD_RATE_57600 = 57600u,
    BAUD_RATE_76800 = 76800u,
} ENUM_BAUD_RATE;

typedef enum
{
	CP_NORMAL = 0u,				                //正常，没有报警
	CP_VFD_REM = 5011u,			                //传动由另外一个信号源控制
    CP_VFD_DIR_LOCK	= 5012u,                    //电机旋转方向被锁定
    CP_VFD_RUN_LOCK = 5013u,                    //封锁按钮信号，因为启动被禁止
	CP_NON_ZERO = 5019u,		                //禁止写入非零值 （只能写入零）
	CP_READ_ONLY = 5022u,		                //参数组或参数处于写保护状态
	CP_VFD_RUN = 5023u,			                //传动单元正处于运行状态，不允许对参数进行修改
	CP_VFD_PARA_INVALID = 5028u,                //参数值无效
	CP_VFD_PARA_BAK_ERR = 5041u,                //参数备份文件不能移入内存
    CP_VFD_PARA_UPLOAD_GIVE_UP = 5050u,			//放弃参数上传
    CP_VFD_PARA_UPLOAD_FAIL = 5052u,			//参数上传失败
    CP_VFD_PARA_DOWNLOAD_GIVE_UP = 5060u,		//放弃参数下装
    CP_VFD_PARA_DOWNLOAD_FAIL = 5062u,			//参数下装失败
    CP_VFD_PARA_CRC_ERR = 5089u,				//下装失败，因为 CRC 错误	
} ENUM_CP_ALARM_CODE;			                //报警代码 ( 基本型控制盘 )

typedef enum
{
    FB_STS_WORD_READY = 0x0001, //准备
    FB_STS_WORD_PERMIT = 0x0002, //允许
    FB_STS_WORD_START = 0x0004, //起动
    FB_STS_WORD_RUN = 0x0008, //运行
    FB_STS_WORD_ZERO_SPEED = 0x0010, //零速
    FB_STS_WORD_ACLR = 0x0020, //加速
    FB_STS_WORD_DELR = 0x0040, //减速
    FB_STS_WORD_REACH_SET_POINT = 0x0080, //达到设定点
    FB_STS_WORD_REACH_LIMIT = 0x0100, //到达限幅值
    FB_STS_WORD_REACH_MONITOR = 0x0200, //到达监视值
    FB_STS_WORD_REV_REF = 0x0400, //反向给定
    FB_STS_WORD_REV_RUN = 0x0800, //反向运行
    FB_STS_WORD_LOC_CTRL = 0x1000, //本地控制模式
    FB_STS_WORD_BUS_CTRL = 0x2000, //总线控制模式
    FB_STS_WORD_EXT2_EFF = 0x4000, //外部 2 有效
    FB_STS_WORD_FAULT = 0x8000, //故障
} ENUM_FB_STS_WORD1; //0303，总线状态字1

typedef enum
{
    FB_STS_WORD_ALARM = 0x0001, //报警
    FB_STS_WORD_ATTN = 0x0002, //注意
    FB_STS_WORD_DIR_LOCK = 0x0004, //方向锁定
    FB_STS_WORD_LOC_LOCK = 0x0008, //本地锁定
    FB_STS_WORD_VECT_CTRL = 0x0010, //矢量控制模式
    FB_STS_WORD_REV1 = 0x0020, //保留
    FB_STS_WORD_REV2 = 0x0040, //保留
    FB_STS_WORD_CTRL_SRC_COPY = 0x0080, //控制源拷贝
    FB_STS_WORD_REF1_COPY = 0x0100, //给定 1 拷贝
    FB_STS_WORD_REF2_COPY = 0x0200, //给定 2 拷贝
    FB_STS_WORD_CMD_WORD_COPY = 0x0400, //拷贝控制字
    FB_STS_WORD_REF1_REQ = 0x0800, //给定 1 请求
    FB_STS_WORD_REF2_REQ = 0x1000, //给定 2 请求
    FB_STS_WORD_EXT_PID_REF2_REQ = 0x2000, //外部 PID 给定 2 请求
    FB_STS_WORD_START_BAN_SIGNAL_EFF = 0x4000, //起动禁止信号有效
    FB_STS_WORD_STOP_SIGNAL_EFF = 0x8000, //停止信号有效    
} ENUM_FB_STS_WORD2; //0304，总线状态字2

typedef struct _func_code_enum
{
    u16 func_code;
    u16 *array;
    u8 len;
} FUNC_CODE_ENUM, *P_FUNC_CODE_ENUM;

typedef struct _func_code_limit
{
	u16 func_code;
	u16 lower;
	u16 upper;
} FUNC_CODE_LIMIT, *P_FUNC_CODE_LIMIT;

extern CODE u8 func_code_unit[FUNC_CODE_UNIT_NUM];
extern CODE FUNC_CODE_ENUM func_code_enum[FUNC_CODE_ENUM_NUM];
extern CODE FUNC_CODE_LIMIT func_code_limit[FUNC_CODE_LIMIT_NUM];


#endif
