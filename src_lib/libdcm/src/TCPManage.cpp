#include "TCPManage.h"
#include <algorithm>

using namespace UIservice ;

TCPManage::TCPManage() 
{ 
//    std::cout << "Constructor TCPManage::TCPManage() !!!!! " << std::endl;
   AllUi.clear();
   Log =  &SKLib::LogSingleton::Singleton::getInstance();
}

TCPManage::~TCPManage()
{

}

void TCPManage::addUi ( tUI * _UI)
{
  Log->log() << "TCPManage::addUi  !!!!! ip ==  " << _UI->getInfo();
  AllUi.push_back(_UI);
};



void TCPManage::stop ()
{
  Log->log() << " TCPManage::stop ()   " ;
  IsWork = 0;
}