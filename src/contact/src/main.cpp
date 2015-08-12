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

/**
   \brief Опрос аппаратов с приоритетом >= 2
   \param low - наименьший индекс аппарата в группе
   \param high - наибольший индекс аппарата в группе
   */
void  upd_high_prio_apparate(  int  low, int  high );

/**
  \brief меняем интерфейс для apparate
   \param buf - номер СА команда и номер порта (nport * 100 + port)
 */
void reSetInterfece(kg & buf);



RapsodiaSet * rs;

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
     
     Log->log("rs = new RapsodiaSet()");
     rs = new RapsodiaSet();
     rs->start();
     
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
     UIservice::TCPServerManage * TcpManage = new UIservice::TCPServerManage ( std::string( "4002") );
     
     struct sa_info foo;
     SKLib::DataInterface * iface;
      std::string  ip = "";
     UiTcpArp *ifaceDcm;
     try
     {
	  while ( db_main->getInitAll( &foo) )
	  {
	    switch ( foo.type)
	    {
	      case Kulon:
		iface = rs->getInterfaceForApp((foo.nport-1) * 16 + foo.port);
		apparate[foo.num] = new kulon(foo, iface);
		if (iface)
		{
		  apparate[foo.num]->isOn=true;
		}
	      break;
	      case Serdolik:
		
		if ( !db->getUiIp( &ip, foo.num ))
		  throw ( "Dont't find foo.num =  " + LexicalCaster(foo.num));
		ifaceDcm  = new UiTcpArp (ip) ;
		TcpManage->addUi( ifaceDcm );
		
		apparate[foo.num] = new serdolik(foo, ifaceDcm);
		apparate[foo.num]->isOn=true;
		break;
	      default:
		throw ( std::string (" Don't know apparate type") + LexicalCaster(foo.type) );
	    }
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
     TcpManage->start();
     
     return 0;
}

// Поток для приема команд.
void* pthreadReceiver(void *unused)
{    
  UNUSED ( unused ) ;
     int sockfd, sock_new, len, ret;
     struct kg buf;

     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();

     
     sockfd = make_socket(7000);

     Log->log("pthreadReceiver started!");

     while (1)
     {
	ret = listen(sockfd, 50);
	if (ret == -1)
	{
	      Log->log("listen(): " + std::string(strerror(errno)));
	      continue;
	}

	sock_new = accept(sockfd, NULL, NULL);
	if (sock_new == -1)
	{
	      Log->log("accept(): " + std::string(strerror(errno)));
	      continue;
	}
	
      //  len = recv(sock_new, &buf, sizeof(struct kg), 0);
      //  close(sock_new);
	while( (len=recv(sock_new, &buf, sizeof(struct kg), 0)) >0 )
	{ 
	if (len == sizeof(struct kg))
	{  
	  Log->log("!!!!!!!!!!!!!!!!! recv command !!!!!!!!!!!");
	      
	      if (buf.number >= 1 && buf.number <= 48)
	      {
		if  (buf.command!=CMD_SM_PORT) // меняем порт спец аппарата
		{ 
		    apparate[buf.number]->checkNewMessage(buf);
		    apparate[buf.number]->setPrio(1);
		} else  
		  {
			Log->log("ReSet Interfece number= " + LexicalCaster(buf.number) + "!!!!!!!!!!!!!");   
			reSetInterfece(buf);
		}
	  //  prAppLst[getThreadIndexForNum(buf.number)]->add(buf.number);
	      }
	      else
	    Log->log("ERROR: buf.number = " + LexicalCaster(buf.number));
	}
     }
     close( sock_new );
  // else
  //     Log->log("buffer too short");
     }
     return NULL;
}

// Поток для смены ключей.
// Номер зоны | без отл.смены/с отл.сменой | номер ключа.
void* pthreadChangeKeys(void *unused)
{
  UNUSED ( unused);
     int sock_new, ret;
     struct kg buf, kdg;
     ushort num;
     std::ostringstream os;
     
     int sockfd = make_socket(7001);
      
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     Log->log("pthreadChangeKeys started!");

     while (1)
     {
	ret = listen(sockfd, 10);
	if (ret < 0)
	{
	      Log->log("listen(): " + std::string(strerror(errno)));
	      continue;
	}

	sock_new = accept(sockfd, NULL, NULL);
	if (sock_new < 0)
	{
	      Log->log("accept(): " + std::string(strerror(errno)));
	      continue;
	}

	ret = recv(sock_new, &buf, sizeof(struct kg), 0);
	close(sock_new);
	
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
	    if (apparate[num]->isOn)
	    {   
	      kdg.number = num;
	      kdg.command = CMD_NEXT_KEY;
	      kdg.param = buf.param;
	      apparate[num]->checkNewMessage(kdg);
	      apparate[num]->setPrio(1);
	    }
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
     int sockfd, sock_new, ret;
     ushort num, buf[4];
     struct kg kdg;
     
     sockfd = make_socket(7006);
    
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     Log->log("pthreadBlockKulons started!");

     while (1)
     {
	ret = listen(sockfd, 10);
	if (ret < 0)
	{
	      Log->log("listen(): " + std::string(strerror(errno)));
	      continue;
	}

	sock_new = accept(sockfd, NULL, NULL);
	if (sock_new < 0)
	{
	      Log->log("accept(): " + std::string(strerror(errno)));
	      continue;
	}

	ret = recv(sock_new, &buf, 8, 0);
	close(sock_new);
	
	if (ret == 8)
	{
	      if ((buf[0] != 0xFEFE) || (buf[3] != 0xEFEF))
	      {
	    close(sock_new);
	    continue;
	      }
	      
	      Log->log("pthreadBlockKulons: receive KZN=" + LexicalCaster(buf[1])
	      + " status=" + LexicalCaster(buf[2]));
	}
	else
	      continue;

	while (db_main->getNumSARPU( &num, buf[1]))
	{
	    if (apparate[num]->isOn)
	    {  
	      kdg.number = num;
	      kdg.command = (buf[2] == 1 ? CMD_SET : CMD_UNSET);
	      kdg.param = APP_ZSBA;
	      apparate[num]->checkNewMessage(kdg);
	      apparate[num]->setPrio(1);
	    }
	    //  prAppLst[getThreadIndexForNum(num)]->add(num);
	}
     }
   return NULL;
}

void sig_handler(int signum)
{
  UNUSED ( signum);
  SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     if (mainThreadPid != getpid())
     {
      Log->log("----- hello! I'am thread. -----");
      pthread_exit(NULL);
      return;
     }

     sleep(5);

 //    for (int i = 0; i < 10; i++)
//   delete prAppLst[i];

//      pthread_mutex_destroy(&Apparate::updateMutex);

     
     // Delete dynamic objects.
     for (int i = 1; i < NDEV; i++)
      delete apparate[i];

     delete db_main;

     delete rs;

     exit(0);
}

void* pthreadARPU (void * _arg)
{
 int *buf = (int *) _arg;
 int n = *buf;
     delete buf;
  SKLib::LogSingleton::getInstance().log("pthreadARPU(" + LexicalCaster(n) + ")");
  for (;;)
  {
   apparate[n]->update_prio(0); 
   usleep(1);
  }
  return NULL;
}

// потоковая функция для последовательного опроса СА подключенных к одному NPortServer
//_arg = 0,1,2,3 - Кулоны
void* pthreadNPort(void * _arg)
{
     int *buf = (int *) _arg;
 int n = *buf;
     delete buf;
     int low, high;
     int jj;
     
     SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
     Log->log("pthreadNPort(" + LexicalCaster(n) + ")");
      sleep( 3 );
     switch( n )
     {	
	case 0:
		 low=1;
               high=8;				 
		 break;
	 case 1: 
		 low=9;
               high=20;
		 break;
	 case 2:
		 low=21;
               high=32;
		 break;
	 case 3: 	
		 low=33;
               high=40;
		 break;
	 case 4:
		 low=41;
               high=44;
		 break;
	 case 5: 
		 low=45;
               high=48;
		 break;
	 default:
		 return NULL;
      }   		 

   

  //   prAppLst[n] = new PriorityApparateList();
     
     while (1)
     {
      for (int i = low; i <= high; i++)
       {    // опрашивать аппараты с приоритетом >= 1
        for(  jj = low; jj<=high; jj++)  
         {      
	       upd_high_prio_apparate( low, high ); // опрашивать аппараты с приоритетом >= 2
               apparate[jj]->update_prio(1 ); // в update() проверка: оставлять ли приоритет
         }
     
        apparate[i]->update_prio(0);
        usleep(1);  
       }
     }
     return NULL;
}
// опрашивать аппараты с приоритетом >= 2
void  upd_high_prio_apparate(  int  low, int  high )
{ 
  int  i, pri;  
    do{ 
      pri=0;
      for(  i = low; i<=high; i++)  
         pri += apparate[i]->update_prio(2);      
    }  while( pri>0 );       
    usleep(1);    
}

void reSetInterfece(kg & buf)
{
  SKLib::Log *Log =  &SKLib::LogSingleton::getInstance();
  if (buf.number>=41 && buf.number<=48)
      return;
     ushort port=buf.param % 100;
     ushort nport=buf.param / 100;
      SKLib::DataInterface * iface;
     Log->log("ReSetInterfese  :: num= " + LexicalCaster(buf.number) + ", nport = "+ LexicalCaster(nport) +
	      " port = "+ LexicalCaster(port));
     pthread_mutex_lock(&(apparate[buf.number]->updateMutex));  

     if ( port && nport && ( iface = rs->getInterfaceForApp((nport-1) * 16 + port)) )  
      {
 	  ((kulon* )apparate[buf.number])->setDataInterface(iface);
	  apparate[buf.number]->isOn=true; 
      } else        
	  apparate[buf.number]->isOn=false; //делаем так что бы больше не опрашивать

     pthread_mutex_unlock(&(apparate[buf.number]->updateMutex));
};
