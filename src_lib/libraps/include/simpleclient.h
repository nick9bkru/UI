/**
   \file simpleclient.h
   \brief Описание функций для отправки сообщений.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#ifndef SIMPLECLIENT_H
#define SIMPLECLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "common_funcs.h"
#include <iostream>
/**
   \brief Отправляет сообщение по сети.
   \param dest_ip - IP-адрес получателя.
   \param dest_port - порт получателя.
   \param buf - указатель на начало области данных для передачи.
   \param len - размер данных для передачи.
   \return Результат передачи данных.
   \retval 1 - если успешно.
   \retval 0 - если неуспешно.
 */
int sendMessage(const std::string & dest_ip, uint16_t dest_port, ushort *buf, int len);

/**
   \brief Отправляет сообщение по сети (IP-адрес и порт используются по умолчанию).
   \param buf - указатель на начало области данных для передачи.
   \param len - размер данных для передачи.
   \return Результат передачи данных.
   \retval 1 - если успешно.
   \retval 0 - если неуспешно.
 */
int sendMessage(ushort *buf, int len);

/**
   \brief Отправляет сообщение по сети (IP-адрес и длина сообщения используются по умолчанию).
   \param dest_port - порт получателя.
   \param kg1 - ссылка на структуру данных для передачи.
   \return Результат передачи данных.
   \retval 1 - если успешно.
   \retval 0 - если неуспешно.
 */
int sendMessage(uint16_t dest_port, struct kg & kg1);

/**
   \brief Отправляет сообщение по сети (IP-адрес, порт и длина сообщения используются по умолчанию).
   \param kg1 - ссылка на структуру данных для передачи.
   \return Результат передачи данных.
   \retval 1 - если успешно.
   \retval 0 - если неуспешно.
 */
int sendMessage(struct kg & kg1);

/**
   \brief Получить последний октет в IP-адресе.
   \return последний октет.
 */
inline int getMyHostname();

// TODO: template.
/*
template <class T>
int sendMessage1(const std::string & dest_ip, uint16_t dest_port, T * buf, int len)
{
     std::cout << std::endl << "sendMessage1(): "
	  << "dest=\"" << dest_ip << ":" << dest_port << "\" "
	  << "len=" << len << std::endl;

     struct sockaddr_in my, dest;
     my.sin_family = AF_INET;
     my.sin_port = htons(7010);
     my.sin_addr.s_addr = inet_addr(RMISH_HOST.c_str());

     dest.sin_family = AF_INET;
     dest.sin_port = htons(dest_port);
     dest.sin_addr.s_addr = inet_addr(dest_ip.c_str());
     
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd == -1)
     {
	  perror("socket()");
	  return 0;
     }

     ret = connect(sockfd, reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
     if (ret < 0)
     {
	  perror("connect()");
	  close(sockfd);
	  return 0;
     }

     ret = send(sockfd, reinterpret_cast<void *>(buf), len, 0);
     if (ret < 0)
     {
     perror("send()");
     close(sockfd);
     return 0;
     }
     else
	  std::cout << "Success: ret=" << ret << " len=" << len << std::endl;     
     
     close(sockfd);
     
     return 1;
};
*/

#endif
