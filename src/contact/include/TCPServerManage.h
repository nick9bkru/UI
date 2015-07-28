#ifndef TCPSERVERMANAGE_H
#define TCPSERVERMANAGE_H

#include <TCPManage.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

/** @brief Класс следит за подключением к УИ настроенными в клиентов 
*/

class TCPServerManage :  public TCPManage
{
  enum {
    BACKLOG = 10     // как много может быть ожидающих соединений
  };
public:
  TCPServerManage(const std::string _port);
  ~TCPServerManage();
  virtual void start();
private:
  void connect( const int fd, const std::string ip);
  void init();
  int sockfd;
  std::string port;
};

#endif // TCPSERVERMANAGE_H
