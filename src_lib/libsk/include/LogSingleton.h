#ifndef LOGSINGLETON_H
#define LOGSINGLETON_H

#include "Log.h"
#include "Singleton.h"

namespace SKLib
{
    class LogSingleton : public Singleton <SKLib::Log>
    {
      public:
	static SKLib::Log& getInstance( std::string nameLog = "" );
      private:
	LogSingleton();
	LogSingleton(const LogSingleton& );
	~LogSingleton();
 	LogSingleton& operator=(const LogSingleton& ) ;
    };
}


#endif // LOGSINGLETON_H
