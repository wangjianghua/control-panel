#include "includes.h"
 
unsigned int para_restore_no;

unsigned int read24lc16_word(unsigned int address);
unsigned char write24lc16_word(unsigned int address,unsigned int dat);
void   para_restore_ram(void);
 
#define scl_low		SCL=0		
#define scl_high	SCL=1		
#define	sda_low		SDA=0	
#define	sda_high	SDA=1	

void   delay(unsigned int count)
{
	unsigned int i;
	for(i=0;i<count;i++)
	{
   	  _nop_(); 
	}
}

void s24(void) 
{
	scl_low;
	sda_high;
	scl_high;
	delay(20); 		 
	sda_low;
	delay(20); 
	scl_low;
	delay(20);
}

void p24(void) 
{
	sda_low;
	scl_high;
	delay(20); 
	sda_high;
	delay(20);
}

unsigned char rd24(void) 
{
	unsigned int i,dd=0;

	for(i = 0; i < 8; i++)
	{
		dd<<=1;
		scl_high;
		delay(20);
		if (SDA) dd|=1;
		delay(20); 
		scl_low;
		delay(20); 
	}
    	
	//sda_high;
	//scl_high;
	//delay(100); 
	//scl_low;
		
	return(dd);
}

void wd24(unsigned int dd) {
	int i;
	for(i = 7; i >= 0; i--)
	{
		if( ((dd >> i) & 1)==1)
			sda_high;
		else	
			sda_low;
		delay(20); 
		scl_high;
		delay(20); 
		scl_low;
		delay(20); 
	}
}


void ack(void)
{
    sda_high; delay(10);
	scl_high;
	delay(20); 
	while(SDA==1){}
	scl_low;
	delay(20);	

}

unsigned int read24lc16_word(unsigned int address){
	unsigned int dd,temp;
	temp=address<<1;
	dd=((temp&0x7ff)/256)<<1;
	s24();
	wd24(0xa0|dd);
	ack();
	wd24(temp&255);
	ack();
	s24();
	wd24(0xa1|dd);
 
	ack();
	temp=rd24();
	p24();
	s24();
	wd24(0xa1|dd);
	ack();
	dd=rd24();
	p24();
	dd=temp+(dd<<8);			
	return(dd);
}

unsigned char write24lc16_word(unsigned int address,unsigned int dd){
	unsigned char ddd;
	ddd=(((address<<1)&0x7ff)/256)<<1;
	
	s24();
	wd24(0xa0|ddd);
	ack();
	wd24((address<<1)&255);
	ack();
	wd24(dd);
	ack();
	wd24((dd>>8)&0xff);
	ack();
	p24();
	delay(20);	
//	ack();

	os_wait(K_TMO,12,0);//5ms 
	return 0;
}	



////////////////////////////////////////
/*
unsigned int allpara_restore_no=0;
void allpara_restore(void)
{
	unsigned int *pdefault=(unsigned int *)(&para_default);
	if(inveter_status.bit.paraini_status)
	return;
	if(inveter_status.bit.allparaini_status==1)
		{
			if(allpara_restore_no<=PARA_ADDRESS(finalpara))
				{
			
				    write24lc16_word(allpara_restore_no,pdefault[allpara_restore_no]);//利用循环时间省略延时
				    delay();
				  para_command.pare[allpara_restore_no]=pdefault[allpara_restore_no];
   				  allpara_restore_no=allpara_restore_no+1;
				}
			else
				{
					inveter_status.bit.allparaini_status=0;
					allpara_restore_no=0;
				}
		}
	else
		{
			allpara_restore_no=0;
		}
}
 
*/


















