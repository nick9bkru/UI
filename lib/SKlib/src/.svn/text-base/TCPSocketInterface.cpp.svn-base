#include "TCPSocketInterface.h"

using namespace SKLib;

extern Log log;

TCPSocketInterface::TCPSocketInterface(const std::string & _ipAddr, int _port)
     : DataInterface(), ipAddr(_ipAddr), port(_port)
{
}

TCPSocketInterface::~TCPSocketInterface()
{
     log.log("~TCPSocketInterface()");
     shutdown(getFd(), 2);
}
     
int TCPSocketInterface::open()
{
     struct sockaddr_in my, dest;

     std::ostringstream os;
     os << "192.168.1." << getMyHostname();

     my.sin_family = AF_INET;
     my.sin_port = htons(port);
     my.sin_addr.s_addr = inet_addr(os.str().c_str());

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

     log.log(getInfo() + " connected");
     
     setFd(sock);
     
     return sock;
}

int TCPSocketInterface::getAnswerForCommand(const std::string & cmdString, void *answer, int size)
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

     usleep(getTimeout());

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

     os << "\"" << ipAddr << ":" << port << "\"";
     return os.str();
}
