/**
   \file log.cpp
   \brief Реализация класса Log.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include <unistd.h>
#include "Log.h"

using namespace SKLib;

Log::Log():fileName("/tmp/contact.log")
{
     pthread_mutex_init(&mut, NULL);
}

Log::Log(const std::string & _fileName):fileName(_fileName)
{
     pthread_mutex_init(&mut, NULL);
}

void Log::setDebugMode(DebugMode dm)
{
     debugMode = dm;
     
     if (debugMode == DebugToFile)
	  logStream.open(fileName.c_str(), ios::out | ios::trunc);
}

Log::~Log()
{
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
	       logStream.open(fileName.c_str(), ios::out | ios::trunc);
	       cnt = 0;
	  }
     }
	  
     if (debugMode == DebugToFile)
	  logStream << get_date_time() << " [" << getpid() << "] " << str << std::endl;
     else
	  std::cout << get_date_time() << " [" << getpid() << "] " << str << std::endl;

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

/*
void Log::log(const char * str, const struct kg & buf)
{
     std::ostringstream message;
     message << str <<": " <<
	  setfill('0') << hex
	     << setw(4) << buf.cyclic_num << " "
	     << setw(1) << buf.number << " "
	     << setw(4) << buf.action << " "
	     << setw(4) << buf.command;

     log(message);
}
*/

// std::string Log::get_date_time()
// {
//      time_t curtime = time(NULL);
//      struct tm *loctime = localtime(&curtime);

//      char buffer[255];
//      strftime(buffer, 255, "%Y-%m-%d %T", loctime);

//      return std::string(buffer);
// }

std::string Log::get_date_time()
{
     struct timeval tv;
     gettimeofday(&tv, NULL);
     
     struct tm *tm = localtime(&tv.tv_sec);

     ostringstream os;
     os << setw(2) << setfill('0') << tm->tm_mday << "."
	<< setw(2) << setfill('0') << (tm->tm_mon + 1) << "."
	<< setw(2) << setfill('0') << (tm->tm_year + 1900) << ":"
	<< setw(2) << setfill('0') << tm->tm_hour << ":"
	<< setw(2) << setfill('0') << tm->tm_min << ":"
	<< setw(2) << setfill('0') << tm->tm_sec << "."
	<< setw(3) << setfill('0') << (tv.tv_usec / 1000);

     return os.str();
}
