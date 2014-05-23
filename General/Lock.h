/*
 * Lock.h
 *
 *  Created on: Jul 19, 2012
 *      Author: tiagom
 */

#ifndef LOCK_H_
#define LOCK_H_

#include "Mutex.h"

/**
 *    This class handles the Mutex object.
 *    When efficiency is not an issue it must be always used in the place
 * of direct accessing the Mutex functions.
 */
class Lock {
public:

	/**
	 *    This constructor locks the mutex. It offer the option to wait the
	 * lock be available or just try to lock without been blocked.
	 *
	 * @param[in] aMutex The pointer to the mutex to be locked.
	 * @param[in] aWaitUntilBeLocked true: The thread wait the the lock
	 * be available. false: Tries to lock, but if the lock is busy it will
	 * not block the thread.
	 */
	explicit Lock(Mutex& aMutex, bool aWaitUntilBeLocked);

	explicit Lock(Mutex& aMutex);

	/**
	 *    If the mutex is locked at the end of the scope the destructor will unlock it.
	 */
	~Lock();

	/**
	 *    This function allows you to unlock the mutex before the lock object
	 * reach the end of the scope.
	 */
	const int  Unlock();

	/**
	 *
	 * @return True if the mutex is locked, False if it is not.
	 */
	const bool IsLocked() const;


	const bool LockAgain(bool aWaitUntilBeLocked = true);


private:
	Mutex* m_mutex;
	bool m_locked;
};

inline Lock::Lock(Mutex& aMutex, bool aWaitUntilBeLocked) :
	m_mutex(&aMutex),
	m_locked(false)
{
	int success = 0;
	if(aWaitUntilBeLocked)
	{
		success = m_mutex->Lock();
		m_locked = (success == 0);
	}
	else
	{
		success = m_mutex->TryToLock();
		m_locked = (success == 0);
	}
}

inline Lock::Lock(Mutex& aMutex) :
	m_mutex(&aMutex),
	m_locked(false)
{
	int success = m_mutex->Lock();
	m_locked = (success == 0);
}

inline const bool Lock::IsLocked() const
{
	return m_locked;
}

inline Lock::~Lock()
{
	if(m_locked)
	{
		m_mutex->Unlock();
	}
}

#endif /* LOCK_H_ */
