#include "InterfaceSet.h"

using namespace SKLib;

// extern Log log;

NPortDI InterfaceSet::mmap;

InterfaceSet::InterfaceSet()
{   
  //log = &SKLib::LogSingleton::Singleton::getInstance();
}

InterfaceSet::~InterfaceSet()
{  NPortDI::iterator  it;
     for (NPortDI::iterator it = mmap.begin(); it != mmap.end(); it++)
	  delete (it->second);
  
     SKLib::LogSingleton::Singleton::getInstance().log("~InterfaceSet()");
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
     Log * log = &SKLib::LogSingleton::Singleton::getInstance();
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
     Log * log = &SKLib::LogSingleton::Singleton::getInstance();
     
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
     {    command = std::string("bash -c 'ping -c 2 ") + ActIP[active] + std::string(" 2>&1>/dev/null'");
          if( system( command.c_str() )==0 )  // есть связь с активным IP
	   {  openAll(ActIP[active], np); 
	      if( ssvaz_nport==0 || first_time==1) 
	      {  os.str("");
	         os << "echo \"select wr_ssvaz_nport(1" << np << ",1,0," << active+1 <<");\" | psql -d frag_pgdb";
                 system( os.str().c_str() );
                 log->log(os);
                 ssvaz_nport=1;
                 first_time=0;
		   }
	  }
          else   // нет связи с активным IP
           { command = std::string("bash -c 'ping -c 2 ") + ActIP[(active+1)%2] + std::string(" 2>&1>/dev/null'");
	if( ssvaz_nport==1 || first_time==1)   // записать в БД
	{        os.str("");
	         os << "echo \"select wr_ssvaz_nport(1" << np << ",0,1," << active+1 << ");\" | psql -d frag_pgdb";
                 system( os.str().c_str() );		  
                 log->log(os);
                 ssvaz_nport=0;
                 first_time=0;
		   }
            if ( system( command.c_str() )==0 )  // есть связь с неактивным IP
            {  os.str("");
	       os << "Нет связи с " << ActIP[active] << "  Есть связь с " << ActIP[(active+1)%2] << "  restart NPortServer";
	       log->log(os);
 
               command = std::string(" /usr/bin/telnet.sh ") + ActIP[(active+1)%2] + std::string(" 50000");
               ret=system( command.c_str() );
               os.str("");
               os << "ret=" << ret << command << strerror( errno );
               log->log(os);
               if( ret != -1 ) // рестарт NPortServer
               { //sleep(2);
                 active = (active+1)%2;
                 log->log( "ok system (/usr/bin/telnet.sh ) " + LexicalCaster( active ));
                 openAll( ActIP[active], np , true);
                 
		 os.str("");
                 os << "echo \"select wr_ssvaz_nport(1" << np << ",1,0," << active+1 << " );\" | psql -d frag_pgdb";
                 system(  os.str().c_str() );
                 log->log(os);
                 ssvaz_nport=1;		 
               }
               os << "ret=" << ret << "  active=" << active << " ActIP[active]=" << ActIP[active];
             } }
	  sleep(1);
     };

     return NULL;
};
