#ifndef APPARATEMANAGE_H
#define APPARATEMANAGE_H


#include <map>
#include "apparate.h"

class apparateManage
{
public:
  apparateManage();
  ~apparateManage();
  void addApparate(int num , Apparate * app);
  
  void newMessage ( struct kg kdg );
private :
  map<int , Apparate *> mapApp;
};

#endif // APPARATEMANAGE_H
