#include "TDcmInteface.h"


TDcmInteface::TDcmInteface (std::string _ipAddr, unsigned short int _port):TDcmCli (_ipAddr,  _port),
         ipAddr(_ipAddr)
{
  log.log() << " _ipAddr "<< _ipAddr << " port" << _port;
  init();
  /*
  if (connect() == -1)
  {
     log.log()<< "TDcmInteface::TDcmInteface Error connect";
  };
  */
};

TDcmInteface::~TDcmInteface ()
{
  log.log()<<"~TDcmInteface ()  ipAddr "<< ipAddr;
} ;

int TDcmInteface::open(const std::string & ipAddr) 
{
  UNUSED(ipAddr);
  return 0;
  
} ;

std::string TDcmInteface::getInfo()  const
{ 
  return "ipAddr = " + ipAddr;
  
};

int TDcmInteface::getAnswerForCommand(const std::string & cmdString, void *answer, int size)
{
  UNUSED(cmdString); UNUSED(size);  UNUSED(answer);
  return 0;
};
