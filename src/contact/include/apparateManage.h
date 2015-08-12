#ifndef APPARATEMANAGE_H
#define APPARATEMANAGE_H


#include <map>
#include "apparate.h"
#include "LogSingleton.h"
#include "RapsodiaSet.h"
#include "kulon.h"
#include "serdolik.h"
#include "TCPServerManage.h"

class apparateManage
{
public:
  apparateManage();
  ~apparateManage();
  void addApparate(int num , Apparate * app);
  void addApparate(struct sa_info foo);
  
  void newMessage ( struct kg kdg );
  
  /**
  \brief меняем интерфейс для apparate
   \param buf - номер СА команда и номер порта (nport * 100 + port)
 */
  void reSetInterfece(kg & buf);
  
  void  updApparate (  int  n );
private :
  /**
   \brief Опрос аппаратов с приоритетом >= 2
   \param low - наименьший индекс аппарата в группе
   \param high - наибольший индекс аппарата в группе
   */
  void  upd_high_prio_apparate(  int  low, int  high );
  
  map<int , Apparate *> mapApp;
  SKLib::Log *Log ;
  RapsodiaSet * rs;
};

#endif // APPARATEMANAGE_H
