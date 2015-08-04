#include "UiTcp.h"

using namespace UIservice ;

UiTcp::UiTcp(std::string _ip, int _port) :  SKLib::TCPSocketInterface(_port), ip(_ip)
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

int UiTcp::open( const std::string & ipAddr )
{
  (void)ipAddr;
  return open(  );
  
};

int UiTcp::close(  )
{
  return SKLib::TCPSocketInterface::close();
};

int UiTcp::open(  )
{
  return ( SKLib::TCPSocketInterface::open( ip ) );
  
};


void UiTcp::setFd(int _fd)
{
  SKLib::TCPSocketInterface::setFd( _fd);
};
