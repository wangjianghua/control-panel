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


XDATA bool runstatus = FALSE;
XDATA CP g_cp_para;

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

CODE u8 con_cmd[][32] = {
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x90, 0x82, 0x00, 0x02},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x01},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x17, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15, 0x38, 0x5C},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01, 0xFB, 0x3D},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0xC9, 0xF6, 0xBA},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x07, 0xA2, 0x50, 0x82, 0x00, 0x65},
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
};

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

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
    			if(TRUE == g_cp_para.run)
    			{
    				g_cp_para.run = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }

    			if(VFD_LOC == g_cp_para.lr)
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

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
    			if(TRUE == g_cp_para.run)
    			{
    				g_cp_para.run = FALSE;
                    
    				UART_TX_BUF[20] |= 0x02;
    			}

                UART_TX_BUF[20] &= ~0x04; //VFD_FWD

                UART_TX_BUF[20] |= 0x08; //VFD_LOC
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
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        if(UART_RX_BUF[11] & 0x10)
                        {
                            g_cp_para.lr = VFD_LOC;
                        }
                        else
                        {
                            g_cp_para.lr = VFD_REM;
                        }

                        if(VFD_LOC == g_cp_para.lr) //本地
                        {
                            led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                        else //远程
                        {
                            led_disp_buf[5] |= LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                    }
                    break;

                case 7:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        if(UART_RX_BUF[11] & 0x10)
                        {
                            g_cp_para.lr = VFD_LOC;
                        }
                        else
                        {
                            g_cp_para.lr = VFD_REM;
                        }
                        
                        if(VFD_LOC == g_cp_para.lr) //本地
                        {
                            led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                        else //远程
                        {
                            led_disp_buf[5] |= LED_LOC_REM_MASK;
                            LEDOE = 0;
                        }
                    }
                    break;
                
                default:
                    break;
                }

                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = 0xff;
                led_disp_buf[2] = 0xff;
                led_disp_buf[1] = led_table['C' - 32];
                led_disp_buf[0] = led_table['P' - 32];
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;

                i = -1; //启动重复发送连接命令
            }
        }
        else
        { 
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
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
    LEDOE = 0;
    
    form_id = FORM_ID_HOME1;

    os_wait(K_TMO, 10, 0);
}

CODE u8 form_home_cmd[MAX_FORM_HOME_CMD][32] = {
    /* FORM_HOME_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
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

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                case FORM_HOME_READ_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xab == UART_RX_BUF[4]))
                    {
                        g_cp_para.disp_para1 = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]);
                        g_cp_para.disp_para2 = ((u16)UART_RX_BUF[11] << 8) | ((u16)UART_RX_BUF[12]);
                        g_cp_para.disp_para3 = ((u16)UART_RX_BUF[13] << 8) | ((u16)UART_RX_BUF[14]);
            
                        switch(form_id)
                        {
                        case FORM_ID_HOME1:
                            led_disp_buf[0] = led_table[g_cp_para.disp_para1 % 10 + 16];
                            led_disp_buf[1] = (g_cp_para.disp_para1 > 9) ? (led_table[g_cp_para.disp_para1 % 100 / 10 + 16]) : (0xff);
                            led_disp_buf[2] = (g_cp_para.disp_para1 > 99) ? (led_table[g_cp_para.disp_para1 % 1000 / 100 + 16]) : (0xff);
                            led_disp_buf[3] = (g_cp_para.disp_para1 > 999) ? (led_table[g_cp_para.disp_para1 % 10000 / 1000 + 16]) : (0xff);
                            led_disp_buf[4] = (g_cp_para.disp_para1 > 9999) ? (led_table[g_cp_para.disp_para1 % 100000 / 10000 + 16]) : (0xff);
                            led_disp_buf[5] |= LED_V_A_Hz_MASK;
                            led_disp_buf[5] &= ~LED_V_MASK;
                            LEDOE = 0;
                            break;

                        case FORM_ID_HOME2:
                            led_disp_buf[0] = led_table[g_cp_para.disp_para2 % 10 + 16];
                            led_disp_buf[1] = led_table[g_cp_para.disp_para2 % 100 / 10 + 16] & (~LED_DP_MASK);
                            led_disp_buf[2] = (g_cp_para.disp_para2 > 99) ? (led_table[g_cp_para.disp_para2 % 1000 / 100 + 16]) : (0xff);
                            led_disp_buf[3] = (g_cp_para.disp_para2 > 999) ? (led_table[g_cp_para.disp_para2 % 10000 / 1000 + 16]) : (0xff);
                            led_disp_buf[4] = (g_cp_para.disp_para2 > 9999) ? (led_table[g_cp_para.disp_para2 % 100000 / 10000 + 16]) : (0xff);
                            led_disp_buf[5] |= LED_V_A_Hz_MASK;
                            led_disp_buf[5] &= ~LED_TORQUE_MASK;
                            LEDOE = 0;
                            break;

                        case FORM_ID_HOME3:
                            led_disp_buf[0] = led_table[g_cp_para.disp_para3 % 10 + 16];
                            led_disp_buf[1] = led_table[g_cp_para.disp_para3 % 100 / 10 + 16] & (~LED_DP_MASK);
                            led_disp_buf[2] = (g_cp_para.disp_para3 > 99) ? (led_table[g_cp_para.disp_para3 % 1000 / 100 + 16]) : (0xff);
                            led_disp_buf[3] = (g_cp_para.disp_para3 > 999) ? (led_table[g_cp_para.disp_para3 % 10000 / 1000 + 16]) : (0xff);
                            led_disp_buf[4] = (g_cp_para.disp_para3 > 9999) ? (led_table[g_cp_para.disp_para3 % 100000 / 10000 + 16]) : (0xff);
                            led_disp_buf[5] |= LED_V_A_Hz_MASK;
                            led_disp_buf[5] &= ~LED_Hz_MASK;
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
}

static int form_home(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
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
    }

    form_home_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_ref_cmd[MAX_FORM_REF_CMD][32] = {
    /* FORM_REF_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_REF_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_REF_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_ref_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_REF_CMD - 2; i++)
    {        
        len = form_ref_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_ref_cmd[i], len);

        switch(i)
        {
        case FORM_REF_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_REF_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_REF_FAULT_CMD:
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
                case FORM_REF_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = led_table['F' - 32];
    led_disp_buf[2] = led_table['E' - 32];
    led_disp_buf[3] = led_table['r' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_ref(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
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

CODE u8 form_ref_val_cmd[MAX_FORM_REF_VAL_CMD][32] = {
    /* FORM_REF_VAL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
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
    
    
    for(i = 0; i < MAX_FORM_REF_VAL_CMD - 2; i++)
    {        
        len = form_ref_val_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_ref_val_cmd[i], len);

        switch(i)
        {
        case FORM_REF_VAL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_REF_VAL_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_REF_VAL_FAULT_CMD:
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
                case FORM_REF_VAL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }

    temp = g_cp_para.ref_temp / 100;
    
    led_disp_buf[0] = led_table[temp % 10 + 16];
    led_disp_buf[1] = led_table[temp % 100 / 10 + 16] & (~LED_DP_MASK);
    led_disp_buf[2] = (temp > 99) ? (led_table[temp % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = (temp > 999) ? (led_table[temp % 10000 / 1000 + 16]) : (0xff);
    led_disp_buf[4] = (temp > 9999) ? (led_table[temp % 100000 / 10000 + 16]) : (0xff);
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= ~LED_Hz_MASK;
    LEDOE = 0;
}

static int form_ref_val(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
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
    }

    form_ref_val_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_para_cmd[MAX_FORM_PARA_CMD][32] = {
    /* FORM_PARA_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_PARA_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_PARA_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_para_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_CMD - 2; i++)
    {        
        len = form_para_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_para_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_PARA_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_PARA_FAULT_CMD:
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
                case FORM_PARA_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = led_table['r' - 32];
    led_disp_buf[2] = led_table['A' - 32];
    led_disp_buf[3] = led_table['P' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_para(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_PARA_GROUP;
            break;

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

CODE u8 form_para_group_cmd[MAX_FORM_PARA_GROUP_CMD][32] = {
    /* FORM_PARA_GROUP_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_PARA_GROUP_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_PARA_GROUP_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_para_group_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_GROUP_CMD - 2; i++)
    {        
        len = form_para_group_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_para_group_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_GROUP_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_PARA_GROUP_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_PARA_GROUP_FAULT_CMD:
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
                case FORM_PARA_GROUP_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table[g_cp_para.group % 10 + 16];
    led_disp_buf[1] = led_table[g_cp_para.group % 100 / 10 + 16];
    led_disp_buf[2] = 0xff;
    led_disp_buf[3] = 0xff;
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_para_group(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_PARA_GRADE;
            break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_PARA;
            break;

        case KEY_MSG_UP:
            g_cp_para.group++;
            g_cp_para.group %= 100;
            break;

        case KEY_MSG_DOWN:
            if(g_cp_para.group)
            {
                g_cp_para.group--;
            }
            else
            {
                g_cp_para.group = 99;
            }
            break;

        default:
            break;
        }
    }

    form_para_group_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_para_grade_cmd[MAX_FORM_PARA_GRADE_CMD][32] = {
    /* FORM_PARA_GRADE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_PARA_GRADE_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_PARA_GRADE_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
    /* FORM_PARA_GRADE_FUNC_CODE_READ_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x05, 0x00, 0x59, 0x00, 0x03, 0x06, 0x0A, 0xA4, 0x50, 0x08, 0x0E, 0x01},
};

void form_para_grade_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_GRADE_CMD - 2; i++)
    {        
        len = form_para_grade_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_para_grade_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_GRADE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_PARA_GRADE_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_PARA_GRADE_FAULT_CMD:
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
                case FORM_PARA_GRADE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table[g_cp_para.grade % 10 + 16];
    led_disp_buf[1] = led_table[g_cp_para.grade % 100 / 10 + 16];
    led_disp_buf[2] = led_table[g_cp_para.group % 10 + 16];
    led_disp_buf[3] = led_table[g_cp_para.group % 100 / 10 + 16];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

bool func_CODE_read(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    bool ret = FALSE;
    
          
    len = form_para_grade_cmd[FORM_PARA_GRADE_FUNC_CODE_READ_CMD][10] + 11;
                    
    memcpy(UART_TX_BUF, form_para_grade_cmd[FORM_PARA_GRADE_FUNC_CODE_READ_CMD], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
    UART_TX_BUF[15] = g_cp_para.group;
    UART_TX_BUF[16] = g_cp_para.grade;

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
            g_cp_para.vfd_para = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]);

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
            LEDOE = 0;
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[i + 16];
        LEDOE = 0;
    }

    uart_recv_clear();

    return (ret);
}

static int form_para_grade(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            func_CODE_read();
            break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_PARA_GROUP;
            break;

        case KEY_MSG_UP:
            g_cp_para.grade++;
            g_cp_para.grade %= 100;
            break;

        case KEY_MSG_DOWN:
            if(g_cp_para.grade)
            {
                g_cp_para.grade--;
            }
            else
            {
                g_cp_para.grade = 99;
            }
            break;

        default:
            break;
        }
    }

    form_para_grade_callback();

    return (FORM_MSG_NONE);
}

CODE u8 form_para_val_cmd[MAX_FORM_PARA_VAL_CMD][32] = {
    /* FORM_PARA_VAL_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_PARA_VAL_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_PARA_VAL_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
    /* FORM_PARA_VAL_FUNC_CODE_WRITE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x04, 0x08, 0x04, 0xA4, 0x50, 0x82, 0x0A, 0x01, 0x00, 0x06},
};

void form_para_val_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_PARA_VAL_CMD - 2; i++)
    {        
        len = form_para_val_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_para_val_cmd[i], len);

        switch(i)
        {
        case FORM_PARA_VAL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_PARA_VAL_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_PARA_VAL_FAULT_CMD:
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
                case FORM_PARA_VAL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table[g_cp_para.vfd_para % 10 + 16];
    led_disp_buf[1] = (g_cp_para.vfd_para > 9) ? (led_table[g_cp_para.vfd_para % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (g_cp_para.vfd_para > 99) ? (led_table[g_cp_para.vfd_para % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = (g_cp_para.vfd_para > 999) ? (led_table[g_cp_para.vfd_para % 10000 / 1000 + 16]) : (0xff);
    led_disp_buf[4] = (g_cp_para.vfd_para > 9999) ? (led_table[g_cp_para.vfd_para % 100000 / 10000 + 16]) : (0xff);
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

bool func_CODE_write(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    bool ret = FALSE;
    
          
    len = form_para_val_cmd[FORM_PARA_GRADE_FUNC_CODE_READ_CMD][10] + 11;
                    
    memcpy(UART_TX_BUF, form_para_val_cmd[FORM_PARA_GRADE_FUNC_CODE_READ_CMD], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
    UART_TX_BUF[15] = g_cp_para.group;
    UART_TX_BUF[16] = g_cp_para.grade;
    UART_TX_BUF[17] = (u8)(g_cp_para.vfd_para >> 8);
    UART_TX_BUF[18] = (u8)g_cp_para.vfd_para;

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
            form_id = FORM_ID_PARA_GRADE;

            ret = TRUE;
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[i + 16];
        LEDOE = 0;
    }

    uart_recv_clear();

    return (ret);
}

static int form_para_val(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            func_CODE_write();
            break;

        case KEY_MSG_EXIT:
            form_id = FORM_ID_PARA_GRADE;
            break;

        case KEY_MSG_UP:
            g_cp_para.vfd_para++;
            break;

        case KEY_MSG_DOWN:
            if(g_cp_para.vfd_para)
            {
                g_cp_para.vfd_para--;
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
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_COPY_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_COPY_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_copy_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_CMD - 2; i++)
    {        
        len = form_copy_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_COPY_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_COPY_FAULT_CMD:
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
                case FORM_COPY_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table['y' - 32];
    led_disp_buf[1] = led_table['P' - 32];
    led_disp_buf[2] = led_table['o' - 32];
    led_disp_buf[3] = led_table['C' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_copy(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
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
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_COPY_UPLOAD_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_COPY_UPLOAD_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_copy_upload_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_UPLOAD_CMD - 2; i++)
    {        
        len = form_copy_upload_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_upload_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_UPLOAD_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_COPY_UPLOAD_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_COPY_UPLOAD_FAULT_CMD:
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
                case FORM_COPY_UPLOAD_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = 0xff;
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['u' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_copy_upload(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_UPLOAD_RATE;

            return (FORM_MSG_DATA);
            break;

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
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_COPY_DOWNLOAD_ALL_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_COPY_DOWNLOAD_ALL_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_copy_download_all_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_DOWNLOAD_ALL_CMD - 2; i++)
    {        
        len = form_copy_download_all_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_download_all_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_DOWNLOAD_ALL_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_COPY_DOWNLOAD_ALL_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_COPY_DOWNLOAD_ALL_FAULT_CMD:
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
                case FORM_COPY_DOWNLOAD_ALL_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table['A' - 32];
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['d' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_copy_download_all(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            form_id = FORM_ID_COPY_DOWNLOAD_ALL_RATE;

            return (FORM_MSG_DATA);
            break;

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
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* FORM_COPY_DOWNLOAD_PART_ALARM_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x1C, 0xA1, 0x50, 0x02},
    /* FORM_COPY_DOWNLOAD_PART_FAULT_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x0E, 0xA1, 0x50, 0x02},
};

void form_copy_download_part_callback(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < MAX_FORM_COPY_DOWNLOAD_PART_CMD - 2; i++)
    {        
        len = form_copy_download_part_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_download_part_cmd[i], len);

        switch(i)
        {
        case FORM_COPY_DOWNLOAD_PART_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
                {
                    UART_TX_BUF[20] |= 0x08;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x08;
                }
            }
            break;

        case FORM_COPY_DOWNLOAD_PART_ALARM_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case FORM_COPY_DOWNLOAD_PART_FAULT_CMD:
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
                case FORM_COPY_DOWNLOAD_PART_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
    
    led_disp_buf[0] = led_table['P' - 32];
    led_disp_buf[1] = 0xff;
    led_disp_buf[2] = led_table['L' - 32];
    led_disp_buf[3] = led_table['d' - 32];
    led_disp_buf[4] = 0xff;
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    LEDOE = 0;
}

static int form_copy_download_part(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            break;

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
    u8 len, timeout, ret = FALSE;
    unsigned int crc;


    len = copy_upload_rate_baudrate_cmd[COPY_UPLOAD_RATE_BAUDRATE][10] + 11;
                    
    memcpy(UART_TX_BUF, copy_upload_rate_baudrate_cmd[COPY_UPLOAD_RATE_BAUDRATE], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
    
    UART_TX_BUF[19] = (u8)(baudrate >> 8);
    UART_TX_BUF[20] = (u8)baudrate;

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
            switch(baudrate)
            {
            case COPY_BAUDRATE:
                UartInit_19200bps();
                break;

            case OTHER_BAUDRATE:
                UartInit_9600bps();
                break;

            default:
                UartInit_9600bps();
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
            LEDOE = 0;
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[COPY_UPLOAD_RATE_BAUDRATE + 16];
        LEDOE = 0;
    }

    uart_recv_clear();

    return (ret);
}

CODE u8 form_copy_upload_rate_init_cmd[][32] = {
    /* COPY_UPLOAD_RATE_INIT */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x11, 0x00, 0x59, 0x00, 0x02, 0x04, 0x10, 0xA4, 0x50, 0x0D},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x05, 0x0A, 0x08, 0xAA, 0x50, 0x02, 0x00, 0x03, 0x01, 0x03, 0xFD, 0x82},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
};

void copy_upload_rate_init(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < 5; i++)
    {        
        len = form_copy_upload_rate_init_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_upload_rate_init_cmd[i], len);

        switch(i)
        {
        case 2:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
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
                case 2:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
}

CODE u8 copy_comm_reset_cmd[][32] = {
    /* FORM_ID_COPY_UPLOAD_RATE */
    /* FORM_ID_COPY_DOWNLOAD_ALL_RATE */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},

    /* FORM_ID_COPY_DOWNLOAD_ALL_RATE */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x1D, 0xA1, 0x50, 0x02, 0x00, 0x01},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x05, 0x0A, 0x1D, 0xA1, 0x50, 0x8C, 0x00, 0x03, 0x0F, 0xAA, 0x00, 0x00},
};

void copy_comm_reset(void)
{
    u8 i, len, timeout, num;
    unsigned int crc;


    if(FORM_ID_COPY_DOWNLOAD_ALL_RATE == form_id)
    {
        num = 6;
    }
    else
    {
        num = 3;
    }
    
    for(i = 0; i < num; i++)
    {        
        len = copy_comm_reset_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, copy_comm_reset_cmd[i], len);

        switch(i)
        {
        case 0:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

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
                case 0:
                case 3:
                    if((0x44 == UART_RX_BUF[3]) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);

                        if(FORM_ID_COPY_UPLOAD_RATE == form_id)
                        {
                            i = num; //跳过复位
                        }
                    }
                    break;

                default:
                    break;
                }             
            }
            else
            {
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
}

CODE u8 copy_upload_rate_cmd[][32] = {
    /* COPY_UPLOAD_RATE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* COPY_UPLOAD_RATE_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x14, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x89, 0x00, 0x08},
    /* COPY_UPLOAD_RATE_TAIL_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x01, 0xA2, 0x50, 0x82, 0x00, 0x04},
};

void form_copy_upload_rate_callback(void)
{
    u8 i, j, len, timeout, size;
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
        
        len = copy_upload_rate_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, copy_upload_rate_cmd[i], len);

        switch(i)
        {
        case COPY_UPLOAD_RATE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case COPY_UPLOAD_RATE_TAIL_CMD:
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
                case COPY_UPLOAD_RATE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
                    }
                    break;

                case COPY_UPLOAD_RATE_CMD:
                    num = ((u16)UART_RX_BUF[9] << 8) | ((u16)UART_RX_BUF[10]); //计数
                    size = UART_RX_BUF[2] - 8; //帧数据长度
                    
                    if((0x41 == UART_RX_BUF[3]) && (0x82 == UART_RX_BUF[4])) //头帧
                    {                        
                        g_cp_para.vfd_para_total = num;

                        IIC_WriteByte(VFD_PARA_FLAG_ADDR, ~VFD_PARA_FLAG); //禁能变频器参数标志

                        os_wait(K_TMO, 12, 0); //5ms
                        
                        IIC_WriteHalfWord(VFD_PARA_LEN_ADDR, g_cp_para.vfd_para_total + 2); //存储变频器参数长度
                        
                        os_wait(K_TMO, 12, 0); //5ms

                        g_cp_para.vfd_para_crc = IIC_ReadByte(VFD_PARA_LEN_ADDR); //变频器参数长度和变频器参数一起校验

                        os_wait(K_TMO, 12, 0); //5ms
                        
                        g_cp_para.vfd_para_crc += IIC_ReadByte(VFD_PARA_LEN_ADDR + 1);

                        os_wait(K_TMO, 12, 0); //5ms

                        for(j = 0; j < (size + 2); j++)
                        {
                            IIC_WriteByte(VFD_PARA_ADDR + g_cp_para.vfd_para_index, UART_RX_BUF[9 + j]);

                            g_cp_para.vfd_para_index++;

                            g_cp_para.vfd_para_crc += UART_RX_BUF[9 + j];

                            os_wait(K_TMO, 12, 0); //5ms
                        }
                    }
                    else if(0 != num) //避开无效帧
                    {
                        g_cp_para.vfd_para_count = num;

                        for(j = 0; j < size; j++)
                        {
                            IIC_WriteByte(VFD_PARA_ADDR + g_cp_para.vfd_para_index, UART_RX_BUF[11 + j]);

                            g_cp_para.vfd_para_index++;

                            g_cp_para.vfd_para_crc += UART_RX_BUF[11 + j];
                                                
                            os_wait(K_TMO, 12, 0); //5ms
                        }
                    }

                    if((0x41 == UART_RX_BUF[3]) && (0x22 == UART_RX_BUF[4])) //倒数第二帧
                    {
                        g_cp_para.vfd_para_count += size; //自身长度
                        
                        last_frame = TRUE;

                        IIC_WriteByte(VFD_PARA_ADDR + (g_cp_para.vfd_para_total + 2), g_cp_para.vfd_para_crc);

                        os_wait(K_TMO, 12, 0); //5ms
                        
                        IIC_WriteByte(VFD_PARA_FLAG_ADDR, VFD_PARA_FLAG); //使能变频器参数标志
                        
                        os_wait(K_TMO, 12, 0); //5ms
                    }

                    g_cp_para.rate = (u8)((fp32)g_cp_para.vfd_para_count / (fp32)g_cp_para.vfd_para_total * 100);
                    break;

                case COPY_UPLOAD_RATE_TAIL_CMD:
                    if((0x41 == UART_RX_BUF[3]) && (0xA2 == UART_RX_BUF[4])) //尾帧
                    {
                        last_frame = FALSE;
                        
                        g_cp_para.vfd_para_count = 0;
                        g_cp_para.vfd_para_total = 0;
                        g_cp_para.rate = 0;
                        
                        g_cp_para.vfd_para_index = 0;
                        g_cp_para.vfd_para_crc = 0;
                        
                        if(TRUE == chang_baudrate(OTHER_BAUDRATE))
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }

    led_disp_buf[0] = led_table[g_cp_para.rate % 10 + 16];
    led_disp_buf[1] = (g_cp_para.rate > 9) ? (led_table[g_cp_para.rate % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (g_cp_para.rate > 99) ? (led_table[g_cp_para.rate % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = led_table['L' - 32];
    led_disp_buf[4] = led_table['u' - 32];
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= ~LED_TORQUE_MASK;
    LEDOE = 0;
}

static int form_copy_upload_rate(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
        if(TRUE == chang_baudrate(COPY_BAUDRATE))
        {
            copy_upload_rate_init();
        }
        else
        {
            form_id = FORM_ID_COPY_UPLOAD;

            return (FORM_MSG_NONE);
        }
    }
    
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            break;

        case KEY_MSG_EXIT:
            if(TRUE == chang_baudrate(OTHER_BAUDRATE))
            {
                copy_comm_reset();
                
                form_id = FORM_ID_COPY_UPLOAD;
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
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x0C, 0x00, 0x59, 0x00, 0x02, 0x04, 0x12, 0xA1, 0x50, 0x15},
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x03, 0x06, 0x06, 0xA2, 0x50, 0x82, 0x10, 0x01},
};

void copy_download_all_rate_init(void)
{
    u8 i, len, timeout;
    unsigned int crc;
    
    
    for(i = 0; i < 4; i++)
    {        
        len = form_copy_download_all_rate_init_cmd[i][10] + 11;
                        
        memcpy(UART_TX_BUF, form_copy_download_all_rate_init_cmd[i], len);

        switch(i)
        {
        case 1:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
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
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }
}

CODE u8 check_vfd_para_cmd[][32] = {
    /* CHECK_VFD_PARA_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
};

void check_vfd_para_callback(void)
{
    u8 len, timeout;
    unsigned int crc;

    
    len = check_vfd_para_cmd[CHECK_VFD_PARA_SET_CMD][10] + 11;
                    
    memcpy(UART_TX_BUF, check_vfd_para_cmd[CHECK_VFD_PARA_SET_CMD], len);

    UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

    if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
        
        if(TRUE == g_cp_para.run)
        {
            g_cp_para.run = FALSE;
            
            UART_TX_BUF[20] |= 0x02;
        }

        if(VFD_REV == g_cp_para.fr)
        {
            UART_TX_BUF[20] |= 0x04;
        }
        else
        {
            UART_TX_BUF[20] &= ~0x04;
        }
        
        if(VFD_LOC == g_cp_para.lr)
        {
            UART_TX_BUF[20] |= 0x08;
        }
        else
        {
            UART_TX_BUF[20] &= ~0x08;
        }
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
            if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
            {
                g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                g_cp_para.count++;

                if(UART_RX_BUF[11] & 0x80)
                {
                    g_cp_para.reset = TRUE;
                }

                g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
            }           
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[CHECK_VFD_PARA_SET_CMD + 16];
            LEDOE = 0;
        }
    }
    else
    {
        led_disp_buf[4] = 0xff;
        led_disp_buf[3] = led_table['E' - 32];
        led_disp_buf[2] = led_table['r' - 32];
        led_disp_buf[1] = led_table['r' - 32];
        led_disp_buf[0] = led_table[CHECK_VFD_PARA_SET_CMD + 16];
        LEDOE = 0;
    }

    uart_recv_clear();
}

bool check_vfd_para(void)
{
    u8 crc;
    u16 i, len;
    bool ret = FALSE;


    if(VFD_PARA_FLAG == IIC_ReadByte(VFD_PARA_FLAG_ADDR))
    {
        os_wait(K_TMO, 12, 0); //5ms
        
        len = IIC_ReadHalfWord(VFD_PARA_LEN_ADDR);

        os_wait(K_TMO, 12, 0); //5ms

        if((len + 3) < AT24CXX)
        {
            g_cp_para.vfd_para_crc = IIC_ReadByte(VFD_PARA_ADDR + len);

            os_wait(K_TMO, 12, 0); //5ms
            
            for(i = 0, crc = 0; i < (len + 2); i++)
            {
                if(0 == (i % 20))
                {
                    check_vfd_para_callback();
                }
                
                crc += IIC_ReadByte(VFD_PARA_LEN_ADDR + i);

                os_wait(K_TMO, 12, 0); //5ms
            }

            if(g_cp_para.vfd_para_crc == crc)
            {
                ret = TRUE;
            }
        }
    }

    return (ret);
}

CODE u8 copy_download_all_rate_cmd[][60] = {
    /* COPY_DOWNLOAD_ALL_RATE_SET_CMD */
	{0xF7, 0x17, 0x00, 0x59, 0x00, 0x0B, 0x00, 0x59, 0x00, 0x09, 0x12, 0x04, 0xA1, 0x50, 0x88, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x00},
    /* COPY_DOWNLOAD_ALL_RATE_CMD */
    /* 0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34    35    36    37    38    39    40    41    42    43    44    45    46    47    48    49    50    51    52    53    54    55    56    57    58    59 */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x15, 0x2A, 0x01, 0x42, 0x50, 0x89, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3E, 0x06, 0x22, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x01, 0x26, 0x01, 0x6E, 0x00, 0x00, 0x00, 0x00},
    /* COPY_DOWNLOAD_ALL_RATE_TAIL_CMD */
    {0xF7, 0x17, 0x00, 0x59, 0x00, 0x03, 0x00, 0x59, 0x00, 0x02, 0x04, 0x02, 0xA2, 0x50, 0x02},
};

void form_copy_download_all_rate_callback(void)
{
    u8 i, j, len, timeout;
    unsigned int crc;
    static u8 frame_num = 0;
    
    
    for(i = COPY_DOWNLOAD_ALL_RATE_SET_CMD; i < COPY_DOWNLOAD_ALL_RATE_TAIL_CMD; i++)
    {            
        if(COPY_DOWNLOAD_ALL_RATE_SET_CMD == i)
        {
            len = copy_download_all_rate_cmd[i][10] + 11;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if((0xff == frame_num) && (COPY_DOWNLOAD_ALL_RATE_CMD == i))
        {
            i = COPY_DOWNLOAD_ALL_RATE_TAIL_CMD;

            len = copy_download_all_rate_cmd[i][10] + 11;
                            
            memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
        }
        else if(COPY_DOWNLOAD_ALL_RATE_CMD == i)
        {
            if(0 == frame_num) //头帧
            {
                frame_num = 1;
                
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60);
            
                UART_TX_BUF[9]  = 0x16;
                UART_TX_BUF[10] = 0x2C;
                UART_TX_BUF[11] = 0x08;
                UART_TX_BUF[12] = 0x8A;
                UART_TX_BUF[14] = 0x86;
                UART_TX_BUF[16] = 0x03;
                UART_TX_BUF[17] = 0x01;
                UART_TX_BUF[18] = 0x03;

                len = UART_TX_BUF[10] + 11;
            
                g_cp_para.vfd_para_index = 0;
                g_cp_para.vfd_para_count = 0;
                g_cp_para.vfd_para_total = IIC_ReadHalfWord(VFD_PARA_LEN_ADDR) - 2;
            
                os_wait(K_TMO, 12, 0); //5ms
            }
            else if(1 == frame_num) //第二帧
            {
                frame_num = 2;
            
                memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60);
            
                UART_TX_BUF[9]  = 0x14;
                UART_TX_BUF[10] = 0x28;

                len = UART_TX_BUF[10] + 11;
            }
            else
            {
                if((g_cp_para.vfd_para_total - g_cp_para.vfd_para_count + 10) < 0x2A)
                {
                    frame_num = 0xfe; //倒数第二帧
                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], 60);
                    
                    UART_TX_BUF[9]  = 0x06;
                    UART_TX_BUF[10] = g_cp_para.vfd_para_total - g_cp_para.vfd_para_count + 10;
                    UART_TX_BUF[12] = 0x22;
                    
                    len = UART_TX_BUF[10] + 11;
                }
                else
                {
                    frame_num = 3; //第二帧以后的帧
                    
                    len = copy_download_all_rate_cmd[i][10] + 11;
                                    
                    memcpy(UART_TX_BUF, copy_download_all_rate_cmd[i], len);
                }
            }
            
            for(j = 0; j < (UART_TX_BUF[10] - 10); j++)
            {
                UART_TX_BUF[21 + j] = IIC_ReadByte(VFD_PARA_ADDR + g_cp_para.vfd_para_index + j);
        
                os_wait(K_TMO, 12, 0); //5ms
            }
        
            g_cp_para.vfd_para_index += UART_TX_BUF[10] - 10;
        
            if(1 == frame_num) //头帧
            {
                UART_TX_BUF[19] = (u8)(g_cp_para.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(g_cp_para.vfd_para_count);

                g_cp_para.vfd_para_count += UART_TX_BUF[10] - 12;
            }
            else
            {
                UART_TX_BUF[19] = (u8)(g_cp_para.vfd_para_count >> 8);
                UART_TX_BUF[20] = (u8)(g_cp_para.vfd_para_count);
                
                g_cp_para.vfd_para_count += UART_TX_BUF[10] - 10;
            }

            g_cp_para.rate = (u8)((fp32)g_cp_para.vfd_para_count / (fp32)g_cp_para.vfd_para_total * 100);
        }

        switch(i)
        {
        case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);

            if((0x04 == (UART_TX_BUF[11] & 0x0f)) && ((0xa1 == (UART_TX_BUF[12] & 0xff))))
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
                
                if(TRUE == g_cp_para.run)
                {
                    g_cp_para.run = FALSE;
                    
                    UART_TX_BUF[20] |= 0x02;
                }

                if(VFD_REV == g_cp_para.fr)
                {
                    UART_TX_BUF[20] |= 0x04;
                }
                else
                {
                    UART_TX_BUF[20] &= ~0x04;
                }
                
                if(VFD_LOC == g_cp_para.lr)
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
            UART_TX_BUF[13] = (UART_TX_BUF[13] & 0xf0) | (g_cp_para.cmd & 0x0f);
            break;

        case COPY_DOWNLOAD_ALL_RATE_TAIL_CMD:
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
                case COPY_DOWNLOAD_ALL_RATE_SET_CMD:
                    if((0x04 == (UART_RX_BUF[3] & 0x0f)) && (0xa1 == UART_RX_BUF[4]))
                    {
                        g_cp_para.count = ((u16)UART_RX_BUF[7] << 8) | ((u16)UART_RX_BUF[8]);
                        g_cp_para.count++;

                        if(UART_RX_BUF[11] & 0x80)
                        {
                            g_cp_para.reset = TRUE;
                        }

                        g_cp_para.ref = ((u16)UART_RX_BUF[15] << 8) | ((u16)UART_RX_BUF[16]);
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
                        
                        g_cp_para.vfd_para_count = 0;
                        g_cp_para.vfd_para_total = 0;
                        g_cp_para.rate = 0;
                        
                        g_cp_para.vfd_para_index = 0;
                        g_cp_para.vfd_para_crc = 0;
                        
                        if(TRUE == chang_baudrate(OTHER_BAUDRATE))
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
                led_disp_buf[4] = 0xff;
                led_disp_buf[3] = led_table['E' - 32];
                led_disp_buf[2] = led_table['r' - 32];
                led_disp_buf[1] = led_table['r' - 32];
                led_disp_buf[0] = led_table[i + 16];
                LEDOE = 0;
            }
        }
        else
        {
            led_disp_buf[4] = 0xff;
            led_disp_buf[3] = led_table['E' - 32];
            led_disp_buf[2] = led_table['r' - 32];
            led_disp_buf[1] = led_table['r' - 32];
            led_disp_buf[0] = led_table[i + 16];
            LEDOE = 0;
        }

        uart_recv_clear();
    }

    led_disp_buf[0] = led_table[g_cp_para.rate % 10 + 16];
    led_disp_buf[1] = (g_cp_para.rate > 9) ? (led_table[g_cp_para.rate % 100 / 10 + 16]) : (0xff);
    led_disp_buf[2] = (g_cp_para.rate > 99) ? (led_table[g_cp_para.rate % 1000 / 100 + 16]) : (0xff);
    led_disp_buf[3] = led_table['L' - 32];
    led_disp_buf[4] = led_table['d' - 32];
    led_disp_buf[5] |= LED_V_A_Hz_MASK;
    led_disp_buf[5] &= ~LED_TORQUE_MASK;
    LEDOE = 0;
}

static int form_copy_download_all_rate(unsigned int key_msg, unsigned int form_msg)
{
    if(FORM_MSG_DATA == form_msg)
    {
#if 1   
        if((TRUE == check_vfd_para()) &&            //校验存储的变频器参数
           (TRUE == chang_baudrate(COPY_BAUDRATE))) //更改变频器参数上传、下载的波特率
        {
            copy_download_all_rate_init();
        }
        else
        {
            form_id = FORM_ID_COPY_DOWNLOAD_ALL;

            return (FORM_MSG_NONE);
        }
#else //调试变频器参数下载
        UartInit_19200bps();
#endif        
    }
    
    if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {        
        case KEY_MSG_RUN:
            g_cp_para.run = TRUE;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_RUN_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_STOP:
            g_cp_para.stop = TRUE;

            led_disp_buf[5] |= LED_RUN_MASK;
            LEDOE = 0;
            break;

        case KEY_MSG_LOC_REM:
            /* 逻辑非(!x)的结果有2种: TRUE(1), FALSE(0)
             * 逻辑非(!x)的等价式: !x = (0 == x)
             * 华兄 */
            g_cp_para.lr = !g_cp_para.lr;

            if(VFD_LOC == g_cp_para.lr)
            {
                led_disp_buf[5] &= ~LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_LOC_REM_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_FWD_REV:
            g_cp_para.fr = !g_cp_para.fr;

            if(VFD_REV == g_cp_para.fr)
            {
                led_disp_buf[5] &= ~LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            else
            {
                led_disp_buf[5] |= LED_FWD_REV_MASK;
                LEDOE = 0;
            }
            break;

        case KEY_MSG_ENTER:
            break;

        case KEY_MSG_EXIT:
            if(TRUE == chang_baudrate(OTHER_BAUDRATE))
            { 
                copy_comm_reset();
                
                form_id = FORM_ID_COPY_DOWNLOAD_ALL;
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

void CPTask(void) _task_ CP_TASK
{
    s8 event;
    unsigned int key_msg;
    unsigned int form_msg;


    os_wait(K_TMO, 250, 0);
    os_wait(K_TMO, 250, 0);
    os_wait(K_TMO, 250, 0);
    os_wait(K_TMO, 250, 0);
    
    os_send_signal(KEY_TASK);
    
    MENU_Init();

#if (IIC_TEST_EN > 0u)
    IIC_Test();
#endif

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

        if(NULL != form_list[form_id])
        {
            if(FORM_MSG_NONE != (form_msg = (*form_list[form_id])(key_msg, FORM_MSG_KEY)))
            {
                (*form_list[form_id])(key_msg, form_msg);
            }
        }
    }
}

