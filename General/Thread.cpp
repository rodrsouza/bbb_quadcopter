/*
 * Thread.cpp
 *
 *  Created on: May 1, 2012
 *      Author: tiago
 *  Modified on: May 24, 2013
 *  	By: Rodrigo
 */
#include "Thread.h"
#include "Lock.h"
#include <stdio.h> 		/* for printf */
#include <stdlib.h>
#include <memory.h>		/* for memset */
#include <errno.h>
#include "Defs.h"

Thread::Thread(int aPriority, int aPolicy):
	m_running(0),
	m_shouldStop(0),
	m_tid(0)
{

	INIT_STRUCT(m_param);
	m_param.sched_priority = aPriority;

	pthread_attr_init(&m_attr);

	pthread_attr_setinheritsched(&m_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_attr_setscope(&m_attr, PTHREAD_SCOPE_SYSTEM);

	pthread_attr_setschedpolicy(&m_attr, aPolicy);

	pthread_attr_setschedparam(&m_attr, &m_param);

	pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);

}

Thread::~Thread()
{
	/* destroy an attribute */
	pthread_attr_destroy(&m_attr);

	this->Stop();
}

static void* runThread(void* arg)
{
    return ((Thread*)arg)->Run();
    pthread_exit(NULL);
}

int Thread::Start()
{
	int result = pthread_create(&m_tid, &m_attr, runThread, this);

	if( result == 0 )
	{
		m_running = 1;

		Lock myLock(m_shouldStopMutex, true);
		m_shouldStop = 0;
		myLock.Unlock();
	}

	return m_running;
}

void Thread::Stop()
{
	Lock myLock(m_shouldStopMutex, true);

	m_shouldStop = 1;
}

int Thread::Should_Stop()
{
	Lock myLock(m_shouldStopMutex, true);

	return m_shouldStop;
}


pthread_t * Thread::GetInstance ()
{
    return &m_tid;
}
