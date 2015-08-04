#include "DcmNsd.h"

#include "Log.h"

extern SKLib::Log log;

DcmNsd::DcmNsd(std::string _ip, int _port , BDStateNsd* _BDst): UiTcp (_ip,  _port), BDst(_BDst)
{
  log.log() << "Constructor DcmNsd::DcmNsd _ip==  " << _ip << " port ==" << _port ;
  setLastState( );
  log.log() << "Constructor DcmNsd::DcmNsd maskPin==  " << maskPin << " lastState ==" << lastState ;
  
  type = typeUI;
}

DcmNsd::~DcmNsd()
{
  
}

/*
void DcmNsd::setMask( int mask )
{
  maskPin = mask;
};*/

void DcmNsd::setNewState ( int state )
{
//   std::cout << " NEW state to ip == " << getInfo()<< " state == " << std::hex << "0x"<< state  << std::dec << std::endl;
  //std::cout << " count == "<< countNewMsg << std::endl;
  lastState ^= state; 
  for ( int i = 0; i< 16; i++)
  {
    if ( lastState & 0x01 )
    {
      log.log()<< " DcmNsd::setNewState to ip == " << getInfo()<< " cont == "<< i << " state ==" << (( state >> i) & 0x01) ;
      BDst -> setNsdState( getInfo(), i, ( state >> i) & 0x01 );
    }
    lastState = lastState >> 1; 
    if ( ! lastState ) 
    {
     break; 
    }
  };
  lastState= state;
};

void DcmNsd::setLastState( )
{
  int max = BDst->getNSDmaxCon( getInfo() ) + 1;
  maskPin = 0;
  lastState = 0;
  int pin;
  for ( int i = 0; i < max ; i++)
  {
    pin = BDst -> getNSDstate( getInfo(), i );
    if ( pin != -1)
    {
      pin &= 1;
      maskPin |= 1 << i ;
      lastState |= pin << i ;
    }
  };
};

void DcmNsd::setUiOn ( bool on )
{
  BDst->setSostUi(getInfo(), on);
};
