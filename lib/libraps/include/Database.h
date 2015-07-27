 /**
   \file Database.h
   \brief Содержит описание класса Database для работы с БД.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>

#include <iostream>
#include <iomanip>
#include <string>

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

/**
   \brief Создание сокета.
   \param port - номер порта.
   \return идентификатор сокета.
 */
int make_socket(uint16_t port);

/**
   \class Database
   \brief Класс для работы с БД.

   Класс содержит все необходимые функции для работы с БД. Простой интерфейс.
*/
class Database
{     
     PGconn *conn; ///< Указатель на объект, связанный с соединением с БД.
     PGresult *pgRes; ///< Указатель на объект, хранящий результат последней операции.

     int nTuples; ///< Количество записей в ответе на запрос.
     int curTuple; ///< Текущий обрабатываемый запрос.

     bool dbg; ///< Отладочный режим: да или нет.
     
     void close(); ///< Закрыть соединение с БД.
     void endQuery(); ///< Завершить последнюю операцию.

     /**
	\brief Получить ответ после SELECT-запроса.
	\param res - указатель на PGresult* Database::pgRes.
	\param cursor - номер текущей обрабатываемой строки ответа.
	\param answer - буфер для ответа.
	\param len - размер answer.
	\return Результат получения ответа.
	\retval 0, если ошибка.
	\retval 1, если функция выполнена успешно.
     */
     int getAnswer(PGresult *res, int cursor, void *answer, int len); 
     
 public:

     /**
	\brief Конструктор.
	\param str - имя хоста с БД.
     */
     Database(const std::string &str);

     /**
	\brief Деструктор.
     */
     virtual ~Database();

     /**
	\brief Включить/выключить отладочную печать.
	\param debug - true(ВКЛЮЧИТЬ), false(ВЫКЛЮЧИТЬ).
	\sa bool Database::dbg.
     */
     void setDebugMode(bool debug);

     /**
	\brief Готов ли объект к работе с БД?
	\return Готовность объекта к работе с БД.
	\retval true - готов.
	\retval false - не готов.
	\sa bool Database::ready.	
     */
     bool isReady() const;

     /**
	\brief Выполнить не-SELECT запрос.
	\param str - строка запроса.
	\return Результат.
	\retval true, запрос выполнен.
	\retval false, запрос не выполнен.
      */
     bool notSelect(const std::string &str);

     /**
	\brief Выполнить SELECT запрос, где предположительно будет много ответов.
	\param str - строка запроса.
	\param answer - буфер для ответа.
	\param len - размер answer.
	\return Результат.
	\retval true, запрос выполнен.
	\retval false, запрос не выполнен.
     */
     bool multipleSelect(const std::string &str, void *answer, int len);

     /**
	\brief Выполнить SELECT запрос, где предположительно будет один ответ.
	\param str - строка запроса.
	\param answer - буфер для ответа.
	\param len - размер answer.
	\return Результат.
	\retval true, запрос выполнен.
	\retval false, запрос не выполнен.
     */
     bool singleSelect(const std::string &str, void *answer, int len);
};

#endif // DATABASE_H
