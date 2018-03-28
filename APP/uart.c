#include "includes.h"


OS_SEM uart_sem;

XDATA u8 uart_tx_buf[MAX_UART_TX_LEN];
XDATA u8 uart_rx_buf[MAX_UART_RX_LEN];
XDATA u8 uart_tx_index = 0, uart_tx_total = 0;
XDATA u8 uart_rx_index = 0, uart_rx_timeout = 0;

void uart_recv_clear(void)
{
    uart_rx_index = 0;

    while(OS_R_TMO != os_sem_wait(&uart_sem, 0));
}

void uart_send(u8 *buf, u8 len)
{    
    uart_recv_clear();

    if(len > MAX_UART_TX_LEN)
    {
        return;
    }

    memcpy(UART_TX_BUF, buf, len);
    
    uart_tx_total = len;
    
    uart_tx_index = 0;

    USART_SendData(CP_UART, UART_TX_BUF[uart_tx_index++]);

    if(uart_tx_index < uart_tx_total)
    {
        USART_ITConfig(CP_UART, USART_IT_TXE, ENABLE);
    }
}

__task void AppTaskUart(void)
{
    while(1)
    {
        if(0 != uart_rx_timeout)
        {
            uart_rx_timeout--;

            if(0 == uart_rx_timeout) //串口接收完成
            {                
                os_sem_send(&uart_sem);
            }
        }

        os_dly_wait(10);
    }    
}

