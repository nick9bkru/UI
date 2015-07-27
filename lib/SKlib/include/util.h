 /**
   \file util.h
   \brief Содержит описание функций общего использования, типов данных, необходимых для работы программы.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ios>

#include <sys/time.h>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>

using  namespace std;
/**
   \typedef __tod_t
   \brief Используется для представления времени.
 */
typedef long long int __tod_t;

template<class T>
std::string LexicalCaster(T t, int base = 10)
{
     std::ostringstream ss;
	 ss << setbase(base) << setiosflags(ios::showbase) << t;
     return ss.str();
}

/**
   \fn ushort toUshort(const char *str);
   \brief Преобразует строку, содержащую шестнадцатиричное значение в ushort.
   \param str - указатель на первый байт строки.
   \return ushort.
 */
ushort toUshort(const char *str);

/**
   \fn ushort toUshortKulon(const char *str);
   \brief Преобразует строку, содержащую шестнадцатиричное значение в ushort для Кулона.
   \param str - указатель на первый байт строки.
   \return ushort.
 */
ushort toUshortKulon(const char *str);

/**
   \brief Делает операцию "result = x - y"
   \param result - разность.
   \param x - уменьшаемое.
   \param y - вычитаемое.
   \return время с точностью до микросекунд в формате std::string.
 */
std::string timeval_subtract(struct timeval* result, struct timeval* x, struct timeval* y);

/**
   \brief Установить дополнительные опции для сокета.
   Устанавливаются параметры SO_LINGER={0,0} и SO_REUSEADDR=1.
   \param sockfd - идентификатор сокета.
*/
void setSocketOptions(int sockfd);

/**
   \fn __tod_t _time_get_tod()
   \brief Получение текущего времени.
   \return текущее время в формате __tod_t.
 */
__tod_t _time_get_tod();

inline int getMyHostname()
{
     static int myHostName = 0;
     
     if (myHostName)
	  return myHostName;
     
     char s[64];

     for (int i = 1; i < 255; i++)
     {
	  sprintf(s, "ifconfig | grep \'192.168.0.%d \'  1>/dev/null", i);
	  
	  if (system(s) == 0)
	       return myHostName = i;
     }

     return 1;
}

std::string removeLastChar(std::ostringstream & os);
std::string removeLastChar(std::string str);

int my_system(const char *command);

#endif // UTIL_H
