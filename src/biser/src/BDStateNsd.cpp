#include "BDStateNsd.h"

BDStateNsd::BDStateNsd(Database * _db) : BDState(_db)
{

}

BDStateNsd::~BDStateNsd()
{

}



int BDStateNsd::getNSDmaxCon( std::string ip )
{
  int ret ;
  std::ostringstream querry;
  querry << "select max(ui_contact) from nsd where \
    ui = (select index from ui_ip where ip='" << ip << "');";  
  pthread_mutex_lock(&mutexdb);
  bool b = db->singleSelect( querry.str() , &ret, sizeof(ret) );
  pthread_mutex_unlock(&mutexdb);
  return  b ? ret : -1;  
};

int BDStateNsd::getNSDstate( std::string ip, int cont )
{
  int ret ;
  
  std::ostringstream querry;
  querry << "select state from nsd where ui_contact =" << cont << " and ui = (\
      select index from ui_ip where ip = '" << ip << "');";
  pthread_mutex_lock(&mutexdb);
  bool b = db->singleSelect( querry.str() , &ret, sizeof(ret) );
  pthread_mutex_unlock(&mutexdb);
  return  b ? ret : -1;
};

bool BDStateNsd::setNsdState( std::string ip, int cont, int state )
{
  std::ostringstream querry;
  querry << " update nsd set state = " << state << " where ui_contact = " << cont<<
	      " and ui = ( select index from ui_ip where ip = '" << ip << "');";
// std::cout << querry.str() << std::endl;	      
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() );
  pthread_mutex_unlock(&mutexdb);
  return b ;
};

bool BDStateNsd::setNsdState( int numUi, int cont, int state )
{
  std::ostringstream querry;
  querry << " update nsd set state = " << state << " where ui_contact = " << cont<<
	      " and ui =" << numUi << " ;";
  pthread_mutex_lock(&mutexdb);
  bool b = db->notSelect( querry.str() ) ;
  pthread_mutex_unlock(&mutexdb);
  return b ;
};
