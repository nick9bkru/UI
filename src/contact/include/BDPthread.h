#ifndef BDPTHREAD_H
#define BDPTHREAD_H

#include "LogSingleton.h"
#include "Database.h"
#include <sstream>
#include <pthread.h>

class BDPthread
{
public:
  BDPthread(std::string host, std::string namebd);
  ~BDPthread();
  
  bool InitRk( ushort * foo, const ushort num );
  bool Init( ushort * foo, const ushort num );
  bool updSOSTSA( const ushort in, const ushort out, const ushort ioSendBuf, const ushort num, const bool dt_key = false);
  bool updSARPU( const ushort god, const ushort kl, const ushort num);
  bool updOUTCHAN( const ushort snx, const std::string grif, const ushort num);
  bool insSigRmo( const ushort num, const ushort kod);
  bool updSARPUlsmena( const ushort p, const ushort num);
  bool isConnect( const ushort num, const bool rk = false);
  bool updSARPUKonf( const ushort p, const ushort num);
  bool getUiIp( std::string * str, const ushort num );
private:
  SKLib::Log *Log ;
  Database *db;
  pthread_mutex_t dbMutex;
};

#endif // BDPTHREAD_H
