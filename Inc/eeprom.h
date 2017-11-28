#ifndef __EEPROM_H__
#define __EEPROM_H__


SBIT SCL = P4^5;
SBIT SDA = P4^1;

#define IIC_TEST_EN             0u

#define AT24C16                 2048u

#define IIC_SCL_LOW()           SCL = 0
#define IIC_SCL_HIGH()	        SCL = 1
#define	IIC_SDA_LOW()		    SDA = 0
#define	IIC_SDA_HIGH()	        SDA = 1

extern u8 IIC_Read(u16 addr);
extern void IIC_Write(u16 addr, u8 dat);
extern u16 IIC_ReadHalfWord(u16 addr);
extern void IIC_WriteHalfWord(u16 addr, u16 dat);
extern void IIC_Test(void);


#endif
