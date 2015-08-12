#ifndef TCPRECIEVER_H
#define TCPRECIEVER_H

#include <cstring>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class tcpReciever
{
public:
    tcpReciever( const int port );
    ~tcpReciever();
    
    int recvMes( char * msg, const int len);
private:
    int sockfd;
    int make_socket(int port);
    
};

#endif // TCPRECIEVER_H
