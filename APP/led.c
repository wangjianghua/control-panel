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
    0xcd,0xcd,0xff,0xff,0x29, //u,v,w,x,y,
    0xff,0xff,0xff,0xff,0xff  //z,{,|,},~,
};

XDATA u8 led_disp_buf[LED_DISP_BUF_SIZE] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; 

XDATA u8 led_blink_pos = 0;
XDATA u8 blink_led = 0;

void led_display(u8 com, u8 data)
{
    LEDX0_DISABLE();
    LEDX1_DISABLE();
    LEDX2_DISABLE();
    LEDX3_DISABLE();
    LEDX4_DISABLE();	
    LEDX5_DISABLE();  
    
    os_dly_wait(LED_DISP_TIME);
    
    GPIO_OutputByte(LED_DISP_PORT, data);
    
    switch(com)
    {
    case 0: 
        LEDX0_ENABLE();
        break;
        
    case 1: 
        LEDX1_ENABLE();
        break;
        
    case 2: 
        LEDX2_ENABLE();
        break;
        
    case 3: 
        LEDX3_ENABLE();
        break;
        
    case 4: 
        LEDX4_ENABLE();
        break;
        
    case 5: 
        LEDX5_ENABLE();
        break;
        
    default: 
        break;
    } 
}

__task void AppTaskDisp(void)
{
    u8 i;

    
    while(1)
    {           
    	for(i = 0; i < LED_BLINK_TIME; i++)
    	{

    		led_display(0, led_disp_buf[0]);	
    		os_dly_wait(LED_DISP_TIME);

    		led_display(1, led_disp_buf[1]);
    		os_dly_wait(LED_DISP_TIME);

    		led_display(2, led_disp_buf[2]);
    		os_dly_wait(LED_DISP_TIME);

    		led_display(3, led_disp_buf[3]);
    		os_dly_wait(LED_DISP_TIME);	

    		led_display(4, led_disp_buf[4]);
    		os_dly_wait(LED_DISP_TIME);	
    	
    		led_display(5, led_disp_buf[5]);		
    		os_dly_wait(LED_DISP_TIME);
    	}
        
    	for(i = 0; i < LED_BLINK_TIME; i++)
    	{
    		if(led_blink_pos > 5)
    		{
    			LEDX0_DISABLE();
                LEDX1_DISABLE();
                LEDX2_DISABLE();
                LEDX3_DISABLE();
                LEDX4_DISABLE();	
                LEDX5_DISABLE();
                
                os_dly_wait(5 * LED_DISP_TIME);
    		}
    		else
    		{
                if(1 == led_blink_pos)
                {
                    LEDX0_DISABLE();
                    LEDX1_DISABLE();
                    LEDX2_DISABLE();
                    LEDX3_DISABLE();
                    LEDX4_DISABLE();	
                    LEDX5_DISABLE();
                    
                    os_dly_wait(LED_DISP_TIME);					  
                }
                else
                {
                    led_display(0, led_disp_buf[0]);
                    
                    os_dly_wait(LED_DISP_TIME);
                }

                if(2 == led_blink_pos)
                {
                    LEDX0_DISABLE();
                    LEDX1_DISABLE();
                    LEDX2_DISABLE();
                    LEDX3_DISABLE();
                    LEDX4_DISABLE();	
                    LEDX5_DISABLE();

                    os_dly_wait(LED_DISP_TIME);	
                }
                else
                {
                    led_display(1, led_disp_buf[1]);

                    os_dly_wait(LED_DISP_TIME);
                }

                if(3 == led_blink_pos)
                {	
                    LEDX0_DISABLE();
                    LEDX1_DISABLE();
                    LEDX2_DISABLE();
                    LEDX3_DISABLE();
                    LEDX4_DISABLE();	
                    LEDX5_DISABLE();

                    os_dly_wait(LED_DISP_TIME);															  
                }
                else
                {
                    led_display(2, led_disp_buf[2]);

                    os_dly_wait(LED_DISP_TIME);
                }

                if(4 == led_blink_pos)
                {
                    LEDX0_DISABLE();
                    LEDX1_DISABLE();
                    LEDX2_DISABLE();
                    LEDX3_DISABLE();
                    LEDX4_DISABLE();	
                    LEDX5_DISABLE();

                    os_dly_wait(LED_DISP_TIME);	
                }
                else
                {
                    led_display(3, led_disp_buf[3]);

                    os_dly_wait(LED_DISP_TIME);
                }
                
                if(5 == led_blink_pos)
                {
                    LEDX0_DISABLE();
                    LEDX1_DISABLE();
                    LEDX2_DISABLE();
                    LEDX3_DISABLE();
                    LEDX4_DISABLE();	
                    LEDX5_DISABLE();

                    os_dly_wait(LED_DISP_TIME);	
                }
                else
                {
                    led_display(4, led_disp_buf[4]);
                    
                    os_dly_wait(LED_DISP_TIME);
                }
    		}

            led_display(5, led_disp_buf[5] | blink_led);
            
            os_dly_wait(LED_DISP_TIME);
    	}
    }
}

