#ifndef BDSTATE_H
#define BDSTATE_H

#include <string>
#include <sstream>
#include <pthread.h>
#include "LogSingleton.h"

#include "Database.h"

class BDState
{
public:
  BDState(Database * _db);
  virtual ~BDState();
  //получить состояние контакта cont, УИ c ip 
  int getNSDstate( std::string ip, int cont);
  //максимальный контакт у Ip 
  int getNSDmaxCon( std::string ip );
  //установить состояние контакта cont, УИ c ip в state
  bool setNsdState( std::string ip, int cont, int state );
  //установить состояние контакта cont, УИ c ip в state
  bool setNsdState( int numUi, int cont, int state );
  // работает УИ или нет!
  bool setSostUi( int numUi, bool state );
  bool setSostUi( std::string ip, bool state );
  
 bool rstSostUi( );
protected:
  pthread_mutex_t mutexdb;
  Database *db;
  
};

#endif // BDSTATE_H
