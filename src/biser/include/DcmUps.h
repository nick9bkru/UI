#ifndef DCMUPS_H
#define DCMUPS_H

#include <vector>
#include "UiTcp.h"
#include "BDStateUps.h"

/** @brief Класс для работы с УИ опрашивающие модули с УПС 
*/

class DcmUps:public UiTcp
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
  DcmUps( std::string _ip, int _port,BDStateUps* _BDst );

  ~DcmUps();
protected:
  virtual void setUiOn ( bool on = 0) ;
private: 
    //восстанавливаем состояния из БД (заполняем lastState  maskPin )
  void setLastState(  );
  //записать в БД новое значение 
  void setNewState ( int state );
    
  std::vector< int> usePin;
  
  BDStateUps* BDst;
};

#endif // DCMUPS_H
