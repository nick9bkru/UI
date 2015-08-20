#ifndef BDMAIN_H
#define BDMAIN_H

#include "LogSingleton.h"
#include "Database.h"
#include "defines.h"

class BDmain
{
public:
  BDmain( );
  BDmain(std::string host, std::string namebd);
  ~BDmain();
  bool getInitAll( sa_info * foo );
  bool updkzn( const ushort param, const ushort number );
  bool getNumSARPU( ushort * num,const ushort kzn ,const CHANGE_KEYS_ID type  );
  bool getNumSARPU( ushort * num,const ushort kzn   );
private:
  SKLib::Log *Log ;
  Database *db;
};

#endif // BDMAIN_H
