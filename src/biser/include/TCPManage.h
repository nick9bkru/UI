#ifndef TCPMANAGE_H
#define TCPMANAGE_H

#include <pthread.h>
#include <vector>

#include "UiTcp.h"


/** @brief Класс следит за подключением к УИ 
*/

class TCPManage
{
public:
  typedef UiTcp tUI;
  typedef std::vector < tUI *> UiVec ; 

  TCPManage();
  ~TCPManage();
  virtual void start () = 0;
  void stop ();
  void addUi (tUI * _UI);
protected:
  bool IsWork;
   UiVec AllUi;
};

#endif // TCPMANAGE_H
