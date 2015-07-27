/**
   \file apparate.cpp
   \brief Реализация класса Apparate.
   
   Аппараты инициализируются значениями, полученными из БД.
   \author Кориков Сергей
*/
#include "apparate.h"
#include <pthread.h>

#define REJECT_EZ

extern BDPthread *db;

pthread_mutex_t Apparate::updateMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Apparate::dbMutex = PTHREAD_MUTEX_INITIALIZER;

Apparate::Apparate(struct sa_info _cfg):
     cfg(_cfg), ExpansionUnitPos(0),
     NextKey(0), dcCall(0), dcNeedSendCall(0)
{
     iface = NULL;
     
     init(INIT_FULL);
     init_pk();
     time_iv = 0;
     
     SN = 3;

     ioBuf[IN] = ioBuf[OUT] = ioBufOld[IN] = ioBufOld[OUT] = 0xF000;
     
     rabota_off = 0;
     prio = 0;
     isOn=false;
     
     if (getType() == Serdolik) 
     {
      std::string  ip;
	if ( !db->getUiIp( &ip, cfg.num ))
	  throw ( "Dont't find ip = " + ip);
	ifaceDcm = new TDcmInteface (ip, 50000);
     };
   
}

Apparate::~Apparate()
{
     log.log("~Apparate()");
     delete ifaceDcm;
     
}
void Apparate::init_pk()
{   // Define dcChannelGeo for Direct Channel. May be situation with no data.
     ushort foo[5];
     
     setCallLong( 0 );  // аппарат не связан с ПК
     
     if (getType() != Serdolik)
     {	    
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

}
void Apparate::init(int flags)
{
     if (getType() == Serdolik)
     {
	setExpansionUnitPos();
	       
      }
     
     // Считываем данные из БД.
     ushort foo[4];
     
     if (db->Init(foo, cfg.num))
     {
      KL  = foo[0];
      KZN = foo[1];
      KONF = foo[2];
      LSMENA = foo[3];
     }

     

     // TODO: LSMENA.
     if (flags == INIT_FULL)
     {
	ioSendBuf = cfg.sendbuf;
   
//    if (LSMENA)
//    {
//         dprintf("%3d: sendbuf = 0x%X\n", cfg.num, cfg.sendbuf);
//         ioSendBuf = cfg.sendbuf;
//    }
//    else
//    {
//         if (KL == 1)
//       ioSendBuf = APP_K1;
//         else if (KL == 2)
//       ioSendBuf = APP_K2;
//         else
//       ioSendBuf = APP_K1;
//    }
     }
}
int  Apparate::update_prio( int val )
{    if( prio>=val && isOn)  
    {  update();
       return 1; } 
    return 0;
          
}
int Apparate::update()
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
     
     // Устанавливаем новое состояние.
    if (getType() == Kulon)
     { 
    
        // при получении индикации ПСБРК снять команду СБР КИ     
        if( (ioBuf[IN] & IND_PSBRK ) && ( ioSendBuf & APP_RESET_KEYS) )
            commandUnset( APP_RESET_KEYS );
       // при получении индикации ПВС снять команду ВС
         if( (ioBuf[IN] & IND_PVS ) && ( ioSendBuf &  APP_VS) )
            commandUnset( APP_VS ); 
       // при получении индикации ВЫЗОВ(АВТ) снять команду ВЫЗОВ(АВТ)  16.10.12  ПК-Аргунь
         if( time_iv > 0 )
         {if( (time(NULL)-time_iv) >= dcCallLong &&
             (ioBuf[OUT] & APP_CALL_AUTO ) &&( ioSendBuf &  APP_CALL_AUTO) )
            commandUnset( APP_CALL_AUTO ); 	 
            }
     
        if (sendReceiveKulon())
             need_update++;
     }
     else   // не Кулон ----------------------
     {
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
		 state_buf = (j==0) ? ifaceDcm->getnotState() : ifaceDcm->gtsState_(); 
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
     }   // конец   else  не Кулон ------------------------

     // Если есть новые данные - обновляем таблицы.
     if (need_update)
     {   
	if (getType() == Kulon)
	{
	  //    if (!KONF && (ioBufOld[IN] & IND_CALL_AUTO) && !(ioBuf[IN] & IND_CALL_AUTO))
	  //  sendCuuCall(750);  //  16/10/12  Кулон-Аргунь
	      
	      if (!KONF && !(ioBufOld[IN] & IND_CALL_AUTO) && (ioBuf[IN] & IND_CALL_AUTO))
		sendCuuCall(65);  
      
	      
	}

	log1("new status: " + getStateString());
	updateTables();
     }
      if (getType() == Kulon)
      {    
	if ( rabota_off==1) // восстановить состояние работа, снятое при отбое ( commandOff() )
	  {      
	    ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();
	    rabota_off=0;
	    log1("восстанавливаем ioSendBuf"); 
	  }
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

void Apparate::updateFreqBuffers(const char *str)
{
     char s[5];

     for (int j = 0; j < 2; j++)
     {
	memcpy(s, str + 4 + 5*j, 4);
	s[4] = '\000';

	FreqBuffer[j] = strtol(s, NULL, 16);
     }
}

void Apparate::updateTables()
{
     ushort snx, kl;
     char grif[5] = "С";

     // Обновить таблицу состояний.

     bool dt_key = ( cfg.num<=40 && (ioBufOld[IN] & IND_NOKK) && (ioBufOld[IN] & IND_NDKK) &&
                       !(ioBuf[IN] & IND_NOKK) && (ioBuf[IN] & IND_NDKK) ) ;
		       
      db->updSOSTSA( ioBuf[IN], ioBuf[OUT], ioSendBuf, cfg.num, dt_key ) ;

     // Определяем годность.
     ushort god = isReadyToConnect() ? 1 : 0;
     
       // при смене грифа С на НС в команде выдаем сообщение raps_linter
    /* if( (KONF==1) && ( ioBuf[OUT] & APP_NS ) && (ioBufOld[OUT] & APP_S ) )
     {   ushort arr[2] = {cfg.num, 3 };
          sendUDPMessage(CUU_HOST.c_str(), 9012, arr, 2);
      }*/
     // Определяем синхронизм.
     if (ioBuf[IN] & IND_SN)  // есть синхронизм
     {
   snx = 0;

   // Определяем гриф.
   if (ioBuf[IN] & IND_S)
        strncpy(&grif[0], "С", 5);
   else
    {    strncpy(&grif[0], "НС", 5);
         // при смене грифа С на НС в индикации выдаем сообщение raps_linter
         if( (isInConnection()) && (ioBufOld[IN] & IND_S ) )
         {  ushort arr[2] = {cfg.num, 3 };
            sendUDPMessage(CUU_HOST.c_str(), 9012, arr, 2);
         }
     }}
     else    // нет синхронизма
     {
   snx = 1;

   // Write blank GRIF string if no synchronism.
   strncpy(&grif[0], " ", 5);
     }
     
     // TODO: Определяем текущий ключ пока только для А80.
     kl = ioBuf[OUT] & APP_K1 ? 1 : (ioBuf[OUT] & APP_K2 ? 2 : 0);
     
     db->updSARPU( god, kl, cfg.num );
     db->updOUTCHAN( snx, std::string ( grif ), cfg.num );


     // Изменение синхронизма передаем на все РМО.
     if (isInConnection())
     {
	if (SN != snx || ((ioBufOld[IN] & IND_S ) != (ioBuf[IN] & IND_S)))
	{
	      SN = snx;
	  //   sendRmoSynch();
	      write_sig_rmo( 80+snx ); 
	}
     }
  
     
    
}
void  Apparate::write_sig_rmo( ushort kod)
{ 
  db->insSigRmo( cfg.num, kod );
}
void Apparate::sendRmoSynch() const
{
     char rmo_host[20];
     ushort arr[2] = {cfg.num, ushort (this->SN + 80 )};
     
     for (int i = 3; i <= 6; i++)
     {
   sprintf(rmo_host, "192.168.1.%d", i);
   sendMessage(rmo_host, 4241, arr, 4);
   log1("Notify RMO(" + std::string(rmo_host) + ") message: \""
        + LexicalCaster(arr[0], 16) + " " + LexicalCaster(arr[1], 16) + "\"");
     }
}

void Apparate::sendCuuChanges()
{
     ushort arr[8];
     
     arr[0] = 0xFFF0;
     arr[1] = 0;
     arr[2] = 0xFFFA;
     arr[3] = cfg.num;
     arr[4] = 0x2A;
     arr[5] = 0xFFF8;
     arr[6] = 0;
     arr[7] = 0;

     sendMessage(CUU_HOST, 1061, arr, 16);
}

void Apparate::sendCuuCall(ushort impulse)
{
     log1("dcCallLong = " + LexicalCaster(dcCallLong)
   + ", dcCall = " + LexicalCaster(dcCall)
   + ", impulse = " + LexicalCaster(impulse) + "ms");

  /*   if (isManualCallType())
     {
       log1("manual");
   if (impulse >= 1 && impulse <= 5000)
   {
        dcCall = C_CALL;
        log1("current signal = " + LexicalCaster(dcCall));
   }
     }
     else if (isAutoCallType())
     {   
   log1("auto");
   if (impulse >= 40 && impulse <= 90)
        dcCall = C_CALL;
   else if (impulse >= 500 && impulse <= 1000)
        dcCall = C_DISCONNECT;
   else
        return;
     }
     else
   return; */
   dcCall = C_CALL;

     log1("NOW dcCall = " + LexicalCaster(dcCall) + " !!!");

     ushort arr[7], len;

     arr[0] = 0xFFF0;
     arr[1] = 0;
     arr[2] = 0xFFF6;
     arr[3] = dcChannelGeo.full;
     
     if (dcCall == C_CALL)
     {
   arr[4] = 0x1;
   arr[5] = 0x5;
   arr[6] = 0xFFF8;
   len = 7;
     }
     else
     {
   arr[4] = 0x5;
   arr[5] = 0xFFF8;
   len = 6;
     }

     sendMessage(CUU_HOST, 1061, arr, len*2);

     // And do same action to RMISH.
     arr[0] = cfg.num;
     arr[1] = 0x100;
     arr[2] = dcCall;

     sendMessage(RMISH_HOST, 7099, arr, 6);
}

bool Apparate::checkNewMessage(const struct kg & buf)
{
     log1("checkNewMessage(): command=" + LexicalCaster(buf.command, 16)
   + ", param=" + LexicalCaster(buf.param, 16));
     
     switch (buf.command)
     {
     case CMD_ON:
   init(INIT_WITHOUT_KEYS);
   commandOn(buf.param);
   break;
     case CMD_OFF:
   commandOff();
   break;
     case CMD_BLOCK:
   init(INIT_WITHOUT_KEYS);
   commandBlock(0);
   break;
     case CMD_UNBLOCK:
   init(INIT_WITHOUT_KEYS);
   commandBlock(1);
   break;
     case CMD_SET:
   commandSet(buf.param);
   break;
     case CMD_UNSET:
   commandUnset(buf.param);
   break;
     case CMD_CALL:
   commandCall(buf.param);
   break;
     case CMD_NEXT_KEY:
   commandNextKey(buf.param);
   break;
     case CMD_LSMENA:
   commandLsmena(buf.param);
   break;
    case CMD_PK:
   init_pk();
   break;
     default:
   break;
     }
     
     return true;
}

void Apparate::commandBlock(ushort p)
{
     if ( db->updSARPUKonf( p, cfg.num ))
	KONF = p;

     switch (getType())
     {
	  case Argun:
	    ioSendBuf = APP_RABOTA | APP_PROVERKA | APP_S | getCurrentKey();
	    break;
	  case Delta:
	    ioSendBuf = APP_PROVERKA | APP_S | getCurrentKey();
	    break;
	  case Kulon:
	    ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();
	    break;   
	  case Serdolik:
	    ioSendBuf = APP_PROVERKA;
	    break;
	  default:
	    ioSendBuf = 0;
	    break;
     }

     updateTables();
     
     sendCuuChanges();
}

ushort Apparate::app2private(ushort sost, ushort io) const
{
     ushort private_sost = 0;
     ushort i = cfg.type / 10;
     ushort j = 0;
     
     //  Индикация (скоре всего не пригодится)
     if (io == IN)
     {
   for (j = 0; j < 16; j++)
   {
        if ( (sost & inds[0][j]) && inds[0][j] )
      private_sost |= inds[i][j];
   }
   return private_sost;
     }
     // Команды
     else if (io == OUT)
     {   
   for (j = 0; j < 16; j++)
   {
        if ( (sost & cmds[0][j]) && cmds[0][j] )
      private_sost |= cmds[i][j];
   }
   return private_sost;
     }
     else
   return private_sost;
}
ushort  Apparate::private2app_Kulon( ushort  sost) const
{  ushort  app_s;
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
ushort Apparate::private2app(ushort sost, ushort io) const
{
     ushort app_sost = 0;
     ushort i = cfg.type / 10;
     ushort j = 0;
     
     //  Индикация
     if (io == IN)
     {
   for (j = 0; j < 16; j++)
   {
        if ((sost & inds[i][j]) && inds[i][j])
      app_sost |= inds[0][j];
   }
   return app_sost;
     }
     // Команды
     else if (io == OUT)
     {   
   for (j = 0; j < 16; j++)
   {
        if ( (sost & cmds[i][j]) && cmds[i][j] )
      app_sost |= cmds[0][j];
   }
   return app_sost;
     }
     else
   return app_sost;
}

inline ushort Apparate::getCurrentKey() const
{
     return (ioSendBuf & (APP_K1 | APP_K2));
}

void Apparate::commandOn(ushort p)
{
#ifdef REJECT_EZ
     if (p & APP_EZ)
   p &= ~APP_EZ;
#endif
     
     switch (getType())
     {
     case Argun:
     case Delta:
     case Kulon:
	ioSendBuf = APP_RABOTA | p | getCurrentKey() ;
	break;   
     case Serdolik:
	ioSendBuf = APP_RABOTA | p;
	break;
     default:
	ioSendBuf = 0;
	break;
     }

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

void Apparate::commandOff()
{
     switch (getType())
     {
     
     case Kulon:
//   ioSendBuf = APP_RABOTA | APP_S | getCurrentKey();
   ioSendBuf =  APP_S | getCurrentKey();
   rabota_off = 1;
   break;   
     case Serdolik:
   ioSendBuf = APP_PROVERKA;
   break;
     default:
   ioSendBuf = 0;
   break;
     }
}

void Apparate::commandSet(ushort p)
{

#ifdef REJECT_EZ
     if (p & APP_EZ)
   p &= ~APP_EZ;
#endif
     
     if ((p & APP_NS) && (ioSendBuf & APP_S))
	ioSendBuf &= ~APP_S;
     
     if ((p & APP_S) && (ioSendBuf & APP_NS))
	ioSendBuf &= ~APP_NS;

     if ((p & APP_K1) && (ioSendBuf & APP_K2))
     {
	if (isInConnection())
	{
	      NextKey = APP_K1;
	      log1("Waiting for K1.");
	      return;
	}
	else
	{
	      NextKey = 0;
	      ioSendBuf &= ~APP_K2;
	}
     }

     if ((p & APP_K2) && (ioSendBuf & APP_K1))
     {
	if (isInConnection())
	{
	      NextKey = APP_K2;
	      log1("Waiting for K2.");
	      return;
	}
	else
	{
	      NextKey = 0;
	      ioSendBuf &= ~APP_K1;
	}
     }
     
     ioSendBuf |= p;
     log1("CMD_SET: ioSendBuf = " + LexicalCaster(ioSendBuf, 16));
}

void Apparate::commandUnset(ushort p)
{
     ioSendBuf &= ~p;
     log1("CMD_UNSET: ioSendBuf = " + LexicalCaster(ioSendBuf, 16));
}

void Apparate::commandCall(ushort p)
{
     dcNeedSendCall = p;

     if (getType() == Kulon)
     {
	if (dcNeedSendCall == C_CALL)
	  {  
	    ioSendBuf |= APP_CALL_AUTO;
	      time_iv = time(NULL);
	  }
	else
	      ioSendBuf &= ~APP_CALL_AUTO;
     }
     else
   ioSendBuf |= APP_CALL_AUTO;
}

void Apparate::commandNextKey(ushort p)
{
     if (p == 1)
   commandSet(APP_K1);
     else if (p == 2)
   commandSet(APP_K2);
     else
   return;
}

void Apparate::commandLsmena(ushort p)
{
     if ( db->updSARPUlsmena( p, cfg.num ) )
	LSMENA = p;
}

inline ApparateType Apparate::getType() const
{
     return static_cast<ApparateType>(cfg.type);
}
bool Apparate::RKisInConnection() const
{
     if (KONF != 1)
      return false;     
   return db->isConnect( cfg.num, true );
}
bool Apparate::isInConnection() const
{
     if (KONF != 1)
	return false;

     bool ok = false;
     
     switch (getType())
     {
    
     case Kulon:
     case Serdolik:
     {
	ok = db->isConnect( cfg.num );
	break;
     }
     default:
      break;
     }
     
     return ok;
}

/*inline bool Apparate::isManualCallType() const
{
     return dcCallType == 1 ? true : false;
}

inline bool Apparate::isAutoCallType() const
{
     return dcCallType == 0 ? true : false;
}

inline bool Apparate::isUnknownCallType() const
{
     return dcCallType == 127 ? true : false;
}*/

inline void Apparate::setCallLong(ushort n)
{    switch((int)n)
  {  case 1:
     case 2:
     case 3: 
       dcCallLong = n;
       break;
     case 4: 
       dcCallLong = 5;
       break;      
     case 5:
       dcCallLong = 7;
       break;  
     case 6:
       dcCallLong = 10;
       break;
     case 7:
       dcCallLong = 15;
       break;  
     default:
       dcCallLong = 0;
       break;       
      }
}      

void Apparate::setExpansionUnitPos()
{
     switch (getType())
     {
	case Serdolik:
	  ExpansionUnitPos = (cfg.num - 41) % 4 + 1;
	  break;
	 default:  //  Kulon
	  ExpansionUnitPos = 1;
	  break;
     
     }

     log1("ExpansionUnitPos=" + LexicalCaster(ExpansionUnitPos));
}

bool Apparate::isReadyToConnect() const
{
     bool ok = false;
     
     if (KONF != 1)
      return ok;

     switch (getType())
     {
     
     case Kulon:
     {
      if ((!(ioBuf[IN] & IND_AVARIA) && (ioBuf[IN] & IND_ISPRAVNO)) &&
	  ((ioBuf[OUT] & APP_RABOTA)))
	    ok = true;
      break;
     }
     case Serdolik:
     {
      if ((!(ioBuf[IN] & IND_AVARIA) || (ioBuf[IN] & IND_ISPRAVNO) ) &&
	  (ioBuf[OUT] & APP_PROVERKA))
	    ok = true;
      break;
     }
     default:
      break;
     }

     return ok;
}

inline std::string Apparate::getStateString() const
{
     static const char * const names_cmd[16] =
   {
        "РАБ", "ПРОВ", "ЭЗ", "ПШ", "К1", "К2", "НС", "С",
        "ВО", "ВО_РУЧН", "ЗСБ-А", "ПОК-К", "ПОК-А", "ВС", "СБРОС", ""
   };
     static const char * const names_ind[16] =
   {
        "ПСБРК", "ПВС", "НС", "С", "АВАР", "ПК2", "ПК1", "ВО",
        "ИСПР", "СН", "НК2(НДК-К)", "НК1(НДК-А)", "ОК", "НОК-К", "НОК-А", "НСД_УСО"
   };
     
     std::ostringstream os;     
     os << "cmds <";
     
     if (ioBuf[OUT])
     {
      for (int i = 0; i < 16; i++)
      {
	    if (ioBuf[OUT] & (1<<i))
	  os << names_cmd[i] << " ";
      }
     }

     os << ">   inds <";

     if (ioBuf[IN])
     {
      for (int i = 0; i < 16; i++)
      {
	    if (ioBuf[IN] & (1<<i))
	  os << names_ind[i] << " ";
      }
     }

     os << ">" << "  prio=" << prio;

     return os.str();
}

void Apparate::log1(const std::string & string1) const
{
     std::ostringstream ostrm;
     ostrm << setw(3) << setfill(' ') << cfg.num << ": " << string1;
     log.log(ostrm);
}

void Apparate::log1(const std::ostringstream & os) const
{     
     log1(os.str());
}

/*inline bool Apparate::isApparateWithFreq() const
{
     return (getType() == Argun || getType() == Delta) ? true : false;
}*/

std::ostream & operator<<(std::ostream & strm, const struct sa_info & foo)
{
     return strm << foo.num << ", " << foo.type << ", " << foo.nport << ", "
        << foo.port << ", " << foo.sendbuf;
}
ushort  Apparate::answerKulon()
{    sprintf(ioCmd[0], "$01DDION0,%04X\r", app2private(ioSendBuf, 1));
     char s1[1024];
     s1[0] = '\000';
    
   strcat(s1, ioCmd[0]); 
    
     char buffer[RCVBUF_SIZE];
     memset(buffer,'\0', sizeof(buffer));
     int  ret = 0;
   
     // посылаем строку s1 три раза с паузой 50 миллисекунд, ответ записываем в buffer
     ret = iface->getAnswerForCommand_new( s1, buffer, 9);
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
      return ( ret==9 ? private2app_Kulon( toUshortKulon(buffer)) : 0);
}
int Apparate::sendReceiveKulon()
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



void Apparate::setDataInterface(SKLib::DataInterface * _iface)
{
	log1("setDataInterface(): ");
     iface = _iface;
}

void  Apparate::setPrio(int  val)
{ 
  if( prio<val)       
    prio = val;
}

