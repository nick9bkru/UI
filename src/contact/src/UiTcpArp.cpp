#include "UiTcpArp.h"
#include "BDPthread.h"

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
  MutexLocker q(mtx);
  return SKLib::TCPSocketInterface::close();
  
};


void UiTcpArp::setFd(int _fd)
{
std::cout << " void UiTcpArp::setFd(int _fd) == " << _fd << std::endl;
   MutexLocker q(mtx);
   timeNoRead = time ( NULL );
   SKLib::Singleton<BDPthread>::getInstance().setUistate( getInfo(), _fd != -1 );
  SKLib::TCPSocketInterface::setFd( _fd);
};

bool UiTcpArp::setState(unsigned char state)
{
  std::ostringstream querry;
  querry << "set" <<  int(state) << "\r";
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
//    std:: cout << getInfo() <<isConnect() << " int (cmd.length()) == " << int (cmd.length()) << std:: endl;
  if ( ! isConnect() )
    return -1;
  
  if ( onlySend (cmd)  != int (cmd.length()) )
  {
     close();
    std:: cout << "send  error " << std:: endl;
    return -1;
  }            
  usleep( 200000 );
  int ret = 0;
  {
    MutexLocker q(mtx);
    ret= onlyRead ( b , 3);
  }
  if ( (ret < 0) )
  {
    if ( (time (NULL) - timeNoRead) > 10 )
    {
     std:: cout << "Read  ret ==  " << ret << " str == " << cmd  << std:: endl;
     close();
    }
    return -1;
  }

//  std:: cout << "Read  ret ==  " << ret << " str == " << cmd  << std:: endl;

  timeNoRead = time (NULL); 
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
