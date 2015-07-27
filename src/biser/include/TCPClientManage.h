#ifndef TCPCLIENTMANAGE_H
#define TCPCLIENTMANAGE_H

#include "TCPManage.h"

/** @brief Класс следит за подключением к УИ настроенными как TCP SERVER 
*/

class TCPClientManage :  public TCPManage
{
public:
  TCPClientManage();
  ~TCPClientManage();
  virtual void start () ;
private:
    bool connect (tUI *ui);
};

#endif // TCPCLIENTMANAGE_H
