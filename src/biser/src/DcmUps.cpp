#include "DcmUps.h"

#include "Log.h"


DcmUps::DcmUps( std::string _ip, int _port,BDStateUps* _BDst ) : UiTcp (_ip,  _port), BDst(_BDst)
{
  Log->log()  << "Constructor DcmUps::DcmUps _ip==  " << _ip << " port ==" << _port ;
  setLastState( );
  type = typeUI;
setUiOn ( false ); 
}

DcmUps::~DcmUps()
{

}

void DcmUps::setNewState ( int state )
{
  Log->log() << " NEW state to ip == " << getInfo()<< " state == " << std::hex << "0x"<< state  << std::dec ;
  lastState ^= state; 
  for ( std::vector<int>::iterator it = usePin.begin(); it!=usePin.end() ; it++)
  {
    if ( ( lastState >>(*it) ) & 0x07 )
    {
      Log->log() << " NEW state to ip == " << getInfo()<< " cont == "<< (*it) << " state ==" << (( state >> (*it)) & 0x07) ;
      BDst -> setUpsState( getInfo(), (*it), ( state >> *it) & 0x07 );
    }
  };
  lastState= state;
};

void DcmUps::setLastState( )
{
  usePin.clear();
  const int max = 16 ;
  maskPin = 0;
  lastState = 0;
  int pin;
  for ( int i = 0; i < max ; i++)
  {
    pin = BDst -> getUpsState( getInfo(), i );
    if ( pin != -1)
    {
      usePin.push_back( i );
      ///pin &= 1;
      maskPin |= 0x7 << i ;
      lastState |= pin << i ;
    }
  };
  /*
  int max = BDst->getNSDmaxCon( getInfo() );
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
  };*/
};

void DcmUps::setUiOn ( bool on )
{
  BDst->setSostUi(getInfo(), on);
};
