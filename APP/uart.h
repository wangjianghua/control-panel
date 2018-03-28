#ifndef __UART_H__
#define __UART_H__


#define CP_UART             USART1

#define UART_RX_TIMEOUT     (30u / 10u)

#define UART_TX_BUF         uart_tx_buf
#define MAX_UART_TX_LEN     MAX_BUF_LEN

#define UART_RX_BUF         uart_rx_buf
#define MAX_UART_RX_LEN     MAX_BUF_LEN

extern OS_SEM uart_sem;
extern XDATA u8 uart_tx_buf[MAX_UART_TX_LEN];
extern XDATA u8 uart_rx_buf[MAX_UART_RX_LEN];
extern XDATA u8 uart_tx_index, uart_tx_total;
extern XDATA u8 uart_rx_index, uart_rx_timeout;

void uart_recv_clear(void);
void uart_send(u8 *buf, u8 len);
__task void AppTaskUart(void);


#endif
