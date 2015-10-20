#include "TDcmCli.h"

   
TDcmCli::TDcmCli( std::string _ipAddr, unsigned short int _port ):TDryContactModule(_ipAddr, _port)
{
};

TDcmCli::~TDcmCli()
{
};

void TDcmCli::initUsedStates()
{
	usedStates = 9;

	//cout << "TDcmCli usedStates === " << usedStates << endl;
};
void TDcmCli::endProcessDcmState()
{
//       dcmState = state;
};
int TDcmCli::getnotState()
{
  int buf= (static_cast<int>(math::pow(2.0,usedStates-1)) - 1);
  int s=getState();
//cout<<"TDcmCli::getnotState() = "<<s<<endl;
  return s==-1 ? -1 : ((s ^ buf) & buf);
}

int TDcmCli::gtsState_()
{
  int buf= (static_cast<int>(math::pow(2.0,usedStates-1)) - 1);
  int s = gtsState();
  // cout<< "TDcmCli::gtsState_() = " << s << endl;
//  cout<<"!!!!!! "<<(buf & (static_cast<int>(pow(2.0,usedStates-1)) - 1))<<endl;
  return  s == -1 ? -1 : (buf & s) ;
}

void TDcmCli::processState( int statePos, bool state )
{
      
	std::cout << "Позиция №" <<  statePos  <<  "         изменилась на " <<   state << std::endl;
};

std::string TDcmCli::getIP() const
{
  return ipAddr;
};

