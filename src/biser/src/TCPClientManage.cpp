#include "TCPClientManage.h"

#include "Log.h"

extern SKLib::Log log;

TCPClientManage::TCPClientManage():TCPManage()
{
//      std::cout << "Constructor TCPClientManage::TCPClientManage() !!!!! " << std::endl;
}

TCPClientManage::~TCPClientManage()
{

}

void TCPClientManage::start ()
{
  log.log() << " TCPManage::start   " ;
  IsWork = 1;
  while ( IsWork )
  { 
    for ( UiVec::iterator it = AllUi.begin() ; it != AllUi.end(); it++)//удаляем из вектора 
    {
       if (( !(*it)->isConnect() ) && connect ( *it ) )
       {
	 log.log() << " TCPManage::start  !!!!! OPEN ip ==  " << (*it)->getInfo() ;

       } 
    };
    usleep( 10000 );
  };
};

bool TCPClientManage::connect (tUI *ui)
{
  bool b = false;
  std::string command = std::string("bash -c 'ping -c 1 -w 1 ") + ui->getInfo() + std::string(" 2>&1>/dev/null '");
  int ret = system( command.c_str() );
  if ( !ret && ( ui->open( ui->getInfo() ) != -1 ) ) 
  {
    b = true;
  };
  return b;
};