/*
 * Mutex.h
 *
 *  Created on: Jul 19, 2012
 *      Author: tiagom
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h> /* for mutex*/


/**
 *    This class implements a Mutex with the pthread_mutex object and functions.
 */
class Mutex {
public:
	/**
	 *    The constructor initialize the mutex.
	 */
	explicit Mutex();

	/**
	 *    The destructor destroy the mutex.
	 */
	~Mutex();

	pthread_mutex_t m_mutex;

	/**
	 *    Lock the mutex.
	 * @return 0 in case of success and non zero otherwise.
	 */
	int Lock();

	/**
	 *    Try to Lock the mutex.
	 * @return 0 in case of success, EBUSY if some one else is holding the lock or
	 * other values in case of error.
	 */
	int TryToLock();

	/**
	 *    Ulock the mutex.
	 * @return 0 in case of success and non zero otherwise.
	 */
	int Unlock();

};

inline int Mutex::Lock()
{
	return pthread_mutex_lock(&m_mutex);
}

inline int Mutex::TryToLock()
{
	return pthread_mutex_trylock(&m_mutex);
}

inline int Mutex::Unlock()
{
	return pthread_mutex_unlock(&m_mutex);
}


#endif /* MUTEX_H_ */
