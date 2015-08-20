#include "recieveCMD/BlockKulons.h"
#include "defines.h"
#include "LogSingleton.h"
#include "BDmain.h"

BlockKulons::BlockKulons() : tcpReciever( 7006 )
{

}

BlockKulons::~BlockKulons()
{

}

void BlockKulons::recvMes()
{
  int ret;
     ushort num, buf[4];
     struct kg kdg;
     
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     BDmain * db_main = &SKLib::Singleton<BDmain>::getInstance();
     
    ret = tcpReciever::recvMes( (char*) buf, 8);
    
    if ( ret == 8 && (buf[0] == 0xFEFE) && (buf[3] == 0xEFEF)  )
    {	  
	  Log->log() << "pthreadBlockKulons: receive KZN= " << buf[1] <<
	    " status= " <<buf[2];
    }
    else
	  return;

    while (db_main->getNumSARPU( &num, buf[1]))
    {
      /*
	if (apparate[num]->isOn)
	{  
	  kdg.number = num;
	  kdg.command = (buf[2] == 1 ? CMD_SET : CMD_UNSET);
	  kdg.param = APP_ZSBA;
	  apparate[num]->checkNewMessage(kdg);
	  apparate[num]->setPrio(1);
	}*/
    }
};
