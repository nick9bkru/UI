#ifndef UITCPARP_H
#define UITCPARP_H

#include "UiTcp.h"
#include <sys/time.h>
#include <pthread.h>
#include "PMutexLocker.h"

/** @brief Класс для работы с УИ 
*/

class UiTcpArp : public UIservice::UiTcp
{/*
  enum
  {
   MAXSEND =  5, //количество посылок без ответа после которых ошибка 
   NEWMSG = 0 // количество повторений нового состояния после чего сохраняем
  };*/
public:
  
  UiTcpArp(std::string _ip, int _port = 0);
  virtual ~UiTcpArp();

  /**
  * @brief Функция установки состяния сухих контактов
  * @return признак успешности выполнения
  **/
  bool setState(unsigned char state);

  /**
  * @brief Функция возвращает состяние с инверсией
  * @return состояние сухих контактов инверсивное
  **/
  int getnotState();
  /**
  * @brief Функция возвращает состяние 
  * @return состояние выходов инверсивное
  **/
  int gtsState();
  
  virtual int close(  );
  
  virtual void setFd(int _fd);
protected:
  
  int lastState; 
  int type; //тип модуля UI
  
private:
  int answAndGet( const std::string cmd, bool Inv = true);
  Mutex mtx ;
time_t timeNoRead;
  
};

#endif // UITCPARP_H
