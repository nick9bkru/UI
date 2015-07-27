/**
   \file log.h
   \brief Описание класса Log для ведения журнала.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/


#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <sys/time.h>

using namespace std;

namespace SKLib
{
     /**
	\class Log
	\brief Создание и ведение журнала.
     */
     class Log
     {
	  /**
	     \var ofstream logStream
	     \brief Путь к файлу журнала.
	  */
	  std::ofstream logStream;

	  
	  /**
	     \var int debugMode
	     \brief Отладка с выводом на экран или в файл журнала.
	     \brief "0"-в файл, "1"-на экран.	
	  */
	  int debugMode;

	  
	  /**
	     \var std::string fileName
	     \brief Имя файла для записи журнала.
	  */
	  std::string fileName;

	  
	  /**
	     \var pthread_mutex_t mut
	     \brief Мьютекс для синхронизации потоков.
	  */
	  pthread_mutex_t mut;
     
     public:
	  /**
	     \enum DebugMode
	     \brief Способы журналирования.
	  */
	  enum DebugMode
	  {
	       DebugToFile,
	       DebugToUserScreen
	  };

	  
	  /**
	     \fn Log()
	     \brief Конструктор.
	  */
	  Log();

	  
	  /**
	     \fn Log()
	     \brief Конструктор.
	     \param _fileName - имя файла.
	  */
	  Log(const std::string & _fileName);

	  
	  /**
	     \fn ~Log()
	     \brief Деструктор.
	  */
	  ~Log();

	  
	  /**
	     \fn void set_debug_mode(int mode)
	     \brief Назначение переменной debug_mode значения mode.
	     \param mode - выбор способа отладки.
	     \sa int Log::debug_mode.
	  */
	  void setDebugMode(DebugMode dm);

	  
	  /**
	     \fn std::string get_date_time()
	     \brief Получение текущего времени в формате std::string.
	     Используется для журналирования событий.
	     \return Текущее время в формате std::string.
	  */
	  std::string get_date_time();

	  
	  /**
	     \fn void Log::log(const ostringstream & os)
	     \brief Перегруженная функция записи в журнал.
	     \param os - сообщение (строковый поток) для записи в журнал.
	
	     \sa void Log::log(const ostringstream & os);
	     \sa void Log::log(const char * str);
	     \sa void Log::log(const char * str, const struct kg & buf);
	     \sa void Log::log(const std::string & s);
	  */
	  void log(const ostringstream & os);

     
	  /**
	     \fn void Log::log(const char * str)
	     \brief Перегруженная функция записи в журнал.
	     \param str - сообщение для журнала.
	
	     \sa void Log::log(const ostringstream & os);
	     \sa void Log::log(const char * str);
	     \sa void Log::log(const char * str, const struct kg & buf);
	     \sa void Log::log(const std::string & s);
	  */
	  void log(const char * str);

	  
	  /**
	     \fn void Log::log(const std::string & s)
	     \brief Перегруженная функция записи в журнал.
	     \param s - сообщение для журнала.
	
	     \sa void Log::log(const ostringstream & os);
	     \sa void Log::log(const char * str);
	     \sa void Log::log(const char * str, const struct kg & buf);
	     \sa void Log::log(const std::string & s);
	  */
	  void log(const std::string & s);
     };
}
#endif
