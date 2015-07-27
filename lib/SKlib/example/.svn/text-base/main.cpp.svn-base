#include "DataInterface.h"
#include "TCPSocketInterface.h"
#include "InterfaceSet.h"

enum appType
{
     Argun = 10,
     Delta = 20,
     Kulon = 30,
     Serdolik = 40
};

// vec[i] = 0..102
// 0-95  - Kulon.
// 96,97 - Data,Freq - USO-SH-1-1
// 98,99 - Data,Freq - USO-SH-1-2
// 100   - Data      - USO-SH-2-1
// 101   - Data      - USO-SH-2-2
// 102   - Data      - USO-SH-2-3
//
class RapsodiaSet : public SKLib::InterfaceSet
{
public:
     RapsodiaSet():InterfaceSet()
	  {
	       char s[32];

	       for (int n = 10; n <= 17; n++)
	       {
		    sprintf(s, "192.168.1.%d", n);
		    for (int i = 1; i <= 12; i++)
			 add(new SKLib::TCPSocketInterface(s, 50000 + i));
	       }

	       add(new SKLib::TCPSocketInterface("192.168.1.13", 50013));
	       add(new SKLib::TCPSocketInterface("192.168.1.13", 50016));
	       add(new SKLib::TCPSocketInterface("192.168.1.14", 50013));
	       add(new SKLib::TCPSocketInterface("192.168.1.14", 50016));
	       add(new SKLib::TCPSocketInterface("192.168.1.13", 50014));
	       add(new SKLib::TCPSocketInterface("192.168.1.13", 50015));
	       add(new SKLib::TCPSocketInterface("192.168.1.14", 50014));
	  };
     ~RapsodiaSet() {};
     SKLib::DataInterface * getInterfaceForApp(int num, int code, bool isFreq = false)
	  {
	       if ((num >= 1 && num <= 96) && code == Kulon)
		    return vec[num - 1];

	       if ((num >= 97 && num <= 100) && code == Argun)
		    return isFreq ? vec[97] : vec[96];

	       if ((num >= 101 && num <= 104) && code == Delta)
		    return isFreq ? vec[99] : vec[98];

	       if ((num >= 105 && num <= 112) && code == Serdolik)
		    return vec[100];

	       if ((num >= 113 && num <= 120) && code == Serdolik)
		    return vec[101];

	       if ((num >= 121 && num <= 128) && code == Serdolik)
		    return vec[102];

	       return NULL;
	  };
};

int main (int argc, char *argv[])
{
     RapsodiaSet * m = new RapsodiaSet();
     m->start();
     m->getInterfaceForApp(2, 10);

//      Apparate *app = new Apparate(...);
//      app->setInterface(m->getInterfaceForApp(app->getNum(), app->getType());

     while (1)
     {
	  cout << getpid() << endl;
	  sleep(1);
     }

     delete m;
     return 1;
}
