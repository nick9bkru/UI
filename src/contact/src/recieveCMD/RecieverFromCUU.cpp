#include "recieveCMD/RecieverFromCUU.h"

#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <algorithm>

RecieverFromCUU::RecieverFromCUU()
{
 tcpRec.push_back( new ReceiveCmd() );
 tcpRec.push_back( new ChangeKeyClass() );
 tcpRec.push_back( new BlockKulons() );
}

RecieverFromCUU::~RecieverFromCUU()
{
  for ( VecRecv::iterator it = tcpRec.begin() ; it != tcpRec.end() ; it ++)
  {
    delete (*it);
  };
}


void RecieverFromCUU::start ()
{
  fd_set rfds;
  struct timeval tv = {1 , 0};
  int retval = -1 ;
    int max = 0;
  while ( 1 )
  {
    FD_ZERO(&rfds);
    for ( VecRecv::iterator it = tcpRec.begin() ; it != tcpRec.end() ; it ++)
    {
      max = std::max( max , (*it)->getSock() );
      FD_SET( (*it)->getSock() , &rfds);
    }
    retval = select(max + 1, &rfds, NULL, NULL, &tv);
    if ( retval < 1 )
      continue;
    for ( VecRecv::iterator it = tcpRec.begin() ; it != tcpRec.end() ; it ++)
    {
      if (  FD_ISSET( (*it)->getSock(), &rfds ) )
      {
	(*it)->recvMes();
      };
    }
    
  };
  
};