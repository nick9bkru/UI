#ifndef __UITCP_H
#define __UITCP_H

#include "TCPSocketInterface.h"

namespace UIservice
{

  /** @brief Класс для работы с УИ 
  */
  class UiTcp : public SKLib::TCPSocketInterface
  {
      public:
	/** enum typeUI
	\brief типы УИ используемые в комплексе для бд
      */
	enum typeUIBD
	{
	  UINSD = 0,
	  UIUPS,
	  UIARPU
	};
	
	UiTcp(std::string _ip, int _port);
	virtual ~UiTcp();
	virtual std::string getInfo() const;
	int open(const std::string & ipAddr); // просто убираем из доступа ipAddr не используется!!
	virtual int open();	
	virtual int close(  );
	
	virtual void setFd(int _fd);
      private:
	std::string ip;
  };
}
#endif // __UITCP_H
