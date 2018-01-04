#include "includes.h"


u8 get_data_length(u32 data)
{
	u8 len;
	

    if(0 == data)
    {
        len = 1;
    }
    else
    {
    	for(len = 0; 0 != data; len++)
    	{
    		data /= 10;
    	}
    }
	
	return (len);
}

void int_to_hex(char *str, u16 data)
{
    /* ʮ�����ơ���д���Ҷ��롢4λ��ȡ����㲹0
     * ��: 2017 -> 07E2
     * ���� */
    sprintf(str, "%04X", data);
}

void int_to_bin(char *buf, u16 data)
{
    u8 i;

    
    for(i = 0; data > 0; i++)
    {
        buf[i] = (data % 2) + '0';
        
        data /= 2;
    }
}

