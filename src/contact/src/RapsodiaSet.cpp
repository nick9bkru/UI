#include "RapsodiaSet.h"

RapsodiaSet::RapsodiaSet():InterfaceSet()
{
  for ( int j = 0; j < 4; j++)
    for ( int i = 1; i <= 16; i++)
	add(j,new SKLib::TCPSocketInterface( 50000 + i));
  SKLib::Log* Log =  &SKLib::LogSingleton::getInstance();
  Log->log("RapsodiaSet::mmap.size() = " + LexicalCaster(mmap.size()));

//         std::vector<SKLib::DataInterface *>::iterator iter = vec.begin();
//         while (iter++ != vec.end())
//       (*iter)->setTimeout(40000);
//        for_each(vec.begin(), vec.end(), mem_fun_ref(SKLib::TCPSocketInterface::setTimeout));
};
  
RapsodiaSet::~RapsodiaSet()
{
    
};

SKLib::DataInterface * RapsodiaSet::getInterfaceForApp(int num)
{   
    int  nport, ind, first_num;
    nport=(num-1) / 16;
    first_num= nport*16 + 1;
  SKLib::NPortDI::iterator it;  		 
    pair< SKLib::NPortDI::iterator, SKLib::NPortDI::iterator > p;	 
    p = mmap.equal_range( nport);	
	  
  ind=0;
  for ( it = p.first; it != p.second; it++) //может надо к p.second добавить 1 что бы видно было последнии элементы
  {  if( ind == (num - first_num ))
	    return(  it->second);
	ind++;
  }
  return  NULL;		
};