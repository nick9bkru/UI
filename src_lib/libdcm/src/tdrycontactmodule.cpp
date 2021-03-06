/**
\file tdrycontactmodule.cpp
\brief Реализация класса TDryContactModule.
\author Гусинская Е.И.
\version 2010-02-01
*/

#include "tdrycontactmodule.h"
//#include <stdstr>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;

namespace _dcm
{
   TDryContactModule::TDryContactModule( std::string _ipAddr, unsigned short int _port )
      : ipAddr(_ipAddr), port(_port), sock(-1)
   {
      signal(SIGPIPE, SIG_IGN);
      dcmAvailState = 0;
//       initUsedStates();//говорят так нельзя - перенес в init()
   }

   TDryContactModule::~TDryContactModule()
   {
      disconnect();
      signal(SIGPIPE, SIG_DFL);
   }

   void TDryContactModule::init()
   {
//       cout << "init TDryContactModule" << endl;
      initUsedStates();
      notConnState = static_cast<int>(pow(2.0,usedStates - 1));
      
      cout << "init TDryContactModule notConnState = " << notConnState << endl;
      bitMask = static_cast<int>(pow(2.0,usedStates)) - 1; //должны получиться все единички в числе бит на
                                                           //единицу больше, чем используемых позиций
                                                           //например если используется 8 позиций (кнопок КСК)
                                                           //маска будет:
                                                           //0000 0001 1111 1111

      dcmState = bitMask;//0xF; // первоначальное состояние вскрытия дверей - все нормально.....
   };

   int TDryContactModule::connect()
   {
      sock = socket(AF_INET, SOCK_STREAM, 0);

      if (sock == -1)
      {
         perror("socket()");
         return -1;
      }

      struct sockaddr_in dest;

      dest.sin_family = AF_INET;
      dest.sin_port = htons(port);
      dest.sin_addr.s_addr = inet_addr(ipAddr.c_str());

      int ret = ::connect(sock, reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
//       cout << "connect ret = " << ret << endl;
      if (ret == -1)
      {
         perror("connect()");
         ::close(sock);
         sock = -1;
      }
      cout<<"TDryContactModule::connect() IP == " << ipAddr << " OK"<<endl;
      return ret;
   }

   void TDryContactModule::disconnect()
   {
      if (sock < 0)
      return;
#ifdef DEBUG
      cout<<"TDryContactModule::disconnect() IP == " << ipAddr<<endl;
#endif
      int ret = shutdown(sock, 2);
      if (ret == -1)
         perror("shutdown()");

      ret = ::close(sock);
      if (ret == -1)
         perror("close()");

      sock = -1;
   }

   int TDryContactModule::getState() // была const
   {
//       char cmd[] = "@01\r";
      char cmd[] = "get\r";
      int mesSize = 1;
      int bufSize = mesSize + 1;
      
      char buf[bufSize];
      int state = -1, ret = -1;
//       const int size = 7, offset = 4;

      if (!isConnected())  //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
      {
         if (connect() == -1)
         {
           // std::cout << "(connect() == -1)" << std::endl;
            //state = state & (!notConnState);
            return -1;
         }
      };

      ret = send(sock, cmd, strlen(cmd), 0);
//       std::cout << "ret = " << ret  << "sock=" << sock << std::endl;
      if (ret == -1)
      {
#ifdef DEBUG
         perror("getState() send()");
#endif
         //state = state & (!notConnState);
         return -1;
      }

      //std::cout << "dcm usleep!!!!!!!!!!" << std::endl;
      usleep(TIME_RECV);

//       int dbgBufSize = 1024;
//       char dbgBuf[dbgBufSize];
//       memset(dbgBuf, 0, dbgBufSize);
      
      ret = recv(sock, buf, bufSize, MSG_DONTWAIT);
     // cout << "ret = " << ret << endl;
      if (ret == mesSize)
      {
         buf[mesSize] = '\0';
        /* for(int i=0; i< ret; i++)
         {
            cout << hex << (unsigned int) buf[i] << "|";
         }
         cout << dec << endl;*/
//          state = strtol(&buf[offset], NULL, 16);
         state = static_cast<int>( buf[0] );
	 if (state==-1) state=0xff;
     //     cout << "state in getState = 0x" << state << endl;
      }
      else if (ret == -1)
      {
#ifdef DEBUG
         perror("getState() recv()");
#endif
         //std::cout << "errno" << errno << "ESPIPE=" << ESPIPE << std::endl;
      }
      else
         std::cout << "GET:: Too big answer! ret == " << ret << " buf[0] ="
          << static_cast<int>( buf[0] ) << " buf[1]" << static_cast<int>( buf[1] )<< std::endl;


      if (state == -1)    //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
         disconnect();
//       std::cout << "state = " << state << std::endl;
      return state;
   }

   int TDryContactModule::gtsState() // была const
   {
//       char cmd[] = "@01\r";

      char cmd[] = "gts\r";
      int mesSize = 1;
      int bufSize = mesSize + 1;
      
      char buf[bufSize];
      int state = -1, ret = -1;

      if (!isConnected())  //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
      {
         if (connect() == -1)
         {
          //  std::cout << "(connect() == -1)" << std::endl;
	    sleep(1);
            return -1;
         }
      };
      ret = send(sock, cmd, strlen(cmd), 0);
      if (ret == -1)
      {
#ifdef DEBUG
         perror("gtsState()  send()");
#endif
         return -1;
      }

      usleep(TIME_RECV);
      
      ret = recv(sock, buf, bufSize, MSG_DONTWAIT);
      if (ret == mesSize)
      {
         buf[mesSize] = '\0';
         state = static_cast<int>( buf[0] );
	 if (state==-1) state=0xff;
         //cout << "state in getState = 0x" <<state << endl;
      }
      else if (ret == -1)
      {
#ifdef DEBUG
         perror("gtsState()  recv()");
#endif
      }
      else
         std::cout << "GTS:: Too big answer!" << std::endl;

      if (state == -1)    //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
         disconnect();
      return state;
   }
   unsigned int TDryContactModule::gtcState(unsigned char state ) // была const
   {
//       char cmd[] = "@01\r";
      const int cmdSize = 5;
      unsigned char cmd[cmdSize] = {'g','t','c',state,'\r'};
      int mesSize = 4;
      int bufSize = mesSize + 1;
      const unsigned int NO_DATA= 0xFFFFFFFF;
      
      char buf[bufSize];
      int ret = -1;
      unsigned int rcvState = NO_DATA;
//       const int size = 7, offset = 4;

      if (!isConnected())  //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
      {
         if (connect() == -1)
         {
          //  std::cout << "(connect() == -1)" << std::endl;
            sleep(1);
		return -1;
         }
      };

      ret = send(sock, cmd, cmdSize, 0);
      if (ret == -1)
      {
#ifdef DEBUG
         perror("gtcState send()");
#endif
         return -1;
      }

      usleep(TIME_RECV);

      
      ret = recv(sock, buf, bufSize, MSG_DONTWAIT);
      if (ret == mesSize)
      {
         buf[mesSize] = '\0';
 /*       for(int i=0; i< ret; i++)
         {
            cout << hex << (((unsigned int) buf[i]) & 0XFF) << "|";
         }
         cout << dec << endl;*/
//          state = strtol(&buf[offset], NULL, 16);
         rcvState=0;
	 for (int i=0; i<ret;i++) rcvState|= (((unsigned int) buf[i]) & 0xFF) << ((ret-1-i)*8);
         rcvState&=0xFFFFFF;
//       cout << "state in gtcState = 0x" << hex << rcvState << endl;
      }
      else if (ret == -1)
      {
#ifdef DEBUG
         perror("gtcState recv()");
#endif
         //std::cout << "errno" << errno << "ESPIPE=" << ESPIPE << std::endl;
      }
      else
         std::cout << "GTC:: Too big answer!" << std::endl;


      if (rcvState == NO_DATA)    //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
         disconnect();
//       std::cout << "state = " << state << std::endl;
      return rcvState;
   }

   bool TDryContactModule::setState(unsigned char state)
   {
      const int cmdSize = 5;
      unsigned char cmd[cmdSize] = {'s','e','t',state,'\r'};
      int mesSize = 1;
      int bufSize = mesSize + 1;
      
      char buf[bufSize];
      int rcvState = -1, ret = -1;

      if (!isConnected())  //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
      {
         if (connect() == -1)
         {
//             std::cout << "(connect() == -1)" << std::endl;
		sleep(1);
            return -1;
         }
      };

      ret = send(sock, cmd, cmdSize, 0);
      if (ret == -1)
      {
#ifdef DEBUG
         perror("setState send()");
#endif
         return -1;
      }

      usleep(TIME_RECV);
    
      ret = recv(sock, buf, bufSize, MSG_DONTWAIT);
//       cout << "ret = " << ret << endl;
      if (ret == mesSize)
      {
         buf[mesSize] = '\0';
        /* for(int i=0; i< ret; i++)
         {
            cout << hex << (unsigned int) buf[i] << "|";
         }
         cout << dec << endl;*/
//          state = strtol(&buf[offset], NULL, 16);
         rcvState = static_cast<unsigned int>( buf[0] );
//          cout << "state in getState = 0x" << hex << rcvState << endl;
         if (rcvState != state)
            return false;
      }
      else if (ret == -1)
      {
#ifdef DEBUG
         perror("setState recv()");
#endif
         //std::cout << "errno" << errno << "ESPIPE=" << ESPIPE << std::endl;
      }
      else
         std::cout << "SET:: Too big answer!" << std::endl;


      if (rcvState == -1)    //Этого здесь не было - добавлено из main, чтобы можно было взять state одной командой
         disconnect();
//       std::cout << "state = " << state << std::endl;
      return true;
   }

   bool TDryContactModule::isConnected () const
   {
//std::cout<<"Connected == "<<sock <<endl;
      return sock >= 0 ? true : false;
   }

   void TDryContactModule::getAndProcessState()
   {
      int state = -1;
      state = getState();

//       if        // -1 - если произошло что-то на сухих контактах
//       {
//          state = 0;
// cout << "tdry  TDryContactModule::getAndProcessState()     state == 0 !!!!!!" << endl;
//       }
//       else
      if ( ( ( state | notConnState ) != dcmState ) || ( state == -1 ) )
      {
         processDCMState ( state );
      }
      else
      {
         std::cout << "Nothing happens " << std::endl;
      };
   };

   void TDryContactModule::processDCMState ( int state )
   {
      if (state < 0)
      {
//          state = 0;
         dcmState = 0;
//          cout << "dcmRouter1 GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG" << endl;
         endProcessDcmState();
         return;
      };
// std::cout << "state in processDCMState= 0x" << state << " usedStates = " << usedStates << std::endl;
      state = state | notConnState;
// std::cout << "state | notConnState = 0x" << state << " usedStates = " << usedStates << std::endl;
      
//       dcmAvailState = ( ( state >> usedStates ) & 0x0001);

//       std::cout << "usedStates = " << bin << usedStates << dec << std::endl;
//        std::cout << "dcmAvailState = " << dcmAvailState << std::endl;

      int stateChange;
      stateChange = state ^ dcmState;
      for(int i = 0; i < usedStates - 1; i++)
      {
         if ((stateChange >> i) & 0x0001)
            processState(i + 1, (state >> i) & 0x0001);
      }

//       if (dcmAvailState)
//          dcmState = state | notConnState;
//       else
      dcmState = state;
//       cout << "dcmState | = " << dcmState << "state =" << state << " notConnState = " << notConnState << endl;
      endProcessDcmState();
   };

   void TDryContactModule::processState( int statePos, bool state)
   {
      switch ( statePos )
      {
         case 0:
            processFirstState(state);
         break;
         case 1:
            processSecondState(state);
         break;
         case 2:
            processThirdState(state);
         break;
         case 3:
            processFourthState(state);
         break;
         case 4:
            processFifthState(state);
         break;
         case 5:
            processSixthState(state);
         break;
         case 6:
            processSeventhState(state);
         break;
         case 7:
            processEighthState(state);
         break;
         default:
         break;
      };
   };
   void TDryContactModule::processFirstState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processSecondState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processThirdState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processFourthState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processFifthState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processSixthState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processSeventhState(bool state)
   {
      if(state)
         return;
   };
   void TDryContactModule::processEighthState(bool state)
   {
      if(state)
         return;
   };
};

