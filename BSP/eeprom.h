#ifndef __EEPROM_H__
#define __EEPROM_H__


#define IIC_PORT                GPIOB
#define IIC_SCL_PIN             GPIO_Pin_6
#define IIC_SDA_PIN             GPIO_Pin_7

#define IIC_SDA                 GPIO_ReadInputDataBit(IIC_PORT, IIC_SDA_PIN)

#define IIC_SCL_LOW()           GPIO_ResetBits(IIC_PORT, IIC_SCL_PIN)
#define IIC_SCL_HIGH()	        GPIO_SetBits(IIC_PORT, IIC_SCL_PIN)

#define	IIC_SDA_LOW()		    GPIO_ResetBits(IIC_PORT, IIC_SDA_PIN)
#define	IIC_SDA_HIGH()	        GPIO_SetBits(IIC_PORT, IIC_SDA_PIN)

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

#define AT24CXX                 AT24C64

#define AT_DEV_ADDR_WRITE       0xA0
#define AT_DEV_ADDR_READ        0xA1

u8 IIC_ReadByte(u16 addr);
void IIC_WriteByte(u16 addr, u8 data);
u16 IIC_ReadHalfWord(u16 addr);
void IIC_WriteHalfWord(u16 addr, u16 data);

#if (IIC_TEST_EN > 0u)
void IIC_Test(void);
#endif


#endif
