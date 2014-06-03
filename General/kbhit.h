/*
 * kbhit.h
 *
 *  Created on: Jun 1, 2014
 *      Author: rodrigo
 */

#ifndef KBHIT_H_
#define KBHIT_H_


enum
{
	NB_ENABLE,
	NB_DISABLE
};


extern void nonblock(int state);
extern int kbhit();


#endif /* KBHIT_H_ */
