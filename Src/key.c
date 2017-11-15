#include "includes.h"

bit IsKeyPress(void)
{
	if(ShiftKey==0 || RunKey==0 || StopKey==0 || EnterKey==0 || DownKey==0 || JogKey==0 || PrgKey==0 || UpKey==0)
	{
	  return 1;
	}
	else
	{
	  return 0;
    }
}

u8 KeyScan(void)
{
  u8 temp=0;
  if(ShiftKey==0)temp=1;
  if(RunKey==0)  temp=2;
  if(StopKey==0) temp=3;
  if(EnterKey==0)temp=4;
  if(DownKey==0) temp=5;
  if(JogKey==0)  temp=6;
  if(PrgKey==0)  temp=7;
  if(UpKey==0)   temp=8;

  if(runstatus)
  {
    if(	(RunKey==0)&&(StopKey==0) )	 temp=9;
  }

  return temp;
}



bit IsSomeKeyPress(u8 tKeyV)
{
	bit temp=1;
	switch(tKeyV)
	{
		case 1: temp = ShiftKey; return !temp;break;
		case 2: temp = RunKey;   return !temp;break;
		case 3:	temp = StopKey;  return !temp;break;
		case 4:	temp = EnterKey; return !temp;break;
		case 5:	temp = DownKey;  return !temp;break;
		case 6:	temp = JogKey;   return !temp;break;	
		case 7:	temp = PrgKey;   return !temp;break;
		case 8:	temp = UpKey;    return !temp;break;	
		case 9: if(	(RunKey==0)&&(StopKey==0)&&(runstatus) ) temp=0;	 return !temp;break;
		default: return 0;						
	}
}
u8 fsm_key=0;  
u8 ReadKeyV(void)
{
   u8 tKeyV=0;
   static u8 tKeyV1=0;

   static u8 cnt=0;

   static u8 freerunstatus=0;

   tKeyV = NoKeyV;

   if(freerunstatus==0)
   {
     if(IsKeyPress())
     {
	   if(FreeRunKey==KeyScan())
       {
	     os_wait(K_TMO,20,0);
	     if(FreeRunKey==KeyScan())
	     {
		   tKeyV1=FreeRunKey;
	       fsm_key=2;
		   freerunstatus=1;
		   return  FreeRunKey;
	     }
	   }
     }
   }

   switch(fsm_key)
   {
	 case 0:
	        if(IsKeyPress())
			{
	          tKeyV1=KeyScan();
	          if(tKeyV1)
			  {
			    fsm_key=1;
			  }
			}
			break;
	 case 1:
	        os_wait(K_TMO,20,0);  
			if(IsKeyPress())      
			{
			  if(IsSomeKeyPress(tKeyV1))
			  {
			    tKeyV = tKeyV1;
			    fsm_key=2;
			    cnt=0;
			  }
			  else
			  {
			    fsm_key=0;
			  }
			}
			else
			{
			  fsm_key=0;
			}
	        break;
	  case 2:
			if(tKeyV1==FreeRunKey)
			{
   		      if(!IsKeyPress())
			  {
			    os_wait(K_TMO,20,0);
			    if(!IsKeyPress())
			    {
			      tKeyV=tKeyV1+45;
				  freerunstatus=0;
			      fsm_key=0;
			    }
			  }
			  else
			  {
			    tKeyV=FreeRunKey;
			  }
			  break;
			}
	  //////////////////////////////////////////
	        if(cnt>66)
			{
			  cnt=0;
			  fsm_key=3;
			  tKeyV=tKeyV1+9;
			  break;
 			}
	        os_wait(K_TMO,20,0);

   		    if(!IsSomeKeyPress(tKeyV1))
			{
			  os_wait(K_TMO,20,0);
			  if(!IsSomeKeyPress(tKeyV1))
			  {
			    tKeyV=tKeyV1+45;
			    fsm_key=0;
			  }
			  else
			  {
			    cnt++;
			  }
			}
			else
			{
			  cnt ++;
			}
	        break;
	  case 3:
	        if(cnt>20)
			{
			  cnt=0;
			  fsm_key=4;
			  tKeyV=tKeyV1+18;
			  break;
 			}
	        os_wait(K_TMO,20,0);
   		    if(!IsSomeKeyPress(tKeyV1))
			{
			  os_wait(K_TMO,20,0);
			  if(!IsSomeKeyPress(tKeyV1))
			  {
			    tKeyV=tKeyV1+45;
			    fsm_key=0;
			  }
			  else
			  {
			    cnt++;
			  }
			}
			else
			{
			  cnt ++;
			}
	        break;
	  case 4:
	        if(cnt>20)
			{
			  cnt=0;
			  fsm_key=5;
			  tKeyV=tKeyV1+27;
			  break;
 			}
	        os_wait(K_TMO,20,0);
   		    if(!IsSomeKeyPress(tKeyV1))
			{
			  os_wait(K_TMO,20,0);
			  if(!IsSomeKeyPress(tKeyV1))
			  {
			    tKeyV=tKeyV1+45;
			    fsm_key=0;
			  }
			  else
			  {
			    cnt++;
			  }
			}
			else
			{
			  cnt ++;
			}
	        break;
	  case 5:
	        if(cnt>20)
			{
			  cnt=0;
			  tKeyV=tKeyV1+36;
			  break;
 			}
	        os_wait(K_TMO,20,0);
   		    if(!IsSomeKeyPress(tKeyV1))
			{
			  os_wait(K_TMO,20,0);
			  if(!IsSomeKeyPress(tKeyV1))
			  {
			    tKeyV=tKeyV1+45;
			    fsm_key=0;
			  }
			  else
			  {
			    cnt++;
			  }
			}
			else
			{
			  cnt++;
			}
	        break;

	  default:
	        fsm_key=0;
	        break; 
	         


   }
   return tKeyV;
} 




u8 KeyV1 = NoKeyV;
void KeyTask(void) _task_ KEY_TASK 
{

 char event;
 char jump=0;
 char status=0;
 KeyV1 = NoKeyV;
 os_wait (K_SIG , 0, 0); 
 ///////////////////////////////////////////////////////////////////
	while (1)
	{                        
		KeyV1 = ReadKeyV();	
 		if(KeyV1 != NoKeyV)	
		{
		  os_send_signal (CP_TASK);
		  jump=1; status=0;
		  while(jump)
		  {
	         event = os_wait (K_SIG , 0, 0);	
	         switch (event)
             {
                case SIG_EVENT:
				             KeyV1 = NoKeyV;
                             os_wait(K_TMO,50,0);  
			     		     jump=0;
                             break;
                default:    
                             break;
             }

		  }
		}
	  }

}



