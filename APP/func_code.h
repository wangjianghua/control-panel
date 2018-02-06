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
	CP_NORMAL = 0u,				                //������û�б���
	CP_VFD_REM = 5011u,			                //����������һ���ź�Դ����
    CP_VFD_DIR_LOCK	= 5012u,                    //�����ת��������
    CP_VFD_RUN_LOCK = 5013u,                    //������ť�źţ���Ϊ��������ֹ
	CP_NON_ZERO = 5019u,		                //��ֹд�����ֵ ��ֻ��д���㣩
	CP_READ_ONLY = 5022u,		                //��������������д����״̬
	CP_VFD_RUN = 5023u,			                //������Ԫ����������״̬��������Բ��������޸�
	CP_VFD_PARA_INVALID = 5028u,                //����ֵ��Ч
	CP_VFD_PARA_BAK_ERR = 5041u,                //���������ļ����������ڴ�
    CP_VFD_PARA_UPLOAD_GIVE_UP = 5050u,			//���������ϴ�
    CP_VFD_PARA_UPLOAD_FAIL = 5052u,			//�����ϴ�ʧ��
    CP_VFD_PARA_DOWNLOAD_GIVE_UP = 5060u,		//����������װ
    CP_VFD_PARA_DOWNLOAD_FAIL = 5062u,			//������װʧ��
    CP_VFD_PARA_CRC_ERR = 5089u,				//��װʧ�ܣ���Ϊ CRC ����	
} ENUM_CP_ALARM_CODE;			                //�������� ( �����Ϳ����� )

typedef enum
{
    FB_STS_WORD_READY = 0x0001, //׼��
    FB_STS_WORD_PERMIT = 0x0002, //����
    FB_STS_WORD_START = 0x0004, //��
    FB_STS_WORD_RUN = 0x0008, //����
    FB_STS_WORD_ZERO_SPEED = 0x0010, //����
    FB_STS_WORD_ACLR = 0x0020, //����
    FB_STS_WORD_DELR = 0x0040, //����
    FB_STS_WORD_REACH_SET_POINT = 0x0080, //�ﵽ�趨��
    FB_STS_WORD_REACH_LIMIT = 0x0100, //�����޷�ֵ
    FB_STS_WORD_REACH_MONITOR = 0x0200, //�������ֵ
    FB_STS_WORD_REV_REF = 0x0400, //�������
    FB_STS_WORD_REV_RUN = 0x0800, //��������
    FB_STS_WORD_LOC_CTRL = 0x1000, //���ؿ���ģʽ
    FB_STS_WORD_BUS_CTRL = 0x2000, //���߿���ģʽ
    FB_STS_WORD_EXT2_EFF = 0x4000, //�ⲿ 2 ��Ч
    FB_STS_WORD_FAULT = 0x8000, //����
} ENUM_FB_STS_WORD1; //0303������״̬��1

typedef enum
{
    FB_STS_WORD_ALARM = 0x0001, //����
    FB_STS_WORD_ATTN = 0x0002, //ע��
    FB_STS_WORD_DIR_LOCK = 0x0004, //��������
    FB_STS_WORD_LOC_LOCK = 0x0008, //��������
    FB_STS_WORD_VECT_CTRL = 0x0010, //ʸ������ģʽ
    FB_STS_WORD_REV1 = 0x0020, //����
    FB_STS_WORD_REV2 = 0x0040, //����
    FB_STS_WORD_CTRL_SRC_COPY = 0x0080, //����Դ����
    FB_STS_WORD_REF1_COPY = 0x0100, //���� 1 ����
    FB_STS_WORD_REF2_COPY = 0x0200, //���� 2 ����
    FB_STS_WORD_CMD_WORD_COPY = 0x0400, //����������
    FB_STS_WORD_REF1_REQ = 0x0800, //���� 1 ����
    FB_STS_WORD_REF2_REQ = 0x1000, //���� 2 ����
    FB_STS_WORD_EXT_PID_REF2_REQ = 0x2000, //�ⲿ PID ���� 2 ����
    FB_STS_WORD_START_BAN_SIGNAL_EFF = 0x4000, //�𶯽�ֹ�ź���Ч
    FB_STS_WORD_STOP_SIGNAL_EFF = 0x8000, //ֹͣ�ź���Ч    
} ENUM_FB_STS_WORD2; //0304������״̬��2

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
