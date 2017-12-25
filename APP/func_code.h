#ifndef __FUNC_CODE_H__
#define __FUNC_CODE_H__


#define FUNC_CODE_UNIT_NUM                      8u

typedef enum
{
    BAUD_RATE_9600 = 9600u,
    BAUD_RATE_19200 = 19200u,
} ENUM_BAUD_RATE;

typedef enum
{
	CP_NORMAL = 0u,				//正常，没有报警
	CP_VFD_REM = 5011u,			//传动由另外一个信号源控制
	CP_NON_ZERO = 5019u,		//禁止写入非零值 （只能写入零）
	CP_READ_ONLY = 5022u,		//参数组或参数处于写保护状态
	CP_VFD_RUN = 5023u,			//传动单元正处于运行状态，不允许对参数进行修改
} ENUM_CP_ALARM_CODE;			//报警代码 ( 基本型控制盘 )

extern CODE u8 func_code_unit[FUNC_CODE_UNIT_NUM];


#endif
