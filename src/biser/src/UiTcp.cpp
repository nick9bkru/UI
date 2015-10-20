#include "../../biser/include/UiTcp.h"

UiTcp::UiTcp(std::string _ip, int _port) :  UIservice::UiTcp (_ip, _port)
{
 //countSend = 0;
 TIMEINTERVAL = 10000000; //1 секунды 
 TIMENEXTASK = 100000;
 countNewMsg= 0;
 Log = &SKLib::LogSingleton::getInstance();
}

UiTcp::~UiTcp()
{
}

int UiTcp::close(  )
{
  std::cout << "UiTcp::close(  ) getInfo  " << getInfo()  << std::endl;
  setUiOn(  );
  if ( isConnect() )
  {
    MutexLocker q(mtx);
   ::close(getFd());
   }
   setFd(-1);

  return 1;
  
};

int UiTcp::open(  )
{
  bool ret = false;
  
  gettimeofday(&timeLastAsk, NULL);;
  timeLastAsk.tv_sec--;
  if ( SKLib::TCPSocketInterface::open( getInfo() ) ) 
  {
     ret = true;
     setUiOn( true );
     resetLastAnsw ();
  }
  return ret;
  
};

bool UiTcp::answState()
{
  struct timeval timeNow;
  gettimeofday(&timeNow, NULL);
  int usec =  (timeNow.tv_sec * 1000000 + timeNow.tv_usec ) - ( timeLastAnsw.tv_sec * 1000000 + timeLastAnsw.tv_usec );
 // countSend ++;
  std::string cmd= "get\r";
  MutexLocker q(mtx);
  
  if ( ! isConnect() || ( usec > TIMEINTERVAL ))
    return 0;
    
  usec =  (timeNow.tv_sec * 1000000 + timeNow.tv_usec ) - ( timeLastAsk.tv_sec * 1000000 + timeLastAsk.tv_usec ); 
 // std::cout << "UiTcp::answState() usec ==   " << usec  << " fd = " << getFd() << " ip == " << getInfo()<< std::endl;
 // std::cout << "UiTcp::answState() usec2 ==   " << usec  << std::endl;
  if ( usec < TIMENEXTASK )
  {
//    std::cout << "ret "  << std::endl;
    return 1;
  };
  
 // gettimeofday(&timeLastAsk, NULL);

    
    bool ask = (onlySend (cmd) == int (cmd.length()));

  if ( ask )
  {
    gettimeofday(&timeLastAsk, NULL);
  }
//     std::cout << "UiTcp::answState() ask ==   " << ask  << std::endl;
  return ask ;
};

bool UiTcp::resetLastAnsw()
{
  gettimeofday(&timeLastAnsw, NULL);
  return true ;
}


bool UiTcp::recieveState()
{
  int buf;
  char b [3];
  
  int ret = 0;
  {
    MutexLocker q(mtx);
    ret= onlyRead ( b , 3);
  }
  /*
   std::cout << " ret == " << ret << " "<< getInfo() ;
  for ( int i = 0; i < 3 ; i++)
  {
   std::cout   << "   0x"<< std::hex<< int (b[i]) <<"  ";
  }
  std::cout<<std::endl;*/

  if ( ( ret != 3 ) || ( ( int (b[0]) & 0xff ) != type) )
    return false;
 
    
  memcpy( &buf, &(b[1]), 2 );
  buf = (~buf) & 0xFFFF;
 //  std::cout << " buf == " << buf << std::endl;
  buf &= maskPin;
//   std::cout << " buf == " << buf << std::endl;
  if ( lastState != buf)
  {
    if ( ++countNewMsg > NEWMSG ) 
    {	
      setNewState(buf);
      countNewMsg = 0;
      /*
      std::cout << " ret == " << ret << " "<< getInfo() ;
      for ( int i = 0; i < 3 ; i++)
      {
      std::cout   << "   0x"<< std::hex<< int (b[i]) <<"  ";
      }
      std::cout<<std::endl;*/
  
    }
  } else 
      countNewMsg = 0;
  
  resetLastAnsw();
  return true;
};

void UiTcp::setFd(int _fd)
{
//  Log << getInfo() << " :: UiTcp::setFd fd == " << _fd;
  setUiOn( _fd != -1 );
  gettimeofday(&timeLastAsk, NULL);;
  timeLastAsk.tv_sec--;
   resetLastAnsw ();
   MutexLocker q(mtx);
   fd = _fd;
//  SKLib::TCPSocketInterface::setFd( _fd);
};
