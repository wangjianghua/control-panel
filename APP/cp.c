/** 
 * @file     cp.c
 * @brief    控制盘
 * @details  
 * @author   华兄
 * @email    jianghua.wang@foxmail.com
 * @date     2017
 * @version  vX.XX
 * @par Copyright (c):  
 *           华兄电子
 * @par History:          
 *   version: author, date, desc\n 
 */

#include "includes.h"


OS_SEM cp_sem;

XDATA CP_PARA_ROM cp_para_rom;
XDATA CP_PARA_RAM cp_para_ram;

static int form_err(unsigned int key_msg, unsigned int form_msg);
static int form_home(unsigned int key_msg, unsigned int form_msg);
static int form_ref(unsigned int key_msg, unsigned int form_msg);
static int form_ref_val(unsigned int key_msg, unsigned int form_msg);
static int form_para(unsigned int key_msg, unsigned int form_msg);
static int form_para_group(unsigned int key_msg, unsigned int form_msg);
static int form_para_grade(unsigned int key_msg, unsigned int form_msg);
static int form_para_val(unsigned int key_msg, unsigned int form_msg);
static int form_copy(unsigned int key_msg, unsigned int form_msg);
static int form_copy_upload(unsigned int key_msg, unsigned int form_msg);
static int form_copy_download_all(unsigned int key_msg, unsigned int form_msg);
static int form_copy_download_part(unsigned int key_msg, unsigned int form_msg);
static int form_copy_upload_rate(unsigned int key_msg, unsigned int form_msg);
static int form_copy_download_all_rate(unsigned int key_msg, unsigned int form_msg);
static int form_copy_download_part_rate(unsigned int key_msg, unsigned int form_msg);

CODE unsigned int wCRC16Table[256] = {   
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,  
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,  
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,  
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,    
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,  
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,  
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,  
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,  
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,     
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,  
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,  
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,  
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,  
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,     
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,  
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,  
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,  
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,  
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,     
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,  
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,  
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,  
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,  
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,    
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,  
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,  
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,  
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,  
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,    
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,  
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

unsigned int CRC16Calculate(unsigned char *J_u8DataIn, unsigned int J_u16DataLen)  
{  
    unsigned int J_u16Result = 0xffff;
    unsigned int J_u16TableNo = 0;
    unsigned int J_u16Index;  
    
    
    if(J_u16DataLen > (UART_MAX_LEN - 2))
    {
        return (0);
    }
    
    for(J_u16Index = 0; J_u16Index < J_u16DataLen; J_u16Index++)  
    {  
        J_u16TableNo = (J_u16Result & 0xff) ^ (J_u8DataIn[J_u16Index] & 0xff);
        J_u16Result = ((J_u16Result >> 8) & 0xff) ^ wCRC16Table[J_u16TableNo];
    }  

    return (J_u16Result);  
}

bool uart_recv_align(void)
{
    u8 i, offset;


    if(uart_rx_count < 2)
    {
        return (FALSE);
    }
    
    for(i = 0, offset = 0; i < uart_rx_count; i++)
    {
        if((0xF7 == UART_RX_BUF[i]) && (0x17 == UART_RX_BUF[i + 1]))
        {
            offset = i;

            break;
        }
    }

    if(offset > 0) //串口接收数据存在赘余
    {
        for(i = 0; i < uart_rx_count; i++) //数据左对齐
        {
            UART_RX_BUF[i] = UART_RX_BUF[i + offset];
        }

        uart_rx_count -= offset;

        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

void err_con(void)
{
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[4] = 0xff;
    led_disp_buf[3] = led_table['E' - 32];
    led_disp_buf[2] = led_table['C' - 32];
    led_disp_buf[1] = led_table['O' - 32];
    led_disp_buf[0] = led_table['N' - 32];
    LEDOE_ENABLE();
}

CODE u8 vfd_con_cmd[][32] = {
    /* 0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34    35    36    37    38    39    40    41    42    43    44    45    46    47    48    49    50    51    52    53    54    55    56    57    58    59 */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x02},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x01},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x17, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0xC9},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0x65},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void vfd_con(void)
{
    OS_RESULT result;
    s8 i;
    u8 len;
    unsigned int crc;
    bool done = FALSE;
    

    memset(&cp_para_ram, 0, sizeof(cp_para_ram));
   
    for(i = 0; i < 8; i++)
    {        
        if((5 == i) && 
           (TRUE == cp_para_ram.func_code_visible) &&
           (FALSE == done))
        {
            done = TRUE;
            
            i = 8;
        }
        
        len = (vfd_con_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, vfd_con_cmd[i], len);

        switch(i)
        {
        case 2:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 3:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 4:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

    			if(TRUE == cp_para_ram.ref_chang)
    			{
    				cp_para_ram.ref_chang = FALSE;
                    
    				UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
    				UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
    			}
                else
    			{
    				UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
    				UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
    			}

    			if(TRUE == cp_para_ram.stop)
    			{
    				cp_para_ram.stop = FALSE;
                    
    				UART_TX_BUF[20] |= 0x01;
    			}
                
    			if(TRUE == cp_para_ram.run)
    			{
    				cp_para_ram.run = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }

    			if(VFD_LOC == cp_para_ram.lr)
    			{
    				UART_TX_BUF[20] |= 0x08;
    			}
                else
    			{
    				UART_TX_BUF[20] &= ~0x08;
    			}               
            }
            break;

        case 5:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 6:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 7:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

    			if(TRUE == cp_para_ram.ref_chang)
    			{
    				cp_para_ram.ref_chang = FALSE;
                    
    				UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
    				UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
    			}
                else
    			{
    				UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
    				UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
    			}

    			if(TRUE == cp_para_ram.stop)
    			{
    				cp_para_ram.stop = FALSE;
                    
    				UART_TX_BUF[20] |= 0x01;
    			}
                
    			if(TRUE == cp_para_ram.run)
    			{
    				cp_para_ram.run = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}

                UART_TX_BUF[20] &= ~0x04; //VFD_FWD

                UART_TX_BUF[20] |= 0x08; //VFD_LOC
            }
            break;

        case 8:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);

        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
                        
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 0:
                    break;
                    
                case 1:
                    cp_para_ram.cmd = UART_RX_BUF[5];
                    break;
                
                case 4:
                case 7:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        if(UART_RX_BUF[11] & 0x10)
                        {
                            cp_para_ram.lr = VFD_LOC;
                        }
                        else
                        {
                            cp_para_ram.lr = VFD_REM;
                        }

                        if(VFD_LOC == cp_para_ram.lr) //本地
                        {
                            led_disp_buf[5] &= LED_LOC_REM;
                            LEDOE_ENABLE();
                        }
                        else //远程
                        {
                            led_disp_buf[5] |= LED_LOC_REM_MASK;
                            LEDOE_ENABLE();
                        }
                    }
                    break;

                case 8:
                    i = 4;
                    break;
                
                default:
                    break;
                }

                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = 0xff;
                led_disp_buf[2] = 0xff;
                led_disp_buf[1] = led_table['C' - 32];
                led_disp_buf[0] = led_table['P' - 32];
                LEDOE_ENABLE();
            }
            else
            {
                err_con();

                i = -1; //启动重复发送连接命令
            }
        }
        else
        { 
            err_con();

            i = -1; //启动重复发送连接命令
        }
    }

    cp_para_ram.ref_mod = cp_para_ram.ref;
}

static const FORM form_list[MAX_FORM_NUM] =
{
    {form_err},
    {form_home},
    {form_home},
    {form_home}, 
    {form_ref},
    {form_ref_val},
    {form_para},
    {form_para_group},
    {form_para_grade},
    {form_para_val},
    {form_copy},
    {form_copy_upload},
    {form_copy_download_all},
    {form_copy_download_part},
    {form_copy_upload_rate},
    {form_copy_download_all_rate},
    {form_copy_download_part_rate},
};

static unsigned int form_id;

void MENU_Init(void)
{
    led_disp_buf[5] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[3] = 0xff;
    led_disp_buf[2] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[0] = 0xff;
    LEDOE_ENABLE();
    
    form_id = FORM_ID_HOME1;

    os_dly_wait(5);
}

void MEM_Init(void)
{
    u16 i, *p_cp_para = NULL;


    p_cp_para = (u16 *)&cp_para_rom;

    for(i = 0; i < (sizeof(cp_para_rom) / 2); i++)
    {
        *p_cp_para = EEPROM_ReadHalfWord(CP_PARA_ADDR + i * 2);
        os_dly_wait(5);

        p_cp_para++;
    }
}

bool form_key_callback(unsigned int key_msg)
{
    bool ret = FALSE;
    
    
    switch(key_msg)
    {
    case KEY_MSG_LOC_REM:
        cp_para_ram.ref_mod = 0;
        cp_para_ram.ref_chang = TRUE;

        cp_para_ram.stop = TRUE;
        led_disp_buf[5] |= LED_RUN_MASK;
        LEDOE_ENABLE();

        ret = TRUE;
        break;

    default:
        break;
    }

    return (ret);
}

CODE u8 keep_vfd_connect_cmd[][32] = {
    /* KEEP_VFD_CONNECT_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

bool keep_vfd_connect(void)
{
    OS_RESULT result;
    u8 len;
    unsigned int crc;
    bool ret = FALSE;

    
    len = (keep_vfd_connect_cmd[KEEP_VFD_CONNECT_CMD][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, keep_vfd_connect_cmd[KEEP_VFD_CONNECT_CMD], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
    UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
    UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

    if(TRUE == cp_para_ram.ref_chang)
    {
        cp_para_ram.ref_chang = FALSE;
        
        UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
        UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
    }
    else
    {
        UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
        UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
    }

    if(TRUE == cp_para_ram.stop)
    {
        cp_para_ram.stop = FALSE;
        
        UART_TX_BUF[20] |= 0x01;
    }
    
    if(TRUE == cp_para_ram.run)
    {
        cp_para_ram.run = FALSE;
        
        UART_TX_BUF[20] |= 0x02;
    }

    if(VFD_REV == cp_para_ram.fr)
    {
        UART_TX_BUF[20] |= 0x04;
    }
    else
    {
        UART_TX_BUF[20] &= ~0x04;
    }
    
    if(VFD_LOC == cp_para_ram.lr)
    {
        UART_TX_BUF[20] |= 0x08;
    }
    else
    {
        UART_TX_BUF[20] &= ~0x08;
    }

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
    
    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {
            if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
            {
                cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                cp_para_ram.count++;

                if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                   (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                {
                    form_id = FORM_ID_ERR;
                }

                cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
            }
            else
            {
                form_id = FORM_ID_ERR;
            }           
        }
        else
        {
            err_con();
        }
    }
    else
    {
        err_con();
    }

    return (ret);
}

bool cp_alarm(u16 alarm_code)
{
	u8 i;
	
	
	if(CP_NORMAL == alarm_code)
	{
		return (FALSE);
	}
	
	for(i = 0; i < 10; i++)
	{
		keep_vfd_connect();

        led_disp_buf[0] = (alarm_code > 0) ? (led_table[alarm_code % 10 + 16]) : (led_table['0' - 32]);
        led_disp_buf[1] = (alarm_code > 9) ? (led_table[alarm_code % 100 / 10 + 16]) : (led_table['0' - 32]);
        led_disp_buf[2] = (alarm_code > 99) ? (led_table[alarm_code % 1000 / 100 + 16]) : (led_table['0' - 32]);
        led_disp_buf[3] = (alarm_code > 999) ? (led_table[alarm_code % 10000 / 1000 + 16]) : (led_table['0' - 32]);
        led_disp_buf[4] = led_table['A' - 32];
        led_disp_buf[5] |= LED_V_A_Hz_MASK;
        led_blink_pos = 0;
        blink_led = 0;
        LEDOE_ENABLE();

        os_dly_wait(100);
	}
	
	return (TRUE);
}

CODE u8 form_err_cmd[MAX_FORM_ERR_CMD][32] = {
    /* FORM_ERR_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* FORM_ERR_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
    /* FORM_ERR_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
};

void form_err_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    static bool fault = FALSE, alarm = FALSE;
    
    
    for(i = FORM_ERR_SET_CMD; i < MAX_FORM_ERR_CMD; i++)
    { 
        if((FORM_ERR_FAULT_CMD == i) && 
           (FALSE == fault))
        {
            i = FORM_ERR_ALARM_CMD;
        }
        else if((FORM_ERR_FAULT_CMD == i) && 
                (TRUE == fault))
        {
            fault = FALSE;
        }

        if((FORM_ERR_ALARM_CMD == i) &&
           (FALSE == alarm))
        {
            return;
        }
        else if((FORM_ERR_ALARM_CMD == i) &&
                (TRUE == alarm))
        {
            alarm = FALSE;
        }
        
        len = (form_err_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_err_cmd[i], len);

        switch(i)
        {
        case FORM_ERR_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.clr_err)
                {
                    cp_para_ram.clr_err = FALSE;
                    
                    UART_TX_BUF[20] |= 0x10;
                }

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_ERR_FAULT_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case FORM_ERR_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_ERR_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if(UART_RX_BUF[11] & 0x80) //0303.Bit15，故障
                        {
                            fault = TRUE;
                        }

                        if(UART_RX_BUF[10] & 0x01) //0304.Bit0，报警
                        {
                            alarm = TRUE;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                case FORM_ERR_FAULT_CMD:
                    cp_para_ram.fault_code = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);

                    if(0 != cp_para_ram.fault_code)
                    {
                        led_disp_buf[0] = (cp_para_ram.fault_code > 0) ? (led_table[cp_para_ram.fault_code % 10 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[1] = (cp_para_ram.fault_code > 9) ? (led_table[cp_para_ram.fault_code % 100 / 10 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[2] = (cp_para_ram.fault_code > 99) ? (led_table[cp_para_ram.fault_code % 1000 / 100 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[3] = (cp_para_ram.fault_code > 999) ? (led_table[cp_para_ram.fault_code % 10000 / 1000 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[4] = led_table['F' - 32];
                        led_disp_buf[5] |= LED_V_A_Hz_MASK;
                        led_blink_pos = 6;
                        LEDOE_ENABLE();
                    }
                    break;

                case FORM_ERR_ALARM_CMD:
                    cp_para_ram.alarm_code = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);

                    if(0 != cp_para_ram.alarm_code)
                    {
                        led_disp_buf[0] = (cp_para_ram.alarm_code > 0) ? (led_table[cp_para_ram.alarm_code % 10 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[1] = (cp_para_ram.alarm_code > 9) ? (led_table[cp_para_ram.alarm_code % 100 / 10 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[2] = (cp_para_ram.alarm_code > 99) ? (led_table[cp_para_ram.alarm_code % 1000 / 100 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[3] = (cp_para_ram.alarm_code > 999) ? (led_table[cp_para_ram.alarm_code % 10000 / 1000 + 16]) : (led_table['0' - 32]);
                        led_disp_buf[4] = led_table['A' - 32];
                        led_disp_buf[5] |= LED_V_A_Hz_MASK;
                        led_blink_pos = 6;
                        LEDOE_ENABLE();
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
}

static int form_err(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_RUN:
            break;

        case KEY_MSG_STOP:
            break;

        case KEY_MSG_LOC_REM:
            break;

        case KEY_MSG_FWD_REV:
            break;

        case KEY_MSG_ENTER:
            break;
            
        case KEY_MSG_EXIT:
            cp_para_ram.clr_err = TRUE;

            form_id = FORM_ID_HOME;
            break;

        case KEY_MSG_UP:
            break;

        case KEY_MSG_DOWN:
            break;

        default:
            break;
        }
    }

    form_err_callback();

    return (FORM_MSG_NONE);
}

void form_home_disp(void)
{
    u8 disp_para_unit, len;
    u16 disp_para_val;
    

    disp_para_unit = cp_para_ram.disp_para_unit[form_id - 1];

    if(TRUE == cp_para_ram.disp_para_sign[form_id - 1])
    {
        disp_para_val = abs((s16)cp_para_ram.disp_para_val[form_id - 1]);

        len = get_data_length(disp_para_val) % (LED_DISP_BUF_SIZE - 1);
    }
    else
    {
        disp_para_val = cp_para_ram.disp_para_val[form_id - 1];
    }
    
    led_disp_buf[0] = led_table[disp_para_val % 10 + 16];    

    switch(cp_para_ram.disp_para_dcl[form_id - 1])
    {
    case 0:
        led_disp_buf[1] = (disp_para_val > 9) ? (led_table[disp_para_val % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.disp_para_sign[form_id - 1]) &&
           (cp_para_ram.disp_para_val[form_id - 1] & 0x8000)) //负数
        {   
            led_disp_buf[len] = led_table['-' - 32]; //-1
        }
        break;

    case 1:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16] & led_table['.' - 32];
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff); 

        if((TRUE == cp_para_ram.disp_para_sign[form_id - 1]) &&
           (cp_para_ram.disp_para_val[form_id - 1] & 0x8000)) //负数
        {   
            if(disp_para_val > 9)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.0
            }
            else
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.1
            }
        }
        break;

    case 2:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16];
        led_disp_buf[2] = led_table[disp_para_val % 1000 / 100 + 16] & led_table['.' - 32];
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.disp_para_sign[form_id - 1]) &&
           (cp_para_ram.disp_para_val[form_id - 1] & 0x8000)) //负数
        {   
            if(disp_para_val > 99)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.00
            }
            else if(disp_para_val > 9)
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.10
            }
            else
            {
                led_disp_buf[len + 2] = led_table['-' - 32]; //-0.01    
            }
        }
        break;

    case 3:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16];
        led_disp_buf[2] = led_table[disp_para_val % 1000 / 100 + 16];
        led_disp_buf[3] = led_table[disp_para_val % 10000 / 1000 + 16] & led_table['.' - 32];
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.disp_para_sign[form_id - 1]) &&
           (cp_para_ram.disp_para_val[form_id - 1] & 0x8000)) //负数
        {   
            if(disp_para_val > 999)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.000
            }
            else if(disp_para_val > 99)
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.100
            }
            else if(disp_para_val > 9)
            {
                led_disp_buf[len + 2] = led_table['-' - 32]; //-0.010
            }
            else
            {
                led_disp_buf[len + 3] = led_table['-' - 32]; //-0.001
            }
        }
        break;

    default:
        led_disp_buf[1] = (disp_para_val > 9) ? (led_table[disp_para_val % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.disp_para_sign[form_id - 1]) &&
           (cp_para_ram.disp_para_val[form_id - 1] & 0x8000)) //负数
        {   
            led_disp_buf[len] = led_table['-' - 32]; //-1
        }
        break;
    }
    
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= ((u8)LED_V_A_Hz != func_code_unit[disp_para_unit]) ? (func_code_unit[disp_para_unit]) : (0xff);

    led_blink_pos = 0;
    blink_led = 0;
    
    LEDOE_ENABLE();
}

CODE u8 form_home_cmd[MAX_FORM_HOME_CMD][32] = {
    /* FORM_HOME_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* FORM_HOME_READ_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x05, 0xAB, 0x50, 0x03},
};

void form_home_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_HOME_CMD; i++)
    {        
        len = (form_home_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_home_cmd[i], len);

        switch(i)
        {
        case FORM_HOME_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_HOME_READ_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_HOME_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            i = FORM_HOME_READ_CMD;
                            
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        i = FORM_HOME_READ_CMD;
                        
                        form_id = FORM_ID_ERR;
                    }
                    break;

                case FORM_HOME_READ_CMD:
                    if((0x45 == UART_RX_BUF[3]) && (0xab == UART_RX_BUF[4]))
                    {
                        cp_para_ram.disp_para_val[0] = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]);
                        cp_para_ram.disp_para_val[1] = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                        cp_para_ram.disp_para_val[2] = ((u16)UART_RX_BUF[21] << 8) | ((u16)UART_RX_BUF[22]);

                        cp_para_ram.disp_para_type[0] = ((u16)UART_RX_BUF[11] << 8) | ((u16)UART_RX_BUF[12]);
                        cp_para_ram.disp_para_type[1] = ((u16)UART_RX_BUF[17] << 8) | ((u16)UART_RX_BUF[18]);
                        cp_para_ram.disp_para_type[2] = ((u16)UART_RX_BUF[23] << 8) | ((u16)UART_RX_BUF[24]);

                        cp_para_ram.disp_para_unit[0] = (UART_RX_BUF[13] < FUNC_CODE_UNIT_NUM) ? (UART_RX_BUF[13]) : (0x00);
                        cp_para_ram.disp_para_unit[1] = (UART_RX_BUF[19] < FUNC_CODE_UNIT_NUM) ? (UART_RX_BUF[19]) : (0x00);
                        cp_para_ram.disp_para_unit[2] = (UART_RX_BUF[25] < FUNC_CODE_UNIT_NUM) ? (UART_RX_BUF[25]) : (0x00);

                        cp_para_ram.disp_para_sign[0] = UART_RX_BUF[14] & 0x01;
                        cp_para_ram.disp_para_enum[0] = UART_RX_BUF[14] & 0x80;
                        cp_para_ram.disp_para_dcl[0] = (UART_RX_BUF[14] & 0x0f) >> 1;
                        
                        cp_para_ram.disp_para_sign[1] = UART_RX_BUF[20] & 0x01;
                        cp_para_ram.disp_para_enum[1] = UART_RX_BUF[20] & 0x80;
                        cp_para_ram.disp_para_dcl[1] = (UART_RX_BUF[20] & 0x0f) >> 1;
                        
                        cp_para_ram.disp_para_sign[2] = UART_RX_BUF[26] & 0x01;
                        cp_para_ram.disp_para_enum[2] = UART_RX_BUF[26] & 0x80;
                        cp_para_ram.disp_para_dcl[2] = (UART_RX_BUF[26] & 0x0f) >> 1;

                        form_home_disp();
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
}

static int form_home(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_UP:
            form_id++;
            
            if(form_id > FORM_ID_HOME3)
            {
                form_id = FORM_ID_HOME1;
            }
            break;

        case KEY_MSG_DOWN:
            if(FORM_ID_HOME1 != form_id)
            {
                form_id--;
            }
            else
            {
                form_id = FORM_ID_HOME3;
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_REF;

            return (FORM_MSG_NONE);
            //break;
            
        default:
            break;
        }
    }

    form_home_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_ref_cmd[MAX_FORM_REF_CMD][32] = {
    /* FORM_REF_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_ref_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_REF_CMD; i++)
    {        
        len = (form_ref_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_ref_cmd[i], len);

        switch(i)
        {
        case FORM_REF_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_REF_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = led_table['F' - 32];
    led_disp_buf[2] = led_table['E' - 32];
    led_disp_buf[3] = led_table['r' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_ref(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_REF_VAL;
            break;
            
        case KEY_MSG_EXIT:
            form_id = FORM_ID_HOME;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_COPY;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_PARA;
            break;

        default:
            break;
        }
    }

    form_ref_callback();

    return (FORM_MSG_NONE);
}

void form_ref_val_disp(void)
{
    u16 disp_para_val;


    disp_para_val = cp_para_ram.ref_mod / 100;
    
    led_disp_buf[0] = led_table[disp_para_val % 10 + 16];
    led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16] & led_table['.' - 32];
    led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
    led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= LED_Hz;
    led_blink_pos = cp_para_ram.vfd_para_shift + 1;
    blink_led = 0;
    LEDOE_ENABLE();
}

CODE u8 form_ref_val_cmd[MAX_FORM_REF_VAL_CMD][32] = {
    /* FORM_REF_VAL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_ref_val_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_REF_VAL_CMD; i++)
    {        
        len = (form_ref_val_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_ref_val_cmd[i], len);

        switch(i)
        {
        case FORM_REF_VAL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_REF_VAL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }

    form_ref_val_disp();
}

static int form_ref_val(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;
                
        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_SHIFT:
            cp_para_ram.vfd_para_shift++;
            
            cp_para_ram.vfd_para_shift %= get_data_length(cp_para_ram.ref_mod / 100);
            break;

        case KEY_MSG_ENTER:
            led_blink_pos = 0;
            cp_para_ram.vfd_para_shift = 0;
            
            cp_para_ram.ref_chang = TRUE;
            
            form_id = FORM_ID_REF;

            return (FORM_MSG_NONE);
            //break;
            
        case KEY_MSG_EXIT:       
            led_blink_pos = 0;
            cp_para_ram.vfd_para_shift = 0;
            
            cp_para_ram.ref_mod = cp_para_ram.ref;
            
            form_id = FORM_ID_REF;
            
            return (FORM_MSG_NONE);
            //break;

        case KEY_MSG_UP:
        case KEY_MSG_UP_LONG:    
            cp_para_ram.ref_mod += pow(10, cp_para_ram.vfd_para_shift) * 100;

            if(cp_para_ram.ref_mod > MAX_REF_VAL)
            {
                cp_para_ram.ref_mod = MAX_REF_VAL;
            }
            break;

        case KEY_MSG_DOWN:
        case KEY_MSG_DOWN_LONG:    
            if(cp_para_ram.ref_mod >= (pow(10, cp_para_ram.vfd_para_shift) * 100))
            {
                cp_para_ram.ref_mod -= pow(10, cp_para_ram.vfd_para_shift) * 100;

                if(cp_para_ram.ref_mod < (pow(10, cp_para_ram.vfd_para_shift) * 100))
                {
                    cp_para_ram.vfd_para_shift = 0;
                }
            }
            else
            {
                cp_para_ram.ref_mod = 0;

                cp_para_ram.vfd_para_shift = 0;
            }
            break;

        default:
            break;
        }
    }

    form_ref_val_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_para_cmd[MAX_FORM_PARA_CMD][32] = {
    /* FORM_PARA_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_para_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_CMD; i++)
    {        
        len = (form_para_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_para_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_PARA_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = led_table['r' - 32];
    led_disp_buf[2] = led_table['A' - 32];
    led_disp_buf[3] = led_table['P' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_para(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_PARA_GROUP;

            return (FORM_MSG_DATA);
            //break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_HOME;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_REF;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_COPY;
            break;

        default:
            break;
        }
    }

    form_para_callback();

    return (FORM_MSG_NONE);
}

CODE u8 para_group_update_cmd[][32] = {
    /* FORM_PARA_GROUP_UPDATE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x04, 0x00, 0x59, 0x00, 0x03, 0x06, 0x04, 0xA3, 0x50, 0x07, 0x00, 0x00},
};

bool group_update(u8 key_msg)
{
    OS_RESULT result;
    u8 len;
    unsigned int crc;
    bool ret = FALSE;


    len = (para_group_update_cmd[0][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, para_group_update_cmd[0], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

    UART_TX_BUF[15] = 0;
    UART_TX_BUF[16] = cp_para_ram.group;

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);

    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {            
            memcpy(cp_para_ram.group_nearby, &UART_RX_BUF[7], sizeof(cp_para_ram.group_nearby));

            switch(key_msg)
            {
            case KEY_MSG_UP:
                cp_para_ram.group = cp_para_ram.group_nearby[3];
                break;
            
            case KEY_MSG_DOWN:
                cp_para_ram.group = cp_para_ram.group_nearby[1];
                break;
                
            default:
                break;
            }
            
            ret = TRUE;
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
            LEDOE_ENABLE();
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
        LEDOE_ENABLE();
    }

    return (ret);
}

CODE u8 form_para_group_cmd[MAX_FORM_PARA_GROUP_CMD][32] = {
    /* FORM_PARA_GROUP_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_para_group_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_GROUP_CMD; i++)
    {        
        len = (form_para_group_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_para_group_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_GROUP_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_PARA_GROUP_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    if(TRUE == cp_para_ram.func_code_visible)
    {
        led_disp_buf[0] = led_table[cp_para_ram.group % 10 + 16] & led_table['.' - 32];
    }
    else
    {
        led_disp_buf[0] = led_table[cp_para_ram.group % 10 + 16];
    }

    led_disp_buf[1] = led_table[cp_para_ram.group % 100 / 10 + 16];
    led_disp_buf[2] = (cp_para_ram.group > 99) ? (led_table[cp_para_ram.group % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_para_group(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        group_update(KEY_MSG_UP);
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:            
            form_id = FORM_ID_PARA_GRADE;

            return (FORM_MSG_DATA);
            //break;

        case KEY_MSG_EXIT:
            cp_para_ram.group = 0;
            cp_para_ram.grade = 0;
            
            form_id = FORM_ID_PARA;

            return (FORM_MSG_NONE);
            //break;

        case KEY_MSG_UP:
        case KEY_MSG_UP_LONG:
            group_update(KEY_MSG_UP);
            break;

        case KEY_MSG_DOWN:
        case KEY_MSG_DOWN_LONG:
            group_update(KEY_MSG_DOWN);
            break;

        default:
            break;
        }
    }

    form_para_group_callback();

    return (FORM_MSG_NONE);
}

CODE u8 para_grade_update_cmd[][32] = {
    /* FORM_PARA_GRADE_UPDATE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x06, 0x00, 0x59, 0x00, 0x03, 0x06, 0x09, 0xA4, 0x50, 0x04, 0x01, 0x00},
};

bool grade_update(u8 key_msg)
{
    OS_RESULT result;
    u8 len;
    unsigned int crc;
    bool ret = FALSE;


    len = (para_grade_update_cmd[0][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, para_grade_update_cmd[0], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
    
    UART_TX_BUF[15] = cp_para_ram.group;
    UART_TX_BUF[16] = cp_para_ram.grade;

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);

    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {            
            memcpy(cp_para_ram.grade_nearby, &UART_RX_BUF[7], sizeof(cp_para_ram.grade_nearby));

            switch(key_msg)
            {
            case KEY_MSG_UP:
                cp_para_ram.group = cp_para_ram.grade_nearby[4];
                cp_para_ram.grade = cp_para_ram.grade_nearby[5];
                break;
            
            case KEY_MSG_DOWN:
                cp_para_ram.group = cp_para_ram.grade_nearby[2];
                cp_para_ram.grade = cp_para_ram.grade_nearby[3];
                break;
                
            default:
                break;
            }
            
            ret = TRUE;
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
            LEDOE_ENABLE();
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
        LEDOE_ENABLE();
    }

    return (ret);
}

CODE u8 form_para_grade_func_code_read_cmd[][32] = {
    /* FORM_PARA_GRADE_FUNC_CODE_READ_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x05, 0x00, 0x59, 0x00, 0x03, 0x06, 0x0A, 0xA4, 0x50, 0x08, 0x0E, 0x01},
};

bool func_code_read(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    bool ret = FALSE;
    
          
    len = (form_para_grade_func_code_read_cmd[0][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, form_para_grade_func_code_read_cmd[0], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

    if(TRUE == cp_para_ram.func_code_visible)
    {
        UART_TX_BUF[5] = 6;
        UART_TX_BUF[14] = 4;
    }
    
    UART_TX_BUF[15] = cp_para_ram.group;
    UART_TX_BUF[16] = cp_para_ram.grade;

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
    
    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {
            cp_para_ram.vfd_para_attr = UART_RX_BUF[7];
            
            cp_para_ram.vfd_para_val = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]);

            cp_para_ram.vfd_para_sign = UART_RX_BUF[11] & 0x01;
            cp_para_ram.vfd_para_enum = UART_RX_BUF[11] & 0x80;
            cp_para_ram.vfd_para_dcl = (UART_RX_BUF[11] & 0x0f) >> 1;
            
            cp_para_ram.vfd_para_unit = (UART_RX_BUF[12] < FUNC_CODE_UNIT_NUM) ? (UART_RX_BUF[12]) : (0x00);

            form_id = FORM_ID_PARA_VAL;

            ret = TRUE;
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE_ENABLE();
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[i + 16];
        LEDOE_ENABLE();
    }

    return (ret);
}

CODE u8 func_code_visible_init_cmd[][32] = {
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x0F, 0xA1, 0x50, 0x02, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x0F, 0xA1, 0x50, 0x82, 0x56, 0x72},
};

bool func_code_visible_init(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    bool ret = FALSE;
    
    
    for(i = 0; i < 2; i++)
    {        
        len = (func_code_visible_init_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, func_code_visible_init_cmd[i], len);

        switch(i)
        {
        case 0:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 1:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 0:
                    if((0x4f == UART_RX_BUF[3]) && (0xa1 == UART_RX_BUF[4]))
                    {
                        ret = TRUE;
                    }
                    break;

                case 1:
                    if((0x4f == UART_RX_BUF[3]) && (0xa1 == UART_RX_BUF[4]))
                    {
                        ret = TRUE;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }

    return (ret);
}

CODE u8 form_para_grade_cmd[MAX_FORM_PARA_GRADE_CMD][32] = {
    /* FORM_PARA_GRADE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_para_grade_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_GRADE_CMD; i++)
    {        
        len = (form_para_grade_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_para_grade_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_GRADE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_PARA_GRADE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = led_table[cp_para_ram.grade % 10 + 16];
    led_disp_buf[1] = led_table[cp_para_ram.grade % 100 / 10 + 16];

    if(TRUE == cp_para_ram.func_code_visible)
    {
        led_disp_buf[2] = led_table[cp_para_ram.group % 10 + 16] & led_table['.' - 32];
    }
    else
    {
        led_disp_buf[2] = led_table[cp_para_ram.group % 10 + 16];
    }
    
    led_disp_buf[3] = led_table[cp_para_ram.group % 100 / 10 + 16];
    led_disp_buf[4] = (cp_para_ram.group > 99) ? (led_table[cp_para_ram.group % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_para_grade(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        grade_update(KEY_MSG_UP);
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            if(TRUE == func_code_read())
            {
                return (FORM_MSG_DATA);
            }
            break;

        case KEY_MSG_EXIT:
            cp_para_ram.grade = 0;
            
            form_id = FORM_ID_PARA_GROUP;

            return (FORM_MSG_NONE);
            //break;

        case KEY_MSG_UP:
        case KEY_MSG_UP_LONG:
            grade_update(KEY_MSG_UP);
            break;

        case KEY_MSG_DOWN:
        case KEY_MSG_DOWN_LONG:
            grade_update(KEY_MSG_DOWN);
            break;

        case KEY_MSG_FUNC_CODE_LONG:
            if(cp_para_ram.group >= 51)
            {
                cp_para_ram.func_code_visible = func_code_visible_init();
            }
            break;

        default:
            break;
        }
    }

    form_para_grade_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_para_val_func_code_write_cmd[][32] = {
    /* FORM_PARA_VAL_FUNC_CODE_WRITE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x04, 0x08, 0x04, 0xA4, 0x50, 0x82, 0x0A, 0x01, 0x00, 0x06},
};

bool func_code_write(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    bool ret = FALSE;
    
          
    len = (form_para_val_func_code_write_cmd[0][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, form_para_val_func_code_write_cmd[0], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
    UART_TX_BUF[15] = cp_para_ram.group;
    UART_TX_BUF[16] = cp_para_ram.grade;
    UART_TX_BUF[17] = (u8)(cp_para_ram.vfd_para_val >> 8);
    UART_TX_BUF[18] = (u8)cp_para_ram.vfd_para_val;

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
    
    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {
            if(0x40 == (UART_RX_BUF[3] & 0xf0))
            {
                form_id = FORM_ID_PARA_GRADE;

                ret = TRUE;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE_ENABLE();
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[i + 16];
        LEDOE_ENABLE();
    }

    return (ret);
}

void form_para_val_disp(void)
{
    u8 disp_para_unit, len;
    u16 disp_para_val;
    

    disp_para_unit = cp_para_ram.vfd_para_unit;

    if(TRUE == cp_para_ram.vfd_para_sign)
    {
        disp_para_val = abs((s16)cp_para_ram.vfd_para_val);

        len = get_data_length(disp_para_val) % (LED_DISP_BUF_SIZE - 1);
    }
    else
    {
        disp_para_val = cp_para_ram.vfd_para_val;
    }
    
    led_disp_buf[0] = led_table[disp_para_val % 10 + 16];

    switch(cp_para_ram.vfd_para_dcl)
    {
    case 0:
        led_disp_buf[1] = (disp_para_val > 9) ? (led_table[disp_para_val % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.vfd_para_sign) &&
           (cp_para_ram.vfd_para_val & 0x8000)) //负数
        {   
            led_disp_buf[len] = led_table['-' - 32]; //-1
        }
        break;

    case 1:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16] & led_table['.' - 32];
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff); 

        if((TRUE == cp_para_ram.vfd_para_sign) &&
           (cp_para_ram.vfd_para_val & 0x8000)) //负数
        {   
            if(disp_para_val > 9)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.0
            }
            else
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.1
            }
        }
        break;

    case 2:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16];
        led_disp_buf[2] = led_table[disp_para_val % 1000 / 100 + 16] & led_table['.' - 32];
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.vfd_para_sign) &&
           (cp_para_ram.vfd_para_val & 0x8000)) //负数
        {   
            if(disp_para_val > 99)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.00
            }
            else if(disp_para_val > 9)
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.10
            }
            else
            {
                led_disp_buf[len + 2] = led_table['-' - 32]; //-0.01    
            }
        }
        break;

    case 3:
        led_disp_buf[1] = led_table[disp_para_val % 100 / 10 + 16];
        led_disp_buf[2] = led_table[disp_para_val % 1000 / 100 + 16];
        led_disp_buf[3] = led_table[disp_para_val % 10000 / 1000 + 16] & led_table['.' - 32];
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.vfd_para_sign) &&
           (cp_para_ram.vfd_para_val & 0x8000)) //负数
        {   
            if(disp_para_val > 999)
            {
                led_disp_buf[len] = led_table['-' - 32]; //-1.000
            }
            else if(disp_para_val > 99)
            {
                led_disp_buf[len + 1] = led_table['-' - 32]; //-0.100
            }
            else if(disp_para_val > 9)
            {
                led_disp_buf[len + 2] = led_table['-' - 32]; //-0.010
            }
            else
            {
                led_disp_buf[len + 3] = led_table['-' - 32]; //-0.001
            }
        }
        break;

    default:
        led_disp_buf[1] = (disp_para_val > 9) ? (led_table[disp_para_val % 100 / 10 + 16]) : (0xff);
        led_disp_buf[2] = (disp_para_val > 99) ? (led_table[disp_para_val % 1000 / 100 + 16]) : (0xff);
        led_disp_buf[3] = (disp_para_val > 999) ? (led_table[disp_para_val % 10000 / 1000 + 16]) : (0xff);
        led_disp_buf[4] = (disp_para_val > 9999) ? (led_table[disp_para_val % 100000 / 10000 + 16]) : (0xff);

        if((TRUE == cp_para_ram.vfd_para_sign) &&
           (cp_para_ram.vfd_para_val & 0x8000)) //负数
        {   
            led_disp_buf[len] = led_table['-' - 32]; //-1
        }
        break;
    }
    
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= ((u8)LED_V_A_Hz != func_code_unit[disp_para_unit]) ? (func_code_unit[disp_para_unit]) : (0xff);

    led_blink_pos = cp_para_ram.vfd_para_shift + 1;
    blink_led = 0;
    
    LEDOE_ENABLE();
}

bool check_vfd_para_attr(void)
{
    u16 alarm_code = CP_NORMAL;
    
    
    switch(cp_para_ram.vfd_para_attr)
    {
    case 0x08:
    case 0x88:    
        if(VFD_RUN == cp_para_ram.vfd_state)
        {
            alarm_code = CP_VFD_RUN;
            
            cp_alarm(alarm_code);
        }
        else
        {
            alarm_code = CP_NORMAL;
        }
        break;
    
    case 0x60:
        alarm_code = CP_READ_ONLY;
        
        cp_alarm(alarm_code);
        break;
        
    case 0x90:
        alarm_code = CP_NON_ZERO;
        
        cp_alarm(alarm_code);
        break;  
        
    case 0xA0:
        alarm_code = CP_READ_ONLY;
        
        cp_alarm(alarm_code);
        break;
        
    default:
        alarm_code = CP_NORMAL;
        break;  
    }

    return (alarm_code);
}

CODE u8 form_para_val_cmd[MAX_FORM_PARA_VAL_CMD][32] = {
    /* FORM_PARA_VAL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_para_val_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_VAL_CMD; i++)
    {        
        len = (form_para_val_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_para_val_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_VAL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_PARA_VAL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    form_para_val_disp();
}

static int form_para_val(unsigned int key_msg, unsigned int form_msg)
{
    static s16 vfd_para_val;


    if(FORM_MSG_DATA == form_msg)
    {
        if(TRUE == cp_para_ram.vfd_para_sign)
        {
            vfd_para_val = (s16)cp_para_ram.vfd_para_val;
        }
        else
        {
            vfd_para_val = 0;
        }
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_SHIFT:
            cp_para_ram.vfd_para_shift++;
            
            if(TRUE == cp_para_ram.vfd_para_sign)
            {
            	cp_para_ram.vfd_para_shift %= get_data_length(abs(vfd_para_val));
            }
            else
            {
            	cp_para_ram.vfd_para_shift %= get_data_length(cp_para_ram.vfd_para_val);
            }
            break;

        case KEY_MSG_ENTER:            
            if(TRUE == func_code_write())
            {
                led_blink_pos = 0;
                cp_para_ram.vfd_para_shift = 0;
                
                return (FORM_MSG_NONE);
            }
            break;

        case KEY_MSG_EXIT:
            led_blink_pos = 0;
            cp_para_ram.vfd_para_shift = 0;
            
            form_id = FORM_ID_PARA_GRADE;

            return (FORM_MSG_NONE);
            //break;

        case KEY_MSG_UP:
        case KEY_MSG_UP_LONG:            
            if(CP_NORMAL == check_vfd_para_attr())
            {
            	if(TRUE == cp_para_ram.vfd_para_sign)
            	{
                	vfd_para_val += pow(10, cp_para_ram.vfd_para_shift);
                	
                	cp_para_ram.vfd_para_val = (u16)vfd_para_val;
                }
                else
                {
                	cp_para_ram.vfd_para_val += pow(10, cp_para_ram.vfd_para_shift);
                }
            }
            break;

        case KEY_MSG_DOWN:
        case KEY_MSG_DOWN_LONG:            
            if(CP_NORMAL == check_vfd_para_attr())
            {
            	if(TRUE == cp_para_ram.vfd_para_sign)
            	{
                	vfd_para_val -= pow(10, cp_para_ram.vfd_para_shift);
                	
                	cp_para_ram.vfd_para_val = (u16)vfd_para_val;
                }
                else
                {
    	            cp_para_ram.vfd_para_val -= pow(10, cp_para_ram.vfd_para_shift);        	
                }
            }
            break;

        default:
            break;
        }
    }

    form_para_val_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_copy_cmd[MAX_FORM_COPY_CMD][32] = {
    /* FORM_COPY_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_copy_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_CMD; i++)
    {        
        len = (form_copy_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_COPY_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = led_table['y' - 32];
    led_disp_buf[1] = led_table['P' - 32];
    led_disp_buf[2] = led_table['o' - 32];
    led_disp_buf[3] = led_table['C' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_copy(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_UPLOAD;
            break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_HOME;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_PARA;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_REF;
            break;

        default:
            break;
        }
    }

    form_copy_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_copy_upload_cmd[MAX_FORM_COPY_UPLOAD_CMD][32] = {
    /* FORM_COPY_UPLOAD_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_copy_upload_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_UPLOAD_CMD; i++)
    {        
        len = (form_copy_upload_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_upload_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_UPLOAD_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_COPY_UPLOAD_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['u' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_copy_upload(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_UPLOAD_RATE;

            return (FORM_MSG_DATA);
            //break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_COPY;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_COPY_DOWNLOAD_PART;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_COPY_DOWNLOAD_ALL;
            break;

        default:
            break;
        }
    }

    form_copy_upload_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_copy_download_all_cmd[MAX_FORM_COPY_DOWNLOAD_ALL_CMD][32] = {
    /* FORM_COPY_DOWNLOAD_ALL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_copy_download_all_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_DOWNLOAD_ALL_CMD; i++)
    {        
        len = (form_copy_download_all_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_download_all_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_DOWNLOAD_ALL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_COPY_DOWNLOAD_ALL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = led_table['A' - 32];
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['d' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_copy_download_all(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_DOWNLOAD_ALL_RATE;

            return (FORM_MSG_DATA);
            //break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_COPY;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_COPY_UPLOAD;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_COPY_DOWNLOAD_PART;
            break;

        default:
            break;
        }
    }

    form_copy_download_all_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_copy_download_part_cmd[MAX_FORM_COPY_DOWNLOAD_PART_CMD][32] = {
    /* FORM_COPY_DOWNLOAD_PART_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void form_copy_download_part_callback(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_DOWNLOAD_PART_CMD; i++)
    {        
        len = (form_copy_download_part_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_download_part_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_DOWNLOAD_PART_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_COPY_DOWNLOAD_PART_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
    
    led_disp_buf[0] = led_table['P' - 32];
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['d' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE_ENABLE();
}

static int form_copy_download_part(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_DOWNLOAD_PART_RATE;

            return (FORM_MSG_DATA);
            //break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_COPY;
            break;

        case KEY_MSG_UP:
            form_id = FORM_ID_COPY_DOWNLOAD_ALL;
            break;

        case KEY_MSG_DOWN:
            form_id = FORM_ID_COPY_UPLOAD;
            break;

        default:
            break;
        }
    }

    form_copy_download_part_callback();

    return (FORM_MSG_NONE);
}

CODE u8 copy_upload_rate_baudrate_cmd[][32] = {
    /* FORM_COPY_UPLOAD_RATE_BAUDRATE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x05, 0x0A, 0x05, 0xA9, 0x50, 0x84, 0x00, 0x01, 0x00, 0x00, 0x4B, 0x00},    
};

bool chang_baudrate(u16 baudrate)
{
    OS_RESULT result;
    u8 len;
    unsigned int crc;
    bool ret = FALSE;


    len = (copy_upload_rate_baudrate_cmd[COPY_UPLOAD_RATE_BAUDRATE][10] + 11) % UART_TX_LEN;
                    
    memcpy(UART_TX_BUF, copy_upload_rate_baudrate_cmd[COPY_UPLOAD_RATE_BAUDRATE], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
    
    UART_TX_BUF[19] = (u8)(baudrate >> 8);
    UART_TX_BUF[20] = (u8)baudrate;

    crc = CRC16Calculate(UART_TX_BUF, len);
    UART_TX_BUF[len++] = (u8)(crc & 0xff);
    UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);

    uart_send(len);

    result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);
    
    if(OS_R_TMO != result)
    {
        uart_recv_align();
        
        if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
        {
            USART_BaudRate(CP_UART, baudrate);
            
            ret = TRUE;
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
            LEDOE_ENABLE();
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
        LEDOE_ENABLE();
    }

    return (ret);
}

CODE u8 form_copy_upload_rate_init_cmd[][32] = {
    /* COPY_UPLOAD_RATE_INIT */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x11, 0x00, 0x59, 0x00, 0x02, 0x04, 0x10, 0xA4, 0x50, 0x0D},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x05, 0x0A, 0x08, 0xAA, 0x50, 0x02, 0x00, 0x03, 0x01, 0x03, 0xFD, 0x82},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
};

void copy_upload_rate_init(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < 5; i++)
    {        
        len = (form_copy_upload_rate_init_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_upload_rate_init_cmd[i], len);

        switch(i)
        {
        case 2:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 2:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
}

CODE u8 copy_comm_reset_cmd[][32] = {
    /* FORM_ID_COPY_UPLOAD_RATE */
    /* FORM_ID_COPY_DOWNLOAD_ALL_RATE */
    /* FORM_ID_COPY_DOWNLOAD_PART_RATE */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
    /* FORM_ID_COPY_DOWNLOAD_ALL_RATE */
    /* FORM_ID_COPY_DOWNLOAD_PART_RATE */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x1D, 0xA1, 0x50, 0x02, 0x00, 0x01},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x05, 0x0A, 0x1D, 0xA1, 0x50, 0x8C, 0x00, 0x03, 0x0F, 0xAA, 0x00, 0x00},
};

void copy_comm_reset(void)
{
    OS_RESULT result;
    u8 i, len, num;
    unsigned int crc;


    if((FORM_ID_COPY_DOWNLOAD_ALL_RATE == form_id) ||
       (FORM_ID_COPY_DOWNLOAD_PART_RATE == form_id))
    {
        num = 6;
    }
    else
    {
        num = 3;
    }
    
    for(i = 0; i < num; i++)
    {        
        len = (copy_comm_reset_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, copy_comm_reset_cmd[i], len);

        switch(i)
        {
        case 0:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case 1:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 2:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 3:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 4:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case 5:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 0:
                case 3:
                    if((0x44 == UART_RX_BUF[3]) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if(FORM_ID_COPY_UPLOAD_RATE == form_id)
                        {
                            i = num; //跳过复位
                        }

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
}

CODE u8 copy_upload_rate_cmd[][32] = {
    /* COPY_UPLOAD_RATE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* COPY_UPLOAD_RATE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x14, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x89, 0x00, 0x08},
    /* COPY_UPLOAD_RATE_TAIL_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x04},
};

void form_copy_upload_rate_callback(void)
{
    OS_RESULT result;
    u8 i, j, len, size;
    unsigned int crc;
    u16 num;
    static bool last_frame = FALSE;
    
    
    for(i = COPY_UPLOAD_RATE_SET_CMD; i < COPY_UPLOAD_RATE_TAIL_CMD; i++)
    {    
        if((TRUE == last_frame) && 
           (COPY_UPLOAD_RATE_CMD == i))
        {
            i = COPY_UPLOAD_RATE_TAIL_CMD;
        }
        
        len = (copy_upload_rate_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, copy_upload_rate_cmd[i], len);

        switch(i)
        {
        case COPY_UPLOAD_RATE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case COPY_UPLOAD_RATE_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case COPY_UPLOAD_RATE_TAIL_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case COPY_UPLOAD_RATE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                case COPY_UPLOAD_RATE_CMD:
                    num = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]); //计数
                    size = UART_RX_BUF[2] - 8; //帧数据长度
                    
                    if((0x41 == UART_RX_BUF[3]) && (0x82 == UART_RX_BUF[4])) //头帧
                    {                        
                        cp_para_ram.vfd_para_total = num;

                        EEPROM_WriteByte(VFD_PARA_FLAG_ADDR, ~VFD_PARA_FLAG); //禁能变频器参数标志

                        os_dly_wait(5); //5ms
                        
                        EEPROM_WriteHalfWord(VFD_PARA_LEN_ADDR, cp_para_ram.vfd_para_total + 2); //存储变频器参数长度
                        
                        os_dly_wait(5); //5ms

                        cp_para_ram.vfd_para_crc = EEPROM_ReadByte(VFD_PARA_LEN_ADDR); //变频器参数长度和变频器参数一起校验

                        os_dly_wait(5); //5ms
                        
                        cp_para_ram.vfd_para_crc += EEPROM_ReadByte(VFD_PARA_LEN_ADDR + 1);

                        os_dly_wait(5); //5ms

                        for(j = 0; j < (size + 2); j++)
                        {
                            EEPROM_WriteByte(VFD_PARA_ADDR + cp_para_ram.vfd_para_index, UART_RX_BUF[9 + j]);

                            cp_para_ram.vfd_para_index++;

                            cp_para_ram.vfd_para_crc += UART_RX_BUF[9 + j];

                            os_dly_wait(5); //5ms
                        }
                    }
                    else if(0 != num) //避开无效帧
                    {
                        cp_para_ram.vfd_para_count = num;

                        for(j = 0; j < size; j++)
                        {
                            EEPROM_WriteByte(VFD_PARA_ADDR + cp_para_ram.vfd_para_index, UART_RX_BUF[11 + j]);

                            cp_para_ram.vfd_para_index++;

                            cp_para_ram.vfd_para_crc += UART_RX_BUF[11 + j];
                                                
                            os_dly_wait(5); //5ms
                        }
                    }

                    if((0x41 == UART_RX_BUF[3]) && (0x22 == UART_RX_BUF[4])) //倒数第二帧
                    {
                        cp_para_ram.vfd_para_count += size; //自身长度
                        
                        last_frame = TRUE;

                        EEPROM_WriteByte(VFD_PARA_ADDR + (cp_para_ram.vfd_para_total + 2), cp_para_ram.vfd_para_crc);

                        os_dly_wait(5); //5ms
                        
                        EEPROM_WriteByte(VFD_PARA_FLAG_ADDR, VFD_PARA_FLAG); //使能变频器参数标志
                        
                        os_dly_wait(5); //5ms
                    }

                    cp_para_ram.rate = (u8)((fp32)cp_para_ram.vfd_para_count / (fp32)cp_para_ram.vfd_para_total * 100);
                    break;

                case COPY_UPLOAD_RATE_TAIL_CMD:
                    if((0x41 == UART_RX_BUF[3]) && (0xA2 == UART_RX_BUF[4])) //尾帧
                    {
                        last_frame = FALSE;
                        
                        cp_para_ram.vfd_para_count = 0;
                        cp_para_ram.vfd_para_total = 0;
                        cp_para_ram.rate = 0;
                        
                        cp_para_ram.vfd_para_index = 0;
                        cp_para_ram.vfd_para_crc = 0;
                        
                        if(TRUE == chang_baudrate(BAUD_RATE_9600))
                        {
                            copy_comm_reset();
                            
                            form_id = FORM_ID_COPY_UPLOAD;
                        }
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }

    led_disp_buf[0] = led_table[cp_para_ram.rate % 10 + 16];
    led_disp_buf[1] = (cp_para_ram.rate > 9) ? (led_table[cp_para_ram.rate % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (cp_para_ram.rate > 99) ? (led_table[cp_para_ram.rate % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = led_table['L' - 32];
    led_disp_buf[4] = led_table['u' - 32];
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= LED_PER_CENT;
    LEDOE_ENABLE();
}

static int form_copy_upload_rate(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        if(TRUE == chang_baudrate(BAUD_RATE_19200))
        {
            copy_upload_rate_init();
        }
        else
        {
            form_id = FORM_ID_COPY_UPLOAD;

            return (FORM_MSG_NONE);
        }
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            break;

        case KEY_MSG_EXIT:
            if(TRUE == chang_baudrate(BAUD_RATE_9600))
            {
                copy_comm_reset();
                
                form_id = FORM_ID_COPY_UPLOAD;

                return (FORM_MSG_NONE);
            }
            break;

        case KEY_MSG_UP:
            break;

        case KEY_MSG_DOWN:
            break;

        default:
            break;
        }
    }

    form_copy_upload_rate_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_copy_download_all_rate_init_cmd[][32] = {
    /* COPY_DOWNLOAD_ALL_RATE_INIT */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x11, 0x00, 0x59, 0x00, 0x02, 0x04, 0x10, 0xA4, 0x50, 0x0D},
    /* COPY_DOWNLOAD_ALL_RATE_SET_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
};

void copy_download_all_rate_init(void)
{
    OS_RESULT result;
    u8 i, len;
    unsigned int crc;
    
    
    for(i = 0; i < 4; i++)
    {        
        len = (form_copy_download_all_rate_init_cmd[i][10] + 11) % UART_TX_LEN;
                        
        memcpy(UART_TX_BUF, form_copy_download_all_rate_init_cmd[i], len);

        switch(i)
        {
        case 1:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        default:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 1:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }
}

bool check_vfd_para(void)
{
    u8 crc;
    u16 i, len;
    bool ret = FALSE;


    if(VFD_PARA_FLAG == EEPROM_ReadByte(VFD_PARA_FLAG_ADDR))
    {
        os_dly_wait(5); //5ms
        
        len = EEPROM_ReadHalfWord(VFD_PARA_LEN_ADDR);

        os_dly_wait(5); //5ms

        if((len + 3) < AT24CXX)
        {
            cp_para_ram.vfd_para_crc = EEPROM_ReadByte(VFD_PARA_ADDR + len);

            os_dly_wait(5); //5ms
            
            for(i = 0, crc = 0; i < (len + 2); i++)
            {
                if(0 == (i % 20))
                {
                    keep_vfd_connect();
                }
                
                crc += EEPROM_ReadByte(VFD_PARA_LEN_ADDR + i);

                os_dly_wait(5); //5ms
            }

            if(cp_para_ram.vfd_para_crc == crc)
            {
                ret = TRUE;
            }
        }
    }

    return (ret);
}

CODE u8 copy_download_all_rate_cmd[][60] = {
    /* COPY_DOWNLOAD_ALL_RATE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* COPY_DOWNLOAD_ALL_RATE_CMD */
    /* 0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34    35    36    37    38    39    40    41    42    43    44    45    46    47    48    49    50    51    52    53    54    55    56    57    58    59 */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x15, 0x2A, 0x01, 0x42, 0x50, 0x89, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3E, 0x06, 0x22, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x01, 0x26, 0x01, 0x6E, 0x00, 0x00, 0x00, 0x00},
    /* COPY_DOWNLOAD_ALL_RATE_TAIL_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x02, 0xA2, 0x50, 0x02},
};

void form_copy_download_all_rate_callback(void)
{
    OS_RESULT result;
    u8 i, j, len;
    unsigned int crc;
    static u8 frame_num = 0;
    
    
    for(i = COPY_DOWNLOAD_ALL_RATE_SET_CMD; i < COPY_DOWNLOAD_ALL_RATE_TAIL_CMD; i++)
    {            
        if(COPY_DOWNLOAD_ALL_RATE_SET_CMD == i)
        {
            len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if((0xff == frame_num) && (COPY_DOWNLOAD_ALL_RATE_CMD == i))
        {
            i = COPY_DOWNLOAD_ALL_RATE_TAIL_CMD;

            len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if(COPY_DOWNLOAD_ALL_RATE_CMD == i)
        {
            if(0 == frame_num) //头帧
            {
                frame_num = 1;
                
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
            
                UART_TX_BUF[9]  = 0x16;
                UART_TX_BUF[10] = 0x2C;
                UART_TX_BUF[11] = 0x08;
                UART_TX_BUF[12] = 0x8A;
                UART_TX_BUF[14] = 0x86;
                UART_TX_BUF[16] = 0x03;
                UART_TX_BUF[17] = 0x01;
                UART_TX_BUF[18] = 0x03;

                len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
            
                cp_para_ram.vfd_para_index = 0;
                cp_para_ram.vfd_para_count = 0;
                cp_para_ram.vfd_para_total = EEPROM_ReadHalfWord(VFD_PARA_LEN_ADDR) - 2;
            
                os_dly_wait(5); //5ms
            }
            else if(1 == frame_num) //第二帧
            {
                frame_num = 2;
            
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
            
                UART_TX_BUF[9]  = 0x14;
                UART_TX_BUF[10] = 0x28;

                len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
            }
            else
            {
                if((cp_para_ram.vfd_para_total - cp_para_ram.vfd_para_count + 10) < 0x2A)
                {
                    frame_num = 0xfe; //倒数第二帧
                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
                    
                    UART_TX_BUF[9]  = 0x06;
                    UART_TX_BUF[10] = cp_para_ram.vfd_para_total - cp_para_ram.vfd_para_count + 10;
                    UART_TX_BUF[12] = 0x22;
                    
                    len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
                }
                else
                {
                    frame_num = 3; //第二帧以后的帧
                    
                    len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
                }
            }
            
            for(j = 0; j < (UART_TX_BUF[10] - 10); j++)
            {
                UART_TX_BUF[21 + j] = EEPROM_ReadByte(VFD_PARA_ADDR + cp_para_ram.vfd_para_index + j);
        
                os_dly_wait(5); //5ms
            }
        
            cp_para_ram.vfd_para_index += UART_TX_BUF[10] - 10;
        
            if(1 == frame_num) //头帧
            {
                UART_TX_BUF[19] = (u8)(cp_para_ram.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(cp_para_ram.vfd_para_count);

                cp_para_ram.vfd_para_count += UART_TX_BUF[10] - 12;
            }
            else
            {
                UART_TX_BUF[19] = (u8)(cp_para_ram.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(cp_para_ram.vfd_para_count);
                
                cp_para_ram.vfd_para_count += UART_TX_BUF[10] - 10;
            }

            cp_para_ram.rate = (u8)((fp32)cp_para_ram.vfd_para_count / (fp32)cp_para_ram.vfd_para_total * 100);
        }

        switch(i)
        {
        case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case COPY_DOWNLOAD_ALL_RATE_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case COPY_DOWNLOAD_ALL_RATE_TAIL_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;
            
        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                case COPY_DOWNLOAD_ALL_RATE_CMD:
                    if((0x41 == UART_RX_BUF[3]) && (0x22 == UART_RX_BUF[4])) //倒数第二帧
                    {
                        frame_num = 0xff;
                    }
                    break;

                case COPY_DOWNLOAD_ALL_RATE_TAIL_CMD:
                    if((0x42 == UART_RX_BUF[3]) && (0xA2 == UART_RX_BUF[4])) //尾帧
                    {
                        frame_num = 0;
                        
                        cp_para_ram.vfd_para_count = 0;
                        cp_para_ram.vfd_para_total = 0;
                        cp_para_ram.rate = 0;
                        
                        cp_para_ram.vfd_para_index = 0;
                        cp_para_ram.vfd_para_crc = 0;
                        
                        if(TRUE == chang_baudrate(BAUD_RATE_9600))
                        {
                            copy_comm_reset();
                            
                            form_id = FORM_ID_COPY_DOWNLOAD_ALL;
                        }
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }

    led_disp_buf[0] = led_table[cp_para_ram.rate % 10 + 16];
    led_disp_buf[1] = (cp_para_ram.rate > 9) ? (led_table[cp_para_ram.rate % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (cp_para_ram.rate > 99) ? (led_table[cp_para_ram.rate % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = led_table['L' - 32];
    led_disp_buf[4] = led_table['d' - 32];
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= LED_PER_CENT;
    LEDOE_ENABLE();
}

static int form_copy_download_all_rate(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        led_disp_buf[0] = led_table['.' - 32];
        led_disp_buf[1] = led_table['.' - 32];
        led_disp_buf[2] = led_table['.' - 32];
        led_disp_buf[3] = led_table['L' - 32];
        led_disp_buf[4] = led_table['d' - 32];
        led_disp_buf[5] |= LED_V_A_Hz_MASK;
        led_disp_buf[5] &= LED_PER_CENT;
        LEDOE_ENABLE();
        
#if 1
        if((TRUE == check_vfd_para()) &&            //校验存储的变频器参数
           (TRUE == chang_baudrate(BAUD_RATE_19200))) //更改变频器参数上传、下载的波特率
        {
            copy_download_all_rate_init();
        }
        else
        {
            form_id = FORM_ID_COPY_DOWNLOAD_ALL;

            return (FORM_MSG_NONE);
        }
#else //调试变频器参数下载
        USART_BaudRate(CP_UART, BAUD_RATE_19200);
#endif        
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            break;

        case KEY_MSG_EXIT:
            if(TRUE == chang_baudrate(BAUD_RATE_9600))
            { 
                copy_comm_reset();
                
                form_id = FORM_ID_COPY_DOWNLOAD_ALL;

                return (FORM_MSG_NONE);
            }
            break;

        case KEY_MSG_UP:
            break;

        case KEY_MSG_DOWN:
            break;

        default:
            break;
        }
    }

    form_copy_download_all_rate_callback();

    return (FORM_MSG_NONE);
}

void form_copy_download_part_rate_callback(void)
{
    OS_RESULT result;
    u8 i, j, len;
    unsigned int crc;
    static u8 frame_num = 0;
    
    
    for(i = COPY_DOWNLOAD_ALL_RATE_SET_CMD; i < COPY_DOWNLOAD_ALL_RATE_TAIL_CMD; i++)
    {            
        if(COPY_DOWNLOAD_ALL_RATE_SET_CMD == i)
        {
            len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if((0xff == frame_num) && (COPY_DOWNLOAD_ALL_RATE_CMD == i))
        {
            i = COPY_DOWNLOAD_ALL_RATE_TAIL_CMD;

            len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if(COPY_DOWNLOAD_ALL_RATE_CMD == i)
        {
            if(0 == frame_num) //头帧
            {
                frame_num = 1;
                
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
            
                UART_TX_BUF[9]  = 0x16;
                UART_TX_BUF[10] = 0x2C;
                UART_TX_BUF[11] = 0x08;
                UART_TX_BUF[12] = 0x8A;
                UART_TX_BUF[14] = 0x86;
                UART_TX_BUF[16] = 0x03;
                UART_TX_BUF[17] = 0x01;
                UART_TX_BUF[18] = 0x04; //变频器参数部分下载与全部下载在此有所区别

                len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
            
                cp_para_ram.vfd_para_index = 0;
                cp_para_ram.vfd_para_count = 0;
                cp_para_ram.vfd_para_total = EEPROM_ReadHalfWord(VFD_PARA_LEN_ADDR) - 2;
            
                os_dly_wait(5); //5ms
            }
            else if(1 == frame_num) //第二帧
            {
                frame_num = 2;
            
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
            
                UART_TX_BUF[9]  = 0x14;
                UART_TX_BUF[10] = 0x28;

                len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
            }
            else
            {
                if((cp_para_ram.vfd_para_total - cp_para_ram.vfd_para_count + 10) < 0x2A)
                {
                    frame_num = 0xfe; //倒数第二帧
                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60 % UART_TX_LEN);
                    
                    UART_TX_BUF[9]  = 0x06;
                    UART_TX_BUF[10] = cp_para_ram.vfd_para_total - cp_para_ram.vfd_para_count + 10;
                    UART_TX_BUF[12] = 0x22;
                    
                    len = (UART_TX_BUF[10] + 11) % UART_TX_LEN;
                }
                else
                {
                    frame_num = 3; //第二帧以后的帧
                    
                    len = (copy_download_all_rate_cmd[i][10] + 11) % UART_TX_LEN;
                                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
                }
            }
            
            for(j = 0; j < (UART_TX_BUF[10] - 10); j++)
            {
                UART_TX_BUF[21 + j] = EEPROM_ReadByte(VFD_PARA_ADDR + cp_para_ram.vfd_para_index + j);
        
                os_dly_wait(5); //5ms
            }
        
            cp_para_ram.vfd_para_index += UART_TX_BUF[10] - 10;
        
            if(1 == frame_num) //头帧
            {
                UART_TX_BUF[19] = (u8)(cp_para_ram.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(cp_para_ram.vfd_para_count);

                cp_para_ram.vfd_para_count += UART_TX_BUF[10] - 12;
            }
            else
            {
                UART_TX_BUF[19] = (u8)(cp_para_ram.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(cp_para_ram.vfd_para_count);
                
                cp_para_ram.vfd_para_count += UART_TX_BUF[10] - 10;
            }

            cp_para_ram.rate = (u8)((fp32)cp_para_ram.vfd_para_count / (fp32)cp_para_ram.vfd_para_total * 100);
        }

        switch(i)
        {
        case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(cp_para_ram.count >> 8);
                UART_TX_BUF[16] = (u8)(cp_para_ram.count & 0xff);

                if(TRUE == cp_para_ram.ref_chang)
                {
                    cp_para_ram.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref_mod >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref_mod >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(cp_para_ram.ref >> 8);
                    UART_TX_BUF[24] = (u8)(cp_para_ram.ref >> 0);
                }

                if(TRUE == cp_para_ram.stop)
                {
                    cp_para_ram.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == cp_para_ram.run)
                {
                    cp_para_ram.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == cp_para_ram.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == cp_para_ram.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case COPY_DOWNLOAD_ALL_RATE_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;

        case COPY_DOWNLOAD_ALL_RATE_TAIL_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (cp_para_ram.cmd & 0x0f);
            break;
            
        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        result = os_sem_wait(&uart_sem, VFD_REPLY_TIMEOUT);

        if(OS_R_TMO != result)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        cp_para_ram.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        cp_para_ram.count++;

                        if((UART_RX_BUF[10] & 0x01) || //0304.Bit0，报警
                           (UART_RX_BUF[11] & 0x80))   //0303.Bit15，故障
                        {
                            form_id = FORM_ID_ERR;
                        }

                        cp_para_ram.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    else
                    {
                        form_id = FORM_ID_ERR;
                    }
                    break;

                case COPY_DOWNLOAD_ALL_RATE_CMD:
                    if((0x41 == UART_RX_BUF[3]) && (0x22 == UART_RX_BUF[4])) //倒数第二帧
                    {
                        frame_num = 0xff;
                    }
                    break;

                case COPY_DOWNLOAD_ALL_RATE_TAIL_CMD:
                    if((0x42 == UART_RX_BUF[3]) && (0xA2 == UART_RX_BUF[4])) //尾帧
                    {
                        frame_num = 0;
                        
                        cp_para_ram.vfd_para_count = 0;
                        cp_para_ram.vfd_para_total = 0;
                        cp_para_ram.rate = 0;
                        
                        cp_para_ram.vfd_para_index = 0;
                        cp_para_ram.vfd_para_crc = 0;
                        
                        if(TRUE == chang_baudrate(BAUD_RATE_9600))
                        {
                            copy_comm_reset();
                            
                            form_id = FORM_ID_COPY_DOWNLOAD_PART;
                        }
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                err_con();
            }
        }
        else
        {
            err_con();
        }
    }

    led_disp_buf[0] = led_table[cp_para_ram.rate % 10 + 16];
    led_disp_buf[1] = (cp_para_ram.rate > 9) ? (led_table[cp_para_ram.rate % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (cp_para_ram.rate > 99) ? (led_table[cp_para_ram.rate % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = led_table['L' - 32];
    led_disp_buf[4] = led_table['d' - 32];
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= LED_PER_CENT;
    LEDOE_ENABLE();
}

static int form_copy_download_part_rate(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        led_disp_buf[0] = led_table['.' - 32];
        led_disp_buf[1] = led_table['.' - 32];
        led_disp_buf[2] = led_table['.' - 32];
        led_disp_buf[3] = led_table['L' - 32];
        led_disp_buf[4] = led_table['d' - 32];
        led_disp_buf[5] |= LED_V_A_Hz_MASK;
        led_disp_buf[5] &= LED_PER_CENT;
        LEDOE_ENABLE();
        
#if 1
        if((TRUE == check_vfd_para()) &&            //校验存储的变频器参数
           (TRUE == chang_baudrate(BAUD_RATE_19200))) //更改变频器参数上传、下载的波特率
        {
            copy_download_all_rate_init();
        }
        else
        {
            form_id = FORM_ID_COPY_DOWNLOAD_PART;

            return (FORM_MSG_NONE);
        }
#else //调试变频器参数下载
        USART_BaudRate(CP_UART, BAUD_RATE_19200);
#endif        
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            if(VFD_LOC == cp_para_ram.lr)
            {
                cp_para_ram.run = TRUE;
                
                cp_para_ram.vfd_state = VFD_RUN;
                
                led_disp_buf[5] &= LED_RUN;
                LEDOE_ENABLE();
            }
            else
            {
                cp_alarm(CP_VFD_REM);
            }
            break;
            
        case KEY_MSG_STOP:
            cp_para_ram.stop = TRUE;
            
            cp_para_ram.vfd_state = VFD_STOP;
        
            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE_ENABLE();
            break;

        case KEY_MSG_LOC_REM:
            cp_para_ram.lr = VFD_REM;      
        
            led_disp_buf[5] |= LED_LOC_REM_MASK;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM);
            break;
            
        case KEY_MSG_LOC_REM_LONG:
            cp_para_ram.lr = VFD_LOC;           
        
            led_disp_buf[5] &= LED_LOC_REM;
            LEDOE_ENABLE();

            form_key_callback(KEY_MSG_LOC_REM_LONG);
            break;

        case KEY_MSG_FWD_REV:
            cp_para_ram.fr = !cp_para_ram.fr;

            if(VFD_REV == cp_para_ram.fr)
            {
                led_disp_buf[5] &= LED_FWD_REV;
                LEDOE_ENABLE();
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE_ENABLE();
            }
            break;

        case KEY_MSG_ENTER:
            break;

        case KEY_MSG_EXIT:
            if(TRUE == chang_baudrate(BAUD_RATE_9600))
            { 
                copy_comm_reset();
                
                form_id = FORM_ID_COPY_DOWNLOAD_PART;

                return (FORM_MSG_NONE);
            }
            break;

        case KEY_MSG_UP:
            break;

        case KEY_MSG_DOWN:
            break;

        default:
            break;
        }
    }

    form_copy_download_part_rate_callback();

    return (FORM_MSG_NONE);
}

__task void AppTaskCP(void)
{
    OS_RESULT result;
    unsigned int key_msg;
    unsigned int form_msg;


    os_dly_wait(1000);
    
    os_sem_send(&key_sem);
    
#if (EEPROM_TEST_EN > 0u)
    EEPROM_Test();
#endif

    MEM_Init();

    MENU_Init();

    vfd_con();
    
    while(1)
    {
        result = os_sem_wait(&cp_sem, 5);

        switch(result)
        {
        case OS_R_TMO: 
            key_msg = KEY_MSG_NONE;
            break;
            
        case OS_R_SEM:
        case OS_R_OK:
            key_msg = KEY_MSG;
            
            os_sem_send(&key_sem);
            break;
            
        default:   
            key_msg = KEY_MSG_NONE;
            break;
        }

        if(NULL != form_list[form_id])
        {
            if(FORM_MSG_NONE != (form_msg = (*form_list[form_id])(key_msg, FORM_MSG_KEY)))
            {
                (*form_list[form_id])(key_msg, form_msg);
            }
        }
    }
}

