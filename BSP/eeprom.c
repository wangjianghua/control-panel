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


void IIC_SDA_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}

void IIC_SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}

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
	IIC_Delay(50); 		 
	IIC_SDA_LOW();
	IIC_Delay(50); 
	IIC_SCL_LOW();
	IIC_Delay(50);
}

void IIC_Stop(void) 
{
	IIC_SDA_LOW();
	IIC_SCL_HIGH();
	IIC_Delay(50); 
	IIC_SDA_HIGH();
	IIC_Delay(50);
	IIC_SCL_LOW();
	IIC_Delay(50);    
}

u8 IIC_RecvByte(void) 
{
	u8 i, data = 0;


    IIC_SDA_Input();
    IIC_Delay(20);

	for(i = 0; i < 8; i++)
	{
		data <<= 1;
        
		IIC_SCL_HIGH();
		IIC_Delay(50);
        
		if(IIC_SDA) 
		{
            data |= 1;
		}
        
		IIC_SCL_LOW();
		IIC_Delay(50); 
	}

    IIC_SDA_Output();
    IIC_Delay(20);

	IIC_SDA_HIGH();
	IIC_Delay(50);

	return (data);
}

void IIC_SendByte(u8 data) 
{
	s8 i;

    
	for(i = 7; i >= 0; i--)
	{
		if((data >> i) & 0x01)
		{
			IIC_SDA_HIGH();
		}
		else	
		{
			IIC_SDA_LOW();
		}
        
		IIC_SCL_HIGH();
		IIC_Delay(50); 
		IIC_SCL_LOW();
		IIC_Delay(50); 
	}

	IIC_SDA_HIGH();
	IIC_Delay(50);    
}

void IIC_WaitAck(void)
{
    u16 timeout = 0;

    
    IIC_SDA_HIGH(); 
    IIC_Delay(20);
	IIC_SCL_HIGH();
	IIC_Delay(50); 

    IIC_SDA_Input();
    IIC_Delay(20);
    
	while(IIC_SDA)
	{
        timeout++;

        if(timeout > 1000)
        {
            break;
        }
    }

    IIC_SDA_Output();
    IIC_Delay(20);
    
	IIC_SCL_LOW();
	IIC_Delay(50);
}

void IIC_Ack(void)
{
	IIC_SDA_LOW();
	IIC_Delay(20);
	IIC_SCL_HIGH();
	IIC_Delay(50);
	IIC_SCL_LOW();
	IIC_Delay(50);
}

u8 IIC_ReadByte(u16 addr) 
{      
    u8 data;

    
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
	data = IIC_RecvByte();
	IIC_Stop();
    
 	return (data); 
}  

void IIC_WriteByte(u16 addr, u8 data)
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
	IIC_SendByte(data);
	IIC_WaitAck();
	IIC_Stop();
}

u16 IIC_ReadHalfWord(u16 addr)
{
    u8 data1, data2;
	u16 data;


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
	data1 = IIC_RecvByte();
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(AT_DEV_ADDR_READ);
	IIC_WaitAck();
	data2 = IIC_RecvByte();
	IIC_Stop();
    
	data = data1 + (data2 << 8);	
    
	return (data);
}

void IIC_WriteHalfWord(u16 addr, u16 data)
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
	IIC_SendByte(data);
	IIC_WaitAck();
	IIC_SendByte((data >> 8) & 0xff);
	IIC_WaitAck();
	IIC_Stop();
}	

#if (IIC_TEST_EN > 0u)
#define YEAR_ADDR               (AT24CXX - 2)
#define MONTH_ADDR              (AT24CXX - 3)
#define DAY_ADDR                (AT24CXX - 4)

void IIC_Test(void)
{
    u8 month = 0, day = 0;
    u16 year = 0;


    year = IIC_ReadHalfWord(YEAR_ADDR); 

    os_dly_wait(5); //5ms

    if(YEAR == year)
    {
        led_disp_buf[0] = led_table[year % 10 + 16];
        led_disp_buf[1] = (year > 9) ? (led_table[year % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (year > 99) ? (led_table[year % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (year > 999) ? (led_table[year % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (year > 9999) ? (led_table[year % 100000 / 10000 + 16]) : (0xff);
        LEDOE_ENABLE();

        os_dly_wait(500);
    }
    else
    {
        IIC_WriteHalfWord(YEAR_ADDR, YEAR);

        os_dly_wait(5); //5ms
    }
    
    month = IIC_ReadByte(MONTH_ADDR);

    os_dly_wait(5); //5ms

    if(MONTH == month)
    {
        led_disp_buf[4] = led_table[month % 100 / 10 + 16];
        led_disp_buf[3] = led_table[month % 10 + 16];
        led_disp_buf[2] = led_table['-' - 32];
    }
    else
    {
        IIC_WriteByte(MONTH_ADDR, MONTH);

        os_dly_wait(5); //5ms
    }

    day = IIC_ReadByte(DAY_ADDR);

    os_dly_wait(5); //5ms

    if(DAY == day)
    {
        led_disp_buf[1] = led_table[day % 100 / 10 + 16];
        led_disp_buf[0] = led_table[day % 10 + 16];
        LEDOE_ENABLE();

        os_dly_wait(500);
    }
    else
    {
        IIC_WriteByte(DAY_ADDR, DAY);

        os_dly_wait(5); //5ms
    }

    led_disp_buf[5] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[3] = 0xff;
    led_disp_buf[2] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[0] = 0xff;
    LEDOE_ENABLE();

    os_dly_wait(10);
}
#endif

