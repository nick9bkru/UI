#ifndef KULON_H
#define KULON_H

#include "apparate.h"
#include <set>

class kulon :  public Apparate
{
public:
  kulon(struct sa_info _cfg, SKLib::DataInterface * _iface);
  ~kulon();
  /**
      \brief Основная функция, вызывающаяся в бесконечном цикле для каждого аппарата.
      
      Сначала, в случае если это необходимо, посылается команда управления.
      Потом опрашивается модуль ввода-вывода на наличие нового состояния аппарата.
      \return Результат опроса аппарата.
      \retval 0, если ошибка.
      \retval 1, если функция выполнена успешно.
     */
     virtual int update();
          /**
	\brief Привязать интерфейс для взаимодействия с модулем или счетчиком.
	\param _iface - указатель на экземпляр класса для интерфейса.
	\param dt - модуль или счетчик.
      */
     void setDataInterface(SKLib::DataInterface * _iface);
private:  
       SKLib::DataInterface * iface; ///< Интерфейсы для взаимодействия с модулем и счетчиком.
     ushort  answerKulon();
     int sendReceiveKulon();
          /**
      \brief Преобразует частное значение  индикации Кулона к APP-значению.
	      При наличии 0 в 11 бите слова состояния добавление индикации ПУС К1
      \param sost -  частное значение.
      \return APP-значение для частного значения sost.
	*/
    ushort  private2app_Kulon( ushort  sost)  const;
     /**
      \brief Проверяется условие: данный аппарат годен к участию в соединениях?
      \return Результат проверки.
      \retval true, если годен.
      \retval false, если нет.
     */
     virtual bool isReadyToConnect() ;
     /**
	\brief Посылает исходящий вызов или отбой.
	\param p - параметр, вызов или отбой.
     */
     virtual void commandCall(ushort p) ;
     /**
      \brief Перевести аппарат в режим готовности к соединению.
     */
     virtual void commandOff() ;
     /**
      \brief Перевести аппарат в режим соединения.
      \param p - дополнительная команда(ы).
     */
     virtual void commandOn(ushort p) ;
     /**
      \brief Ввести-вывести аппарат из конфигурации.
      \param p - вывести(0), ввести(1).
     */
     virtual void commandBlock(ushort p) ;
    virtual void init_pk() ;
    
    
    /**
     \brief проверяем валидное ли состояние аппарата 
    */
    virtual bool isValidState( const int & val)  ;
    
    /**
     \brief набор всех ошибок ( выбрал set быстрого поиска) 
    */
    std::set < int > errState;
    };

#endif // KULON_H
