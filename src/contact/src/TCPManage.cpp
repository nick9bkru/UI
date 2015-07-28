#include "TCPManage.h"
#include <algorithm>

TCPManage::TCPManage() 
{
//    std::cout << "Constructor TCPManage::TCPManage() !!!!! " << std::endl;
   AllUi.clear();
}

TCPManage::~TCPManage()
{

}

void TCPManage::addUi ( tUI * _UI)
{
  std::cout << "TCPManage::addUi  !!!!! ip ==  " << _UI->getInfo()<< std::endl;
  AllUi.push_back(_UI);
};



void TCPManage::stop ()
{
  std::cout << " TCPManage::stop ()   "  << std::endl;
  IsWork = 0;
}