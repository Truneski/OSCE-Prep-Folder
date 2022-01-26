/*
 * Definition of the Clientcounter class
 */
#define _CRT_SECURE_NO_WARNINGS
#include "clientcounter.h"
#include "logger.h"
#include "memcounter.h"

/*
 * Constructor
 */
Clientcounter *clientcounter_create(int max_clients)
{
	Clientcounter	*new_obj;

	new_obj = malloc(sizeof(Clientcounter));
	meminc(CLIENTCOUNTER);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_count = 0;
		new_obj->_max_clients = max_clients;
		InitializeCriticalSection(&new_obj->_critical_section);
	}
	return new_obj;
}

/*
 * Destructor
 */
void clientcounter_delete(Clientcounter *self)
{
	assert(self);
	if (self)
	{
		DeleteCriticalSection(&self->_critical_section);
		free(self);
		memdec(CLIENTCOUNTER);
	}
}

/*
 * This method decides if a new client can connect
 */
int	clientcounter_hold_client(Clientcounter *self)
{
	int res;

	/* Take the mutex*/
	EnterCriticalSection(&self->_critical_section);
	/*Check that we dont' create to many simultaneous threads*/
	if (self->_count < self->_max_clients)
	{
		/*Increase number of clienthandler threads*/
		self->_count++;
		res = 1;
	}
	else
	{
		res = 0;
		logger_print(g_logger, 1, "Client rejected");
	}
	/* Give the mutex*/
	LeaveCriticalSection(&self->_critical_section);
	return res;
}

/*
 * This method is used to free a clients place
 */
void clientcounter_release_client(Clientcounter *self)
{
	EnterCriticalSection(&self->_critical_section);
	self->_count--;
	LeaveCriticalSection(&self->_critical_section);
}
