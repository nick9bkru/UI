#include "BDPthread.h"


BDPthread::BDPthread(std::string host, std::string namebd)
{
  
  pthread_mutex_init(&dbMutex, NULL);
  std::cout<< "BDPthread::BDPthread ip ==" << host << " name =" << namebd  << std::endl;
  db = new Database ( host, namebd);
  if ( !db->isReady() )
  {
    delete db;
    throw ( std::string (" Don't open BD"));
  };
  Log =  &SKLib::LogSingleton::Singleton::getInstance();
}

BDPthread::~BDPthread()
{
  std::cout<< "BDPthread::~BDPthread()" << std::endl;
  delete db;
};
 
bool BDPthread::InitRk( ushort * foo, const ushort num )
{
  std::ostringstream query;
  query << "select KO,KREIT,CELL,CHAN,LINE from OUT_CHAN where N_SA_RPU= " << num << " and TYPE='ПК';";
  
  pthread_mutex_lock(&dbMutex);
   bool ok = db->singleSelect(query.str(), foo, 5*sizeof(ushort));
   pthread_mutex_unlock(&dbMutex);
   return ok;
}
 
bool BDPthread::Init( ushort * foo, const ushort num )
{
  std::ostringstream query;
  query << "select DKL, KZN, KONF, LSMENA from SA_RPU where NUM= " << num << " ;";
  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->singleSelect(query.str(), &foo, 4*sizeof(ushort));
   pthread_mutex_unlock(&dbMutex);
   return ok;
}

bool BDPthread::updSOSTSA( const ushort in, const ushort out, const ushort ioSendBuf, const ushort num, const bool dt_key )
{
  std::ostringstream query;
  query << "update SOST_SA_RPU set FROM_A= " << in << " , TO_A= " << out <<" , SENDBUF= "<< ioSendBuf;
  if (dt_key )
    query  <<", dt_key = current_timestamp" ;
  query  << " where num= "<< num << " ;";
  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);
   return ok;
}

	
bool BDPthread::updSARPU( const ushort god, const ushort kl, const ushort num)
{
  std::ostringstream query;
  query << "update SA_RPU set GOD= " << god << " , KL= " << kl <<"  where num= "<< num << " ;";

  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);
   //лог
   Log->log(query.str() + " - " + (ok ? "OK" : "ERROR"));
   return ok;
}


bool BDPthread::updOUTCHAN( const ushort snx, const std::string grif, const ushort num)
{
  std::ostringstream query;
  query << "update OUT_CHAN set SNX= " << snx << " , GRIF='" << grif <<"' where n_sa_rpu= "<< num << " ;";

  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);

   return ok;
}

bool BDPthread::updSARPUlsmena( const ushort p, const ushort num)
{
  std::ostringstream query;
  query << "update SA_RPU set LSMENA= " << p << " where NUM= " << num << " ;";

  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);
Log->log(query.str() + " - " + (ok ? "OK" : "ERROR"));
   return ok;
}

bool BDPthread::updSARPUKonf( const ushort p, const ushort num)
{
  std::ostringstream query;
  query << "update SA_RPU set KONF= " << p << " where NUM= " << num << " ;";

  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);
Log->log(query.str() + " - " + (ok ? "OK" : "ERROR"));
   return ok;
}

bool BDPthread::insSigRmo( const ushort num, const ushort kod)
{
  std::ostringstream query;
  query << "insert into sig_rmo values( current_timestamp, " << num << " , " << kod <<" );";

  
    pthread_mutex_lock(&dbMutex);
   bool ok = db->notSelect( query.str() );
   pthread_mutex_unlock(&dbMutex);
Log->log(query.str() + " - " + (ok ? "OK" : "ERROR"));
   return ok;
}
   
bool BDPthread::isConnect( const ushort num, const bool rk)
{
  std::ostringstream query;
  query << "select NUM from OUT_CHAN where N_SA_RPU= " << num ;
  if ( rk )
    query << " and TYPE='РК' and (SOST=3 or SOST=4)";
  query << ";";
    pthread_mutex_lock(&dbMutex);
   ushort n;
   bool ok = db->singleSelect(query.str(), &n, sizeof(ushort));
   pthread_mutex_unlock(&dbMutex);
   return ok;
}

bool BDPthread::getUiIp( std::string * str, const ushort num )
{
  std::ostringstream query;
  query << "select ip from ui_ip where index =\
    ( select nport from sost_sa_rpu where num = " << num << " );";
  char ip[20];
   pthread_mutex_lock(&dbMutex);
   bool ok = db->singleSelect(query.str(), ip, sizeof(ip));
   pthread_mutex_unlock(&dbMutex);
   *str = std::string(ip);
   return ok;
}
