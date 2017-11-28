#include "includes.h"


void init_system(void)
{
    P4SW |= 0x70;
    AUXR |= 0x91;
    AUXR1 = 0X80;
    IPH = 0X08;
    IP = 0X08;
    //TR1 = 1;

    init_uart();

    led_disp_buf[5] = 0x00;
    led_disp_buf[4] = led_table['8' - 32];
    led_disp_buf[3] = led_table['8' - 32];
    led_disp_buf[2] = led_table['8' - 32];
    led_disp_buf[1] = led_table['8' - 32];
    led_disp_buf[0] = led_table['8' - 32];
    LEDOE = 0;
}

void StartTask(void) _task_ START_TASK  
{
    init_system();
    
    os_create_task(DISP_TASK);	 
    os_create_task(KEY_TASK);	 
    os_create_task(UART_TASK);
    os_create_task(CP_TASK);

    os_delete_task(START_TASK);
}

void Timer1_ISR(void) interrupt 3 using 2
{	
	TH1 = 0xff;
	TL1 = 0x6a;
}

