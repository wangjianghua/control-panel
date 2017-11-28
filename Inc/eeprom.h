#ifndef __EEPROM_H__
#define __EEPROM_H__


SBIT IIC_SCL = P4^5;
SBIT IIC_SDA = P4^1;

#define IIC_SCL_LOW()           IIC_SCL = 0
#define IIC_SCL_HIGH()	        IIC_SCL = 1
#define	IIC_SDA_LOW()		    IIC_SDA = 0
#define	IIC_SDA_HIGH()	        IIC_SDA = 1

#define IIC_TEST_EN             1u

#define AT24C01                 128u  
#define AT24C02                 256u  
#define AT24C04                 512u  
#define AT24C08                 1024u  
#define AT24C16                 2048u  
#define AT24C32                 4096u  
#define AT24C64                 8192u  
#define AT24C128                16384u  
#define AT24C256                32768u

#define AT24CXX                 AT24C16

#define AT_DEV_ADDR_WRITE       0xA0
#define AT_DEV_ADDR_READ        0xA1

extern u8 IIC_ReadByte(u16 addr);
extern void IIC_WriteByte(u16 addr, u8 dat);
extern u16 IIC_ReadHalfWord(u16 addr);
extern void IIC_WriteHalfWord(u16 addr, u16 dat);
extern void IIC_Test(void);


#endif
