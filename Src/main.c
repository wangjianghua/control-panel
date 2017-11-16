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

    LED_BUFF[4] = led_tab['H' - 32];
    LED_BUFF[3] = led_tab['E' - 32];
    LED_BUFF[2] = led_tab['L' - 32];
    LED_BUFF[1] = led_tab['L' - 32];
    LED_BUFF[0] = led_tab['O' - 32];
    LEDOE = 0;
}

void StartTask(void) _task_ START_TASK  
{
    init_system();
    
    os_create_task(LED_TASK);	 
    os_create_task(KEY_TASK);	 
    os_create_task(UART_TASK);
    os_create_task(CP_TASK);

    os_delete_task(START_TASK);
}

void Timer1(void) interrupt 3 using 2
{	
	TH1 = 0xff;
	TL1 = 0x6a;
}

