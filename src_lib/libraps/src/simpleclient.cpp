/**
   \file simpleclient.cpp
   \brief Реализация функций для отправки сообщений.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include "simpleclient.h"


int sendMessage(const std::string & dest_ip, uint16_t dest_port, ushort *buf, int len)
{
     std::cout << std::endl << "sendMessage(): "
	  << "dest=\"" << dest_ip << ":" << dest_port << "\" "
	  << "buf[" << len/2 << "]=\"";     
     for (int j = 0; j < len/2; j++)
	  std::cout << buf[j] << " ";     
     std::cout << "\"" << std::endl;

     struct sockaddr_in dest;
   
     dest.sin_family = AF_INET;
     dest.sin_port = htons(dest_port);
     dest.sin_addr.s_addr = inet_addr(dest_ip.c_str());
     
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd == -1)
     {
	  perror("socket()");
	  return 0;
     }

     struct linger lng = {0, 0};
     int flag = 1;

     setsockopt(sockfd, SOL_SOCKET, SO_LINGER, static_cast<void *>(&lng), sizeof(struct linger));
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&flag), sizeof(int));

     int ret = connect(sockfd, reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
     if (ret == -1)
     {
	  perror("connect()");
	  close(sockfd);
	  return 0;
     }

     ret = send(sockfd, buf, len, 0);
     if (ret == -1)
     {
	  perror("send()");
	  close(sockfd);
	  return 0;
     }
     else
	  std::cout << "Success: ret=" << ret << " len=" << len << std::endl;

     close(sockfd);     
     return 1;
}

int sendMessage(uint16_t dest_port, struct kg & kg1)
{
     return sendMessage(CUU_HOST, dest_port, reinterpret_cast<ushort *>(&kg1), sizeof(struct kg));
}

int sendMessage(struct kg & kg1)
{
     return sendMessage(7000, kg1);
}

int sendMessage(ushort *buf, int len)
{
     return sendMessage(CUU_HOST, 7000, buf, len);
}

inline int getMyHostname()
{
     static int myHostName = 0;
     
     if (myHostName)
	  return myHostName;
     
     char s[64];

     for (int i = 1; i < 255; i++)
     {
	  sprintf(s, "ifconfig | grep \'192.168.1.%d \' 1>/dev/null", i);
	  
	  if (system(s) == 0)
	       return myHostName = i;
     }

     return 1;
}
