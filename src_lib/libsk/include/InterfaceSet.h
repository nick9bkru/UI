#ifndef INTERFACESET_H
#define INTERFACESET_H

#include <map>
#include <pthread.h>

#include "DataInterface.h"

using namespace  std;

namespace SKLib
{    typedef std::multimap< int,DataInterface *,less<int> > NPortDI;
     class InterfaceSet
	  {
	  public:
	       InterfaceSet();
	       virtual ~InterfaceSet();
	       
	       virtual DataInterface *getInterfaceForApp(int num) = 0;
	       void add(int nport, DataInterface *ai);
	       void start() const;
	       void setLog(Log * _log);
	       
	  protected:
              
	       static  NPortDI  mmap;
	       static void* pthreadFunc(void*);
	       static void openAll(const std::string & ipAddr, int nport, bool restart=false);
	  };
}

#endif // INTERFACESET_H
