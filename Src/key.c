#include "includes.h"


XDATA u8 key_value = KEY_VAL_NONE;
XDATA u8 key_fsm = 0;  

BIT CheckKeyPress(void)
{
    if((0 == ShiftKey) || 
       (0 == RunKey) || 
       (0 == StopKey) || 
       (0 == EnterKey) || 
       (0 == DownKey) || 
       (0 == JogKey) || 
       (0 == PrgKey) || 
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

    
    if(0 == ShiftKey)
        temp = 1;
    
    if(0 == RunKey)  
        temp = 2;
    
    if(0 == StopKey) 
        temp = 3;
    
    if(0 == EnterKey)
        temp = 4;
    
    if(0 == DownKey) 
        temp = 5;
    
    if(0 == JogKey)  
        temp = 6;
    
    if(0 == PrgKey)  
        temp = 7;
    
    if(0 == UpKey)   
        temp = 8;

    if(runstatus)
    {
        if((0 == RunKey) && (0 == StopKey))	 
            temp = 9;
    }

    return (temp);
}

BIT ReadKeyPress(u8 key)
{
    BIT temp = 1;

    
    switch(key)
    {
    	case 1: 
            temp = ShiftKey; 
            return (!temp);
            break;
            
    	case 2: 
            temp = RunKey;   
            return (!temp);
            break;
            
    	case 3:	
            temp = StopKey;  
            return (!temp);
            break;
            
    	case 4:	
            temp = EnterKey; 
            return (!temp);
            break;
            
    	case 5:	
            temp = DownKey;  
            return (!temp);
            break;
            
    	case 6:	
            temp = JogKey;   
            return (!temp);
            break;   
            
    	case 7:	
            temp = PrgKey;   
            return (!temp);
            break;
            
    	case 8:	
            temp = UpKey;    
            return (!temp);
            break;    
            
    	case 9: 
            if((0 == RunKey) && 
               (0 == StopKey) && 
               (runstatus)) 
                temp = 0;   
            
            return (!temp);
            break;
            
    	default: 
            return (0);
            break;                     
    }
}

u8 ReadKeyVal(void)
{
    u8 key_val = 0;
    static u8 key = 0;
    static u8 count = 0;
    static u8 free_run_status = 0;


    key_val = KEY_VAL_NONE;

    if(0 == free_run_status)
    {
        if(CheckKeyPress())
        {
            if(FreeRunKey == KeyScan())
            {
                os_wait(K_TMO, 20, 0);
                
                if(FreeRunKey == KeyScan())
                {
                    key = FreeRunKey;
                    key_fsm = 2;
                    free_run_status = 1;
                    
                    return (FreeRunKey);
                }
            }
        }
    }

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
        break;
        
    case 1:
        os_wait(K_TMO, 20, 0);  
        
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
        if(FreeRunKey == key)
        {
            if(!CheckKeyPress())
            {
                os_wait(K_TMO, 20, 0);
                
                if(!CheckKeyPress())
                {
                    key_val = key + 45;
                    free_run_status = 0;
                    key_fsm = 0;
                }
            }
            else
            {
                key_val = FreeRunKey;
            }
            
            break;
        }

        if(count > 66)
        {
            count = 0;
            key_fsm = 3;
            key_val = key + 9;
            
            break;
        }
        
        os_wait(K_TMO, 20, 0);

        if(!ReadKeyPress(key))
        {
            os_wait(K_TMO, 20, 0);
            
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
        
        os_wait(K_TMO, 20, 0);
        
        if(!ReadKeyPress(key))
        {
            os_wait(K_TMO, 20, 0);
            
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
        
        os_wait(K_TMO, 20, 0);
        
        if(!ReadKeyPress(key))
        {
            os_wait(K_TMO, 20, 0);
            
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
        
        os_wait(K_TMO, 20, 0);
        
        if(!ReadKeyPress(key))
        {
            os_wait(K_TMO, 20, 0);
            
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

void KeyTask(void) _task_ KEY_TASK 
{
    s8 event;
    s8 jump = 0;

    
    key_value = KEY_VAL_NONE;
    
    os_wait(K_SIG, 0, 0); 

    while(1)
    {                        
        key_value = ReadKeyVal();	
        
        if(KEY_VAL_NONE != key_value)	
        {
            os_send_signal(CP_TASK);
            
            jump = 1; 
            
            while(jump)
            {
                event = os_wait(K_SIG, 0, 0);

                switch(event)
                {
                case SIG_EVENT:
                    key_value = KEY_VAL_NONE;
                    os_wait(K_TMO, 50, 0);  
                    jump = 0;
                    break;
                    
                default:    
                    break;
                }
            }
        }
    }
}

