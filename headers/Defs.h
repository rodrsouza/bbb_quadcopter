
#ifndef DEFS_H_
#define DEFS_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ProjDefs.h"


// Numero de elementos de arrays
#define SIZEOF(x)  (sizeof(x)/sizeof(x[0]))


#ifndef __cplusplus
typedef short bool;
#endif

// para inicializar estruturas
#define INIT_STRUCT(a)  memset((void*)&a, 0, sizeof (a))

#ifndef true
#define true  (bool)(1)
#define false (bool)(0)
#endif

#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

// -- Macros para quebrar um word em bytes
#define LSB(w)  (unsigned char)(w & 0x00ff)
#define MSB(w)  (unsigned char)((w >> 8)&0xff)


// Macro para transformar um decimal em formato BCD
#define BCD(x) (unsigned char) ( ((x/10) << 4) + (x % 10) )
#define BCD2DEC(x) (((x>>4)*10) + (x & 0x0F))

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#endif //DEFS_H
