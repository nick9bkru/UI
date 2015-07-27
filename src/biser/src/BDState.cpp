#include "BDState.h"

BDState::BDState(Database * _db): mutexdb (PTHREAD_MUTEX_INITIALIZER), db ( _db )
{

}

BDState::~BDState()
{
  
}

bool BDState::setSostUi( int numUi, bool state )
{
  std::ostringstream querry;
  querry << "  update ui_ip set sost= '" << state << "' where index = " << numUi<< " ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  return b ;
};

bool BDState::setSostUi( std::string ip, bool state )
{
  std::ostringstream querry;
  querry << "  update ui_ip set sost= '" << state << "' where ip = '" << ip<< "' ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  return b ;
};