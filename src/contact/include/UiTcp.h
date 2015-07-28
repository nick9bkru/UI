#ifndef UITCP_H
#define UITCP_H

#include <sys/time.h>
#include <pthread.h>
#include "PMutexLocker.h"

#include "TCPSocketInterface.h"


/** @brief Класс для работы с УИ 
*/
class UiTcp : public SKLib::TCPSocketInterface
{

  enum
  {
   MAXSEND =  5, //количество посылок без ответа после которых ошибка 
   NEWMSG = 0 // количество повторений нового состояния после чего сохраняем
  };
public:
   /** enum typeUI
  \brief типы УИ используемые в комплексе для бд
*/
  enum typeUIBD
  {
    UINSD = 0,
    UIUPS,
    UIARPU
  };
  
  UiTcp(std::string _ip, int _port = 0);
  virtual ~UiTcp();
  
  virtual std::string getInfo() const;

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
  
  int close(  );
  
  void setFd(int _fd);
protected:
  
  int lastState; 
  int type; //тип модуля UI
  
private:
  int answAndGet( const std::string cmd);
  Mutex mtx ;
   
  std::string ip;
  
};

#endif // UITCP_H
