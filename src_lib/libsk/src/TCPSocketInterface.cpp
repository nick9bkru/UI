#include "TCPSocketInterface.h"

using namespace SKLib;

extern Log log;

TCPSocketInterface::TCPSocketInterface( int _port)
     : DataInterface(),  port(_port)
{
}

TCPSocketInterface::~TCPSocketInterface()
{
     log.log("~TCPSocketInterface()");
     shutdown(getFd(), 2);
}
     
int TCPSocketInterface::open(const std::string & ipAddr)
{
     struct sockaddr_in dest;

     dest.sin_family = AF_INET;
     dest.sin_port = htons(port);
     dest.sin_addr.s_addr = inet_addr(ipAddr.c_str());
     
     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock == -1)
     {
	  setErrMsg("socket");
	  return sock;
     }
     
     struct linger lng = {0, 0};
     int flag = 1;
     
     setsockopt(sock, SOL_SOCKET, SO_LINGER, static_cast<void *>(&lng), sizeof(struct linger));
     setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&flag), sizeof (int));
     
     int ret = connect(sock, reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
     if (ret == -1)
     {
	  setErrMsg("connect()");
	  ::close(sock);
	  return -1;
     }

     log.log( ipAddr + getInfo() + " connected");
     
     setFd(sock);
     
     return sock;
}

int TCPSocketInterface::getAnswerForCommand(const std::string & cmdString, void *answer, int size)
{
     if ( onlySend ( cmdString ) < 0 )
	  return -2;

    usleep(getTimeout());

    return onlyRead ( answer, size );
}

int TCPSocketInterface::getAnswerForCommand_new(const std::string & cmdString, void *answer, int size)
{
     if (getFd() < 0)
	  return -2;

     // Clear socket buffer.
     int sz;
     ioctl(getFd(), SIOCINQ, &sz);
     if (sz > 20)
     {
	  char buf[sz];
	  recv(getFd(), &buf, sz, MSG_DONTWAIT);
//	  log.log("sz = " + LexicalCaster(sz));
     }

     // Send message.
     int ret;
     for( int i=0; i<3; i++)
     { ret = send(getFd(), cmdString.c_str(), cmdString.size(), 0);
       if (ret == -1)
       {
     	  setErrMsg("send()");

	  if (errno == EPIPE)
	  {
	       log.log("EPIPE in send()");
	       this->close();
	  }
	  
	  return -1;
       }
       usleep(getTimeout());
     } 
     // Receive answer.
     ret = recv(getFd(), answer, size, MSG_DONTWAIT);
     if (ret == -1)
     {
	  setErrMsg("recv()");

	  if (errno == EPIPE)
	  {
	       log.log("EPIPE in recv()");
	       this->close();
	  }
	  
	  return -1;
     }

     return ret;
}



std::string TCPSocketInterface::getInfo() const
{
     std::ostringstream os;

     os << "\""  << port << "\"";
     return os.str();
}

int TCPSocketInterface::onlySend(const std::string & cmdString)
{
  if (getFd() < 0)
	  return -2;

     // Clear socket buffer.
     int sz;
     ioctl(getFd(), SIOCINQ, &sz);
     if (sz > 20)
     {
	  char buf[sz];
	  recv(getFd(), &buf, sz, MSG_DONTWAIT);
//	  log.log("sz = " + LexicalCaster(sz));
     }

     // Send message.
     int ret = send(getFd(), cmdString.c_str(), cmdString.size(), 0);
     if (ret == -1)
     {
     	  setErrMsg("send()");

	  if (errno == EPIPE)
	  {
	       log.log("EPIPE in send()");
	       this->close();
	  }
	  
	  return -1;
     }
};

int TCPSocketInterface::onlyRead(void *answer, int size)
{
  // Receive answer.
     int ret = recv(getFd(), answer, size, MSG_DONTWAIT);
     if (ret == -1)
     {
	  setErrMsg("recv()");

	  if (errno == EPIPE)
	  {
	       log.log("EPIPE in recv()");
	       this->close();
	  }
	  
	  return -1;
     }

     return ret;
};
