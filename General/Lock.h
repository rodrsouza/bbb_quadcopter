/*
 * Lock.h
 *
 *  Created on: Jul 19, 2012
 *      Author: tiagom
 */

#ifndef LOCK_H_
#define LOCK_H_


class Mutex;

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
	explicit Lock(Mutex& aMutex, bool aWaitUntilBeLocked = true);

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

inline const bool Lock::IsLocked() const
{
	return m_locked;
}

#endif /* LOCK_H_ */
