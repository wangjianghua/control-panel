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
	CP_NORMAL = 0u,				//������û�б���
	CP_VFD_REM = 5011u,			//����������һ���ź�Դ����
	CP_NON_ZERO = 5019u,		//��ֹд�����ֵ ��ֻ��д���㣩
	CP_READ_ONLY = 5022u,		//��������������д����״̬
	CP_VFD_RUN = 5023u,			//������Ԫ����������״̬��������Բ��������޸�
} ENUM_CP_ALARM_CODE;			//�������� ( �����Ϳ����� )

extern CODE u8 func_code_unit[FUNC_CODE_UNIT_NUM];


#endif
