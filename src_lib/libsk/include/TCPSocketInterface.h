#ifndef TCPSOCKETINTERFACE_H
#define TCPSOCKETINTERFACE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <string>

#include "DataInterface.h"

using namespace std;

namespace SKLib
{
     class TCPSocketInterface : public DataInterface
	  {
	  public:
	       TCPSocketInterface( int _port);
	       virtual ~TCPSocketInterface();
	       
	       int open(const std::string & ipAddr);
	       int getAnswerForCommand(const std::string & cmdString, void *answer, int size);
	       std::string getInfo() const;
	       int onlySend(const std::string & cmdString);
	       int onlyRead(void *answer, int size);
	       int getAnswerForCommand_new(const std::string & cmdString, void *answer, int size);

	  private:
	      
	       int port;
	  };
}

#endif // TCPSOCKETINTERFACE_H
