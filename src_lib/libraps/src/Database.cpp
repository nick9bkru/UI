/**
   \file Database.h
   \brief Реализация класса Database.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include "Database.h"
#include <iostream>

Database::Database(const std::string  hostname, const std::string dbname , const std::string user , const std::string pass): nTuples(-1), curTuple(-1), dbg(false)
{

     conn = PQsetdbLogin(hostname.c_str(), "5432", NULL, NULL, dbname.c_str(), user.c_str(), pass.c_str());

     std::cout << "Database(" << hostname << "): "
	  << "status ... " << (isReady() ? "OK" : "ERROR") << std::endl;
}

Database::~Database()
{
     this->close();
     std::cout << "~Database()" << std::endl;
}

bool Database::isReady () const
{
     return PQstatus(conn) == CONNECTION_OK ? true : false;;
}

bool Database::multipleSelect(const std::string & query, void* answer, int len)
{
     if (dbg)
	  std::cout << "Tag 1\n";
     
     if (nTuples < 0)
     {
	  if (dbg)
	       std::cout << "Tag 2\n";
	  
	  pgRes = PQexec(conn, query.c_str());

	  if (dbg)
	       std::cout << "Tag 3\n";
	  
	  if (PQresultStatus(pgRes) != PGRES_TUPLES_OK)
	  { 
	       std::cout << "Error! multipleSelect(): " << PQerrorMessage(conn) << std::endl;
	       return false;
	  }

	  if (dbg)
	       std::cout << "Tag 4\n";
	  
	  curTuple = 0;
	  nTuples = PQntuples(pgRes);

	  if (dbg)
	       std::cout << "Tag 5\n";
	  
	  if (dbg)
	       std::cout << "curTuple=" << curTuple << " nTuples=" << nTuples << std::endl;
     }

     if (dbg)
	  std::cout << "Tag 6\n";
     
     if (curTuple < nTuples)
     {
	  if (dbg)
	       std::cout << "Tag 7\n";

	  if (getAnswer(pgRes,  curTuple,  answer, len))
	       ++curTuple;

	  if (dbg)
	       std::cout << "Tag 8\n";
	  
	  return true;   
     }
     else
     {
	  if (dbg)
	       std::cout << "Tag 9\n";
	  
	  endQuery();
	  return false;
     }
}

void Database::close()
{
     std::cout << "before PQfinish(conn)\n";
     PQfinish(conn);
     std::cout << "after PQfinish(conn)\n";
}

bool Database::notSelect(const std::string & query)
{ 
     bool ok;

     if (dbg)
	  std::cout << query.c_str() << std::endl;
     
     pgRes = PQexec(conn, query.c_str());
     ok = (PQresultStatus(pgRes) == PGRES_COMMAND_OK);

     if (!ok)
	  std::cout << "Error! notSelect(): " << PQerrorMessage(conn) << std::endl;
     
     endQuery();
     
     return ok;
}

bool Database::singleSelect(const std::string & query, void *answer, int len)
{
     pgRes = PQexec(conn, query.c_str());
   
     if (PQresultStatus(pgRes) != PGRES_TUPLES_OK)
     {
	  std::cout << "Error! singleSelect(): " << PQerrorMessage(conn) << std::endl;
	  endQuery();
	  return false;
     }
  
     nTuples = PQntuples(pgRes);
     
     if (nTuples == 0)
     {
	  endQuery();
	  return false;
     }
     
     getAnswer(pgRes, 0,  answer, len);

     endQuery();
     return  true; 
}

int Database::getAnswer(PGresult *res, int cursor,  void* answer ,int len)
{
     int size = 0, buffer, i ;
     char *rStr = (char *)malloc(len);

     memset(rStr, 0, len);
     
     for (i = 0; i < PQnfields(res); i++)
     {  
	  if (PQfsize(res, i) > 0) // Для целых PQftypes(res, i)== 20(INT8) 21(INT2) 23(INT4).
	  { 
	       buffer = atoi(PQgetvalue(res, cursor, i));
	       memcpy(rStr + size, &buffer, PQfsize(res, i));
	       size += PQfsize(res, i);

	       if (dbg)
		    std::cout << "buffer=" <<  buffer << " size=" << size << std::endl; 
	  }
	  else if (PQfsize(res, i) == -1) // Для строк.
	  {
// 	       memcpy(rStr + size, PQgetvalue(res, cursor, i), strlen(PQgetvalue(res, cursor, i)));
// 	       size += strlen(PQgetvalue(res, cursor, i));

	       memcpy(rStr + size, PQgetvalue(res, cursor, i), PQgetlength(res, cursor, i));
	       size += PQgetlength(res, cursor, i);
	       if (dbg)
		    std::cout << "PQgetlength = " << PQgetlength(res, cursor, i) << std::endl;
	  }
	  else
	  {
	       if (dbg)
		    std::cout << "2" << std::endl;
//	       free(rStr);
	       return 0;  
	  }
     }
     
     memcpy(answer, rStr, len);
     free(rStr);
     
     if (dbg)
	  std::cout << "len=" << len << " size=" << size << std::endl;

     return 1;
}

inline void Database::endQuery()
{
     curTuple = -1;
     nTuples = -1;
     PQclear(pgRes);
     
     if (dbg)
	  std::cout << "endQuery(): " << curTuple << " " << nTuples << std::endl;
}

void Database::setDebugMode(bool _dbg)
{
     dbg = _dbg;
}

//-----------------------
// Other useful functions
//-----------------------

// Создание сокета.
int make_socket(uint16_t port)
{
     int sock;
     struct sockaddr_in name;
     
     // Create the socket
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
	  perror("socket");
	  exit(EXIT_FAILURE);
     }
     
     // Give the socket a name
     name.sin_family = AF_INET;
     name.sin_port = htons(port);
     name.sin_addr.s_addr = htonl(INADDR_ANY);
     if (bind(sock, reinterpret_cast<struct sockaddr *>(&name), sizeof(name)) < 0)
     {
	  perror("bind");
	  exit(EXIT_FAILURE);
     }
     
     return sock;
}
