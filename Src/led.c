#include "includes.h"


code u8 led_tab[] = {
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

u8  FlashTime=80;

u8  LED_BUFF[6]={0x39,0x25,0x05,0xf1,0x01,0xff};   

u8          FlashLed    = 0; 
u8          FlashPosition = 0;


void led_display( u8 com,u8 disdata )
{
     u8 i;
	 LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;  
	 for(i=0;i<6;i++);	   
	 P0 = disdata;		    
  	 switch(com)
	 {
	   case 0x00: LEDX0=0;break;
 	   case 0x01: LEDX1=0;break;
	   case 0x02: LEDX2=0;break;
	   case 0x03: LEDX3=0;break;
	   case 0x04: LEDX4=0;break;
	   case 0x05: LEDX5=0;break;
	   default: break;
	 } 
}

void DispTask(void) _task_ LED_TASK  
{
 	u8 i;
	while (1)
	{           
	                
		for(i = 0;i <FlashTime;i++)
		{

			led_display(0,LED_BUFF[0]);	
			os_wait(K_TMO,DisKeepTime,0);

			led_display(1,LED_BUFF[1]);
			os_wait(K_TMO,DisKeepTime,0);

			led_display(2,LED_BUFF[2]);
			os_wait(K_TMO,DisKeepTime,0);

			led_display(3,LED_BUFF[3]);
			os_wait(K_TMO,DisKeepTime,0);	

			led_display(4,LED_BUFF[4]);
			os_wait(K_TMO,DisKeepTime,0);	
		
			led_display(5,LED_BUFF[5]);		
			os_wait(K_TMO,DisKeepTime,0);
		}
		for(i = 0;i < FlashTime;i++)
		{
			if(FlashPosition>5)
			{
				LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				os_wait(K_TMO,5*DisKeepTime,0);			
			}
			else
			{
			    if(FlashPosition==1)
				{
  				  LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				  os_wait(K_TMO,DisKeepTime,0);					  
				}
				else
				{
				  led_display(0,LED_BUFF[0]);
				  os_wait(K_TMO,DisKeepTime,0);
				}

				if(FlashPosition==2)
				{
				  LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				  os_wait(K_TMO,DisKeepTime,0);	
				}
				else
				{
				  led_display(1,LED_BUFF[1]);
				  os_wait(K_TMO,DisKeepTime,0);
				}

				if(FlashPosition==3)
				{	
				  LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				  os_wait(K_TMO,DisKeepTime,0);															  
				}
				else
				{
				  led_display(2,LED_BUFF[2]);
				  os_wait(K_TMO,DisKeepTime,0);
				}
                
				if(FlashPosition==4)
				{
				  LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				  os_wait(K_TMO,DisKeepTime,0);	
				}
				else
				{
				  led_display(3,LED_BUFF[3]);
				  os_wait(K_TMO,DisKeepTime,0);
				}
				if(FlashPosition==5)
				{
				  LEDX0 = 1;LEDX1 = 1;LEDX2 = 1;LEDX3 = 1;LEDX4 = 1;	LEDX5=1;
				  os_wait(K_TMO,DisKeepTime,0);	
				}
				else
				{
				  led_display(4,LED_BUFF[4]);
				  os_wait(K_TMO,DisKeepTime,0);
				}
			}

			led_display(5,LED_BUFF[5]|FlashLed);
			os_wait(K_TMO,DisKeepTime,0);
   		}

	}
}

