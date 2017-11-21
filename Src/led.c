#include "includes.h"


CODE u8 led_table[] = {
    0xff,0xf7,0xdf,0xfd,0xfb, //sp,!,",#,$,
    0x7f,0xbf,0xef,0xfe,0xff, //%,&,',(,),
    0xff,0xff,0xff,0xbf,0xfe, //*,+,,,-,.,
    0xff,0x41,0xf9,0x83,0xA1, ///,0,1,2,3,
    0x39,0x25,0x05,0xf1,0x01, //4,5,6,7,8,
    0x21,0xff,0xff,0xff,0xff, //9,:,;,<,=,
    0xff,0xff,0xff,0x11,0x0d, //>,?,@,A,B,
    0x47,0x89,0x07,0x17,0x45, //C,D,E,F,G,
    0x19,0xf9,0xff,0xff,0x4f, //H,I,J,K,L,
    0xff,0x51,0x41,0x13,0xff, //M,N,O,P,Q,
    0x57,0x25,0x57,0x49,0x49, //R,S,T,U,V,
    0xff,0xff,0xff,0xff,0xff, //W,X,Y,Z,[,
    0xff,0xff,0xff,0xff,0xff, //\,],^,_,',
    0xff,0x0d,0x8f,0x89,0xff, //a,b,c,d,e,
    0xff,0x45,0x1d,0xf9,0xff, //f,g,h,i,j,	 
    0xff,0xcf,0xff,0x9d,0x8d, //k,l,m,n,o,
    0xff,0xff,0x9f,0xff,0xff, //p,q,r,s,t,
    0xcd,0xcd,0xff,0xff,0xff, //u,v,w,x,y,
    0xff,0xff,0xff,0xff,0xff  //z,{,|,},~,
};

XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};   

XDATA u8 led_blink_pos = 0;
XDATA u8 blink_led = 0;

void led_display(u8 com, u8 disp_data)
{
    u8 i;

     
    LEDX0 = 1;
    LEDX1 = 1;
    LEDX2 = 1;
    LEDX3 = 1;
    LEDX4 = 1;	
    LEDX5 = 1;  
    
    for(i = 0; i < 6; i++);
    
    LED_DISP_PORT = disp_data;
    
    switch(com)
    {
    case 0: 
        LEDX0 = 0;
        break;
        
    case 1: 
        LEDX1 = 0;
        break;
        
    case 2: 
        LEDX2 = 0;
        break;
        
    case 3: 
        LEDX3 = 0;
        break;
        
    case 4: 
        LEDX4 = 0;
        break;
        
    case 5: 
        LEDX5 = 0;
        break;
        
    default: 
        break;
    } 
}

void DispTask(void) _task_ DISP_TASK  
{
    u8 i;

    
    while(1)
    {           
    	for(i = 0; i < LED_BLINK_TIME; i++)
    	{

    		led_display(0, led_disp_buf[0]);	
    		os_wait(K_TMO, LED_DISP_TIME, 0);

    		led_display(1, led_disp_buf[1]);
    		os_wait(K_TMO, LED_DISP_TIME, 0);

    		led_display(2, led_disp_buf[2]);
    		os_wait(K_TMO, LED_DISP_TIME, 0);

    		led_display(3, led_disp_buf[3]);
    		os_wait(K_TMO, LED_DISP_TIME, 0);	

    		led_display(4, led_disp_buf[4]);
    		os_wait(K_TMO, LED_DISP_TIME, 0);	
    	
    		led_display(5, led_disp_buf[5]);		
    		os_wait(K_TMO, LED_DISP_TIME, 0);
    	}
        
    	for(i = 0; i < LED_BLINK_TIME; i++)
    	{
    		if(led_blink_pos > 5)
    		{
    			LEDX0 = 1;
                LEDX1 = 1;
                LEDX2 = 1;
                LEDX3 = 1;
                LEDX4 = 1;	
                LEDX5 = 1;
                
    			os_wait(K_TMO, 5 * LED_DISP_TIME, 0);			
    		}
    		else
    		{
                if(1 == led_blink_pos)
                {
                    LEDX0 = 1;
                    LEDX1 = 1;
                    LEDX2 = 1;
                    LEDX3 = 1;
                    LEDX4 = 1;	
                    LEDX5 = 1;
                    
                    os_wait(K_TMO, LED_DISP_TIME, 0);					  
                }
                else
                {
                    led_display(0, led_disp_buf[0]);
                    
                    os_wait(K_TMO, LED_DISP_TIME, 0);
                }

                if(2 == led_blink_pos)
                {
                    LEDX0 = 1;
                    LEDX1 = 1;
                    LEDX2 = 1;
                    LEDX3 = 1;
                    LEDX4 = 1;	
                    LEDX5 = 1;

                    os_wait(K_TMO, LED_DISP_TIME, 0);	
                }
                else
                {
                    led_display(1, led_disp_buf[1]);

                    os_wait(K_TMO, LED_DISP_TIME, 0);
                }

                if(3 == led_blink_pos)
                {	
                    LEDX0 = 1;
                    LEDX1 = 1;
                    LEDX2 = 1;
                    LEDX3 = 1;
                    LEDX4 = 1;	
                    LEDX5 = 1;

                    os_wait(K_TMO, LED_DISP_TIME, 0);															  
                }
                else
                {
                    led_display(2, led_disp_buf[2]);

                    os_wait(K_TMO, LED_DISP_TIME, 0);
                }

                if(4 == led_blink_pos)
                {
                    LEDX0 = 1;
                    LEDX1 = 1;
                    LEDX2 = 1;
                    LEDX3 = 1;
                    LEDX4 = 1;	
                    LEDX5 = 1;

                    os_wait(K_TMO, LED_DISP_TIME, 0);	
                }
                else
                {
                    led_display(3, led_disp_buf[3]);

                    os_wait(K_TMO, LED_DISP_TIME, 0);
                }
                
                if(5 == led_blink_pos)
                {
                    LEDX0 = 1;
                    LEDX1 = 1;
                    LEDX2 = 1;
                    LEDX3 = 1;
                    LEDX4 = 1;	
                    LEDX5 = 1;

                    os_wait(K_TMO, LED_DISP_TIME, 0);	
                }
                else
                {
                    led_display(4, led_disp_buf[4]);
                    
                    os_wait(K_TMO, LED_DISP_TIME, 0);
                }
    		}

            led_display(5, led_disp_buf[5] | blink_led);
            
            os_wait(K_TMO, LED_DISP_TIME, 0);
    	}
    }
}

