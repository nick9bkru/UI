#include "apparateManage.h"

apparateManage::apparateManage()
{
  Log =  & SKLib::LogSingleton::getInstance();
  rs = new RapsodiaSet();
  rs->start();
}

apparateManage::~apparateManage()
{
  delete rs;
  for ( map<int , Apparate *>::iterator it = mapApp.begin(); it != mapApp.end(); it ++  )
  {
    if ( (*it).second != NULL )
    {
      delete ((*it).second ) ;
    }
  };
};

void apparateManage::addApparate(struct sa_info foo)
{
  Apparate * app = NULL ;
  SKLib::DataInterface * iface;
  std::string  ip = "";
  UiTcpArp *ifaceDcm;
  switch ( foo.type)
  {
    case Apparate::Kulon:
      iface = rs->getInterfaceForApp((foo.nport-1) * 16 + foo.port);
      app = new kulon(foo, iface);
      if (iface)
      {
	app->isOn=true;
      }
    break;
    case Apparate::Serdolik:
      
      if ( ! (SKLib::Singleton<BDPthread>::getInstance().getUiIp( &ip, foo.num ))  )
	throw ( "apparateManage Dont't find foo.num =  " + LexicalCaster(foo.num));
      ifaceDcm  = new UiTcpArp (ip) ;
      SKLib::Singleton<UIservice::TCPServerManage>::getInstance().addUi( ifaceDcm );
      
      app = new serdolik(foo, ifaceDcm);
      app->isOn=true;
      break;
    default:
      throw ( std::string ("apparateManage Don't know apparate type") + LexicalCaster(foo.type) );
  };
  mapApp[ foo.num] = app; 
};

void apparateManage::addApparate( int num , Apparate * app)
{
 mapApp[ num ] = app; 
};

void apparateManage::newMessage (struct kg kdg )
{
  Apparate * app = mapApp[ kdg.number ] ;
  if ( app && app->isOn)
  {  
      app->checkNewMessage(kdg);
      app->setPrio(1);
  }  
};

void apparateManage::reSetInterfece(kg & buf)
{
  if (buf.number>=41 && buf.number<=48)
      return;
  Apparate * app = mapApp[ buf.number ] ;
  ushort port=buf.param % 100;
  ushort nport=buf.param / 100;
    SKLib::DataInterface * iface;
  Log->log("ReSetInterfese  :: num= " + LexicalCaster(buf.number) + ", nport = "+ LexicalCaster(nport) +
	    " port = "+ LexicalCaster(port));
  pthread_mutex_lock(&(app->updateMutex));  

  if ( port && nport && ( iface = rs->getInterfaceForApp((nport-1) * 16 + port)) )  
    {
	((kulon* )app)->setDataInterface(iface);
	app->isOn=true; 
    } else        
	app->isOn=false; //делаем так что бы больше не опрашивать

  pthread_mutex_unlock(&(app->updateMutex));
}

// опрашивать аппараты с приоритетом >= 2
void  apparateManage::upd_high_prio_apparate(  int  low, int  high )
{ 
  int  i, pri;  
    do{ 
      pri=0;
      for(  i = low; i<=high; i++)  
         pri += mapApp[ i ]->update_prio(2);      
    }  while( pri>0 );       
    usleep(1);    
}

void  apparateManage::updApparate (  int  n )
{
  int low, high;
     int jj;
  switch( n )
     {	
	case 0:
		 low=1;
               high=8;				 
		 break;
	 case 1: 
		 low=9;
               high=20;
		 break;
	 case 2:
		 low=21;
               high=32;
		 break;
	 case 3: 	
		 low=33;
               high=40;
		 break;
	 case 4:
		 low=41;
               high=44;
		 break;
	 case 5: 
		 low=45;
               high=48;
		 break;
	 default:
		 throw ( std::string ( "apparateManage::updApparate n == "  ) +  LexicalCaster(n) );
      }   		 
  //   prAppLst[n] = new PriorityApparateList();
     
     while (1)
     {
      for (int i = low; i <= high; i++)
       {    // опрашивать аппараты с приоритетом >= 1
        for(  jj = low; jj<=high; jj++)  
         {      
	       upd_high_prio_apparate( low, high ); // опрашивать аппараты с приоритетом >= 2
               mapApp[ jj ]->update_prio(1 ); // в update() проверка: оставлять ли приоритет
         }
     
        mapApp[ i ]->update_prio(0);
        usleep(1);  
       }
     }
     
};