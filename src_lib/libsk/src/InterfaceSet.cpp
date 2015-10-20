#include "InterfaceSet.h"

using namespace SKLib;

// extern Log log;

NPortDI InterfaceSet::mmap;

InterfaceSet::InterfaceSet()
{   
  //log = &SKLib::LogSingleton::getInstance();
}

InterfaceSet::~InterfaceSet()
{  NPortDI::iterator  it;
     for (NPortDI::iterator it = mmap.begin(); it != mmap.end(); it++)
	  delete (it->second);
  
     SKLib::LogSingleton::getInstance().log("~InterfaceSet()");
}

void InterfaceSet::add(int nport, DataInterface *ai)
{
     mmap.insert(make_pair(nport,ai));
}


void InterfaceSet::start() const
{
     pthread_attr_t attr1;
     pthread_attr_init(&attr1);
     pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);

     pthread_t pthread1[4];
    
     for( int i=0; i<4; i++)	 
        pthread_create(&pthread1[i], &attr1, pthreadFunc, new int(i));
}

void InterfaceSet::openAll(const std::string & ipAddr, int nport, bool restart)
{   DataInterface *ai;
     std::ostringstream os;
     Log * log = &SKLib::LogSingleton::getInstance();
    // log.log("InterfaceSet::openAll() IP="+ ipAddr);
	 
     pair< NPortDI::iterator, NPortDI::iterator > p;	 
     p = mmap.equal_range( nport );	 
     
     for (NPortDI::iterator it = p.first; it != p.second; it++)
     {          ai = it->second;
 	  if(restart)  ai->close();
	  if (ai->getFd() < 1)
	       ai->open(ipAddr);

	  if ( ai->getFd() < 1)
	  {
	       os.str("");
	       os << ipAddr << ai->getInfo() << " : " << ai->getErrMsg() << ", fd=" << ai->getFd();
	       log->log(os);
	  }
     }
}

void* InterfaceSet::pthreadFunc(void* arg)
{
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
     int  np = *(int *)arg;  // np 0-3
     delete (int *)arg;
     Log * log = &SKLib::LogSingleton::getInstance();
     
      std::ostringstream  os, oip1, oip2;
     
      oip1 << "192.168.1.1" << np;
      oip2 << "192.168.2.1" << np;
      std::string  ActIP[] = { oip1.str(), oip2.str() };
     
      std::string  command;
      int  active, ret, first_time;
      int  ssvaz_nport;  // состояние связи с NPortServer
      ssvaz_nport = 0;
      first_time = 1;
      active=0;
     while (1)
     {   
          if( pingNport( np, active )==0 )  // есть связь с активным IP
	   {  openAll(ActIP[active], np); 
	      if( ssvaz_nport==0 || first_time==1) 
	      {  
		 writeToBd ( np, true, active);
                 ssvaz_nport=1;
                 first_time=0;
	      }
	  }
          else   // нет связи с активным IP
           { 
	    if( ssvaz_nport==1 || first_time==1)   // записать в БД
	    {        
		    writeToBd ( np, false, active);
		      ssvaz_nport=0;
		      first_time=0;
	    }
            if ( pingNport( np, (active + 1) % 2)==0 )  // есть связь с неактивным IP
            { 
	      log->log() << "Нет связи с " << ActIP[active] 
			  << "  Есть связь с " << ActIP[(active+1)%2] << "  restart NPortServer";;
		
		ret = rstNport( np, (active+1)%2);
               if( ret != -1 ) // рестарт NPortServer
               { //sleep(2);
                 active = (active+1)%2;
                 log->log( "ok system (/usr/bin/telnet.sh ) " + LexicalCaster( active ));
                 openAll( ActIP[active], np , true);
                 
		 writeToBd( np, true, active);
                 ssvaz_nport=1;		 
               }
               os << "ret=" << ret << "  active=" << active << " ActIP[active]=" << ActIP[active];
             } 
	  }
	  sleep(1);
     };

     return NULL;
};

int InterfaceSet::pingNport( int np, int active)
{
  std::ostringstream oip;
     
  oip << "192.168." << (active + 1)<< ".1" << np;
  std::string command = std::string("bash -c 'ping -c 2 ") + oip.str() + std::string(" 2>&1>/dev/null'");
  return system ( command.c_str() );
};

int InterfaceSet::rstNport( int np, int active)
{
  std::ostringstream oip;
     
  oip << "192.168." << (active + 1)<< ".1" << np;
  std::string command = std::string(" /usr/bin/telnet.sh ") + oip.str() + std::string(" 50000");
  int ret = system ( command.c_str() );
  oip.str("");
  oip << "ret=" << ret << command << strerror( errno );
  SKLib::LogSingleton::getInstance().log(oip);
  return ret;
};

void InterfaceSet::writeToBd( int np, bool b, int active)
{
  std::ostringstream os;
     
  os << "echo \"select wr_ssvaz_nport(1" << np << (b ? ",1,0," : ",0,1,")<< (active+1) << " );\" | psql -d frag_pgdb";
  system(  os.str().c_str() );
  SKLib::LogSingleton::getInstance().log(os);
  return;
};

