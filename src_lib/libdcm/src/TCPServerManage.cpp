#include "TCPServerManage.h"

#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>


using namespace UIservice ;


TCPServerManage::TCPServerManage( const int _port ) : TCPManage(), port(_port) //port слушающий порт
{
  Log->log() << "Constructor TCPServerManage::TCPServerManage() !!!!!  PORT ==" << port ;
  init();
}

TCPServerManage::~TCPServerManage()
{
  close(sockfd);
}

void TCPServerManage::start()
{
   struct sockaddr_in their_addr; // информация об адресе клиента
  int  new_fd; 
  socklen_t sin_size = sizeof(their_addr);

  fd_set rfds;
  struct timeval tv = {1 , 0};
  int retval = -1 ;
    
  while ( IsWork )
  {
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);    
    retval = select(sockfd + 1, &rfds, NULL, NULL, &tv);
    if (! ( retval &&  FD_ISSET( sockfd, &rfds )) )
    {
      continue;
    };
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if ( new_fd != -1)
    {
      Log->log() << " Connect new UI!!! === " << inet_ntoa (their_addr.sin_addr) << " !!!";      Log->log() << " Connect new UI!!! === " << inet_ntoa (their_addr.sin_addr) << " !!!";
      connect( new_fd, std::string (inet_ntoa (their_addr.sin_addr)));
    };
//     std::cout << " !!! === " << strerror(errno) << " !!!" << std::endl;
   // sleep( 1) ;
  };
}

void TCPServerManage::init()
{
  struct sockaddr_in addr;
  int rc=1;
    int err;
//int port = 4001;     
 memset( &addr , 0, sizeof(addr) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons ( port );
  
  addr.sin_addr.s_addr = INADDR_ANY ; 

  
   if( -1 == ( sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) )) 
   {
    perror( "Socket can not created!\n");
    return ;
    };
 setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &rc, sizeof(rc) );
 // задаем сокету опцию SO_REUSEADDR
 // повторное использование локальных адресов для функц. bind() 

  // связываем адрес с дескриптором слушающего сокета
  if ( (err = bind( sockfd, (struct sockaddr*) &addr, sizeof(addr) )) < 0 ) 
  {
  close(sockfd);
  perror( "bind error! " );
  return ;
  }
  // начинаем слушать сокет
  if ( listen( sockfd, 25) < 0) 
  { 
    close(sockfd);
    perror( "listen error!\n");
    return ;
  }

}

void TCPServerManage::connect( const int fd, const std::string ip)
{
  UiVec::iterator it;
  for (it = AllUi.begin(); it!= AllUi.end(); it ++ )
  {
    if ( ( *it )->getInfo () == ip )
    {
     if ( (*it)->isConnect() ) 
     {
       (*it)->close();
     }
     ( *it)->setFd( fd );
     return;
    }
  };
  //если не нашли такого УИ то закрываем сокет
  if ( it == AllUi.end( ))
  {
    Log->log() << "TCPServerManage::connect don't find ip addr from BD";
    ::close(fd);
  };
};

