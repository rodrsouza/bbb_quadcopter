/*
 * Mutex.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: tiagom
 */

#include "Mutex.h"
#include <stdio.h>
#include <stdlib.h>


Mutex::Mutex() {
	pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&m_mutex);
}

