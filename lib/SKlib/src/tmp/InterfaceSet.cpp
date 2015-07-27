#include "InterfaceSet.h"

using namespace SKLib;

extern Log log;

std::vector<DataInterface *> InterfaceSet::vec;

InterfaceSet::InterfaceSet()
{
}

InterfaceSet::~InterfaceSet()
{     
     for (std::vector<DataInterface *>::iterator it = vec.begin(); it != vec.end(); it++)
	  delete *it;

     log.log("~InterfaceSet()");
}

void InterfaceSet::add(DataInterface *ai)
{
     vec.push_back(ai);
}


void InterfaceSet::start() const
{
     pthread_attr_t attr1;
     pthread_attr_init(&attr1);
     pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_DETACHED);

     pthread_t pthread1;
     pthread_create(&pthread1, &attr1, pthreadFunc, NULL);
}

void InterfaceSet::openAll()
{
     std::ostringstream os;

//     log.log("InterfaceSet::openAll()");
     
     for (std::vector<DataInterface *>::iterator it = vec.begin(); it != vec.end(); it++)
     {
	  if ((*it)->getFd() < 1)
	       (*it)->open();

	  if ((*it)->getFd() < 1)
	  {
	       os.str("");
	       os << (*it)->getInfo() << " : " << ((*it)->getErrMsg()) << ", fd=" << (*it)->getFd();
	       log.log(os);
	  }
     }
}

void* InterfaceSet::pthreadFunc(void* arg)
{
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
     
     while (1)
     {
	  openAll();
	  sleep(1);
     }

     return NULL;
}
