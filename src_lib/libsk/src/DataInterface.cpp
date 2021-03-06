#include "DataInterface.h"

using namespace SKLib;

// extern Log log;

DataInterface::DataInterface(): timeout(50000), fd(-1), answerSize(64)
{
  log = &SKLib::LogSingleton::getInstance();
}

DataInterface::~DataInterface()
{
     log->log("~DataInterface()");
     this->close();
}

int DataInterface::close()
{
     if (getFd() > 0)
	  ::close(getFd());
     
     setFd(-1);
     
     return 1;
}

int DataInterface::getAnswerForCommand(const std::string & cmdString, void *answer)
{
     if (getFd() < 1)
	  return -1;
     
     return getAnswerForCommand(cmdString, answer, getAnswerSize());
}

int DataInterface::getAnswerForCommand_new(const std::string & cmdString, void *answer)
{
     if (getFd() < 1)
	  return -1;
     
     return getAnswerForCommand_new(cmdString, answer, getAnswerSize());
}

void DataInterface::setFd(int _fd)
{
     fd = _fd;
}

int DataInterface::getFd() const
{
     return fd;
}

void DataInterface::setTimeout(int _timeout)
{
     timeout = _timeout;
}

int DataInterface::getTimeout() const
{
     return timeout;
}

void DataInterface::setAnswerSize(int _answerSize)
{
     answerSize = _answerSize;
}

int DataInterface::getAnswerSize() const
{
     return answerSize;
}

void DataInterface::setErrMsg(std::string _errMsg)
{
     errMsg = "\"" + _errMsg + ": " + std::string(strerror(errno)) + "\"";
}

std::string DataInterface::getErrMsg() const
{
     return errMsg;
}

bool DataInterface::isConnect() const
{
 return fd >=0; 
};
