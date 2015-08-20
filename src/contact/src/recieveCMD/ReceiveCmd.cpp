#include "recieveCMD/ReceiveCmd.h"
#include "defines.h"
#include "LogSingleton.h"


ReceiveCmd::ReceiveCmd() : tcpReciever(7000)
{

}

ReceiveCmd::~ReceiveCmd()
{

}

void ReceiveCmd::recvMes()
{
  struct kg buf;
  int len = tcpReciever::recvMes( (char *)&buf, sizeof(struct kg) );
  SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();	    
  
  if (len == sizeof(struct kg))
    {  
      Log->log("!!!!!!!!!!!!!!!!! recv command !!!!!!!!!!!");
	  
	  if (buf.number >= 1 && buf.number <= 48)
	  {
	    if  (buf.command!=CMD_SM_PORT) // меняем порт спец аппарата
	    { /*
		apparate[buf.number]->checkNewMessage(buf);
		apparate[buf.number]->setPrio(1);*/
	    } else  
	      {
		    Log->log() << "ReSet Interfece number= " <<  buf.number << "!!!!!!!!!!!!!";   
// 		    reSetInterfece(buf);
	    }
	  }
	  else
	    Log->log() << "ERROR: buf.number = " <<  buf.number ;
    }
   return; 
};