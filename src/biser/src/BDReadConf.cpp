#include "BDReadConf.h"

BDReadConf::BDReadConf(Database *_db): db(_db)
{
  
}

BDReadConf::~BDReadConf()
{

}

BDReadConf::vecIp BDReadConf::getUiIp( int type )
{
  std::ostringstream querry;
  querry<< "select ip from ui_ip where type = " << int( type )<< ";";
  vecIp ret;
  const int8_t size = 20;
  char str[ size ];
  
  
  while ( db->multipleSelect (  querry.str(),  str, size ))
  {
    //std::cout << std::string ( str)<< "!"<<std::endl; 
    ret.push_back(std::string (str) );
  };
  return ret;
};

int BDReadConf::getCont( int8_t numUi)
{
  std::ostringstream querry;
  querry<< "select ui_contact from nsd where ui =  " << int(numUi) << " order by ui_contact;";
  int ret = 0;
  int cont;
  
  
  while ( db->multipleSelect (  querry.str(),  &cont, sizeof(cont) ))
  {
    ret |= 1 << cont;
    //std::cout << "ret = " << ret << std::endl;
  };
  return ret;
};

BDReadConf::mapCont BDReadConf::getCont( )
{
    std::string querry;
  querry = "select ui, ui_contact from nsd  order by ui_contact ;";
  mapCont ret;
  int cont[2];
  
  //db -> setDebugMode( 1 );
  while ( db->multipleSelect (  querry,  &cont, sizeof(cont) ))
  {
  //  std::cout << "cont.num = " << cont[0]<< " ! "<<"cont = " << cont[1]  <<std::endl;
    
    ret[ cont[0] ] |= 1<<cont[1];
  };
  return ret;
};
