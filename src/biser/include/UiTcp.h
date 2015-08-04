#ifndef UITCP_H
#define UITCP_H

#include <sys/time.h>
#include <pthread.h>
#include "PMutexLocker.h"
#include "../libdcm/include/UiTcp.h"


/** @brief Класс для работы с УИ 
*/
class UiTcp : public UIservice::UiTcp
{

  enum
  {
   MAXSEND =  5, //количество посылок без ответа после которых ошибка 
   NEWMSG = 0 // количество повторений нового состояния после чего сохраняем
  };
public:

  UiTcp(std::string _ip, int _port);
  virtual ~UiTcp();
  virtual int open();
  bool answState(); // запрашивает состояние с сухих контактов
  virtual bool recieveState();
  
  bool resetLastAnsw();
  virtual int close(  );
  
  virtual void setFd(int _fd);
protected:
  //записать в БД новое значение 
  virtual void setNewState ( int state ) = 0;
  virtual void setUiOn ( bool on = 0) = 0;
  int maskPin; //используемые контакты
  int lastState; 
  int type; //тип модуля UI
  
private:
  Mutex mtx ;
  
  
  struct timeval timeLastAnsw; //сколько не принемалось ответ после отправки запросы
  //int countSend; //сколько раз был зделан запрос без ответа
  int TIMEINTERVAL ; // время после которого считам УИ не рабочим и перестаём посылать кодограммы
  struct timeval timeLastAsk; // последнего запроса 
  int TIMENEXTASK ; // время в микро секундах чрез которое можно слать еще раз запро
  
  
  
  
  
  int countNewMsg; //счетчик новых сообщений что бы отсекать ложные слабатывания

  
};

#endif // UITCP_H
