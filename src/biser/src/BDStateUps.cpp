#include "BDStateUps.h"

BDStateUps::BDStateUps(Database * _db): BDState(_db)
{
    //clearUps( );
}

BDStateUps::~BDStateUps()
{

}

int BDStateUps::getUpsState( std::string ip, int cont )
{
  int ret ;
  
  std::ostringstream querry;
  querry <<"select sost from ups where ui_contact =" << cont<<
    " and ui = ( select index from ui_ip where ip = '" << ip << "')" ;

  pthread_mutex_lock(&mutexdb);
  bool b = db->singleSelect( querry.str() , &ret, sizeof(ret) );
  pthread_mutex_unlock(&mutexdb);
  return  b ? ret : -1;
};

bool BDStateUps::setUpsState( std::string ip, int cont, int state )
{
  std::ostringstream querry;
  querry << " update ups set sost = " << state << " where ui_contact = " << cont<<
	      " and ui = ( select index from ui_ip where ip = '" << ip << "');";
// std::cout << querry.str() << std::endl;	      
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() );
  pthread_mutex_unlock(&mutexdb);
  return b ;
};

bool BDStateUps::setUpsState( int numUi, int cont, int state )
{
  std::ostringstream querry;
  querry << " update ups set sost = " << state << " where ui_contact = " << cont<<
	      " and ui =" << numUi << " ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  return b ;
};

bool BDStateUps::clearUps( )
{
  std::ostringstream querry;
  querry << " update ups set sost = 0 ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  return b ;
};


