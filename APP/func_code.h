#ifndef __FUNC_CODE_H__
#define __FUNC_CODE_H__


#define FUNC_CODE_UNIT_NUM                      8u
#define FUNC_CODE_ENUM_NUM                      14u
#define FUNC_CODE_LIMIT_NUM                     300u

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
	CP_VFD_PARA_BAK_ERR = 5041u,                //参数备份文件不能移入内存
    CP_VFD_PARA_UPLOAD_GIVE_UP = 5050u,			//放弃参数上传
    CP_VFD_PARA_UPLOAD_FAIL = 5052u,			//参数上传失败
    CP_VFD_PARA_DOWNLOAD_GIVE_UP = 5060u,		//放弃参数下装
    CP_VFD_PARA_DOWNLOAD_FAIL = 5062u,			//参数下装失败
    CP_VFD_PARA_CRC_ERR = 5089u,				//下装失败，因为 CRC 错误	
} ENUM_CP_ALARM_CODE;			                //报警代码 ( 基本型控制盘 )

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
