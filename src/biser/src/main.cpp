
#include <memory>
#include <signal.h>
#include <functional>

#include "Log.h"
#include "util.h"

#include "nsdManage.h"

#include "TCPClientManage.h"
#include "TCPServerManage.h"

#include "Database.h"

#include "BDReadConf.h"


using namespace std;

///< Экземпляр класса для журналирования.
SKLib::Log log("./biser.log");

///< PID главного потока.
int mainThreadPid;

///<  Функция запускаемая в потоке
void *pthreadTcpMng (void * );

///<  класс следит за подключением к УИ
TCPManage *TcpMng;
///<  класс опроса всех УИ
nsdManage * NsdMng;

/**
   \brief функция вызывающаяся при перехвате сигнала
*/

void sig_handler(int signum)
{
  TcpMng->stop();
  NsdMng->stop();
     UNUSED(signum);
     if (mainThreadPid != getpid())
     {
	log.log("----- hello! I'am thread. -----");
	pthread_exit(NULL);
	return;
     }
}

/**
   \brief функция вызывающаяся устанавливаем перехватчик сигналов
*/

void setSignal()
{
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
}

/**
   \brief Основная функция. Запуск всех потоков, открытие каналов к БД.
   \param argc - число аргументов командной строки.
   \param argv - массив аргументов командной строки.
*/
int main(int argc, char **argv)
{  
  UNUSED(*argv);
   pthread_t pidPthreadTcmMng; 
  if (argc == 1)
      daemon(0, 0);
  mainThreadPid = getpid();
  //Логирующий класс
  log.setDebugMode(argc > 1 ? SKLib::Log::DebugToUserScreen : SKLib::Log::DebugToFile);

  //настраиваем ловушки сигналов)
  setSignal();
  // цепляемся к БД
  std::auto_ptr< Database > db ( new Database( "127.0.0.1","frag_pgdb" )  ); ///< Указатель на объект, связанный с БД.
  if ( !db->isReady() )   
  {
    log.log("Error !! Database not started !!");
    exit(-1);
  }
  log.log("Database is start!!");
  
  try
  {
      TcpMng = new TCPServerManage(4001);
    ///<  Идентификаторы потока
     
      pthread_create(&pidPthreadTcmMng, NULL, pthreadTcpMng, NULL); 
      
      NsdMng = new nsdManage ( db.get());
      NsdMng->init();
      TCPManage::UiVec vec = NsdMng->getUiVec();
      for ( TCPManage::UiVec::iterator it=vec.begin(); it!=vec.end(); it++)
      {
	TcpMng->addUi(*it);
      }
      NsdMng->start();
  } catch ( std::string e)
  {
    log.log("!!!! ERROR == :" + e);
    sig_handler(1);
  }
  pthread_join(pidPthreadTcmMng, NULL);
  return 0;
};

void *pthreadTcpMng (void * unused )
{
  UNUSED ( unused );
  log.log("pthreadTcpMng is start!!");
  TcpMng->start();
  return NULL;
};
