#ifndef RAPSODIASET_H
#define RAPSODIASET_H

#include "DataInterface.h"
#include "DeviceInterface.h"
#include "TCPSocketInterface.h"
#include "InterfaceSet.h"
#include "LogSingleton.h"

class RapsodiaSet : public SKLib::InterfaceSet
{
public:
     RapsodiaSet();
     ~RapsodiaSet();
     virtual SKLib::DataInterface * getInterfaceForApp(int num);
};

#endif // RAPSODIASET_H
