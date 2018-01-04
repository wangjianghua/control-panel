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
	CP_NORMAL = 0u,				                //������û�б���
	CP_VFD_REM = 5011u,			                //����������һ���ź�Դ����
    CP_VFD_DIR_LOCK	= 5012u,                    //�����ת��������
    CP_VFD_RUN_LOCK = 5013u,                    //������ť�źţ���Ϊ��������ֹ
	CP_NON_ZERO = 5019u,		                //��ֹд�����ֵ ��ֻ��д���㣩
	CP_READ_ONLY = 5022u,		                //��������������д����״̬
	CP_VFD_RUN = 5023u,			                //������Ԫ����������״̬��������Բ��������޸�
	CP_VFD_PARA_BAK_ERR = 5041u,                //���������ļ����������ڴ�
    CP_VFD_PARA_UPLOAD_GIVE_UP = 5050u,			//���������ϴ�
    CP_VFD_PARA_UPLOAD_FAIL = 5052u,			//�����ϴ�ʧ��
    CP_VFD_PARA_DOWNLOAD_GIVE_UP = 5060u,		//����������װ
    CP_VFD_PARA_DOWNLOAD_FAIL = 5062u,			//������װʧ��
    CP_VFD_PARA_CRC_ERR = 5089u,				//��װʧ�ܣ���Ϊ CRC ����	
} ENUM_CP_ALARM_CODE;			                //�������� ( �����Ϳ����� )

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
