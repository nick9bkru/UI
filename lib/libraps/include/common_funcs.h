/**
   \file common_funcs.h
   \brief Содержит описание функций общего использования, типов данных, необходимых для работы программы.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H

#include <string>
#include <sys/time.h>
#include <sys/socket.h>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>

#include "defines.h"
#include "simpleclient.h"

/**
   \brief Преобразует APP-значение команды или индикации к частному.
   \param sost - APP-значение.
   \param io - IN или OUT.
   \param type - значение из APP_ID.
   \return частное значение для APP-значения sost.
*/
ushort toPrivate(ushort sost, ushort io, int type);

/**
   \brief Преобразует частное значение команды или индикации к APP-значению.
   \param sost - APP-значение.
   \param io - IN или OUT.
   \param type - значение из APP_ID.
   \return APP-значение для частного значения sost.
*/
ushort toDefault(ushort sost, ushort io, int type);     

#endif // COMMON_FUNCS_H
