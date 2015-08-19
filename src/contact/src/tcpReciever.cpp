#include "tcpReciever.h"

tcpReciever::tcpReciever( const int port )
{
  sockfd = make_socket( port );
}

tcpReciever::~tcpReciever()
{

}

// Создание сокета.
int tcpReciever::make_socket(int port)
{
     int sock;
     struct sockaddr_in name;
     
     // Create the socket
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
	  throw ( std::string ( " tcpReciever::make_socket soket ") + std::string(strerror(errno))  );
     }
     
     // Give the socket a name
     name.sin_family = AF_INET;
     name.sin_port = htons(port);
     name.sin_addr.s_addr = htonl(INADDR_ANY);
     if (bind(sock, reinterpret_cast<struct sockaddr *>(&name), sizeof(name)) < 0)
     {
	  throw ( std::string ( " tcpReciever::make_socket bind " ) + std::string(strerror(errno))  );
	
     }
     if (listen(sockfd, 50) == -1)
      {
	    throw ( std::string ( " tcpReciever::recvMes listen(): " ) + std::string(strerror(errno))  );
	    sock = -1;
      };
     return sock;
}

int tcpReciever::recvMes( char * msg, const int len)
{
  struct timeval tv;
  int sock_new ;

  sock_new = accept(sockfd, NULL, NULL);
  if (sock_new == -1)
  {
	throw ( std::string ( " tcpReciever::recvMes accept(): " ) + std::string(strerror(errno))  );
	return -1;
  };
  fd_set rfds;
    
  FD_ZERO(&rfds);
  FD_SET( sock_new , &rfds);
  /* Ждем не больше пяти секунд. */
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  int ret = select(sock_new + 1, &rfds, NULL, NULL, &tv);
  if ( ret < 0)
    throw ( std::string ( " tcpReciever::recvMes select(): " ) + std::string(strerror(errno))  ); 
  if ( ret && FD_ISSET( sock_new, &rfds ))
  {
    ret = recv(sock_new, &msg, len, 0);
  };
  close( sock_new );
  return ret;
};

int tcpReciever::getSock( ) const
{
  return sockfd;
};
