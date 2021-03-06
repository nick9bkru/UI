#include "BDState.h"

BDState::BDState(Database * _db):  db ( _db )
{
  pthread_mutex_init(&mutexdb, NULL);
  rstSostUi(  );
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
  SKLib::LogSingleton::getInstance().log()<< querry.str()<< " - "<< (b ? "OK" : "ERROR");
  return b ;
};

bool BDState::setSostUi( std::string ip, bool state )
{
  std::ostringstream querry;
  querry << "  update ui_ip set sost= '" << state << "' where ip = '" << ip<< "' ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  SKLib::LogSingleton::getInstance().log()<< querry.str() << " - "<< (b ? "OK" : "ERROR");
  return b ;
};

bool BDState::rstSostUi(  )
{
  std::ostringstream querry;
  querry << "UPDATE ui_ip SET sost  = 0 where type = 0;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  SKLib::LogSingleton::getInstance().log()<< querry.str()<< " - "<< (b ? "OK" : "ERROR");
  return b ;
};

