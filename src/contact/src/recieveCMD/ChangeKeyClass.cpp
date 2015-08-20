#include "recieveCMD/ChangeKeyClass.h"
#include "defines.h"
#include "LogSingleton.h"
#include "BDmain.h"
#include "util.h"

ChangeKeyClass::ChangeKeyClass() : tcpReciever(7001)
{

}

ChangeKeyClass::~ChangeKeyClass()
{

}

void ChangeKeyClass::recvMes()
{
    struct kg buf, kdg;
    ushort num;
    
    int ret = tcpReciever::recvMes( (char *) &buf, sizeof(struct kg) );
    SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
    BDmain * db_main = &SKLib::Singleton<BDmain>::getInstance();
    
    if (ret == sizeof(struct kg))
    {
	  Log->log() << "pthreadChangeKeys: KZN= " << buf.number;
	  Log->log(LexicalCaster(buf.number, 16) + " " + 
	  LexicalCaster(buf.command, 16) + " " +
	  LexicalCaster(buf.param, 16));
    }
    else
	  return;
    // меняем действующий ключ  для ключевой зоны в таблице kzn_dkl        
    db_main->updkzn ( buf.param, buf.number );
    
    //-------------------------------------------------
    // В таблице sa_rpu DKL меняет триггер upd_dkl_sa_rpu
    //-------------------------------------------------
    
    while ( db_main->getNumSARPU( &num, buf.number,CHANGE_KEYS_WITHOUT_LSMENA ) )
    {/*
	if (apparate[num]->isOn)
	{   
	  kdg.number = num;
	  kdg.command = CMD_NEXT_KEY;
	  kdg.param = buf.param;
	  apparate[num]->checkNewMessage(kdg);
	  apparate[num]->setPrio(1);
	}*/
    }
};