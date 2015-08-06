#include "LogSingleton.h"

using namespace SKLib;

SKLib::Log& LogSingleton::getInstance( std::string nameLog )
{
  if(!p_instance)     
  {
      p_instance = new SKLib::Log(nameLog);
      destroyer.initialize( p_instance);     
  }
  return *p_instance;
};