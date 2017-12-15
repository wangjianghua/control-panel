#include "includes.h"


XDATA u8 uart_tx_buf[UART_TX_LEN];
XDATA u8 uart_rx_buf[UART_RX_LEN];
XDATA u8 uart_tx_index = 0, uart_tx_count = 0;
XDATA u8 uart_rx_count = 0;
XDATA u16 uart_rx_timeout = 0;
XDATA bool uart_rx_complete = FALSE;

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

    USART_SendData(CP_UART, UART_TX_BUF[uart_tx_index++]);

    if(uart_tx_index < uart_tx_count)
    {
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
}

__task void AppTaskUart(void)
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

        os_dly_wait(10);
    }    
}

