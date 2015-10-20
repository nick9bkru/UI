#include "UiTcpArp.h"
#include "BDPthread.h"

#include <sys/select.h>

UiTcpArp::UiTcpArp(std::string _ip, int _port) :  UIservice::UiTcp( _ip, _port ) , type ( 0 )
{
    setFd( -1 );
}

UiTcpArp::~UiTcpArp()
{
  close();
}

int UiTcpArp::close(  )
{
  std::cout << "UiTcpArp::close(  ) getInfo  " << getInfo()  << std::endl;
  if ( isConnect() )
   ::close(getFd());
   setFd(-1);
  return 1;
  
};


void UiTcpArp::setFd(int _fd)
{
std::cout << " void UiTcpArp::setFd(int _fd) == " << _fd << std::endl;
   MutexLocker q(mtx);
   SKLib::Singleton<BDPthread>::getInstance().setUistate( getInfo(), _fd != -1 );
  fd= _fd;
};

bool UiTcpArp::setState(unsigned char state)
{
  std::ostringstream querry;
  querry << "set" <<  char(state) << "\r";
// querry << "set1" << "\r";
  //std::cout << querry.str()  << std::endl;
  
  return (answAndGet ( querry.str() ) != -1 );
};

int UiTcpArp::getnotState()
{
  
  
  std::string cmd = "get\r";
  return answAndGet (cmd);

};

int UiTcpArp::gtsState()
{
  std::string cmd = "gts\r";
  return  answAndGet (cmd, false );
};

int UiTcpArp::answAndGet( const std::string cmd, bool Inv)
{
  int buf;
  char b [3];

//    std:: cout << "isConnect =  " << isConnect() << " ip = " <<getInfo()  << std:: endl;

  if ( ! isConnect() )
    return -1;
//std::cout << cmd << std::endl;  
  if ( (onlySend (cmd)  != int (cmd.length())) &&  (onlySend (cmd)  != int (cmd.length())) )
  {
     close();
    std:: cout << "send  error " << std:: endl;
    return -1;
  }            
  
  if ( ! isRecv()  ) // если через секунду нет ничего на сокете, то закрываем его 
  {
      std:: cout << " recv " << std:: endl;
    close();
    return -1;
  };
  int ret = 0;
  {
    MutexLocker q(mtx);
    ret= onlyRead ( b , 3);
  }
  if ( (ret < 0) )
  {
     std:: cout << "Read  ret ==  " << ret << " str == " << cmd  << std:: endl;
//     close();
    return -1;
  }

//  std:: cout << "Read  ret ==  " << ret << " str == " << cmd  << std:: endl;

  /*
   std::cout << " ret == " << ret << " "<< getInfo() ;
  for ( int i = 0; i < 3 ; i++)
  {
   std::cout   << "   0x"<< std::hex<< int (b[i]) <<"  ";
  }
  std::cout<<std::endl;*/

  if ( ( ret != 2 ) || ( ( int (b[0]) & 0xff ) != type) )
    return -1;
    
  memcpy( &buf, &(b[1]), 1 );
  if ( Inv )
    buf = (~buf);
  buf &= 0xFF;
//   std::cout<< "IP == " << getInfo() << " cmd = " << cmd  <<" buf == " << buf  <<std::endl;
  return buf ;
};

bool UiTcpArp::isRecv()
{
 fd_set rfds;
 struct timeval tv;
 int retval = -1 ;

 FD_ZERO(&rfds);
    /* Ждем не больше секунд. */
 tv.tv_sec = 5;
 tv.tv_usec = 0; 
 FD_SET( getFd() , &rfds);
 retval = select( getFd() + 1, &rfds, NULL, NULL, &tv);
 /* Не полагаемся на значение tv! */
 if ( retval < 0)
	    throw errno;
// 	std::cout << " retval ==   " << retval  << std::endl;
 return ( retval && FD_ISSET( getFd(), &rfds ) );
}

