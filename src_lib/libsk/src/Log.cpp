/**
   \file log.cpp
   \brief Реализация класса Log.
   \author Кориков Сергей, Жадовский
   \version 0.2
   \date 2014-02-28
*/

#include <unistd.h>
#include "Log.h"

using namespace SKLib;
message::~message()
 {
      if (!clone)
      {
	delete log_write;
	clone--;
      } else 
	if ((*log_write))
        {
	 log->log(buf);
	 *log_write = false;
	// 
        }
 };
 
message::message(Log* _log):buf(""), log(_log), clone(0)
{
	 log_write = new bool ;
	 *log_write = 0 ;
};

message::message(const message& _message)
{
	 *this = _message;
	 clone++;
	 (*log_write) = true; 
};

void message::set(const std::string &j) 
{
    buf = j;
};

std::string  message::get()
{
  return buf;
}

Log::Log():fileName("/tmp/contact.log"),first(true),buf_str("")
{
     pthread_mutex_init(&mut, NULL);
}

Log::Log(const std::string & _fileName):fileName(_fileName),first(true),buf_str("")
{
//   std::cout << "Log() " << _fileName << std::endl;
     pthread_mutex_init(&mut, NULL);
}

void Log::setDebugMode(DebugMode dm)
{
     debugMode = dm;
     
     if (debugMode == DebugToFile)
	  logStream.open(fileName.c_str(),  std::ios::app| std::ios::out | std::ios::trunc);
}

Log::~Log()
{
//   std::cout << "~Log() " << fileName << std::endl;
     log("~Log()");
     
     if (debugMode == DebugToFile)
	  logStream.close();
}
void Log::log(const char * str)
{
//     May be using Log::mutex?!
//     flockfile(stdout);
     pthread_mutex_lock(&mut);

     if (debugMode == DebugToFile)
     {
	  static int cnt = 0;
	  if (cnt++ > 1000000)
	  {
	       logStream.close();
	       logStream.open(fileName.c_str(), std::ios::out | std::ios::trunc);
	      
	       cnt = 0;
	  }
     }
     if (debugMode == DebugToFile)
     {
          logStream << get_date_time() <<  "  " << str << std::endl;
     }
     else
	  std::cout << get_date_time() << "  "  << str << std::endl;
     pthread_mutex_unlock(&mut);
     
//     funlockfile(stdout);
}

void Log::log(const std::ostringstream & os)
{
     log(os.str().c_str());
}

void Log::log(const std::string & s)
{
     log(s.c_str());
}


std::string Log::get_date_time()
{
     struct timeval tv;
     gettimeofday(&tv, NULL);
     
     struct tm *tm = localtime(&tv.tv_sec);

     std::ostringstream os;
     os << std::setw(2) <<std:: setfill('0') << tm->tm_mday << "."
	<< std::setw(2) << std::setfill('0') << (tm->tm_mon + 1) << "."
	<< std::setw(2) << std::setfill('0') << (tm->tm_year + 1900) << ":"
	<< std::setw(2) << std::setfill('0') << tm->tm_hour << ":"
	<< std::setw(2) << std::setfill('0') << tm->tm_min << ":"
	<< std::setw(2) << std::setfill('0') << tm->tm_sec << "."
	<< std::setw(3) << std::setfill('0') << (tv.tv_usec / 1000);

     return os.str();
}

message Log::log() 
{ 
  
  return (message(this));
};



