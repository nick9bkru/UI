#ifndef BDSTATENSD_H
#define BDSTATENSD_H

#include "BDState.h"

class BDStateNsd: public BDState
{
public:
  BDStateNsd(Database * _db);
  ~BDStateNsd();
  //получить состояние контакта cont, УИ c ip 
  int getNSDstate( std::string ip, int cont);
  //максимальный контакт у Ip 
  int getNSDmaxCon( std::string ip );
  //установить состояние контакта cont, УИ c ip в state
  bool setNsdState( std::string ip, int cont, int state );
  //установить состояние контакта cont, УИ c ip в state
  bool setNsdState( int numUi, int cont, int state );
private:

};

#endif // BDSTATENSD_H
