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
    /* 十六进制、大写、右对齐、4位宽度、不足补0
     * 例: 2017 -> 07E2
     * 华兄 */
    sprintf(str, "%04X", data);
}

