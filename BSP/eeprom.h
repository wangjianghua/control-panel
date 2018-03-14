#ifndef __EEPROM_H__
#define __EEPROM_H__


#define EEPROM_PORT             GPIOB
#define EEPROM_SCL_PIN          GPIO_Pin_6
#define EEPROM_SDA_PIN          GPIO_Pin_7

#define EEPROM_SDA              GPIO_ReadInputDataBit(EEPROM_PORT, EEPROM_SDA_PIN)

#define EEPROM_SCL_LOW()        GPIO_ResetBits(EEPROM_PORT, EEPROM_SCL_PIN)
#define EEPROM_SCL_HIGH()	    GPIO_SetBits(EEPROM_PORT, EEPROM_SCL_PIN)

#define	EEPROM_SDA_LOW()	    GPIO_ResetBits(EEPROM_PORT, EEPROM_SDA_PIN)
#define	EEPROM_SDA_HIGH()	    GPIO_SetBits(EEPROM_PORT, EEPROM_SDA_PIN)

#define EEPROM_NO_ACK           0u
#define EEPROM_ACK              1u

#define EEPROM_DELAY_TIME       20u

#define EEPROM_TEST_EN          0u

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

u8 EEPROM_ReadByte(u16 addr);
void EEPROM_WriteByte(u16 addr, u8 data);
u16 EEPROM_ReadHalfWord(u16 addr);
void EEPROM_WriteHalfWord(u16 addr, u16 data);

#if (EEPROM_TEST_EN > 0u)
void EEPROM_Test(void);
#endif


#endif
