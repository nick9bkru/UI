/**
   \file apparate.h
   \brief Описание класса Apparate.
   \author Кориков Сергей
*/
#ifndef APPARATE_H
#define APPARATE_H

#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <time.h>

#include "defines.h"
#include "common_funcs.h"
#include "simpleclient.h"

#include "BDPthread.h"

#include "DataInterface.h"
#include "Log.h"

extern SKLib::Log log;

/**
   \class Apparate
   \brief Класс для работы со всеми типами аппаратов.
*/
class Apparate
{    
protected:
     /**
 \enum initFlags
 \brief Флаги для инициализации объекта класса.
     */
     enum initFlags
     {
   INIT_FULL = 1,    ///< Полная инициализация.
   INIT_WITHOUT_KEYS ///< Инициализация без чтения информации о ключах.
     };
     
     struct sa_info cfg; ///< Конфигурация и настройки аппарата из БД.


      
     ushort ExpansionUnitPos; ///< Позиция модуля в корзине расширения.
     
     int     ioPos[2];    ///< Позиция модулей ввода-вывода.
     char    ioCmd[2][10];    ///< Команда для опроса модулей ввода и вывода.
     ushort  ioBuf[2];    ///< Последний ответ от модулей.
     ushort  ioBufOld[2]; ///< Предыдущий ответ от модулей.
     ushort  ioSendBuf;   ///< Если не нуль, то делается попытка установить эту команду.
     ushort  FreqBuffer[2];    ///< Текущее состояние счетчика.
     ushort  FreqBufferOld[2]; ///< Предыдущее состояние счетчика.
     ushort  NextKey; ///< Признак для перехода на другой ключ.

     ushort KL;     ///< Текущий ключ: К1(1) или К2(2).
     ushort KZN;    ///< Номер ключевой зоны, присвоенной данному аппарату.
     ushort KONF;   ///< Аппарат в конфигурации(1) или нет(0).
     ushort LSMENA; ///< Наличие признака "отложенная смена" на данный момент: есть(1) или нет(0).
     ushort SN;     ///< Наличие синхронизма на данный момент: есть(1) или нет(0).
     ushort  dcCallLong;     ///< Длительность вызова: 1-15 с или 0 - аппарат не связан с ПК.
     ushort  dcCall;         ///< Последний принятый импульс: вызов(1) или отбой(0).
     ushort  dcNeedSendCall; ///< Требуется послать импульс: вызов(2), отбой(6), ничего(0).
     union chanNum dcChannelGeo; ///< Географический адрес интерфейса аппарата.
   // time_t  problem_sost; ///< Время попадания СА в ИСПР, нет ОК, нет СН,  РАБОТА
   // ushort  was_disconnect_ch;///< Выдан сигнал разорвать канал между СА и ЦКО, чтобы получить инд. ОК
     int  rabota_off;///< Признак того, что после отбоя АБ была снята работа у этого аппарата
     int  prio;  ///< Приоритет аппарата

     time_t  time_iv; 
     /**
 \brief Инициализация объекта.
 \param flags - параметры инициализации.
 \sa Apparate::initFlags.
     */
     void init(int flags);
     virtual void init_pk() = 0;
     /**
 \brief Преобразует APP-значение команды или индикации к частному.
 \param sost - APP-значение.
 \param io - IN или OUT.
 \return частное значение для APP-значения sost.
     */     
     ushort app2private(ushort sost, ushort io) const;

     /**
 \brief Преобразует частное значение команды или индикации к APP-значению.
 \param sost - APP-значение.
 \param io - IN или OUT.
 \return APP-значение для частного значения sost.
     */
     ushort private2app(ushort sost, ushort io) const;
     


     /**
 \brief Получить информацию о текущем ключе.
 \return APP_K1, APP_K2 или 0.
     */
     ushort getCurrentKey() const;

     /**
 \brief Добавить запись в журнал.
 \param str - строка.
     */
     void log1(const std::string & str) const;

     /**
 \brief Добавить запись в журнал.
 \param os - строковый поток.
     */
     void log1(const std::ostringstream & os) const;

     /**
 \brief Получить строку с состоянием аппарата.
 \return Строка с состоянием.
     */
      std::string getStateString() const;

     /**
 \brief Тип вызова - ручной?
 \return Результат.
 \retval true, если да.
 \retval false, если нет.
     */
//     inline bool isManualCallType() const;

     /**
 \brief Тип вызова - автоматический?
 \return Результат.
 \retval true, если да.
 \retval false, если нет.
     */
//     inline bool isAutoCallType() const;

     /**
 \brief Тип вызова - неизвестен?
 \return Результат.
 \retval true, если да.
 \retval false, если нет.
     */
//     inline bool isUnknownCallType() const;

     /**
 \brief Проверяется условие: данный аппарат - Аргунь?
 \return Результат проверки.
 \retval true, если проверка прошла успешно.
 \retval false, если нет.
     */
     ApparateType getType() const;

     /**
 \brief Проверяется условие: данный аппарат годен к участию в соединениях?
 \return Результат проверки.
 \retval true, если годен.
 \retval false, если нет.
     */
     virtual bool isReadyToConnect() =0;

     /**
 \brief Для данного аппарата используется счетчик?
 \return Результат проверки.
 \retval true, если используется.
 \retval false, если нет.
     */
   //  inline bool isApparateWithFreq() const;

     /**
 \brief Информирование сервера о приходе вызова или отбоя.
 \param impulse - длина принятого импульса.
     */
     void sendCuuCall(ushort impulse);

     /**
 \brief Информирование сервера об изменении в составе аппаратов, находящихся в конфигурации.
     */
     void sendCuuChanges();

     /**
 \brief Отсылает сообщение к РМО об изменении синхронизма.
 \sa ushort Apparate::SN.
     */
     void sendRmoSynch() const;     
     /**
 \brief Записывает в таблицу sig_rmo сигнал есть/нет снх или изменение грифа
 */
     void  write_sig_rmo( ushort kod);

     
     /**
 \brief Устанавливает тип вызова.
 \param n - длительность вызова для данного аппарата из БД.
 \sa ushort Apparate::dcCallLong.
     */
     void setCallLong(ushort n);

   
     
     /**
 \brief Записывает новые данные в БД.
     */
     void updateTables();
 
     /**
 \brief Записывает состояние счетчика.
 \param str - строка состояния, полученная от счетчика.
 \sa ushort Apparate::FreqBuffer[2], ushort Apparate::FreqBufferOld[2].
     */
     void updateFreqBuffers(const char *str);
bool ok;
public:

     /**
 \brief Конструктор.
 \param foo - структура для инициализации объекта.
     */
     Apparate(struct sa_info foo);

     /**
 \brief Деструктор.
     */
     virtual ~Apparate();

      pthread_mutex_t updateMutex;     
     volatile bool isOn; 	///< подключен ли аппарат к nport


     int  update_prio( int val );
     void    setPrio(int  val);  
     /**
      \brief Основная функция, вызывающаяся в бесконечном цикле для каждого аппарата.
      
      Сначала, в случае если это необходимо, посылается команда управления.
      Потом опрашивается модуль ввода-вывода на наличие нового состояния аппарата.
      \return Результат опроса аппарата.
      \retval 0, если ошибка.
      \retval 1, если функция выполнена успешно.
     */
     virtual int update() = 0;

     /**
 \brief Проверяется условие: данный аппарат сейчас в соединении?
 \return Результат проверки.
 \retval true, если да.
 \retval false, если нет.
     */
     bool isInConnection() const;
     
     bool RKisInConnection() const;

     /**
 \brief Получает кодограмму для дальнейшей обработки.
 \param buf - принятая извне кодограмма.
 \return Результат предварительной обработки.
 \retval true, проверка прошла успешно.
 \retval false, если неуспешно.
     */
     bool checkNewMessage(const struct kg & buf);
     
     /**
      \brief Перевести аппарат в режим соединения.
      \param p - дополнительная команда(ы).
     */
     virtual void commandOn(ushort p) = 0;

     /**
      \brief Перевести аппарат в режим готовности к соединению.
     */
     virtual void commandOff() = 0;

     /**
      \brief Ввести-вывести аппарат из конфигурации.
      \param p - вывести(0), ввести(1).
     */
     virtual void commandBlock(ushort p) = 0;

     /**
 \brief Установить команду заданную параметром p.
 \param p - команда(ы).
     */
     void commandSet(ushort p);

     /**
 \brief Снять команду заданную параметром p.
 \param p - команда(ы).
     */
     void commandUnset(ushort p);

     /**
 \brief Посылает исходящий вызов или отбой.
 \param p - параметр, вызов или отбой.
     */
     virtual void commandCall(ushort p) = 0;

     /**
 \brief Устанавливает новый ключ в данном аппарате.
 \param p - новый ключ.
 \sa ushort Apparate::NextKey.
     */
     void commandNextKey(ushort p);

     /**
 \brief Устанавливает/снимает признак "отложенная смена" в данном аппарате.
 \param p - значение признака: есть признак(1) или нет(0).
 \sa ushort Apparate::LSMENA.
     */
     void commandLsmena(ushort p);
};

/**
   \brief Перегрузка оператора operator<<.
   \param strm - поток вывода.
   \param foo - конфигурация и настройки аппарата из БД.
 */
std::ostream & operator<< (std::ostream & strm, const struct sa_info & foo);

/**
   \brief Послать UDP-сообщение на выбранный хост:порт.
*/
int sendUDPMessage(const char * destIP, int destPort, ushort *arr, int len);

// template<class T>
// int sendUDPMessage(const char * destIP, int destPort, std::vector<T> & t)
// {
//      struct sockaddr_in dest, my;
//      dest.sin_family = AF_INET;
//      dest.sin_port = htons(destPort);
//      dest.sin_addr.s_addr = inet_addr(destIP);

//      my.sin_family = AF_INET;
//      my.sin_port = htons(7033);
//      my.sin_addr.s_addr = inet_addr("192.168.1.1");

//      int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//      if (sockfd == -1)
//      {
// 	  log.log("sendUDPMessage(): socket()" + std::string(strerror(errno)));
// 	  return -1;
//      }

//      int ret = bind(sockfd, (struct sockaddr *) &my, sizeof(my));
//      if (ret < 0)
//      {
// 	  log.log("sendUDPMessage(): bind()" + std::string(strerror(errno)));
// 	  close(sockfd);
// 	  return -1;
//      }

//      ret = 0;
//      for (vector<T>::iterator it = t.begin(); it != t.end(); it++)
//      {
// 	  T val = *it;
// 	  ret += sendto(sockfd, &val, sizeof(T), 0, (struct sockaddr *)&dest, sizeof(dest));
//      }

//      int sz = t.size() * sizeof(T);
//      if (ret == sz)
// 	  log.log("Success!");
     
//      close(sockfd);

//      return 1;
// }

#endif
