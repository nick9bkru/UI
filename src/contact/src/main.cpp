/**
   \file main.cpp
   \brief Основной цикл программы, создание всех объектов, прием кодограмм управления.

   Создается экземпляр класса для каждого аппарата. Далее в бесконечном цикле идет
   опрос по очереди каждого аппарата. Аппараты с приоритетом опрашиваются чаще других.
   \author Кориков Сергей
*/

#include <pthread.h>

#include <memory>
#include <signal.h>
#include <functional>

#include "kulon.h"
#include "serdolik.h"

#include "defines.h"
#include "RapsodiaSet.h"
#include "BDmain.h"
#include "TCPServerManage.h"

#include "LogSingleton.h"
#include "apparateManage.h"
#include "tcpReciever.h"

/// PID главного потока.
int mainThreadPid;

/**
   \var NDEV
   \brief Максимальное количество аппаратов. Больше на 1, чем реальное количество, для удобства адресации.
*/
const int NDEV = 49;

/**
   \var apparate[NDEV].
   \brief Массив аппаратов.
*/
Apparate *apparate[NDEV];
     
pthread_t pthread[3]; ///<  Идентификаторы потоков для приема команд и смены ключей.
pthread_t pthread_np[4];
pthread_t pthread_ARPU[2];

//Database *db; ///< Указатель на объект, связанный с БД.
BDPthread *db; ///< Указатель на объект, связанный с БД.
//Database *db_main; ///< Указатель на объект, связанный с БД для работы в основном потоке программы.
BDmain *db_main; ///< Указатель на объект, связанный с БД для работы в основном потоке программы.
//PriorityApparateList *prAppLst[10];

/**
   \brief Ожидает прихода кодограмм управления от сервера.

   Запускается в потоке. Без входных данных и ничего не возвращает.
*/
void* pthreadReceiver(void *unused);

/**
   \brief Ожидает прихода кодограмм для смены ключей от сервера.
   
   Запускается в потоке. Без входных данных и ничего не возвращает.
*/
void* pthreadChangeKeys(void *unused);

/**
   \brief Ожидает прихода кодограмм с командой ЗСБ-А для Кулонов.
   
   Запускается в потоке. Без входных данных и ничего не возвращает.
*/
void* pthreadBlockKulons(void *unused);

void* pthreadNPort(void * _arg);
void* pthreadARPU (void * _arg);

void sig_handler(int signum);

apparateManage * appMng;

/**
   \brief Основная функция. Запуск всех потоков, открытие каналов к БД.
   \param argc - число аргументов командной строки.
   \param argv - массив аргументов командной строки.
*/
int main(int argc, char **argv)
{     
  UNUSED (argv);
     if (argc == 1)
      daemon(0, 0);
     mainThreadPid = getpid();
     
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance( "/tmp/qwe.log");
     
     Log->setDebugMode(argc > 1 ? SKLib::Log::DebugToUserScreen : SKLib::Log::DebugToFile);
     try 
     {
       db = new BDPthread("127.0.0.1", "frag_pgdb");
       db_main = new BDmain("127.0.0.1", "frag_pgdb");
       SKLib::Singleton<BDPthread>::init( db );
     } catch ( std::string e)
     {
       std::cout << " ERROR :::: "<< e << std::endl; 
       return -1 ;
     }

     signal(SIGHUP,  sig_handler);
     signal(SIGINT,  sig_handler);
     signal(SIGQUIT, sig_handler);
     signal(SIGABRT, sig_handler);
     signal(SIGTERM, sig_handler);
     signal(SIGPIPE, SIG_IGN);

     sigset_t sigs_to_block;
     sigemptyset(&sigs_to_block);
     sigaddset(&sigs_to_block, SIGPIPE);
     pthread_sigmask(SIG_BLOCK, &sigs_to_block, NULL);
//      UIservice::TCPServerManage * TcpManage = new UIservice::TCPServerManage ( std::string( "4002") );
     SKLib::Singleton<UIservice::TCPServerManage>::init( new UIservice::TCPServerManage ( std::string( "4002") ) );
     
     appMng = new apparateManage ();
     try
     {
	  struct sa_info foo;
	  while ( db_main->getInitAll( &foo) )
	  {
	    appMng->addApparate( foo  );
	  }
     }
     catch ( std::string e)
     {
       Log->log() << "!!! ERROR :::: "<< e ;
      return -1; 
     }
     pthread_create(&pthread[0], NULL, pthreadReceiver, NULL);
     pthread_create(&pthread[1], NULL, pthreadChangeKeys, NULL);
     pthread_create(&pthread[2], NULL, pthreadBlockKulons, NULL);

     int *n;
//Kulon     
     for ( int i = 0 ; i < 6 ; i ++ )
     {
       n = new int (i);
       pthread_create(&pthread_np[*n], NULL, pthreadNPort, n);
     };
//ARPU
     /*
     for ( int i = 41 ; i < 49 ; i ++ )
     {
       n = new int (i);
       pthread_create(&pthread_ARPU[*n], NULL, pthreadARPU, n);
     }; */   
     SKLib::Singleton<UIservice::TCPServerManage>::getInstance().start();
     
     return 0;
}

// Поток для приема команд.
void* pthreadReceiver(void *unused)
{    
  UNUSED ( unused ) ;
     int len;
     struct kg buf;
    tcpReciever * tctrecv;
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
      Log->log("pthreadReceiver started!");
     try
     {
	tctrecv = new tcpReciever(7000);
     } catch( std::string e)
	{
	  Log->log() << "pthreadReceiver ERROR :::: "<< e; 
	  return NULL;
	}

     

     while (1)
     {
	len = tctrecv->recvMes( (char *)&buf, sizeof(struct kg) );
	while( len >0 )
	{ 
	  if (len == sizeof(struct kg))
	  {  
	    Log->log("!!!!!!!!!!!!!!!!! recv command !!!!!!!!!!!");
		
		if (buf.number >= 1 && buf.number <= 48)
		{
		  if  (buf.command!=CMD_SM_PORT) // меняем порт спец аппарата
		  { 
		    appMng->newMessage (buf);
		  } 
		    else  
		    {
			  Log->log("ReSet Interfece number= " + LexicalCaster(buf.number) + "!!!!!!!!!!!!!");   
			  appMng->reSetInterfece(buf);
		  }
	    //  prAppLst[getThreadIndexForNum(buf.number)]->add(buf.number);
		}
		else
	      Log->log("ERROR: buf.number = " + LexicalCaster(buf.number));
	  }
        }
     }
     return NULL;
}

// Поток для смены ключей.
// Номер зоны | без отл.смены/с отл.сменой | номер ключа.
void* pthreadChangeKeys(void *unused)
{
  UNUSED ( unused);
     int  ret;
     struct kg buf, kdg;
     ushort num;
     std::ostringstream os;
    tcpReciever * tctrecv;
    SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     try
     {
       tctrecv = new tcpReciever(7001);
     } catch( std::string e)
	{
	  Log->log() << "pthreadChangeKeys ERROR :::: "<< e; 
	  return NULL;
	}
     Log->log("pthreadChangeKeys started!");

     while (1)
     {
 	ret = tctrecv->recvMes( (char *) &buf, sizeof(struct kg) );
	if (ret == sizeof(struct kg))
	{
	      Log->log("pthreadChangeKeys: KZN=" + LexicalCaster(buf.number));
	      Log->log(LexicalCaster(buf.number, 16) + " " + 
	      LexicalCaster(buf.command, 16) + " " +
	      LexicalCaster(buf.param, 16));
	}
	else
	      continue;
	// меняем действующий ключ  для ключевой зоны в таблице kzn_dkl        
	db_main->updkzn ( buf.param, buf.number );
	
	//-------------------------------------------------
	// В таблице sa_rpu DKL меняет триггер upd_dkl_sa_rpu
	//-------------------------------------------------
	
	while (db_main->getNumSARPU( &num, buf.number,CHANGE_KEYS_WITHOUT_LSMENA ))
	{   
	      kdg.number = num;
	      kdg.command = CMD_NEXT_KEY;
	      kdg.param = buf.param;
	      appMng->newMessage( kdg );
	    //  prAppLst[getThreadIndexForNum(num)]->add(num);
	}

	continue;
  
     }
     return NULL;
}

// Посылаем команду ЗСБ-А для каждого Кулона данной К-зоны.
void* pthreadBlockKulons(void *unused)
{
  UNUSED ( unused);
     int  ret;
     ushort num, buf[4];
     struct kg kdg;
     tcpReciever * tctrecv;
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     try
     {
       tctrecv = new tcpReciever(7006);
     } catch( std::string e)
	{
	  Log->log() << "pthreadBlockKulons ERROR :::: "<< e; 
	  return NULL;
	}
     Log->log("pthreadBlockKulons started!");

     while (1)
     {
       try
       {
	    ret = tctrecv->recvMes( (char*) buf, 8);
	    if (ret == 8)
	    {
		  if ((buf[0] != 0xFEFE) || (buf[3] != 0xEFEF))
		  {
			continue;
		  }
		  
		  Log->log("pthreadBlockKulons: receive KZN=" + LexicalCaster(buf[1])
		  + " status=" + LexicalCaster(buf[2]));
	    }
	    else
		  continue;

	    while (db_main->getNumSARPU( &num, buf[1]))
	    {  
		  kdg.number = num;
		  kdg.command = (buf[2] == 1 ? CMD_SET : CMD_UNSET);
		  kdg.param = APP_ZSBA;
		  appMng->newMessage( kdg );
		//  prAppLst[getThreadIndexForNum(num)]->add(num);
	    }
       }catch ( std::string e)
	{
	  Log->log() << "pthreadBlockKulons ERROR :::: "<< e; 
	}
     }
   return NULL;
}

void sig_handler(int signum)
{
  UNUSED ( signum);
     if (mainThreadPid != getpid())
     {
      SKLib::LogSingleton::getInstance().log("----- hello! I'am thread. -----");
      pthread_exit(NULL);
      return;
     }

     delete db_main;


     exit(0);
}

void* pthreadARPU (void * _arg)
{/*
 int *buf = (int *) _arg;
 int n = *buf;
     delete buf;
  SKLib::LogSingleton::getInstance().log("pthreadARPU(" + LexicalCaster(n) + ")");
  for (;;)
  {
   apparate[n]->update_prio(0); 
   usleep(1);
  }*/
  return NULL;
}

// потоковая функция для последовательного опроса СА подключенных к одному NPortServer
//_arg = 0,1,2,3 - Кулоны
void* pthreadNPort(void * _arg)
{
     int *buf = (int *) _arg;
      int n = *buf;
     delete buf;
     
     SKLib::LogSingleton::getInstance().log("pthreadNPort(" + LexicalCaster(n) + ")");
      sleep( 3 );
      try
      {
	appMng->updApparate( n );
      } catch ( std:: string e)
      {
	SKLib::LogSingleton::getInstance().log()<< "EROOR !!!! " << e ;
      }
      
     return NULL;
}
