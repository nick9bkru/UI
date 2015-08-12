#include "nsdManage.h"
#include <sys/select.h>
#include  <algorithm>
#include <cerrno> 
#include "Log.h"

extern SKLib::Log log;

nsdManage::nsdManage( Database * _db): db( _db )
{
  Log = &SKLib::LogSingleton::getInstance();
  Log->log() << "Constructor nsdManage::nsdManage !!!!!!!!!" ;
  BDNsd = new BDStateNsd ( _db);
  BDUps = new BDStateUps ( _db);
  
}

nsdManage::~nsdManage()
{
  cleaVecDcm();
}

void nsdManage::cleaVecDcm()
{
  Dcm.clear();
}

void nsdManage::init()
{

  cleaVecDcm();
//   Dcm.push_back( new DcmNsd( "192.168.1.122", 50000, BDNsd) );
  
  BDReadConf *ConfRead =  new BDReadConf ( db ) ;
  
  BDReadConf::vecIp UI = ConfRead->getUiIp( UiTcp::UIUPS );
  
  for ( BDReadConf::vecIp::iterator it = UI.begin(); it != UI.end(); it++  )
  {
    Dcm.push_back( new DcmUps ( *it, 50000, BDUps) );
  };

  UI = ConfRead->getUiIp( UiTcp::UINSD );
  
  for ( BDReadConf::vecIp::iterator it = UI.begin(); it != UI.end(); it++  )
  {
    Dcm.push_back( new DcmNsd( *it, 50000, BDNsd) );
  };

  delete ConfRead;
}

void nsdManage::start ()
{
  Log->log() << "void nsdManage::start ()";
  IsWork = 1;
  while (IsWork)
  {
    //посылаем запрос!! 
    for (UIservice::TCPManage::UiVec::iterator it = Dcm.begin(); it != Dcm.end(); it ++)
     {
      if ( ! ( (UiTcp* )( *it ))->answState() && (*it)-> isConnect() )
      {
	(*it)->close();
      }
     };
     selectAll (); // смотри не пришло ли чего от контактов
     
  };  
};

int nsdManage::selectAll ()
{
    fd_set rfds;
    struct timeval tv;
    int retval = -1 ;
    int maxFd = 0;

    FD_ZERO(&rfds);
    /* Ждем не больше пяти секунд. */
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    try
    {
	for (UIservice::TCPManage::UiVec::iterator it = Dcm.begin(); it != Dcm.end(); it ++)
	{
	  if ( (*it)-> isConnect())
	  {
// 	    std::cout << " select  ip ==   " << (*it)->getInfo() << " fd = " << (*it)->getFd()  << std::endl;
	    maxFd = std::max ( maxFd, (*it)->getFd());
	    FD_SET((*it)->getFd(), &rfds);
	  };
	};
	
	retval = select(maxFd + 1, &rfds, NULL, NULL, &tv);
	/* Не полагаемся на значение tv! */
	if ( retval < 0)
	    throw errno;
// 	std::cout << " retval ==   " << retval  << std::endl;
	
	if ( retval )
	{
	  
	  for (UIservice::TCPManage::UiVec::iterator it = Dcm.begin(); it != Dcm.end(); it ++)
	  {
	    
	    if ( (*it)-> isConnect()  &&  FD_ISSET( (*it)->getFd(), &rfds ) )
	    {
// 	      std::cout << " Receive from  " << (*it)->getInfo()  << std::endl;
	      ((UiTcp* )( *it ))-> recieveState ();
	    };
	  };
	};
	  
    } catch ( int error)
    {
       Log->log() << " ERROR SELECT ==  " << std::strerror( error) ;
      // ошибка!!!!
    };
    return retval;
};

void nsdManage::stop ()
{
  Log->log() << "void nsdManage::stop ()" ;
  IsWork = 0;
}

UIservice::TCPManage::UiVec nsdManage::getUiVec() const
{
  return Dcm;
};

