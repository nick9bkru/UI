#include "BDmain.h"

BDmain::BDmain(std::string host, std::string namebd)
{
  std::cout<< "BDmain::BDmain ip ==" << host << " name =" << namebd  << std::endl;
  db = new Database ( host, namebd);
  if ( !db->isReady() )
  {
    delete db;
    throw ( std::string (" Don't open BD"));
  };
//   Log =  &SKLib::LogSingleton::Singleton::getInstance();
}

BDmain::~BDmain()
{
  std::cout<< "BDPthread::~BDPthread()" << std::endl;
  delete db;
}

   
bool BDmain::getInitAll( sa_info * foo )
{
  //std::ostringstream query;
  std::string query = "\
select F.NUM,F.KOD,S.NPORT,S.PORT,S.SENDBUF \
from SOST_SA_RPU S, SA_RPU F \
where S.NUM=F.NUM and S.NUM<=48 order by F.NUM;";

   return db->multipleSelect(query, foo, sizeof(struct sa_info));
}


bool BDmain::updkzn( const ushort param, const ushort number )
{
  std::ostringstream query ;
  query << "update kzn_dkl set dkl= " << param << "  where kzn= " << number << " ;" ;

   return db->notSelect(query.str());
}

bool BDmain::getNumSARPU( ushort * num,const ushort kzn ,const CHANGE_KEYS_ID type  )
{
  std::ostringstream query;

  query << "select NUM from SA_RPU where (KZN= " << kzn <<" ) and (LSMENA";
  switch (type)
  {
      case CHANGE_KEYS_WITHOUT_LSMENA:
	    query<<"<>1) and (KOD>0 and KOD<40) order by NUM;";
	    break;
      case CHANGE_KEYS_WITH_LSMENA:
	    query<<"=1) and (KOD>0 and KOD<40) order by NUM;";
	    break;
      default:
	 return false;
  };

   return db->multipleSelect(query.str(), num, sizeof(ushort));
}

bool BDmain::getNumSARPU( ushort * num,const ushort kzn   )
{
  std::ostringstream query;
  query << "select NUM from SA_RPU where KZN= " << kzn <<" and KOD=30 order by NUM;";
  return db->multipleSelect(query.str(), num, sizeof(ushort));
}