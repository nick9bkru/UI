#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <termios.h>
#include <fcntl.h>

#include "DataInterface.h"

namespace SKLib
{
     class DeviceInterface : public DataInterface
	  {
	  public:
	       DeviceInterface(const std::string & _device);
	       virtual ~DeviceInterface();
	       
	       int open();
	       int getAnswerForCommand(const std::string & cmdString, void *answer, int size);
	       std::string getInfo() const;
	       
	  private:
	       std::string device;
	       int i7k_send_readt(int fd, const char * const cmd, char * const rbuf,
				  int rmax, __tod_t * const texp);
	  };
}

#endif // DEVICEINTERFACE_H
