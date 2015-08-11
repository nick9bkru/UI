#include "DeviceInterface.h"

using namespace SKLib;

extern Log log;

DeviceInterface::DeviceInterface(const std::string & _device)
     : DataInterface(), device(_device)
{
}

DeviceInterface::~DeviceInterface()
{
     this->close();
     log->log("~DeviceInterface()");
}
     
int DeviceInterface::open()
{
     // To prevent long time while opening device,
     // at beginning try to open in NONBLOCK-mode.
     int flags = O_RDWR | O_NOCTTY | O_NONBLOCK;

     int fdes = ::open(device.c_str(), flags);

     if (fdes == -1)
     {
	  setErrMsg("open() with O_NONBLOCK");
	  return -1;
     }

     // Device was opened successfully. Try to open with working flags.
     ::close(fdes);     
     flags &= ~O_NONBLOCK;
     fdes = ::open(device.c_str(), flags);
     if (fdes == -1)
     {
	  setErrMsg("open()");
	  return -1;
     }
     
     struct termios options;
     options.c_cflag = B115200 | CREAD | CLOCAL | CS8;
     options.c_iflag = 0;
     options.c_oflag = 0;
     options.c_lflag = ~(ICANON | ECHO | ISIG);
     options.c_cc[VMIN] = 0;
     options.c_cc[VTIME] = 1;

     tcsetattr(fdes, TCSAFLUSH, &options);

     setFd(fdes);
     
     return fdes;
}

int DeviceInterface::getAnswerForCommand(const std::string & cmdString, void *answer, int size)
{
     if (getFd() < 0)
	  return -2;
     
     int ret = write(getFd(), cmdString.c_str(), cmdString.size());

     if (ret == -1)
     {
     	  setErrMsg("write()");
	  return -1;
     }

     usleep(getTimeout());

     ret = read(getFd(), answer, size);
     if (ret == -1)
     {
     	  setErrMsg("read()");
	  return -1;
     }

     return ret;
}

std::string DeviceInterface::getInfo() const
{
     return ("\"" + device + "\"");
}

int DeviceInterface::i7k_send_readt(int fd, const char * const cmd, char * const rbuf,
				    int rmax, __tod_t * const texp)
{
	char c;
	char *p;
	unsigned long limit;
	__tod_t t_start;
	int r;

	t_start = _time_get_tod();
	p = rbuf;
	limit = (unsigned long) p + rmax;

	if (write(fd, cmd, strlen(cmd)) == -1)
	{
	     *texp = _time_get_tod() - t_start;
	     return (-1);
	}

	while ((unsigned long) p < limit)
	{
	     r = read(fd, &c, sizeof(c));
	     if ((r == -1) || (r == 0))
	     {
		  *texp = _time_get_tod() - t_start;
		  return (r);
	     }

	     *p++ = c;

	     if (c == 13) // ASCII_CR
		  break;
	}
	*p = 0;
	r = p - rbuf;

	*texp = _time_get_tod() - t_start;
	return (r);
}
