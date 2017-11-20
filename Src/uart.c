#include "includes.h"


xdata u8 uart_tx_buf[UART_TX_LEN];
xdata u8 uart_rx_buf[UART_RX_LEN];
xdata u8 uart_tx_index = 0, uart_tx_count = 0;
xdata u8 uart_rx_count = 0;
xdata u16 uart_rx_timeout = 0;
xdata bool uart_rx_complete = FALSE;

/* 【参数】
 * 波特率: 9600bps
 * 数据位: 8位
 * 校验位: 无
 * 停止位: 1位
 * 华兄 */
void UartInit_9600bps(void) //9600bps@11.0592MHz
{
    PCON |= 0x80;           //使能波特率倍速位SMOD
    SCON = 0x50;            //8位数据,可变波特率
    AUXR &= 0xFB;           //独立波特率发生器时钟为Fosc/12,即12T
    BRT = 0xFA;             //设定独立波特率发生器重装值
    AUXR |= 0x01;           //串口1选择独立波特率发生器为波特率发生器
    AUXR |= 0x10;           //启动独立波特率发生器

    IE |= 0x9a;    
}

/* 【参数】
 * 波特率: 57600bps
 * 数据位: 8位
 * 校验位: 无
 * 停止位: 1位
 * 华兄 */
void UartInit_57600bps(void) //57600bps@11.0592MHz
{
	PCON |= 0x80;		     //使能波特率倍速位SMOD
	SCON = 0x50;		     //8位数据,可变波特率
	AUXR |= 0x04;		     //独立波特率发生器时钟为Fosc,即1T
	BRT = 0xF4;		         //设定独立波特率发生器重装值
	AUXR |= 0x01;		     //串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		     //启动独立波特率发生器

    IE |= 0x9a;
}

void init_uart(void)
{ 
#if 0    
    SCON  = 0x50;
    TMOD |= 0x11; 
    BRT   = 0xfd;
    PCON |= 0x80;
    TH1   = 0xfc;
    TL1   = 0x18;
    IE   |= 0x9a;
#else
    UartInit_9600bps();
#endif
}

void Serial_TRx(void) interrupt 4 using 3 
{ 
    if(RI)   
    { 
        RI = 0;

        uart_rx_complete = FALSE;
        
        uart_rx_timeout = UART_RX_TIMEOUT; //超时判断串口接收完成

        UART_RX_BUF[uart_rx_count++] = SBUF;

        uart_rx_count %= UART_RX_LEN;
    }
    else
    {
        TI = 0;
        
        if(uart_tx_index < uart_tx_count)
        {
            SBUF = UART_TX_BUF[uart_tx_index++];
        }
        else
        {
            uart_tx_index = 0;
            uart_tx_count = 0;
        }
    }
}

void UartTask(void) _task_ UART_TASK  
{
    while(1)
    {
        if(0 != uart_rx_count)
        {
            if(uart_rx_timeout)
            {
                uart_rx_timeout--;
            }
            else //串口接收完成
            {                
                uart_rx_complete = TRUE;
            }
        }
        else
        {
            uart_rx_timeout = 0;
        }

        os_wait(K_TMO, 5, 0);
    }    
}

void uart_recv_clear(void)
{
    uart_rx_count = 0;
    uart_rx_complete = FALSE;
}

void uart_send(u8 len)
{    
    uart_recv_clear();
    
    uart_tx_count = len;
    
    uart_tx_index = 0;
    SBUF = UART_TX_BUF[uart_tx_index++];
}

