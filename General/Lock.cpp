/*
 * Lock.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: tiagom
 */

#include "Lock.h"
#include "Mutex.h"
#include <errno.h>
#include <stdio.h>		/* for CHECK, fread*/
#include <stdlib.h>		/* for CHECK , strtoul */





const bool Lock::LockAgain(bool aWaitUntilBeLocked)
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

	return (success == 0);
}

const int Lock::Unlock()
{
	int success = 0;
	if(m_locked)
	{
		success = m_mutex->Unlock();
		m_locked = !(success == 0);
	}
	else
	{
		success = ENOLCK;
	}

	return success;
}
