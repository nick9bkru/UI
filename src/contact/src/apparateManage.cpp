#include "apparateManage.h"

apparateManage::apparateManage()
{

}

apparateManage::~apparateManage()
{

}

void apparateManage::addApparate( int num , Apparate * app)
{
 mapApp[ num ] = app; 
};

void apparateManage::newMessage (struct kg kdg )
{
  Apparate * app = mapApp[ kdg.number ] ;
  if ( app->isOn)
  {  
      app->checkNewMessage(kdg);
      app->setPrio(1);
    }
  
};