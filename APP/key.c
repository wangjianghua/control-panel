#include "includes.h"


OS_SEM key_sem;

XDATA u8 key_value = KEY_VAL_NONE;
XDATA u8 key_fsm = 0;  

BIT CheckKeyPress(void)
{
    if((0 == EnterKey) || 
       (0 == RunKey) || 
       (0 == StopKey) || 
       (0 == FwdRevKey) || 
       (0 == DownKey) || 
       (0 == LocRemKey) || 
       (0 == ExitKey) || 
       (0 == UpKey))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

u8 KeyScan(void)
{
    u8 temp = 0;

    
    if(0 == EnterKey)
        temp = 1;
    
    if(0 == RunKey)  
        temp = 2;
    
    if(0 == StopKey) 
        temp = 3;
    
    if(0 == FwdRevKey)
        temp = 4;
    
    if(0 == DownKey) 
        temp = 5;
    
    if(0 == LocRemKey)  
        temp = 6;
    
    if(0 == ExitKey)  
        temp = 7;
    
    if(0 == UpKey)   
        temp = 8;

    if(0 == FuncCodeKey)	 
        temp = 9;

    return (temp);
}

BIT ReadKeyPress(u8 key)
{
    BIT temp = 1;

    
    switch(key)
    {
    	case 1: 
            temp = EnterKey; 
            return (!temp);
            //break;
            
    	case 2: 
            temp = RunKey;   
            return (!temp);
            //break;
            
    	case 3:	
            temp = StopKey;  
            return (!temp);
            //break;
            
    	case 4:	
            temp = FwdRevKey; 
            return (!temp);
            //break;
            
    	case 5:	
            temp = DownKey;  
            return (!temp);
            //break;
            
    	case 6:	
            temp = LocRemKey;   
            return (!temp);
            //break;   
            
    	case 7:	
            temp = ExitKey;   
            return (!temp);
            //break;
            
    	case 8:	
            temp = UpKey;    
            return (!temp);
            //break;    
            
    	case 9: 
            temp = FuncCodeKey;         
            return (!temp);
            //break;
            
    	default: 
            return (0);
            //break;                     
    }
}

u8 ReadKeyVal(void)
{
    u8 key_val = 0;
    static u8 key = 0;
    static u8 count = 0;


    key_val = KEY_VAL_NONE;

    switch(key_fsm)
    {
    case 0:
        if(CheckKeyPress())
        {
            key = KeyScan();
            
            if(key)
            {
                key_fsm = 1;
            }
        }
        else
        {
            os_dly_wait(5);
        }
        break;
        
    case 1:
        os_dly_wait(10);  
        
        if(CheckKeyPress())      
        {
            if(ReadKeyPress(key))
            {
                key_val = key;
                key_fsm = 2;
                count = 0;
            }
            else
            {
                key_fsm = 0;
            }
        }
        else
        {
            key_fsm = 0;
        }
        break;
        
    case 2:
        if(count > 66)
        {
            count = 0;
            key_fsm = 3;
            key_val = key + 9;
            
            break;
        }
        
        os_dly_wait(10);

        if(!ReadKeyPress(key))
        {
            os_dly_wait(10);
            
            if(!ReadKeyPress(key))
            {
                key_val = key + 45;
                key_fsm = 0;
            }
            else
            {
                count++;
            }
        }
        else
        {
            count++;
        }
        break;
        
    case 3:
        if(count > 20)
        {
            count = 0;
            key_fsm = 4;
            key_val = key + 18;
            
            break;
        }
        
        os_dly_wait(10);
        
        if(!ReadKeyPress(key))
        {
            os_dly_wait(10);
            
            if(!ReadKeyPress(key))
            {
                key_val = key + 45;
                key_fsm = 0;
            }
            else
            {
                count++;
            }
        }
        else
        {
            count++;
        }
        break;
        
    case 4:
        if(count > 20)
        {
            count = 0;
            key_fsm = 5;
            key_val = key + 27;
            
            break;
        }
        
        os_dly_wait(10);
        
        if(!ReadKeyPress(key))
        {
            os_dly_wait(10);
            
            if(!ReadKeyPress(key))
            {
                key_val = key + 45;
                key_fsm = 0;
            }
            else
            {
                count++;
            }
        }
        else
        {
            count++;
        }
        break;
        
    case 5:
        if(count > 20)
        {
            count = 0;
            key_val = key + 36;
            
            break;
        }
        
        os_dly_wait(10);
        
        if(!ReadKeyPress(key))
        {
            os_dly_wait(10);
            
            if(!ReadKeyPress(key))
            {
                key_val = key + 45;
                key_fsm = 0;
            }
            else
            {
                count++;
            }
        }
        else
        {
            count++;
        }
        break;

    default:
        key_fsm = 0;
        break; 
   }
    
   return (key_val);
} 

__task void AppTaskKey(void)
{
    OS_RESULT result;

    
    key_value = KEY_VAL_NONE;
    
    os_sem_wait(&key_sem, 0xffff);

    while(1)
    {                        
        key_value = ReadKeyVal();	
        
        if(KEY_VAL_NONE != key_value)	
        {            
            os_sem_send(&cp_sem);
            
            result = os_sem_wait(&key_sem, 0xffff);

            switch(result)
            {
            case OS_R_SEM:    
            case OS_R_OK:
                key_value = KEY_VAL_NONE;
                
                os_dly_wait(20);
                break;
                
            default:    
                break;
            }
        }
    }
}

