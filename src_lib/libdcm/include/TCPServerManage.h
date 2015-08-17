#ifndef TCPSERVERMANAGE_H
#define TCPSERVERMANAGE_H

#include <TCPManage.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

namespace UIservice
{
    
  /** @brief Класс следит за подключением к УИ настроенными в клиентов 
  */

  class TCPServerManage :  public TCPManage
  {
    enum {
      BACKLOG = 10     // как много может быть ожидающих соединений
    };
  public:
    TCPServerManage(const int _port);
    ~TCPServerManage();
    virtual void start();
  private:
    void connect( const int fd, const std::string ip);
    void init();
    int sockfd;
    int port;
  };

}
#endif // TCPSERVERMANAGE_H
