/*
 * Thread.h
 *
 *  Created on: May 1, 2012
 *      Author: tiago
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include "Lock.h"
#include "Mutex.h"
#include <Priority.h>

class Thread
{
public:
	Thread(int aPriority, int aPolicy);
	virtual ~Thread();

	int Start();
	pthread_t* GetInstance ();
	pthread_t GetThreadID();

	virtual void* Run() = 0;
	void Stop();

	int Should_Stop();

private:
	int 			m_running;
	pthread_t  		m_tid;

	pthread_attr_t 	m_attr;
	sched_param 	m_param;

	Mutex			m_shouldStopMutex;
	int				m_shouldStop;
};

#endif /* THREAD_H_ */
