#ifndef SERDOLIK_H
#define SERDOLIK_H

#include "apparate.h"
#include "UiTcpArp.h"

class serdolik : public  Apparate
{
public:
  serdolik(struct sa_info _cfg, UiTcpArp * ifaceDcm);
  ~serdolik();
  /**
      \brief Основная функция, вызывающаяся в бесконечном цикле для каждого аппарата.
      
      Сначала, в случае если это необходимо, посылается команда управления.
      Потом опрашивается модуль ввода-вывода на наличие нового состояния аппарата.
      \return Результат опроса аппарата.
      \retval 0, если ошибка.
      \retval 1, если функция выполнена успешно.
     */
     virtual int update();
private:
  
      UiTcpArp * ifaceDcm; ///< Интерфейсы для взаимодействия с модулем и счетчиком.
       /**
      \brief Определяет позицию модулей в корзине.
      \sa ushort Apparate::ExpansionUnitPos.
     */
     void setExpansionUnitPos();  
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
     virtual void commandBlock(ushort p);
     virtual void init_pk() ;
};

#endif // SERDOLIK_H
