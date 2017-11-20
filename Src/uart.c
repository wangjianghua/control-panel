#include "includes.h"


xdata u8 uart_tx_buf[UART_TX_LEN];
xdata u8 uart_rx_buf[UART_RX_LEN];
xdata u8 uart_tx_index = 0, uart_tx_count = 0;
xdata u8 uart_rx_count = 0;
xdata u16 uart_rx_timeout = 0;
xdata bool uart_rx_complete = FALSE;

/* ��������
 * ������: 9600bps
 * ����λ: 8λ
 * У��λ: ��
 * ֹͣλ: 1λ
 * ���� */
void UartInit_9600bps(void) //9600bps@11.0592MHz
{
    PCON |= 0x80;           //ʹ�ܲ����ʱ���λSMOD
    SCON = 0x50;            //8λ����,�ɱ䲨����
    AUXR &= 0xFB;           //���������ʷ�����ʱ��ΪFosc/12,��12T
    BRT = 0xFA;             //�趨���������ʷ�������װֵ
    AUXR |= 0x01;           //����1ѡ����������ʷ�����Ϊ�����ʷ�����
    AUXR |= 0x10;           //�������������ʷ�����

    IE |= 0x9a;    
}

/* ��������
 * ������: 57600bps
 * ����λ: 8λ
 * У��λ: ��
 * ֹͣλ: 1λ
 * ���� */
void UartInit_57600bps(void) //57600bps@11.0592MHz
{
	PCON |= 0x80;		     //ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		     //8λ����,�ɱ䲨����
	AUXR |= 0x04;		     //���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xF4;		         //�趨���������ʷ�������װֵ
	AUXR |= 0x01;		     //����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		     //�������������ʷ�����

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
        
        uart_rx_timeout = UART_RX_TIMEOUT; //��ʱ�жϴ��ڽ������

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
            else //���ڽ������
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

