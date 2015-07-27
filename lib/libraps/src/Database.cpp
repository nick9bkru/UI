/**
   \file Database.h
   \brief Реализация класса Database.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include "Database.h"
#include <iostream.h>

Database::Database(const std::string & hostname): nTuples(-1), curTuple(-1), dbg(false)
{
     //conn = PQsetdb("", "5432", NULL, NULL, "raps_pgdb");
     conn = PQsetdbLogin(hostname.c_str(), "5432", NULL, NULL, "frag_pgdb", "root", "rootuser");

     cout << "Database(" << hostname << "): "
	  << "status ... " << (isReady() ? "OK" : "ERROR") << endl;
}

Database::~Database()
{
     this->close();
     cout << "~Database()" << endl;
}

inline bool Database::isReady () const
{
     return PQstatus(conn) == CONNECTION_OK ? true : false;;
}

bool Database::multipleSelect(const std::string & query, void* answer, int len)
{
     if (dbg)
	  cout << "Tag 1\n";
     
     if (nTuples < 0)
     {
	  if (dbg)
	       cout << "Tag 2\n";
	  
	  pgRes = PQexec(conn, query.c_str());

	  if (dbg)
	       cout << "Tag 3\n";
	  
	  if (PQresultStatus(pgRes) != PGRES_TUPLES_OK)
	  { 
	       cout << "Error! multipleSelect(): " << PQerrorMessage(conn) << endl;
	       return false;
	  }

	  if (dbg)
	       cout << "Tag 4\n";
	  
	  curTuple = 0;
	  nTuples = PQntuples(pgRes);

	  if (dbg)
	       cout << "Tag 5\n";
	  
	  if (dbg)
	       cout << "curTuple=" << curTuple << " nTuples=" << nTuples << endl;
     }

     if (dbg)
	  cout << "Tag 6\n";
     
     if (curTuple < nTuples)
     {
	  if (dbg)
	       cout << "Tag 7\n";

	  if (getAnswer(pgRes,  curTuple,  answer, len))
	       ++curTuple;

	  if (dbg)
	       cout << "Tag 8\n";
	  
	  return true;   
     }
     else
     {
	  if (dbg)
	       cout << "Tag 9\n";
	  
	  endQuery();
	  return false;
     }
}

void Database::close()
{
     cout << "before PQfinish(conn)\n";
     PQfinish(conn);
     cout << "after PQfinish(conn)\n";
}

bool Database::notSelect(const std::string & query)
{ 
     bool ok;

     if (dbg)
	  cout << query.c_str() << endl;
     
     pgRes = PQexec(conn, query.c_str());
     ok = (PQresultStatus(pgRes) == PGRES_COMMAND_OK);

     if (!ok)
	  cout << "Error! notSelect(): " << PQerrorMessage(conn) << endl;
     
     endQuery();
     
     return ok;
}

bool Database::singleSelect(const std::string & query, void *answer, int len)
{
     pgRes = PQexec(conn, query.c_str());
   
     if (PQresultStatus(pgRes) != PGRES_TUPLES_OK)
     {
	  cout << "Error! singleSelect(): " << PQerrorMessage(conn) << endl;
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
		    cout << "buffer=" <<  buffer << " size=" << size << endl; 
	  }
	  else if (PQfsize(res, i) == -1) // Для строк.
	  {
// 	       memcpy(rStr + size, PQgetvalue(res, cursor, i), strlen(PQgetvalue(res, cursor, i)));
// 	       size += strlen(PQgetvalue(res, cursor, i));

	       memcpy(rStr + size, PQgetvalue(res, cursor, i), PQgetlength(res, cursor, i));
	       size += PQgetlength(res, cursor, i);
	       if (dbg)
		    cout << "PQgetlength = " << PQgetlength(res, cursor, i) << endl;
	  }
	  else
	  {
	       if (dbg)
		    cout << "2" << endl;
//	       free(rStr);
	       return 0;  
	  }
     }
     
     memcpy(answer, rStr, len);
     free(rStr);
     
     if (dbg)
	  cout << "len=" << len << " size=" << size << endl;

     return 1;
}

inline void Database::endQuery()
{
     curTuple = -1;
     nTuples = -1;
     PQclear(pgRes);
     
     if (dbg)
	  cout << "endQuery(): " << curTuple << " " << nTuples << endl;
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
