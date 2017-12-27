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


void EEPROM_SDA_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = EEPROM_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);
}

void EEPROM_SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = EEPROM_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);
}

void EEPROM_Delay(u16 dly)
{
    u16 i;

    
	for(i = 0; i < dly; i++)
	{
   	    NOP();
	}
}

void EEPROM_Start(void) 
{
	EEPROM_SDA_HIGH();
	EEPROM_SCL_HIGH();
	EEPROM_Delay(50); 		 
	EEPROM_SDA_LOW();
	EEPROM_Delay(50); 
	EEPROM_SCL_LOW();
	EEPROM_Delay(50);
}

void EEPROM_Stop(void) 
{
	EEPROM_SDA_LOW();
	EEPROM_SCL_HIGH();
	EEPROM_Delay(50); 
	EEPROM_SDA_HIGH();
	EEPROM_Delay(50);
	EEPROM_SCL_LOW();
	EEPROM_Delay(50);    
}

u8 EEPROM_RecvByte(void) 
{
	u8 i, data = 0;


    EEPROM_SDA_Input();
    EEPROM_Delay(20);

	for(i = 0; i < 8; i++)
	{
		data <<= 1;
        
		EEPROM_SCL_HIGH();
		EEPROM_Delay(50);
        
		if(EEPROM_SDA) 
		{
            data |= 1;
		}
        
		EEPROM_SCL_LOW();
		EEPROM_Delay(50); 
	}

    EEPROM_SDA_Output();
    EEPROM_Delay(20);

	EEPROM_SDA_HIGH();
	EEPROM_Delay(50);

	return (data);
}

void EEPROM_SendByte(u8 data) 
{
	s8 i;

    
	for(i = 7; i >= 0; i--)
	{
		if((data >> i) & 0x01)
		{
			EEPROM_SDA_HIGH();
		}
		else	
		{
			EEPROM_SDA_LOW();
		}
        
		EEPROM_SCL_HIGH();
		EEPROM_Delay(50); 
		EEPROM_SCL_LOW();
		EEPROM_Delay(50); 
	}

	EEPROM_SDA_HIGH();
	EEPROM_Delay(50);    
}

void EEPROM_WaitAck(void)
{
    u16 timeout = 0;

    
    EEPROM_SDA_HIGH(); 
    EEPROM_Delay(20);
	EEPROM_SCL_HIGH();
	EEPROM_Delay(50); 

    EEPROM_SDA_Input();
    EEPROM_Delay(20);
    
	while(EEPROM_SDA)
	{
        timeout++;

        if(timeout > 1000)
        {
            break;
        }
    }

    EEPROM_SDA_Output();
    EEPROM_Delay(20);
    
	EEPROM_SCL_LOW();
	EEPROM_Delay(50);
}

void EEPROM_Ack(void)
{
	EEPROM_SDA_LOW();
	EEPROM_Delay(20);
	EEPROM_SCL_HIGH();
	EEPROM_Delay(50);
	EEPROM_SCL_LOW();
	EEPROM_Delay(50);
}

u8 EEPROM_ReadByte(u16 addr) 
{      
    u8 data;

    
	EEPROM_Start();

    if(AT24CXX > AT24C16)
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE);
        EEPROM_WaitAck();
        EEPROM_SendByte(addr / 256);
    }
    else
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	EEPROM_WaitAck();
	EEPROM_SendByte(addr % 256);
	EEPROM_WaitAck();
	EEPROM_Start();
	EEPROM_SendByte(AT_DEV_ADDR_READ);
	EEPROM_WaitAck();
	data = EEPROM_RecvByte();
	EEPROM_Stop();
    
 	return (data); 
}  

void EEPROM_WriteByte(u16 addr, u8 data)
{      
	EEPROM_Start();

    if(AT24CXX > AT24C16)
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE);
        EEPROM_WaitAck();
        EEPROM_SendByte(addr / 256);
    }
    else
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	EEPROM_WaitAck();
	EEPROM_SendByte(addr % 256);
	EEPROM_WaitAck();
	EEPROM_SendByte(data);
	EEPROM_WaitAck();
	EEPROM_Stop();
}

u16 EEPROM_ReadHalfWord(u16 addr)
{
    u8 data1, data2;
	u16 data;


	EEPROM_Start();

    if(AT24CXX > AT24C16)
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE);
        EEPROM_WaitAck();
        EEPROM_SendByte(addr / 256);
    }
    else
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	EEPROM_WaitAck();
	EEPROM_SendByte(addr % 256);
	EEPROM_WaitAck();
	EEPROM_Start();
	EEPROM_SendByte(AT_DEV_ADDR_READ);
	EEPROM_WaitAck();
	data1 = EEPROM_RecvByte();
	EEPROM_Stop();
	EEPROM_Start();
	EEPROM_SendByte(AT_DEV_ADDR_READ);
	EEPROM_WaitAck();
	data2 = EEPROM_RecvByte();
	EEPROM_Stop();
    
	data = data1 + (data2 << 8);	
    
	return (data);
}

void EEPROM_WriteHalfWord(u16 addr, u16 data)
{
	EEPROM_Start();

    if(AT24CXX > AT24C16)
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE);
        EEPROM_WaitAck();
        EEPROM_SendByte(addr / 256);
    }
    else
    {
        EEPROM_SendByte(AT_DEV_ADDR_WRITE + ((addr / 256) << 1));
    }

	EEPROM_WaitAck();
	EEPROM_SendByte(addr % 256);
	EEPROM_WaitAck();
	EEPROM_SendByte(data);
	EEPROM_WaitAck();
	EEPROM_SendByte((data >> 8) & 0xff);
	EEPROM_WaitAck();
	EEPROM_Stop();
}	

#if (EEPROM_TEST_EN > 0u)
#define YEAR_ADDR               (AT24CXX - 2)
#define MONTH_ADDR              (AT24CXX - 3)
#define DAY_ADDR                (AT24CXX - 4)

void EEPROM_Test(void)
{
    u8 month = 0, day = 0;
    u16 year = 0;


    year = EEPROM_ReadHalfWord(YEAR_ADDR); 

    os_dly_wait(5); //5ms

    if(YEAR == year)
    {
        led_disp_buf[0] = led_table[year % 10 + 16];
        led_disp_buf[1] = (year > 9) ? (led_table[year % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (year > 99) ? (led_table[year % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (year > 999) ? (led_table[year % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (year > 9999) ? (led_table[year % 100000 / 10000 + 16]) : (0xff);
        LED_OE_ENABLE();

        os_dly_wait(500);
    }
    else
    {
        EEPROM_WriteHalfWord(YEAR_ADDR, YEAR);

        os_dly_wait(5); //5ms
    }
    
    month = EEPROM_ReadByte(MONTH_ADDR);

    os_dly_wait(5); //5ms

    if(MONTH == month)
    {
        led_disp_buf[4] = led_table[month % 100 / 10 + 16];
        led_disp_buf[3] = led_table[month % 10 + 16];
        led_disp_buf[2] = led_table['-' - 32];
    }
    else
    {
        EEPROM_WriteByte(MONTH_ADDR, MONTH);

        os_dly_wait(5); //5ms
    }

    day = EEPROM_ReadByte(DAY_ADDR);

    os_dly_wait(5); //5ms

    if(DAY == day)
    {
        led_disp_buf[1] = led_table[day % 100 / 10 + 16];
        led_disp_buf[0] = led_table[day % 10 + 16];
        LED_OE_ENABLE();

        os_dly_wait(500);
    }
    else
    {
        EEPROM_WriteByte(DAY_ADDR, DAY);

        os_dly_wait(5); //5ms
    }

    led_disp_buf[5] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[3] = 0xff;
    led_disp_buf[2] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[0] = 0xff;
    LED_OE_ENABLE();

    os_dly_wait(10);
}
#endif

