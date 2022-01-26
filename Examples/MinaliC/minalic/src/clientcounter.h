/*
 * Declaration of the Clientcounter class.
 * This class is responsible counting the number
 * of clients that is connected to the sever.
 */

#ifndef _CLIENTCOUNTER
#define _CLIENTCOUNTER

#include <windows.h>
#include "common.h"

/* 
 * The Clientcounter data type.
 */
typedef struct Clientcounter
{
	int					_count;	
	int					_max_clients;
	CRITICAL_SECTION	_critical_section;
} Clientcounter;

/* The Clientcounter methods*/
Clientcounter *clientcounter_create(int max_clients);
void clientcounter_delete(Clientcounter *self);
int	clientcounter_hold_client(Clientcounter *self);
void clientcounter_release_client(Clientcounter *self);

#endif /*_CLIENTCOUNTER*/
