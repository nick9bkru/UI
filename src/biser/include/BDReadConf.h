#ifndef BDREADCONF_H
#define BDREADCONF_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "Database.h"
#include "defines.h"

//класс получает конфиги из БД
class BDReadConf
{
public:
  typedef std::vector< std::string > vecIp;
  typedef std::vector< int8_t > vecCont;
  typedef std::map< int8_t, int> mapCont;
  
  BDReadConf(Database *_db);
  ~BDReadConf();
  
  //получаем адреса всех УИ с типом
  vecIp getUiIp( int type );
  //получаем получаем используемые контакты numIU из таблицы NSD
  int getCont( int8_t numUi);
  //получаем получаем используемые контакты для всех UI из таблицы NSD
  mapCont getCont( );
private:
  Database *db;
};

#endif // BDREADCONF_H
