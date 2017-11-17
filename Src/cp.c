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


static int form_home(unsigned int key_msg, unsigned int form_msg);
static int form_ref(unsigned int key_msg, unsigned int form_msg);
static int form_ref_val(unsigned int key_msg, unsigned int form_msg);

bool runstatus = FALSE;
CP g_cp_para;

code unsigned int wCRC16Table[256] = {   
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

code u8 special_cmd[][32] = {
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x04, 0x08, 0x04, 0xA4, 0x50, 0x82, 0x0A, 0x01, 0x00, 0x06},
};

unsigned int CRC16Calculate(unsigned char *J_u8DataIn, unsigned int J_u16DataLen)  
{  
    unsigned int J_u16Result = 0xffff;
    unsigned int J_u16TableNo = 0;
    unsigned int J_u16Index;  
    
    
    if(J_u16DataLen > 50)
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

code u8 con_cmd[][32] = {
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x90, 0x82, 0x00, 0x02},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x01},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x17, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15, 0x38, 0x5C},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01, 0xFB, 0x3D},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0xC9, 0xF6, 0xBA},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xDA},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0x65},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
};

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

void vfd_con(void)
{
    s8 i;
    u8 len, timeout;
    unsigned int crc;
    

    memset(&g_cp_para, 0, sizeof(g_cp_para));
   
    for(i = 0; i < 8; i++)
    {        
        len = con_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, con_cmd[i], len);

        switch(i)
        {
        case 2:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case 3:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case 4:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case 5:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((4 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(g_cp_para.count >> 8);
                UART_TX_BUF[16] = (u8)(g_cp_para.count & 0xff);

    			if(TRUE == g_cp_para.reset)
    			{
    				g_cp_para.reset = FALSE;
                    
    				UART_TX_BUF[20] |= 0x10;
    			}

    			if(TRUE == g_cp_para.ref_chang)
    			{
    				g_cp_para.ref_chang = FALSE;
                    
    				UART_TX_BUF[23] = (u8)(g_cp_para.ref_temp >> 8);
    				UART_TX_BUF[24] = (u8)(g_cp_para.ref_temp >> 0);
    			}
                else
    			{
    				UART_TX_BUF[23] = (u8)(g_cp_para.ref >> 8);
    				UART_TX_BUF[24] = (u8)(g_cp_para.ref >> 0);
    			}

    			if(TRUE == g_cp_para.stop)
    			{
    				g_cp_para.stop = FALSE;
                    
    				UART_TX_BUF[20] |= 0x01;
    			}
                
    			if(TRUE == g_cp_para.start)
    			{
    				g_cp_para.start = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}

    			if(OPER_LOC == g_cp_para.oper)
    			{
    				UART_TX_BUF[20] |= 0x08;
    			}
                else
    			{
    				UART_TX_BUF[20] &= ~0x08;
    			}               
            }
            break;

        case 6:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case 7:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((4 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(g_cp_para.count >> 8);
                UART_TX_BUF[16] = (u8)(g_cp_para.count & 0xff);

    			if(TRUE == g_cp_para.reset)
    			{
    				g_cp_para.reset = FALSE;
                    
    				UART_TX_BUF[20] |= 0x10;
    			}

    			if(TRUE == g_cp_para.ref_chang)
    			{
    				g_cp_para.ref_chang = FALSE;
                    
    				UART_TX_BUF[23] = (u8)(g_cp_para.ref_temp >> 8);
    				UART_TX_BUF[24] = (u8)(g_cp_para.ref_temp >> 0);
    			}
                else
    			{
    				UART_TX_BUF[23] = (u8)(g_cp_para.ref >> 8);
    				UART_TX_BUF[24] = (u8)(g_cp_para.ref >> 0);
    			}

    			if(TRUE == g_cp_para.stop)
    			{
    				g_cp_para.stop = FALSE;
                    
    				UART_TX_BUF[20] |= 0x01;
    			}
                
    			if(TRUE == g_cp_para.start)
    			{
    				g_cp_para.start = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}
                        
                if(OPER_LOC == g_cp_para.oper)
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

        /* CPTask与KeyTask已经有信号在通信，受限于RTX-51 TINY弱小的功能，
         * 这里CPTask不能使用同一信号与UartTask进行通信，否则可能产生冲突，导致丢失信号
         * 华兄 */
        for(timeout = 0; timeout <= VFD_REPLY_TIMEOUT; timeout++) //等待变频器应答
        {
            /* 2500 = 1s */
            os_wait(K_TMO, 25, 0);

            if(TRUE == uart_rx_complete) //串口接收数据完毕
            {
                break;
            }
        }

        if(TRUE == uart_rx_complete)
        {
            uart_recv_align();
                        
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case 1:
                    g_cp_para.cmd = UART_RX_BUF[5];
                    break;
                
                case 5:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if (UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        g_cp_para.oper = UART_RX_BUF[11] & 0x10;

                        if(OPER_LOC == g_cp_para.oper) //本地
                        {
                            LED_BUFF[5] &= ~LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                        else //远程
                        {
                            LED_BUFF[5] |= LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                    }
                    break;

                case 7:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if (UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        g_cp_para.oper = UART_RX_BUF[11] & 0x10;
                        
                        if(OPER_LOC == g_cp_para.oper) //本地
                        {
                            LED_BUFF[5] &= ~LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                        else //远程
                        {
                            LED_BUFF[5] |= LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                    }
                    break;
                
                default:
                    break;
                }

                LED_BUFF[4] = 0xff;
                LED_BUFF[3] = 0xff;
                LED_BUFF[2] = 0xff;
                LED_BUFF[1] = led_tab['C' - 32];
                LED_BUFF[0] = led_tab['P' - 32];
                LEDOE = 0;
            }
            else
            {
                LED_BUFF[4] = 0xff;
                LED_BUFF[3] = led_tab['E' - 32];
                LED_BUFF[2] = led_tab['r' - 32];
                LED_BUFF[1] = led_tab['r' - 32];
                LED_BUFF[0] = led_tab[i + 16];
                LEDOE = 0;

                i = -1; //启动重复发送连接命令
            }
        }
        else
        { 
            LED_BUFF[4] = 0xff;
            LED_BUFF[3] = led_tab['E' - 32];
            LED_BUFF[2] = led_tab['r' - 32];
            LED_BUFF[1] = led_tab['r' - 32];
            LED_BUFF[0] = led_tab[i + 16];
            LEDOE = 0;

            i = -1; //启动重复发送连接命令
        }
        
        uart_recv_clear();
    }
}

static const FORM form_list[MAX_FORM_NUM] =
{
    {form_home},
    {form_home},
    {form_home}, 
    {form_ref},
    {form_ref_val},
};

static unsigned int form_id;

void MENU_Init(void)
{
    form_id = FORM_ID_HOME1;
    
    form_home(FORM_ID_HOME, FORM_MSG_DATA);
}

code u8 form_home_cmd[MAX_FORM_HOME_CMD][32] = {
    /* FORM_HOME_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00 ,0x04, 00, 00, 00, 0x08, 00, 00, 0x09, 0xC4, 00, 00, 00, 00},
    /* FORM_HOME_READ_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x06, 0x00, 0x59, 0x00, 0x02, 0x04, 0x04, 0xAB, 0x50, 0x08},
    /* FORM_HOME_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_HOME_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_home_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_HOME_CMD; i++)
    {        
        len = form_home_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_home_cmd[i], len);

        switch(i)
        {
        case FORM_HOME_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((4 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(g_cp_para.count >> 8);
                UART_TX_BUF[16] = (u8)(g_cp_para.count & 0xff);

                if(TRUE == g_cp_para.reset)
                {
                    g_cp_para.reset = FALSE;
                    
                    UART_TX_BUF[20] |= 0x10;
                }

                if(TRUE == g_cp_para.ref_chang)
                {
                    g_cp_para.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref_temp >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref_temp >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref >> 0);
                }

                if(TRUE == g_cp_para.stop)
                {
                    g_cp_para.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == g_cp_para.start)
                {
                    g_cp_para.start = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }
                
                if(OPER_LOC == g_cp_para.oper)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_HOME_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_HOME_FAULT_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        /* CPTask与KeyTask已经有信号在通信，受限于RTX-51 TINY弱小的功能，
         * 这里CPTask不能使用同一信号与UartTask进行通信，否则可能产生冲突，导致丢失信号
         * 华兄 */
        for(timeout = 0; timeout <= VFD_REPLY_TIMEOUT; timeout++) //等待变频器应答
        {
            /* 2500 = 1s */
            os_wait(K_TMO, 25, 0);

            if(TRUE == uart_rx_complete) //串口接收数据完毕
            {
                break;
            }
        }
        
        if(TRUE == uart_rx_complete)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_HOME_SET_CMD:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if (UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                case FORM_HOME_READ_CMD:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xab == UART_RX_BUF[4]))
                    {
                        g_cp_para.disp_para1 = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]);
                        g_cp_para.disp_para2 = ((u16)UART_RX_BUF[11] << 8) | ((u16)UART_RX_BUF[12]);
                        g_cp_para.disp_para3 = ((u16)UART_RX_BUF[13] << 8) | ((u16)UART_RX_BUF[14]);
            
                        switch(form_id)
                        {
                        case FORM_ID_HOME1:
                            LED_BUFF[0] = led_tab[g_cp_para.disp_para1 % 10 + 16];
                            LED_BUFF[1] = (g_cp_para.disp_para1 > 9) ? (led_tab[g_cp_para.disp_para1 % 100 / 10 + 16]) : (0xff);
                            LED_BUFF[2] = (g_cp_para.disp_para1 > 99) ? (led_tab[g_cp_para.disp_para1 % 1000 / 100 + 16]) : (0xff);
                            LED_BUFF[3] = (g_cp_para.disp_para1 > 999) ? (led_tab[g_cp_para.disp_para1 % 10000 / 1000 + 16]) : (0xff);
                            LED_BUFF[4] = (g_cp_para.disp_para1 > 9999) ? (led_tab[g_cp_para.disp_para1 % 100000 / 10000 + 16]) : (0xff);
                            LED_BUFF[5] |= LED_V_A_Hz_MASK;
                            LED_BUFF[5] &= ~LED_V_MASK;
                            LEDOE = 0;
                            break;

                        case FORM_ID_HOME2:
                            LED_BUFF[0] = led_tab[g_cp_para.disp_para2 % 10 + 16];
                            LED_BUFF[1] = led_tab[g_cp_para.disp_para2 % 100 / 10 + 16] & (~LED_DP_MASK);
                            LED_BUFF[2] = (g_cp_para.disp_para2 > 99) ? (led_tab[g_cp_para.disp_para2 % 1000 / 100 + 16]) : (0xff);
                            LED_BUFF[3] = (g_cp_para.disp_para2 > 999) ? (led_tab[g_cp_para.disp_para2 % 10000 / 1000 + 16]) : (0xff);
                            LED_BUFF[4] = (g_cp_para.disp_para2 > 9999) ? (led_tab[g_cp_para.disp_para2 % 100000 / 10000 + 16]) : (0xff);
                            LED_BUFF[5] |= LED_V_A_Hz_MASK;
                            LED_BUFF[5] &= ~LED_TORQUE_MASK;
                            LEDOE = 0;
                            break;

                        case FORM_ID_HOME3:
                            LED_BUFF[0] = led_tab[g_cp_para.disp_para3 % 10 + 16];
                            LED_BUFF[1] = led_tab[g_cp_para.disp_para3 % 100 / 10 + 16] & (~LED_DP_MASK);
                            LED_BUFF[2] = (g_cp_para.disp_para3 > 99) ? (led_tab[g_cp_para.disp_para3 % 1000 / 100 + 16]) : (0xff);
                            LED_BUFF[3] = (g_cp_para.disp_para3 > 999) ? (led_tab[g_cp_para.disp_para3 % 10000 / 1000 + 16]) : (0xff);
                            LED_BUFF[4] = (g_cp_para.disp_para3 > 9999) ? (led_tab[g_cp_para.disp_para3 % 100000 / 10000 + 16]) : (0xff);
                            LED_BUFF[5] |= LED_V_A_Hz_MASK;
                            LED_BUFF[5] &= ~LED_Hz_MASK;
                            LEDOE = 0;
                            break;

                        default:
                            break;
                        }
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                LED_BUFF[4] = 0xff;
                LED_BUFF[3] = led_tab['E' - 32];
                LED_BUFF[2] = led_tab['r' - 32];
                LED_BUFF[1] = led_tab['r' - 32];
                LED_BUFF[0] = led_tab[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            LED_BUFF[4] = 0xff;
            LED_BUFF[3] = led_tab['E' - 32];
            LED_BUFF[2] = led_tab['r' - 32];
            LED_BUFF[1] = led_tab['r' - 32];
            LED_BUFF[0] = led_tab[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
}

static int form_home(unsigned int key_msg, unsigned int form_msg)
{
    form_msg = form_msg;
    
    switch(key_msg)
    {
    case KEY_MSG_START:
        g_cp_para.start = TRUE;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_RUN_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_STOP:
        g_cp_para.stop = TRUE;

        LED_BUFF[5] |= LED_RUN_MASK;
        LEDOE = 0;
        break;

    case KEY_MSG_LOC_REM:
        /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
         * 逻辑非(!x)的等价式: !x = (0 == x)
         * 华兄 */
        g_cp_para.oper = !g_cp_para.oper;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        else
        {
            LED_BUFF[5] |= LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_UP:
        form_id++;
        form_id %= 3;
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
        break;
        
    default:
        break;
    }

    form_home_callback();

    return (TRUE);
}

code u8 form_ref_cmd[MAX_FORM_REF_CMD][32] = {
    /* FORM_REF_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00 ,0x04, 00, 00, 00, 0x08, 00, 00, 0x09, 0xC4, 00, 00, 00, 00},
    /* FORM_REF_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_REF_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_ref_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_REF_CMD; i++)
    {        
        len = form_ref_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_ref_cmd[i], len);

        switch(i)
        {
        case FORM_HOME_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((4 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(g_cp_para.count >> 8);
                UART_TX_BUF[16] = (u8)(g_cp_para.count & 0xff);

                if(TRUE == g_cp_para.reset)
                {
                    g_cp_para.reset = FALSE;
                    
                    UART_TX_BUF[20] |= 0x10;
                }

                if(TRUE == g_cp_para.ref_chang)
                {
                    g_cp_para.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref_temp >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref_temp >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref >> 0);
                }

                if(TRUE == g_cp_para.stop)
                {
                    g_cp_para.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == g_cp_para.start)
                {
                    g_cp_para.start = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }
                
                if(OPER_LOC == g_cp_para.oper)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_HOME_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_HOME_FAULT_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        /* CPTask与KeyTask已经有信号在通信，受限于RTX-51 TINY弱小的功能，
         * 这里CPTask不能使用同一信号与UartTask进行通信，否则可能产生冲突，导致丢失信号
         * 华兄 */
        for(timeout = 0; timeout <= VFD_REPLY_TIMEOUT; timeout++) //等待变频器应答
        {
            /* 2500 = 1s */
            os_wait(K_TMO, 25, 0);

            if(TRUE == uart_rx_complete) //串口接收数据完毕
            {
                break;
            }
        }
        
        if(TRUE == uart_rx_complete)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_HOME_SET_CMD:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if (UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                LED_BUFF[4] = 0xff;
                LED_BUFF[3] = led_tab['E' - 32];
                LED_BUFF[2] = led_tab['r' - 32];
                LED_BUFF[1] = led_tab['r' - 32];
                LED_BUFF[0] = led_tab[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            LED_BUFF[4] = 0xff;
            LED_BUFF[3] = led_tab['E' - 32];
            LED_BUFF[2] = led_tab['r' - 32];
            LED_BUFF[1] = led_tab['r' - 32];
            LED_BUFF[0] = led_tab[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    LED_BUFF[0] = 0xff;
    LED_BUFF[1] = led_tab['F' - 32];
    LED_BUFF[2] = led_tab['E' - 32];
    LED_BUFF[3] = led_tab['r' - 32];
    LED_BUFF[4] = 0xff;
    LED_BUFF[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_ref(unsigned int key_msg, unsigned int form_msg)
{
    form_msg = form_msg;
    
    switch(key_msg)
    {
    case KEY_MSG_START:
        g_cp_para.start = TRUE;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_RUN_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_STOP:
        g_cp_para.stop = TRUE;

        LED_BUFF[5] |= LED_RUN_MASK;
        LEDOE = 0;
        break;

    case KEY_MSG_LOC_REM:
        /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
         * 逻辑非(!x)的等价式: !x = (0 == x)
         * 华兄 */
        g_cp_para.oper = !g_cp_para.oper;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        else
        {
            LED_BUFF[5] |= LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_ENTER:
        form_id = FORM_ID_REF_VAL;
        break;
        
    case KEY_MSG_EXIT:
        form_id = FORM_ID_HOME1;
        break;

    default:
        break;
    }

    form_ref_callback();

    return (TRUE);
}

code u8 form_ref_val_cmd[MAX_FORM_REF_VAL_CMD][32] = {
    /* FORM_REF_VAL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00 ,0x04, 00, 00, 00, 0x08, 00, 00, 0x09, 0xC4, 00, 00, 00, 00},
    /* FORM_REF_VAL_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_REF_VAL_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_ref_val_callback(void)
{
    u8 i, len, timeout;
    u16 temp;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_REF_VAL_CMD; i++)
    {        
        len = form_ref_val_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_ref_val_cmd[i], len);

        switch(i)
        {
        case FORM_HOME_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((4 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
            {
                UART_TX_BUF[15] = (u8)(g_cp_para.count >> 8);
                UART_TX_BUF[16] = (u8)(g_cp_para.count & 0xff);

                if(TRUE == g_cp_para.reset)
                {
                    g_cp_para.reset = FALSE;
                    
                    UART_TX_BUF[20] |= 0x10;
                }

                if(TRUE == g_cp_para.ref_chang)
                {
                    g_cp_para.ref_chang = FALSE;
                    
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref_temp >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref_temp >> 0);
                }
                else
                {
                    UART_TX_BUF[23] = (u8)(g_cp_para.ref >> 8);
                    UART_TX_BUF[24] = (u8)(g_cp_para.ref >> 0);
                }

                if(TRUE == g_cp_para.stop)
                {
                    g_cp_para.stop = FALSE;
                    
                    UART_TX_BUF[20] |= 0x01;
                }
                
                if(TRUE == g_cp_para.start)
                {
                    g_cp_para.start = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }
                
                if(OPER_LOC == g_cp_para.oper)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_HOME_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_HOME_FAULT_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        default:
            break;
        }

        crc = CRC16Calculate(UART_TX_BUF, len);
        UART_TX_BUF[len++] = (u8)(crc & 0xff);
        UART_TX_BUF[len++] = (u8)((crc & 0xff00) >> 8);
        
        uart_send(len);

        /* CPTask与KeyTask已经有信号在通信，受限于RTX-51 TINY弱小的功能，
         * 这里CPTask不能使用同一信号与UartTask进行通信，否则可能产生冲突，导致丢失信号
         * 华兄 */
        for(timeout = 0; timeout <= VFD_REPLY_TIMEOUT; timeout++) //等待变频器应答
        {
            /* 2500 = 1s */
            os_wait(K_TMO, 25, 0);

            if(TRUE == uart_rx_complete) //串口接收数据完毕
            {
                break;
            }
        }
        
        if(TRUE == uart_rx_complete)
        {
            uart_recv_align();
            
            if(0 == CRC16Calculate(UART_RX_BUF, uart_rx_count))
            {
                switch(i)
                {
                case FORM_HOME_SET_CMD:
                    if((4 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if (UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                LED_BUFF[4] = 0xff;
                LED_BUFF[3] = led_tab['E' - 32];
                LED_BUFF[2] = led_tab['r' - 32];
                LED_BUFF[1] = led_tab['r' - 32];
                LED_BUFF[0] = led_tab[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            LED_BUFF[4] = 0xff;
            LED_BUFF[3] = led_tab['E' - 32];
            LED_BUFF[2] = led_tab['r' - 32];
            LED_BUFF[1] = led_tab['r' - 32];
            LED_BUFF[0] = led_tab[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }

    temp = g_cp_para.ref_temp / 100;
    
    LED_BUFF[0] = led_tab[temp % 10 + 16];
    LED_BUFF[1] = led_tab[temp % 100 / 10 + 16] & (~LED_DP_MASK);
    LED_BUFF[2] = (temp > 99) ? (led_tab[temp % 1000 / 100 + 16]) : (0xff);
    LED_BUFF[3] = (temp > 999) ? (led_tab[temp % 10000 / 1000 + 16]) : (0xff);
    LED_BUFF[4] = (temp > 9999) ? (led_tab[temp % 100000 / 10000 + 16]) : (0xff);
    LED_BUFF[5] |= LED_V_A_Hz_MASK;
    LED_BUFF[5] &= ~LED_Hz_MASK;
    LEDOE = 0;
}

static int form_ref_val(unsigned int key_msg, unsigned int form_msg)
{
    form_msg = form_msg;
    
    switch(key_msg)
    {        
    case KEY_MSG_START:
        g_cp_para.start = TRUE;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_RUN_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_STOP:
        g_cp_para.stop = TRUE;

        LED_BUFF[5] |= LED_RUN_MASK;
        LEDOE = 0;
        break;

    case KEY_MSG_LOC_REM:
        /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
         * 逻辑非(!x)的等价式: !x = (0 == x)
         * 华兄 */
        g_cp_para.oper = !g_cp_para.oper;

        if(OPER_LOC == g_cp_para.oper)
        {
            LED_BUFF[5] &= ~LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        else
        {
            LED_BUFF[5] |= LED_LOC_REM_MASK;
            LEDOE = 0;
        }
        break;

    case KEY_MSG_ENTER:
        g_cp_para.ref_chang = TRUE;
        form_id = FORM_ID_REF;
        break;
        
    case KEY_MSG_EXIT:
        g_cp_para.ref_temp = g_cp_para.ref;
        form_id = FORM_ID_REF;
        break;

    case KEY_MSG_UP:
        g_cp_para.ref_temp += 100;
        g_cp_para.ref_temp %= MAX_REF_VAL;
        break;

    case KEY_MSG_DOWN:
        if(g_cp_para.ref_temp > 100)
        {
            g_cp_para.ref_temp -= 100;
        }
        else
        {
            g_cp_para.ref_temp = MAX_REF_VAL;
        }
        break;

    default:
        break;
    }

    form_ref_val_callback();

    return (TRUE);
}

void CPTask(void) _task_ CP_TASK
{
    u8 event;
    unsigned int key_msg;
    unsigned int form_msg;


    os_wait(K_TMO, 250, 0);
    os_wait(K_TMO, 250, 0);
    
    os_send_signal(KEY_TASK);
    
    MENU_Init();

    vfd_con();
    
    while(1)
    {
        event = os_wait(K_SIG + K_TMO, 5, 0);

        switch(event)
        {
        case TMO_EVENT: 
            key_msg = KEY_MSG_NONE;
            break;
            
        case SIG_EVENT:
            key_msg = KEY_MSG;
            os_send_signal(KEY_TASK);
            break;
            
        default:   
            key_msg = KEY_MSG_NONE;
            break;
        }
        
        (*form_list[form_id])(key_msg, form_msg);
    }
}

