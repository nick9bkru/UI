#ifndef PMUTEXLOCKER_H
#define PMUTEXLOCKER_H
#include <pthread.h>
#include <string>

/** @class Mutex
	@brief Мьютекс
*/
class Mutex {
public:
	Mutex() ;
	Mutex(const std::string& _description) ;
	Mutex(const Mutex& m) ;
	~Mutex();

	friend class MutexLocker;
private:
	Mutex& operator=(const Mutex&);
	/** @brief Мьютекс операционной системы */
	pthread_mutex_t mtx;
	/** @brief Описание мьютекса */
	std::string description;
};

/** @class MutexLocker
	@brief Класс, управляющий блокированием мьютексов
*/
class MutexLocker {
public:
	explicit MutexLocker(Mutex& _mtx);
	~MutexLocker();

private:
	MutexLocker(const MutexLocker&);
	/** @brief Оператор присваивания */
	MutexLocker& operator=(const MutexLocker&);

	/** @brief Мьютекс */
	Mutex& mtx;

};

class PMutexLocker
{
public:
  PMutexLocker(pthread_mutex_t &_mutex);
  ~PMutexLocker();
private:
  
	/** @brief Оператор присваивания */
	PMutexLocker& operator=(const PMutexLocker&);
 // PMutexLocker ( const PMutexLocker & ) {};
	pthread_mutex_t &mutex;
};

#endif // PMUTEXLOCKER_H
