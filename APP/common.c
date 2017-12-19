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

