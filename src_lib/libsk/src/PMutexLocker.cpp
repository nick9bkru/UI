#include "PMutexLocker.h"

Mutex::Mutex() : description("")
{
		pthread_mutex_init(&mtx, NULL);
}

Mutex::Mutex(const std::string& _description) : description(_description)
{
		pthread_mutex_init(&mtx, NULL);
}

Mutex::Mutex(const Mutex& m) : description(m.description) 
{
	pthread_mutex_init(&mtx, NULL);
}

Mutex::~Mutex() 
{
	pthread_mutex_destroy(&mtx);
}
/////////////////////////////////////////////////////////////////
MutexLocker::MutexLocker(Mutex& _mtx) : mtx(_mtx)
{
	pthread_mutex_lock(&mtx.mtx);
}

MutexLocker::~MutexLocker()
{
	pthread_mutex_unlock(&mtx.mtx);
}
/////////////////////////////////////////////////////////////////


PMutexLocker::PMutexLocker(pthread_mutex_t &_mutex) : mutex (_mutex)
{
  pthread_mutex_lock(&mutex);
}

PMutexLocker::~PMutexLocker()
{
  pthread_mutex_unlock(&mutex);
};

