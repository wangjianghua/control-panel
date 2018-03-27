#ifndef __UART_H__
#define __UART_H__


#define CP_UART             USART1

#define UART_MAX_LEN        128u
#define UART_RX_TIMEOUT     (30u / 10u)

#define UART_TX_BUF         uart_tx_buf
#define UART_TX_LEN         UART_MAX_LEN

#define UART_RX_BUF         uart_rx_buf
#define UART_RX_LEN         UART_MAX_LEN

/* ´®¿Úµ÷ÊÔ
 * »ªÐÖ */
#define ECHO()                                                 \
        if(0 != uart_rx_index)                                 \
        {                                                      \
            memcpy(UART_TX_BUF, UART_RX_BUF, uart_rx_index);   \
            uart_send(uart_rx_index);                          \
        }                                                      \

extern OS_SEM uart_sem;
extern XDATA u8 uart_tx_buf[UART_TX_LEN];
extern XDATA u8 uart_rx_buf[UART_RX_LEN];
extern XDATA u8 uart_tx_index, uart_tx_count;
extern XDATA u8 uart_rx_index;
extern XDATA u16 uart_rx_timeout;

void uart_recv_clear(void);
void uart_send(u8 len);
__task void AppTaskUart(void);


#endif
