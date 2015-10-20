#ifndef SINGLETON_H
#define SINGLETON_H


namespace SKLib
{
    template < typename S>
    class SingletonDestroyer
    {
      private:
	S* p_instance;
      public:    
	~SingletonDestroyer() 
	{
	  delete p_instance;
	};
	void initialize( S* p ) 
	{
	  p_instance = p;
	};
    };
    
    /** class singleton
	    @brief Реализация шаблона проектирования "Одиночка"
    */
    template < typename T>
    class Singleton
    {
      protected:
	static T* p_instance;
	static SingletonDestroyer<T> destroyer;
      private:
	
	Singleton();
	Singleton(const Singleton& );
	~Singleton();
 	Singleton& operator=(const Singleton& ) ;
      public:
	static T& getInstance( );
	static void init ( T * p );
	
    };
    
    template < typename T>
    T* Singleton<T>::p_instance = 0;
    
    template < typename T>    
    SingletonDestroyer<T> Singleton<T>::destroyer;
    
    template < typename T>
    T& Singleton<T>::getInstance( )
    {
	  if(!p_instance)     
	  {
	      p_instance = new T;
	      destroyer.initialize( p_instance);     
	  }
	  return *p_instance;
    };
    
    template < typename T>
    void Singleton<T>::init ( T * p )
    {
       if(!p_instance)     
       {
	      p_instance = p;
	      destroyer.initialize( p);     
       } 
       else 
	 throw ( std::string ( "void Singleton<T>::init ( T * p ) error !!! "));
    };
};
#endif // SINGLETON_H


