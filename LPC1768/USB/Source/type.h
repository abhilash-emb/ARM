/*****************************************************************************
 *   type.h:  Type definition Header file for NXP LPC17xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <stdint.h>

#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#define MIN_OF(x,y)	((x)<(y)?(x):(y))
#define MAX_OF(x,y)	((x)>(y)?(x):(y))

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

/* Set bit field macro */
#define _BIT(n)	(1<<n) 
#define _SBF(f,v) (v<<f) 
#define _BITMASK(field_width) ( _BIT(field_width) - 1) 


typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, SetState;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;

typedef enum
{
	NONE_BLOCKING = 0,		/**< None Blocking type */
	BLOCKING,				/**< Blocking type */
} TRANSFER_BLOCK_Type;



#endif  /* __TYPE_H__ */
