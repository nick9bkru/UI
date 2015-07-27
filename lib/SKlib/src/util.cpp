/**
   \file util.cpp
   \brief Реализация функций, описанных в util.h.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include "util.h"

std::string timeval_subtract(struct timeval* result, struct timeval* x, struct timeval* y)
{    
     // Perform the carry for the later subtraction by updating Y.
     if (x->tv_usec < y->tv_usec)
     {
	  int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
	  y->tv_usec -= 1000000 * nsec;
	  y->tv_sec += nsec;
     }

     if (x->tv_usec - y->tv_usec > 1000000)
     {
	  int nsec = (x->tv_usec - y->tv_usec) / 1000000;
	  y->tv_usec += 1000000 * nsec;
	  y->tv_sec -= nsec;
     }
     
     // Compute the time remaining to wait.
     // `tv_usec' is certainly positive.
     result->tv_sec = x->tv_sec - y->tv_sec;
     result->tv_usec = x->tv_usec - y->tv_usec;

     char s[32];
     sprintf(s, "%ld.%06ld sec", result->tv_sec, result->tv_usec);
     
     return std::string(s);
     
     // Return 1 if result is negative.
     // return x->tv_sec < y->tv_sec;
}

__tod_t _time_get_tod()
{
	struct timeval now;
	struct timezone tz;

	gettimeofday(&now, &tz);

	return ((__tod_t) now.tv_sec * 1000000 + now.tv_usec);
}

ushort toUshort(const char *str)
{
     char s[5];

     memcpy(s, str + 1, 4);
     s[4] = '\000';

     return strtol(s, NULL, 16);
}

ushort toUshortKulon(const char *str)
{
     char s[5];

     memcpy(s, str + 4, 4);
     s[4] = '\000';

     ushort n = strtol(s, NULL, 16);

     // For debug.
//     cout << "toUshortKulon(\"" << s << "\") = " << n <<
//	  ", source=\"" << std::string(str, strlen(str) - 1) << "\"" << endl;
     
     return n;	  
}

void setSocketOptions(int sockfd)
{
     struct linger lng = {0, 0};
     int flag = 1;

     setsockopt(sockfd, SOL_SOCKET, SO_LINGER, static_cast<void *>(&lng), sizeof(struct linger));
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&flag), sizeof(int));
}

int my_system(const char *command)
{
     const char * SHELL = "/bin/sh";
     int status;
     pid_t pid;
     
     pid = fork();
     if (pid == 0)
     {
	  // This is the child process. Execute the shell command.
//	  system(command);
	  execl(SHELL, SHELL, "-c", command, NULL);
	  _exit(EXIT_FAILURE);
     }
     else if (pid < 0)
     {
	  // The fork failed. Report failure.
	  status = -1;
     }
     else
     {
	    // This is the parent process. Wait for the child to complete.
	    if (waitpid(pid, &status, 0) != pid)
		 status = -1;
     }
     
     return status;
}

std::string removeLastChar(std::ostringstream & os)
{
     return removeLastChar(os.str());
}

std::string removeLastChar(std::string str)
{
     std::string s = str;
     s.resize(s.size() - 1);
     return s;
}
