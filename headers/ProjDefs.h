/*
 * ProjDefs.h
 *
 *  Created on: Oct 20, 2013
 *      Author: rodrigo
 */

#ifndef _PROJDEFS_H_
#define _PROJDEFS_H_

#include "../General/Thread.h"
#include "../General/Mutex.h"
#include "../General/Lock.h"
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>

#define SOCKET_PORT			30000

typedef struct
{
	int Header;
	unsigned short CheckSum;
	unsigned char Message[0xFF];

} SOCKET_MESSAGE;




#endif /* _PROJDEFS_H_ */
