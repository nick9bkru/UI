#include "serdolik.h"

serdolik::serdolik(struct sa_info _cfg, UiTcpArp * _ifaceDcm) : Apparate(_cfg), ifaceDcm( _ifaceDcm )
{
    setExpansionUnitPos();
    init_pk();
    /*
      std::string  ip;
	if ( !db->getUiIp( &ip, cfg.num ))
	  throw ( "Dont't find ip = " + ip);
	ifaceDcm = new TDcmInteface (ip, 50000);
 */
}

serdolik::~serdolik()
{

}

void serdolik::setExpansionUnitPos()
{
     ExpansionUnitPos = (cfg.num - 41) % 4 + 1;

     log1("ExpansionUnitPos=" + LexicalCaster(ExpansionUnitPos));
}

int serdolik::update()
{
     pthread_mutex_lock(&updateMutex);
     ushort nval = 0, need_update = 0;
     char buffer[RCVBUF_SIZE];
     std::ostringstream os;

     strcpy(buffer, "");

     // Смена ключей.
     if (NextKey)
   //if (LSMENA == 0)
        commandSet(NextKey);
     
     // не Кулон ----------------------
	  if (ioSendBuf != ioBuf[OUT])
	  {
 	     ifaceDcm->setState(app2private(ioSendBuf, 1));
	    
	    
	    
	      /*     
	    if (state_buf > 0 && dcNeedSendCall)
	      {
	    log1("DO NOW: dcNeedSendCall=" + LexicalCaster(dcNeedSendCall));
	    
	    
	    if (isAutoCallType())
	    time = dcNeedSendCall == C_CALL ? 50 : (dcNeedSendCall == C_DISCONNECT ? 600 : 0);
	    
	    usleep(time * 1000);
	    ioSendBuf &= ~chain;

		state_buf = ifaceDcm->setState(app2private(ioSendBuf, 1));
		if (state_buf > 0)
		dcNeedSendCall = 0;
	    
	      }*/
	  }
	    
	  // Получаем состояние команд и индикаций.
	  for (int j = 0; j < 2; j++)
	  {
		// Обрабатываем полученный ответ.
	         int state_buf;
 		 state_buf = (j==0) ? ifaceDcm->getnotState() : ifaceDcm->gtsState(); 
// 		 std::cout << " NUm : " << cfg.num << " state_buf == " << state_buf << std::endl;
		if ( state_buf > 0)
		{
		  nval = state_buf;
		  nval = private2app(nval, j);
		      
		}
		else
		{
			nval = 0;
		}
		  
		// Сохраняем старое и новое состояние.
		if (ioBuf[j] != nval)
		{
		  ioBufOld[j] = ioBuf[j];
		  ioBuf[j] = nval;
		  need_update++;
		}
	  }
      // конец   else  не Кулон ------------------------

     // Если есть новые данные - обновляем таблицы.
     if (need_update)
     { 
	log1("new status: " + getStateString());
	updateTables();
     }
     
    //    prio =( RKisInConnection() ) ? 1 : 0;
     if( prio>1 )  
	prio = prio - 1;
	  else 
	    if( isInConnection() ) 
	      prio = 1;
	      else
		prio = 0;
   
     pthread_mutex_unlock(&updateMutex);
     return 0;
}

void serdolik::init_pk()
{   // Define dcChannelGeo for Direct Channel. May be situation with no data.

     setCallLong( 0 );  // аппарат не связан с ПК
}


bool serdolik::isReadyToConnect()
{
     bool ok = false;
     
     if (KONF != 1)
      return ok;

    
      if ((!(ioBuf[IN] & IND_AVARIA) || (ioBuf[IN] & IND_ISPRAVNO) ) &&
	  (ioBuf[OUT] & APP_PROVERKA))
	    ok = true;
    return ok;
}

void serdolik::commandCall(ushort p)
{
     dcNeedSendCall = p;
     ioSendBuf |= APP_CALL_AUTO;
}


void serdolik::commandOff()
{
   ioSendBuf = APP_PROVERKA;

}

void serdolik::commandOn(ushort p)
{
#ifdef REJECT_EZ
     if (p & APP_EZ)
   p &= ~APP_EZ;
#endif
    ioSendBuf = APP_RABOTA | p;


     SN = 0xFFFF;

     // Для предотвращения ввода в конфигурацию выключенного аппарата
     // сначала его опрашиваем, потом посылаем соответствующее сообщение в ЦУУ.
     update();
     
     ushort arr[2] = {cfg.num, ushort ( ioBuf[IN] == 0 ? 2 : 1) };
     if (getType() == Argun)
      arr[1] = 1;
     sendUDPMessage(CUU_HOST.c_str(), 9012, arr, 2);
     if (arr[1] == 2)
	commandBlock(0);
     else
         prio = 7;     
}

void serdolik::commandBlock(ushort p)
{
     if ( db->updSARPUKonf( p, cfg.num ))
	KONF = p;

     ioSendBuf = APP_PROVERKA;

     updateTables();
     
     sendCuuChanges();
};

