#include "UiTcp.h"

UiTcp::UiTcp(std::string _ip, int _port) :  SKLib::TCPSocketInterface(_port), type ( 0 ), ip(_ip)
{
 SKLib::TCPSocketInterface::setFd( -1 );
}

UiTcp::~UiTcp()
{
  close();
}

std::string UiTcp::getInfo() const
{
  return ip;
};

int UiTcp::close(  )
{
  std::cout << "UiTcp::close(  ) getInfo  " << ip  << std::endl;
  MutexLocker q(mtx);
  return SKLib::TCPSocketInterface::close();
  
};


void UiTcp::setFd(int _fd)
{
   MutexLocker q(mtx);
  SKLib::TCPSocketInterface::setFd( _fd);
};

bool UiTcp::setState(unsigned char state)
{
  std::ostringstream querry;
  querry << "set" <<  state << "\t";
  return (answAndGet ( querry.str() ) != -1 );
};

int UiTcp::getnotState()
{
  
  
  std::string cmd = "get\r";
  return answAndGet (cmd);

};

int UiTcp::gtsState()
{
  std::string cmd = "gts\r";
  return  answAndGet (cmd);
};

int UiTcp::answAndGet( const std::string cmd)
{
  int buf;
  char b [3];
  if ( ! isConnect() || ( onlySend (cmd)  == int (cmd.length()) ) )
    return -1;
    
  usleep( 100000 );
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

  if ( ( ret != 2 ) || ( ( int (b[0]) & 0xff ) != type) )
    return -1;
 
    
  memcpy( &buf, &(b[1]), 1 );
  buf = (~buf) & 0xFF;
  return buf ;
};