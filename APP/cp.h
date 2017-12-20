#ifndef __CP_H__
#define __CP_H__


#define MAX_REF_VAL             (500u * 100u)

#define VFD_LOC                 TRUE  //��ʾ��Ƶ�����ڱ��ؿ��ƣ��������������Կ�����
#define VFD_REM                 FALSE //��ʾ��Ƶ������Զ�̿��ƣ�����I/O (X1)���ֳ�����

#define VFD_REV                 TRUE  //��ת
#define VFD_FWD                 FALSE //��ת

#define VFD_REPLY_TIMEOUT       1000u //1s

#define VFD_PARA_FLAG           ('H')                       //��Ƶ��������־

#define CP_PARA_ADDR            0u                          //�����̲����洢��ַ

#define VFD_PARA_FLAG_ADDR      (AT24CXX - (3u * 1024u))    //1B, ��Ƶ��������־�洢��ַ
#define VFD_PARA_LEN_ADDR       (VFD_PARA_FLAG_ADDR + 1u)   //2B, ��Ƶ���������ȴ洢��ַ
#define VFD_PARA_ADDR           (VFD_PARA_LEN_ADDR + 2u)    //nB, ��Ƶ�������洢��ַ

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
	u16 ref; //����ֵ
} CP_PARA_ROM, *P_CP_PARA_ROM;

typedef struct _cp_para_ram 
{
    bool clr_err; //�������
    bool lr; //����Զ�̲�����ʽ������LOC��Զ��REM
    bool run; //����
    bool stop; //ֹͣ
    bool ref_chang; //�������ı�־
    bool fr; //��ת״̬����תFWD����תREV  
    bool func_code_visible; //���ع�������ӱ�־
    bool disp_para_sign[3]; //��������ʾ��������λ
    bool disp_para_enum[3]; //��������ʾ����ö��λ
    u8 cmd; //������
	u8 group; //��Ƶ������
	u8 grade; //��Ƶ���ּ�
	u8 rate; //����
	u8 vfd_para_crc; //��Ƶ��������ȡУ����
    u8 group_nearby[4]; //��ǰ�������
    u8 grade_nearby[8]; //��ǰ��������
    u8 vfd_para_unit; //��Ƶ��������λ
    u8 disp_para_unit[3]; //��������ʾ������λ
    u8 disp_para_dcl[3]; //��������ʾ����С��λ
    u8 vfd_para_shift; //��Ƶ������λ�л�
    u16 count; //������
    u16 ref; //��Ƶ������ֵ
    u16 ref_mod; //��������ֵ
    u16 disp_para_val[3]; //��������ʾ����ֵ
    u16 disp_para_type[3]; //��������ʾ��������
    u16 vfd_para_val; //��Ƶ������ֵ
    u16 vfd_para_count; //��Ƶ��������ǰ��Ŀ
    u16 vfd_para_total; //��Ƶ����������Ŀ
    u16 vfd_para_index; //��Ƶ��������ȡ����
    u16 fault_code; //������
    u16 alarm_code; //������
} CP_PARA_RAM, *P_CP_PARA_RAM;

extern OS_SEM cp_sem;
extern XDATA CP_PARA_ROM cp_para_rom;
extern XDATA CP_PARA_RAM cp_para_ram;

__task void AppTaskCP(void);


#endif
