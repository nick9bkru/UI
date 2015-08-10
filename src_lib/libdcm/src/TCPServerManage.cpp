#include "TCPServerManage.h"

#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>

#include "Log.h"

using namespace UIservice ;

extern SKLib::Log log;

TCPServerManage::TCPServerManage( const std::string _port ) : TCPManage(), port(_port) //port слушающий порт
{
  log.log() << "Constructor TCPServerManage::TCPServerManage() !!!!!  PORT ==" << port ;
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
      log.log() << " Connect new UI!!! === " << inet_ntoa (their_addr.sin_addr) << " !!!";
      connect( new_fd, std::string (inet_ntoa (their_addr.sin_addr)));
    };
//     std::cout << " !!! === " << strerror(errno) << " !!!" << std::endl;
   // sleep( 1) ;
  };
}

void TCPServerManage::init()
{/*
  struct sockaddr_in addr;
  int rc=1;
    int err,len;
     
 memset( &addr , 0, sizeof(addr) );
  addr.sin_family = AF_INET;
  addr.sin_port = htons (port);
  
  addr.sin_addr.s_addr = INADDR_ANY ; 

  
   if( -1 == ( sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) )) 
   {
    perror( "Socket can not created!\n");
    return ;
    }
 setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &rc, sizeof(rc) );
 // задаем сокету опцию SO_REUSEADDR
 // повторное использование локальных адресов для функц. bind() 

  // связываем адрес с дескриптором слушающего сокета
  if ( err = bind( sockfd, (struct sockaddr*) &addr, sizeof(addr) ) < 0 ) 
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
  }*/

    struct addrinfo hints, *servinfo, *p;
     int yes=1;
    int rv;
    
//     std::string PORT = "4001";
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
	throw (std::string ("TCPServerManage::init() == getaddrinfo: ")  + gai_strerror(rv));
         return;
    }

    // цикл через все результаты, чтобы забиндиться на первом возможном
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1)
	{
	    log.log() << "TCPServerManage::init() == server: socket";
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1)
	{
	    throw (std::string ( "TCPServerManage::init() == setsockopt :") + strerror(errno));
            return;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
	{
            close(sockfd);
            log.log() << "TCPServerManage::init()server: bind";
            continue;
        }

        break;
    }

    if (p == NULL)  
    {
      throw (std::string ( "TCPServerManage::init() == server: failed to bindn:")+ strerror(errno) );
      return;
    }

    freeaddrinfo(servinfo); // всё, что можно, с этой структурой мы сделали

    if (listen(sockfd, BACKLOG) == -1) 
    {
	throw (std::string ( "listen" ));
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
    log.log() << "TCPServerManage::connect don't find ip addr from BD";
    ::close(fd);
  };
};

