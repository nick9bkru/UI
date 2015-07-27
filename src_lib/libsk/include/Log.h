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
/*
string operator<<(string &str,string &str2 )
{
return str+str2;
};
*/
namespace SKLib
{
  /**
	\class Log
	\brief Создание и ведение журнала.
  */
  class Log;
  /**
	\class message
	\brief класс хранящий строку для вывода и отсчитывае
	\brief кол-во созданных копий класса(Копирующий конструктор).
     */
/// /////////////////////////////////////////////////////////////////////////////
  class message 
  {
     public:
       /**
	     \fn message(Log* _log);
	     \brief Конструктор.
	     \param log - указатель на класс лога.
	  */
       message(Log* _log);
       /**
	     \fn message(Log* _log);
	     \brief Копирующий Конструктор
	     \brief (объявлять его не хорошо, но что поделаешь).
	     \param _message - указатель на самого себя.
	  */
       //Копирующий конструктор
       message(const message& _message);
       ~message();
       void set(const string &j) 
       {
	  buf = j;
       };
       string get()
       {
	 return buf;
       }
     private:
     /**
	     \var string buf
	     \brief Строка сообщения	
     */    
       string buf;
     /**
	     \var Log * log
	     \brief Указатель на сласс лога	
     */
       Log * log;
     /**
	     \var int clone
	     \brief Количесво клонов созданых копирующим конструктором	
     */
       int clone;
     protected:
     /**
	     \var bool *log_write
	     \brief Указатель на переменную который 
	     \brief показывает производилась ли запись в лог	
     */
       bool *log_write;
  } ;
/// /////////////////////////////////////////////////////////////////////////////
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
	         std::string s;
     
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
	  /**og::l
	     \fn operator<< (const int &str);
	     \brief Перегруженная оператор записи в журнал.
	     \param str - сообщение для журнала.
	   */
	  message log() 
	  { 
	    
	   return (message(this));
	  };

     private:
	  bool first ;
	  string buf_str;
     };
     template<typename T>
  message operator<< (message l, const T& t)
  {
    ostringstream k;
    k<<l.get()<<t;
    l.set(k.str());
    
    return l;
  };
};

#endif
