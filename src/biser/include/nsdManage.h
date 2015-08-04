#ifndef NSDMANAGE_H
#define NSDMANAGE_H

#include "BDReadConf.h"
#include "BDStateNsd.h"

#include "DcmNsd.h"
#include "DcmUps.h"
#include "TCPManage.h"

/** @brief Класс опрашивает все уи
*/

class nsdManage
{
public:
  nsdManage(Database * _db);
  ~nsdManage();
  void init();
  void start ();
  void stop ();
  UIservice::TCPManage::UiVec getUiVec() const;
private:
  bool IsWork;
  Database * db;
  BDStateNsd* BDNsd;
  BDStateUps* BDUps;

  UIservice::TCPManage::UiVec Dcm;
  //очищаем вектор
  void cleaVecDcm();
  int selectAll ();
};

#endif // NSDMANAGE_H
