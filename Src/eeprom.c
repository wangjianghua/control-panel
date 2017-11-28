#include "includes.h"


void IIC_Delay(u16 dly)
{
    u16 i;

    
	for(i = 0; i < dly; i++)
	{
   	    _nop_(); 
	}
}

void IIC_Start(void) 
{
	IIC_SCL_LOW();
	IIC_SDA_HIGH();
	IIC_SCL_HIGH();
	IIC_Delay(20); 		 
	IIC_SDA_LOW();
	IIC_Delay(20); 
	IIC_SCL_LOW();
	IIC_Delay(20);
}

void IIC_Stop(void) 
{
	IIC_SDA_LOW();
	IIC_SCL_HIGH();
	IIC_Delay(20); 
	IIC_SDA_HIGH();
	IIC_Delay(20);
}

u8 IIC_ReadByte(void) 
{
	u8 i, dat = 0;


	for(i = 0; i < 8; i++)
	{
		dat <<= 1;
		IIC_SCL_HIGH();
		IIC_Delay(20);
        
		if(SDA) 
		{
            dat |= 1;
		}
        
		IIC_Delay(20); 
		IIC_SCL_LOW();
		IIC_Delay(20); 
	}
		
	return (dat);
}

void IIC_WriteByte(u8 dat) 
{
	s8 i;

    
	for(i = 7; i >= 0; i--)
	{
		if(1 == ((dat >> i) & 1))
		{
			IIC_SDA_HIGH();
		}
		else	
		{
			IIC_SDA_LOW();
		}
        
		IIC_Delay(20); 
		IIC_SCL_HIGH();
		IIC_Delay(20); 
		IIC_SCL_LOW();
		IIC_Delay(20); 
	}
}

void IIC_Ack(void)
{
    u16 timeout = 0;

    
    IIC_SDA_HIGH(); 
    IIC_Delay(10);
	IIC_SCL_HIGH();
	IIC_Delay(20); 
    
	while(1 == SDA)
	{
        timeout++;

        if(timeout > 1000)
        {
            break;
        }
    }
    
	IIC_SCL_LOW();
	IIC_Delay(20);
}

u8 IIC_Read(u16 addr) 
{      
    u8 dat;
	u16 temp;

    
	temp = (((addr << 1) & 0x7ff) / 256) << 1;
	IIC_Start();
	IIC_WriteByte(0xa0 | temp);
	IIC_Ack();
	IIC_WriteByte((addr << 1) & 255);
	IIC_Ack();
	IIC_Start();
	IIC_WriteByte(0xa1 | temp);
	IIC_Ack();
	dat = IIC_ReadByte();
	IIC_Stop();
    IIC_Delay(20);
    
 	return (dat); 
}  

void IIC_Write(u16 addr, u8 dat)
{      
	u16 temp;

    
	temp = (((addr << 1) & 0x7ff) / 256) << 1;
	IIC_Start();
	IIC_WriteByte(0xa0 | temp);
	IIC_Ack();
	IIC_WriteByte((addr << 1) & 255);
	IIC_Ack();
	IIC_WriteByte(dat);
	IIC_Ack();
	IIC_Stop();
	IIC_Delay(20);

    os_wait(K_TMO, 10, 0);
}

u16 IIC_ReadHalfWord(u16 addr)
{
    u8 dat1, dat2;
	u16 dat, temp;

    
	temp = (((addr << 1) & 0x7ff) / 256) << 1;
	IIC_Start();
	IIC_WriteByte(0xa0 | temp);
	IIC_Ack();
	IIC_WriteByte((addr << 1) & 255);
	IIC_Ack();
	IIC_Start();
	IIC_WriteByte(0xa1 | temp);
	IIC_Ack();
	dat1 = IIC_ReadByte();
	IIC_Stop();
	IIC_Start();
	IIC_WriteByte(0xa1 | temp);
	IIC_Ack();
	dat2 = IIC_ReadByte();
	IIC_Stop();
    IIC_Delay(20);
    
	dat = dat1 + (dat2 << 8);	
    
	return (dat);
}

void IIC_WriteHalfWord(u16 addr, u16 dat)
{
	u16 temp;

    
	temp = (((addr << 1) & 0x7ff) / 256) << 1;
	IIC_Start();
	IIC_WriteByte(0xa0 | temp);
	IIC_Ack();
	IIC_WriteByte((addr << 1) & 255);
	IIC_Ack();
	IIC_WriteByte(dat);
	IIC_Ack();
	IIC_WriteByte((dat >> 8) & 0xff);
	IIC_Ack();
	IIC_Stop();
	IIC_Delay(20);	

	os_wait(K_TMO, 10, 0);
}	

void IIC_Test(void)
{
    u8 byte = 0;
    u16 half_word = 0;


    led_disp_buf[5] |= LED_FWD_REV_MASK | LED_LOC_REM_MASK;
    LEDOE = 0;
    os_wait(K_TMO, 10, 0);
    
    byte = IIC_Read(0);

    if(0xa5 == byte)
    {
        led_disp_buf[5] &= ~LED_FWD_REV_MASK;
        LEDOE = 0;
    }
    else
    {
        IIC_Write(0, 0xa5);
    }

    half_word = IIC_ReadHalfWord(AT24C16 - 1); 

    if(0xa5a5 == half_word)
    {
        led_disp_buf[5] &= ~LED_LOC_REM_MASK;
        LEDOE = 0;
    }
    else
    {
        IIC_WriteHalfWord(AT24C16 - 1, 0xa5a5);
    }

    while(1);
}

