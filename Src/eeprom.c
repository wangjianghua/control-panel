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
		if((dat >> i) & 0x01)
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
    u8 i = 0, month = 0, day = 0;
    u16 year = 0;


    year = IIC_ReadHalfWord(AT24CXX - 2); 

    os_wait(K_TMO, 12, 0); //5ms

    if(YEAR == year)
    {
        led_disp_buf[0] = led_table[year % 10 + 16];
        led_disp_buf[1] = (year > 9) ? (led_table[year % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (year > 99) ? (led_table[year % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (year > 999) ? (led_table[year % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (year > 9999) ? (led_table[year % 100000 / 10000 + 16]) : (0xff);
        LEDOE = 0;

        for(i = 0; i < 50; i++)
        {
            os_wait(K_TMO, 25, 0);
        }
    }
    else
    {
        IIC_WriteHalfWord(AT24CXX - 2, YEAR);

        os_wait(K_TMO, 12, 0); //5ms
    }
    
    month = IIC_ReadByte(AT24CXX - 3);

    os_wait(K_TMO, 12, 0); //5ms

    if(MONTH == month)
    {
        led_disp_buf[4] = led_table[month % 100 / 10 + 16];
        led_disp_buf[3] = led_table[month % 10 + 16];
        led_disp_buf[2] = led_table['-' - 32];
    }
    else
    {
        IIC_WriteByte(AT24CXX - 3, MONTH);

        os_wait(K_TMO, 12, 0); //5ms
    }

    day = IIC_ReadByte(AT24CXX - 4);

    os_wait(K_TMO, 12, 0); //5ms

    if(DAY == day)
    {
        led_disp_buf[1] = led_table[day % 100 / 10 + 16];
        led_disp_buf[0] = led_table[day % 10 + 16];
        LEDOE = 0;

        for(i = 0; i < 50; i++)
        {
            os_wait(K_TMO, 25, 0);
        }
    }
    else
    {
        IIC_WriteByte(AT24CXX - 4, DAY);

        os_wait(K_TMO, 12, 0); //5ms
    }

    led_disp_buf[5] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[3] = 0xff;
    led_disp_buf[2] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[0] = 0xff;
    LEDOE = 0;

    os_wait(K_TMO, 25, 0);
}

