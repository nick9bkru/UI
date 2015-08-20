#ifndef RECIEVERFROMCUU_H
#define RECIEVERFROMCUU_H

#include <vector>
#include <recieveCMD/ChangeKeyClass.h>
#include <recieveCMD/ReceiveCmd.h>
#include <recieveCMD/BlockKulons.h>

class RecieverFromCUU
{
  public:
    RecieverFromCUU();
    ~RecieverFromCUU();
    void start ();
private: 
  typedef std::vector < tcpReciever *> VecRecv;
  VecRecv tcpRec ;
};

#endif // RECIEVERFROMCUU_H
