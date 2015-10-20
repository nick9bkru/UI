#ifndef TDCMCLI_H
#define TDCMCLI_H


#include "lib_dcm.h"
#include <string>
#include "Log.h"
namespace math //так сделал по тому что в "math.h" есть макрос с названием log
{
  #include "math.h"
}

//extern SKLib::Log log;
   /**
	 * \class TDcmCli
	 * @brief Класс для работы с модулем сухихи контактов из командной строки
    **/
	class TDcmCli : public _dcm::TDryContactModule
	{
		public:
         /**
		 * @brief Конструктор
		 * @param usedStates - используемое состояние
		 * @param _ipAddr - IP-адрес
		 * @param _port - номер порта Defaults to 50000.
	  **/
			TDcmCli( std::string _ipAddr, unsigned short int _port = 50000 );
         /**
			 * @brief Деструктор
	  **/

	/**
          		* @brief Функция возвращает состяние с инверсией
          		* @return состояние сухих контактов инверсивное
          **/
         		int getnotState();
  	 /**
          		* @brief Функция возвращает состяние 
          		* @return состояние выходов инверсивное
          **/
         		int gtsState_();
			virtual ~TDcmCli();
         /**
			 * @brief  Функция инициализации используемых состояний сухих контактов
	  **/
			virtual void initUsedStates();
	/**
			* @brief Функция возвращает IP модуля 
			* @return строка содержащая Ip адрес
	 **/
			std::string getIP() const;
		private:
         /**
		 * @brief  Функция окончания обработки состояния сухих контактов
	  **/
			virtual void endProcessDcmState ();    
         /**
			 * @brief  Функция обработки состояния сухих контактов
			 * @param statePos - номер контакта
			 * @param state - состояние контакта
	  **/
			virtual void processState( int statePos, bool state);
	
	};

#endif	//TDCMCLI_H
