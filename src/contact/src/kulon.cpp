#include "kulon.h"

kulon::kulon(struct sa_info _cfg, SKLib::DataInterface * _iface) : Apparate(_cfg), iface (_iface)
{
  init_pk();
  errState.insert ( 0x0001 );
  errState.insert ( 0x0100 );
  errState.insert ( 0x0000 );
  errState.insert ( 0x003f );
  errState.insert ( 0x007f );
}

kulon::~kulon()
{

}

int kulon::update()
{
     pthread_mutex_lock(&updateMutex);
     ushort need_update = 0;
     char buffer[RCVBUF_SIZE];
     std::ostringstream os;

     strcpy(buffer, "");

     // Смена ключей.
     if (NextKey)
   //if (LSMENA == 0)
        commandSet(NextKey);
 
    
        // при получении индикации ПСБРК снять команду СБР КИ     
        if( (ioBuf[IN] & IND_PSBRK ) && ( ioSendBuf & APP_RESET_KEYS) )
            commandUnset( APP_RESET_KEYS );
       // при получении индикации ПВС снять команду ВС
         if( (ioBuf[IN] & IND_PVS ) && ( ioSendBuf &  APP_VS) )
            commandUnset( APP_VS ); 
       // при получении индикации ВЫЗОВ(АВТ) снять команду ВЫЗОВ(АВТ)  16.10.12  ПК-Аргунь
         if( time_iv > 0 )
         {
	   if( (time(NULL)-time_iv) >= dcCallLong &&
             (ioBuf[OUT] & APP_CALL_AUTO ) &&( ioSendBuf &  APP_CALL_AUTO) )
            commandUnset( APP_CALL_AUTO ); 	 
	 }
     
        if (sendReceiveKulon())
             need_update++;


     // Если есть новые данные - обновляем таблицы.
     if (need_update)
     { 
	  //    if (!KONF && (ioBufOld[IN] & IND_CALL_AUTO) && !(ioBuf[IN] & IND_CALL_AUTO))
	  //  sendCuuCall(750);  //  16/10/12  Кулон-Аргунь
	      
	if (!KONF && !(ioBufOld[IN] & IND_CALL_AUTO) && (ioBuf[IN] & IND_CALL_AUTO))
		sendCuuCall(65);  
      
	log1("new status: " + getStateString());
	updateTables();
     }    
     if ( rabota_off==1) // восстановить состояние работа, снятое при отбое ( commandOff() )
	  {      
	    ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();
	    rabota_off=0;
	    log1("восстанавливаем ioSendBuf"); 
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
};

ushort  kulon::answerKulon()
{    
  sprintf(ioCmd[0], "$01DDION0,%04X\r", app2private(ioSendBuf, 1));
     char s1[1024];
     s1[0] = '\000';
    
   strcat(s1, ioCmd[0]); 
    
     char buffer[RCVBUF_SIZE];
     memset(buffer,'\0', sizeof(buffer));
     int  ret = 0;
   
     // посылаем строку s1 три раза с паузой 50 миллисекунд, ответ записываем в buffer
     ret = iface->getAnswerForCommand_new( s1, buffer, 9);
//     if ( cfg.num >= 33  &&  cfg.num <= 35 ) 
//       log1( buffer );
     // ошибка при приеме информации от Кулона  и не было ошибки ранее. опрашиваем устройство еще раз
     if( (ret<9) && (ioBuf[IN] != 0) )
     {    log1("getAnswerForCommand  ret=" + LexicalCaster(ret) + "  ErrMsg=" + iface->getErrMsg()) ;  
       
          ret = iface->getAnswerForCommand_new( s1, buffer, 9); 
          if( ret<9)
	  {  
	    log1("getAnswerForCommand  ret=" + LexicalCaster(ret) + "  ErrMsg=" + iface->getErrMsg()) ;  
	    ret = iface->getAnswerForCommand_new( s1, buffer, 9); 
	  }
      }
    
     if (strstr(buffer, "\?01")!=NULL)
        {    	  
    	  //log1(" Not kullon on state ret ="+ LexicalCaster(ret) + " buff ==" 
    	    //+ string(buffer) + "==!!!!");
    	  ret = 0;
        }
        const int val = toUshortKulon(buffer) ;
        
      return ( ( ret==9 && isValidState ( val ) ) ? private2app_Kulon( val ) : 0);
}

int kulon::sendReceiveKulon()
{
     ushort need_update = 0;
     ushort nval = 0;
     ushort  nval1 = 0;
     //---------------------------
     // мажоритарность при опросе
     nval = answerKulon();
     if( nval != ioBuf[IN] )
     {     nval1 = answerKulon();
           if( nval1 != nval && nval1== ioBuf[IN] )
               nval=nval1;
       }  
     
       
     for (int i = 0; i < 2; i++)
     {   if( i==OUT ) nval = ioSendBuf;

         if (ioBuf[i] != nval)
   {    if ( i == IN ) 
    	   log1("send == " + string(ioCmd[0]));
    	   
        ioBufOld[i] = ioBuf[i];
        ioBuf[i] = nval;
        need_update++;

        if (i == IN)
        {
//      if ((ioBufOld[IN] & IND_CALL_AUTO) && (!(ioBuf[IN] & IND_CALL_AUTO)))
//    sendCuuCall(750);   //16.10.12  
	  
	  if ((!(ioBufOld[IN] & IND_CALL_AUTO)) && (ioBuf[IN] & IND_CALL_AUTO))
	      sendCuuCall(65);
        }
   }
     }

     return need_update;
}

ushort  kulon::private2app_Kulon( ushort  sost) const
{  
//    log1( LexicalCaster(sost ));
    ushort  app_s;
    app_s = private2app( sost, 0 );
    if ( (sost & inds[3][5])==0)  // нет К2
     app_s  |= IND_PUSTK1;         // установить К1
    
    app_s &= ~IND_S;
    app_s &= ~IND_NS;
   // гриф индицируется только, когда есть синхронизм
    if ( app_s & IND_SN)  // есть синхронизм
    {  if( (sost & 0x20) && ((sost & 0x40) == 0 ) )
       {  app_s |= IND_S;
          return  app_s; 
       }
       app_s |= IND_NS;
   }
    return app_s;
}

void kulon::init_pk()
{   // Define dcChannelGeo for Direct Channel. May be situation with no data.
     ushort foo[5];
     
     setCallLong( 0 );  // аппарат не связан с ПК
    
	if ( db->InitRk( foo, cfg.num) )
	{   if( foo[4]>0 ) // в БД выставлен признак работы через СА out_chan.line>0
	    { dcChannelGeo.full = 0;
	      dcChannelGeo.a.ko    = foo[0];
	      dcChannelGeo.a.kreit = foo[1];
	      dcChannelGeo.a.cell  = foo[2];
	      dcChannelGeo.a.chan  = foo[3];
	      
	      // Длительность  вызова.
	      setCallLong(foo[4]);
	    }
	}


}

bool kulon::isReadyToConnect()
{
     bool ok = false;
Log->log()<< "isReady =" << KONF;     
     if (KONF != 1)
      return ok;

     if ((!(ioBuf[IN] & IND_AVARIA) && (ioBuf[IN] & IND_ISPRAVNO)) &&
	  ((ioBuf[OUT] & APP_RABOTA)))
     ok = true;
     

     return ok;
}

void kulon::commandCall(ushort p)
{
     dcNeedSendCall = p;

	if (dcNeedSendCall == C_CALL)
	  {  
	    ioSendBuf |= APP_CALL_AUTO;
	      time_iv = time(NULL);
	  }
	else
	      ioSendBuf &= ~APP_CALL_AUTO;

}


void kulon::commandOff()
{
   //   ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();
   ioSendBuf =  APP_S | getCurrentKey();
   rabota_off = 1;
  
}


void kulon::commandOn(ushort p)
{
#ifdef REJECT_EZ
     if (p & APP_EZ)
   p &= ~APP_EZ;
#endif
    ioSendBuf = APP_RABOTA | p | getCurrentKey() ;
	

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

void kulon::commandBlock(ushort p)
{
     if ( db->updSARPUKonf( p, cfg.num ))
	KONF = p;

     ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();

     updateTables();
     
     sendCuuChanges();
}

void kulon::setDataInterface(SKLib::DataInterface * _iface)
{
	log1("setDataInterface(): ");
     iface = _iface;
};

bool kulon::isValidState( const int & val) //const 
{
 bool ret = false; 
// Log->log() << " isValidState ====== " << val; 
 ret =  errState.find ( val ) == errState.end();
 return  ret ;
} ;
