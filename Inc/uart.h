#ifndef __UART_H__
#define __UART_H__


#define UART_MAX_LEN        150u
#define UART_RX_TIMEOUT     (30u / SYS_TICK_PERIOD)

#define UART_TX_BUF         uart_tx_buf
#define UART_TX_LEN         UART_MAX_LEN

#define UART_RX_BUF         uart_rx_buf
#define UART_RX_LEN         UART_MAX_LEN

/* ���ڵ���
 * ���� */
#define ECHO()                                                 \         
        if(0 != uart_rx_count)                                 \
        {                                                      \
            memcpy(UART_TX_BUF, UART_RX_BUF, uart_rx_count);   \
            uart_send(uart_rx_count);                          \
        }                                                      \

extern XDATA u8 uart_tx_buf[UART_TX_LEN];
extern XDATA u8 uart_rx_buf[UART_RX_LEN];
extern XDATA u8 uart_tx_index, uart_tx_count;
extern XDATA u8 uart_rx_count;
extern XDATA u16 uart_rx_timeout;
extern XDATA bool uart_rx_complete;

extern void UartInit_9600bps(void);
extern void UartInit_19200bps(void);
extern void UartInit_57600bps(void);
extern void init_uart(void);
extern void uart_recv_clear(void);
extern void uart_send(u8 len);


#endif
