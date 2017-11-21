#ifndef __EEPROM_H__
#define __EEPROM_H__


SBIT      SCL   =  P4^5;
SBIT      SDA   =  P4^1;

extern unsigned int read24lc16_word(unsigned int address);
extern unsigned char write24lc16_word(unsigned int address,unsigned int dat);

extern void delay(unsigned int count);


#endif
