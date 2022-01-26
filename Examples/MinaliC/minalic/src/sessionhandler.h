/*
 * Declaration of the Sessionhandler class.
 * This class takes care of holding all session id´s. 
 * It knows when a session has timed out and how 
 * to generate new session id´s.
 */

#ifndef _SESSIONHANDLER
#define _SESSIONHANDLER

#include <windows.h>
#include "common.h"
#include "map.h"

/* 
 * The Sessionhandler data type.
 */
typedef struct Sessionhandler
{
	int	_timeout;
	Map *_sessions;
	int _differ;
	/* Sessionhandler is used by several threads and need a critical section*/
	CRITICAL_SECTION _critical_section; 

} Sessionhandler;

/* The Sessionhandler methods*/
Sessionhandler *sessionhandler_create();
void sessionhandler_delete(Sessionhandler *self);
int sessionhandler_get_valid_id(Sessionhandler *self, char *session_id);
void sessionhandler_delete_old(Sessionhandler *self);

#endif /*_SESSIONHANDLER*/
