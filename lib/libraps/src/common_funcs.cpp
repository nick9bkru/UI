/**
   \file common_funcs.cpp
   \brief Реализация функций, описанных в common_funcs.h.
   \author Кориков Сергей
   \version 0.1
   \date 2008-08-26
*/

#include "common_funcs.h"

ushort toPrivate(ushort sost, ushort io, int type)
{
     ushort private_sost = 0;
     ushort j = 0;
     
     //  Индикация (скоре всего не пригодится)
     if (io == IN)
     {
	  for (j = 0; j < 16; j++)
	  {
	       if ( (sost & inds[0][j]) && inds[0][j] )
		    private_sost |= inds[type][j];
	  }
	  return private_sost;
     }
     // Команды
     else if (io == OUT)
     {	  
	  for (j = 0; j < 16; j++)
	  {
	       if ( (sost & cmds[0][j]) && cmds[0][j] )
		    private_sost |= cmds[type][j];
	  }
	  return private_sost;
     }
     else
	  return private_sost;
}

ushort toDefault(ushort sost, ushort io, int type)
{
     ushort app_sost = 0;
     ushort j = 0;
     
     //  Индикация
     if (io == IN)
     {
	  for (j = 0; j < 16; j++)
	  {
	       if ( (sost & inds[type][j]) && inds[type][j] )
		    app_sost |= inds[0][j];
	  }
	  return app_sost;
     }
     // Команды
     else if (io == OUT)
     {	  
	  for (j = 0; j < 16; j++)
	  {
	       if ( (sost & cmds[type][j]) && cmds[type][j] )
		    app_sost |= cmds[0][j];
	  }
	  return app_sost;
     }
     else
	  return app_sost;
}
