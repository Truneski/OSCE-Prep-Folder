/*
 * Definition of the Clienthandlerplugin class
 */
#define _CRT_SECURE_NO_WARNINGS
#include "clienthandlerplugin.h"
#include "request.h"
#include "response.h"
#include "logger.h"
#include "clientcounter.h"
#include "memcounter.h"

/* Global Clientcounter object*/
extern Clientcounter *g_clientcounter;

/*
 * Constructor
 */
Clienthandlerplugin *clienthandlerplugin_create(SOCKET s, struct sockaddr_in addr, HANDLE_REQUEST func)
{
	Clienthandlerplugin	*new_obj = NULL;

	if (clientcounter_hold_client(g_clientcounter))
	{
		/* Allocate memory for this object*/
		new_obj = malloc(sizeof(Clienthandlerplugin));
		meminc(CLIENTHANDLERPLUGIN);
		assert(new_obj);
		if (new_obj)
		{
			new_obj->_base_thread._run = clienthandlerplugin_run;
			/*Set virtual destructor to the destructor of this class*/
			new_obj->_base_thread._delete = (void (*)(void*))clienthandlerplugin_delete;
			new_obj->_sockio = s;
			new_obj->_addr = addr;
			new_obj->_handle_request = func;
			/*Start the work of the thread for this object*/
			thread_start(&new_obj->_base_thread);	
		}
	}

	/*If failed to create a clienthandler we close the socket*/
	if (new_obj == NULL)
	{
		/* Close the socket for this object*/
		shutdown(s, 2);
		closesocket(s);
	}

	return new_obj;
}

/*
 * Destructor
 */
void clienthandlerplugin_delete(Clienthandlerplugin *self)
{
	assert(self);
	if (self)
	{
		/* Close the socket for this object*/
		shutdown(self->_sockio, 2);
		closesocket(self->_sockio);
		/* Close the handle to the thread*/
		CloseHandle(self->_base_thread._handle);
		/* Free memory for this object*/
		free(self);
        memdec(CLIENTHANDLERPLUGIN);
		/*Count down number of simultanous clienthandler threads*/
		clientcounter_release_client(g_clientcounter);
	}
}

/* 
 * Thread function of Clienthandlerplugin
 */
static void clienthandlerplugin_run(void* self)
{
	Clienthandlerplugin	*me = (Clienthandlerplugin*)self;
	
	me->_handle_request(me->_sockio,me->_addr);
}









