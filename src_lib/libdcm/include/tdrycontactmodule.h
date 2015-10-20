/**
   \file tdrycontactmodule.h
   \brief Описание класса TDryContactModule.
   \author Зайцев А.А., Лихобабин Е.А.
   \version 2011-01-02
*/
#ifndef __TDRYCONTACTMODULE_H__
#define __TDRYCONTACTMODULE_H__

//#define DEBUG

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

namespace _dcm
{
// Время на ожидание отклика от модуля
   const int TIME_RECV = 100000;
   //использование - создать объект и вызывать getState() - когда необходимо.
    /**
     * \class TDryContactModule
      * @brief Абстрактный класс для обслуживания модулей сухихи контактов
      **/
   class TDryContactModule
   {
      public:
         /**
          * @brief Конструктор
          * @param usedStates - используемое состояние
          * @param _ipAddr - IP-адрес
          * @param _port - номер порта Defaults to 50000.
          **/
         TDryContactModule( std::string _ipAddr, unsigned short int _port = 50000 );
         /**
          * @brief Деструктор
          **/
         virtual ~TDryContactModule();

         /**
          * @brief Функция соединения
          * @return признак установки соединения
          **/
         int connect();
         /**
          * @brief Функция разрыва соединения
          **/
         void disconnect();

         /**
          * @brief Функция возвращает состяние
          * @return состояние сухих контактов
          **/
         int getState();// была const
         /**
          * @brief Функция установки состяния сухих контактов
          * @return признак успешности выполнения
          **/
         bool setState(unsigned char state);
         /**
          * @brief Функция обработки состояния сухих контактов
          * @param state - состояние сухих контактов
          **/
         void processDCMState ( int state );
         int dcmState;   ///< состояние для контроля вскрытия дверей
          /**
          * @brief Функция запроса и обработки состояния сухих контактов
           **/
         void getAndProcessState();
         /**
          * @brief Функция запроса доступных сухих контактов
          * @return Признак успешного выполнения функции
          **/
         bool getDcmAvailState(){return dcmAvailState;};
         /**
          * @brief  Функция инициализации объекта КСК
          **/
         void init();
         /**
          * @brief  Функция инициализации используемых состояний сухих контактов
          **/
	 /**
	  * @brief Функция проверки наличия соединения
	  * @return признак наличия соединения
	  **/
	 bool isConnected() const;
	 /**
	  * @brief Функция опроса счетчика
	  * @param state - номер канала
	  * @return ответ счетчика 2 байта 1 байты номер импульса 2 продолжительность
	  **/
	 unsigned int gtcState(unsigned char state );
	 /**
          * @brief Функция возвращает состяние выводов
          * @return состояние сухих контактов
          **/
	 int gtsState();

         virtual void initUsedStates() = 0;
      protected:
         int usedStates;   ///< состояние контроля вскрытия дверей
         int notConnState; ///< состояние - нет соединения
         int bitMask;   ///< битовая маска, накладываемая на значение состояний
         std::string ipAddr;  ///< IP-адрес
      private:
         
         unsigned short int port; ///< номер порта
         int sock;      ///< дескриптор сокета
         bool dcmAvailState; ///<  Признак доступных сухих контактов
         /**
          * @brief  Функция окончания обработки состояния сухих контактов
          **/
         virtual void endProcessDcmState ()=0 ;         
         /**
          * @brief  Функция обработки состояния сухих контактов
          * @param statePos - номер контакта
          * @param state - состояние контакта
          **/
         virtual void processState( int statePos, bool state);
         /**
          * @brief  Функция обработки состояния 1-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processFirstState(bool state);
         /**
          * @brief  Функция обработки состояния 2-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processSecondState(bool state);
         /**
          * @brief  Функция обработки состояния 3-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processThirdState(bool state);
         /**
          * @brief  Функция обработки состояния 4-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processFourthState(bool state);
         /**
          * @brief  Функция обработки состояния 5-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processFifthState(bool state);
         /**
          * @brief  Функция обработки состояния 6-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processSixthState(bool state);
         /**
          * @brief  Функция обработки состояния 7-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processSeventhState(bool state);
         /**
          * @brief  Функция обработки состояния 8-го контактов
          *  @param state - состояние контакта
          **/
         virtual void processEighthState(bool state);
   };
};
#endif // __DRYCONTACTMODULE_H__
