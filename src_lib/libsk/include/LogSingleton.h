#ifndef LOGSINGLETON_H
#define LOGSINGLETON_H
#include "Log.h"

namespace SKLib
{
    class LogSingleton;

    template < typename S>
    class SingletonDestroyer
    {
      private:
	S* p_instance;
      public:    
	~SingletonDestroyer() 
	{
	  delete p_instance;
	};
	void initialize( S* p ) 
	{
	  p_instance = p;
	};
    };

    class LogSingleton
    {
      private:
	static SKLib::Log* p_instance;
	static SingletonDestroyer<SKLib::Log> destroyer;
	LogSingleton();
	LogSingleton(const LogSingleton& );
	~LogSingleton();
 	LogSingleton& operator=(const LogSingleton& ) ;
      public:
	static SKLib::Log& getInstance( std::string nameLog = "")
	{
	  if(!p_instance)     
	  {
	      p_instance = new SKLib::Log(nameLog);
	      destroyer.initialize( p_instance);     
	  }
	  return *p_instance;
	};
	
    };
}


#endif // LOGSINGLETON_H
