/**
   \file TDcmInteface.h
   \brief Описание класса TDcmInteface.
   \author Жадовский Николай
*/
#ifndef TDCMINTERFACE_H
#define TDCMINTERFACE_H

#include "DataInterface.h"
#include <string>
#include <iostream>
#include "TDcmCli.h"
//using namespace std;
extern SKLib::Log log;

class  TDcmInteface : public TDcmCli
{
public:
  TDcmInteface (std::string _ipAddr, unsigned short int _port) ;
 ~TDcmInteface ();
 //пустоу метод
  virtual int open(const std::string & ipAddr);
 //пустоу метод
  virtual int getAnswerForCommand(const std::string & cmdString, void *answer, int size);
  std::string getInfo() const;
  private:
   std::string ipAddr ;
 // int getAnswerForCommand(const std::string & cmdString, void *answer, int size) ;
};

#endif
