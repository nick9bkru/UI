#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <iostream>
#include <string>
#include <sstream>


#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>

#include "util.h"
#include "LogSingleton.h"

namespace SKLib
{
     class DataInterface
     {
     public:
	  DataInterface();
	  virtual ~DataInterface();
	  
	  virtual int open(const std::string & ipAddr)=0 ;
	  virtual int close() ;
	  virtual int getAnswerForCommand(const std::string & cmdString, void *answer, int size) =0 ;
	  virtual int getAnswerForCommand_new(const std::string & cmdString, void *answer, int size) =0 ;
	  
	  virtual int onlySend(const std::string & cmdString)=0;
	  virtual int onlyRead(void *answer, int size)=0;
	  int getAnswerForCommand(const std::string & cmdString, void *answer);
	   int getAnswerForCommand_new(const std::string & cmdString, void *answer);
	  

	  
	  int getFd() const;
	  bool isConnect() const;
	  int getTimeout() const;
	  std::string getErrMsg() const;
	  int getAnswerSize() const;
	  virtual std::string getInfo() const = 0;
	  
	  void setTimeout(int _timeout);
	  void setAnswerSize(int _answerSize);
	  
     protected:
	  void setFd(int _fd);
	  void setErrMsg(std::string _errMsg);
	  Log * log;
     private:
	  int timeout; // in microseconds.
	  int fd; // file descriptor.
	  int answerSize;
	  std::string errMsg;
     };
}

#endif // DATAINTERFACE_H
