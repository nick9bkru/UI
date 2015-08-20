#ifndef RECEIVECMD_H
#define RECEIVECMD_H

#include <recieveCMD/tcpReciever.h>

class ReceiveCmd : public tcpReciever
{
  public:
    ReceiveCmd();
    ~ReceiveCmd();
    virtual void recvMes();
};

#endif // RECEIVECMD_H
