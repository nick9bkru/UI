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
	  std::cout << "~SingletonDestroyer() " << std::endl;
	  delete p_instance;
	};
	void initialize( S* p ) 
	{
	  std::cout << "initialize " << std::endl;
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
};
#endif // SINGLETON_H


