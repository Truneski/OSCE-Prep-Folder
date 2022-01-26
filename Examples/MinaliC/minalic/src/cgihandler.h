/*
 * Declaration of the Cgihandler class.
 * This class handles the timeout of cgi processes.
 */

#ifndef _CGIHANDLER
#define _CGIHANDLER

#include "request.h"
#include "response.h"

/* 
 * The Cgihandler data type.
 */
typedef struct Cgihandler
{
	int	_timeout;
	Map *_processes;
	/* Cgihandler is used by several threads and need a critical section*/
	CRITICAL_SECTION _critical_section; 

} Cgihandler;

/* The Cgihandler methods*/
Cgihandler *cgihandler_create();
void cgihandler_delete(Cgihandler *self);
void cgihandler_set_active(Cgihandler *self, unsigned long pid);
int cgihandler_remove(Cgihandler *self, unsigned long pid);
void cgihandler_delete_old(Cgihandler *self);


#endif /*_CGIHANDLER*/
