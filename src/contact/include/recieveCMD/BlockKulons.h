#ifndef BLOCKKULONS_H
#define BLOCKKULONS_H

#include <recieveCMD/tcpReciever.h>

class BlockKulons : public  tcpReciever
{
  public:
    BlockKulons();
    ~BlockKulons();
    virtual void recvMes();
};

#endif // BLOCKKULONS_H
