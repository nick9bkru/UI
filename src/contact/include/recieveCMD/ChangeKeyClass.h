#ifndef CHANGEKEYCLASS_H
#define CHANGEKEYCLASS_H

#include <recieveCMD/tcpReciever.h>

class ChangeKeyClass :  public tcpReciever
{
public:
  ChangeKeyClass();
  ~ChangeKeyClass();
  virtual void recvMes();
};

#endif // CHANGEKEYCLASS_H
