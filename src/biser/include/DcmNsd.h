#ifndef DCMNSD_H
#define DCMNSD_H

#include "UiTcp.h"
#include "BDStateNsd.h"

/** @brief Класс для работы с УИ опрашивающие модули с НСД 
*/
class DcmNsd:public UiTcp
{
  enum
  {
   typeUI = 1 
  };
  enum
  {
    NEWMSG = 0 // количество повторений 
  };
public:
  DcmNsd( std::string _ip, int _port,BDStateNsd* _BDst);
  ~DcmNsd();

//   void setMask( int mask );
protected:
  virtual void setUiOn ( bool on = 0) ;
private:
  //восстанавливаем состояния из БД (заполняем lastState  maskPin )
  void setLastState(  );
  //записать в БД новое значение 
  void setNewState ( int state );
  
 
  BDStateNsd* BDst;
  

};

#endif // DCMNSD_H
