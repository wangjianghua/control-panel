/** 
 * @file     eeprom.c
 * @brief    AT24CXX驱动
 * @details  
 * @author   华兄
 * @email    jianghua.wang@foxmail.com
 * @date     2017
 * @version  vX.XX
 * @par Copyright (c):  
 *           华兄电子
 * @par History:          
 *   version: author, date, desc\n 
 */
#include "includes.h"


void IIC_Delay(u16 dly)
{
    u16 i;

    
	for(i = 0; i < dly; i++)
	{
   	    NOP();
	}
}

void IIC_Start(void) 
{
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
	IIC_SCL_LOW();
	IIC_Delay(20);    
}

u8 IIC_RecvByte(void) 
{
	u8 i, dat = 0;


	for(i = 0; i < 8; i++)
	{
		dat <<= 1;
        
		IIC_SCL_HIGH();
		IIC_Delay(20);
        
		if(IIC_SDA) 
		{
            dat |= 1;
		}
        
		IIC_SCL_LOW();
		IIC_Delay(20); 
	}

	IIC_SDA_HIGH();
	IIC_Delay(20);

	return (dat);
}

void IIC_SendByte(u8 dat) 
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
        
		IIC_SCL_HIGH();
		IIC_Delay(20); 
		IIC_SCL_LOW();
		IIC_Delay(20); 
	}

	IIC_SDA_HIGH();
	IIC_Delay(20);    
}

void IIC_WaitAck(void)
{
    u16 timeout = 0;

    
    IIC_SDA_HIGH(); 
    IIC_Delay(10);
	IIC_SCL_HIGH();
	IIC_Delay(20); 
    
	while(IIC_SDA)
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

void IIC_Ack(void)
{
	IIC_SDA_LOW();
	IIC_Delay(10);
	IIC_SCL_HIGH();
	IIC_Delay(20);
	IIC_SCL_LOW();
	IIC_Delay(20);
}

u8 IIC_ReadByte(u16 addr) 
{      
    u8 dat;

    
	IIC_Start();

    if(AT24CXX > AT24C16)
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE);
        IIC_WaitAck();
        IIC_SendByte(addr / 256);
    }
    else
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	IIC_WaitAck();
	IIC_SendByte(addr % 256);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(AT_DEV_ADDR_READ);
	IIC_WaitAck();
	dat = IIC_RecvByte();
	IIC_Stop();
    
 	return (dat); 
}  

void IIC_WriteByte(u16 addr, u8 dat)
{      
	IIC_Start();

    if(AT24CXX > AT24C16)
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE);
        IIC_WaitAck();
        IIC_SendByte(addr / 256);
    }
    else
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	IIC_WaitAck();
	IIC_SendByte(addr % 256);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

u16 IIC_ReadHalfWord(u16 addr)
{
    u8 dat1, dat2;
	u16 dat;


	IIC_Start();

    if(AT24CXX > AT24C16)
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE);
        IIC_WaitAck();
        IIC_SendByte(addr / 256);
    }
    else
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	IIC_WaitAck();
	IIC_SendByte(addr % 256);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(AT_DEV_ADDR_READ);
	IIC_WaitAck();
	dat1 = IIC_RecvByte();
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(AT_DEV_ADDR_READ);
	IIC_WaitAck();
	dat2 = IIC_RecvByte();
	IIC_Stop();
    
	dat = dat1 + (dat2 << 8);	
    
	return (dat);
}

void IIC_WriteHalfWord(u16 addr, u16 dat)
{
	IIC_Start();

    if(AT24CXX > AT24C16)
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE);
        IIC_WaitAck();
        IIC_SendByte(addr / 256);
    }
    else
    {
        IIC_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	IIC_WaitAck();
	IIC_SendByte(addr % 256);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_SendByte((dat >> 8) & 0xff);
	IIC_WaitAck();
	IIC_Stop();
}	

void IIC_Test(void)
{
    u8 byte = 0;
    u16 half_word = 0;


    led_disp_buf[5] |= LED_FWD_REV_MASK | LED_LOC_REM_MASK;
    LEDOE = 0;
    os_wait(K_TMO, 12, 0);
    
    byte = IIC_ReadByte(0);

    if(0x5a == byte)
    {
        led_disp_buf[5] &= ~LED_FWD_REV_MASK;
        LEDOE = 0;
    }
    else
    {
        IIC_WriteByte(0, 0x5a);
    }

    half_word = IIC_ReadHalfWord(AT24CXX - 2); 

    if(0x5a5a == half_word)
    {
        led_disp_buf[5] &= ~LED_LOC_REM_MASK;
        LEDOE = 0;
    }
    else
    {
        IIC_WriteHalfWord(AT24CXX - 2, 0x5a5a);
    }

    os_wait(K_TMO, 12, 0);

    while(1);
}

