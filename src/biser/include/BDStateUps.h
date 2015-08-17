#ifndef BDSTATEUPS_H
#define BDSTATEUPS_H

#include "BDState.h"


class BDStateUps: public BDState
{
public:
  BDStateUps(Database * _db);
  ~BDStateUps();
  //получить состояние контакта cont, УИ c ip 
  int getUpsState( std::string ip, int cont);
  
   bool setUpsState( std::string ip, int cont, int state );
  //установить состояние контакта cont, УИ c ip в state
  bool setUpsState( int numUi, int cont, int state );
  
protected:
  
private :
    bool clearUps( );
};

#endif // BDSTATEUPS_H
