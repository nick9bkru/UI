#ifndef RAPSODIASET_H
#define RAPSODIASET_H

#include "DataInterface.h"
#include "DeviceInterface.h"
#include "TCPSocketInterface.h"
#include "InterfaceSet.h"
#include "Log.h"

class RapsodiaSet : public SKLib::InterfaceSet
{
public:
     RapsodiaSet();
     ~RapsodiaSet();
     SKLib::DataInterface * getInterfaceForApp(int num, int code);
};

#endif // RAPSODIASET_H
