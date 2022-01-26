/*
 * Definition of the Acceptor class
 */
#define _CRT_SECURE_NO_WARNINGS
#include "acceptor.h"
#include "clienthandler.h"
#include "logger.h"
#include "memcounter.h"

/*Server shut down flag*/
extern int g_shut_down;

/* Declaration of global logger object*/

/*
 * Constructor
 */
Acceptor *acceptor_create(SOCKET s)
{
	Acceptor	*new_obj;

	new_obj = malloc(sizeof(Acceptor));
	meminc(ACCEPTOR);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_base_thread._run = acceptor_run;
		new_obj->_base_thread._delete = (void (*)(void*))acceptor_delete;
		new_obj->_srv_socket = s;
		new_obj->_addr_len = sizeof(new_obj->_addr);
		thread_start(&new_obj->_base_thread);
	}
	return new_obj;
}

/*
 * Destructor
 */
void acceptor_delete(Acceptor *self)
{
	assert(self);
	if (self)
	{
		shutdown(self->_sockio, 2);
		closesocket(self->_sockio);
		CloseHandle(self->_base_thread._handle);
		free(self);
		memdec(ACCEPTOR);
	}
}

/* 
 * Thread function of Acceptor that performs
 * the real work.
 */
static void acceptor_run(void* self)
{
	Clienthandler	*ch;
	Acceptor		*a = (Acceptor*)self;

	assert(a);
	if (a)
	{
		while (1)
		{	
			/* Wait for a client to connect to the server socket*/
			a->_sockio = accept(a->_srv_socket, (struct sockaddr*)&a->_addr, &a->_addr_len);
			
			/*If accept is released due to shut down terminate acceptor thread*/
			if (g_shut_down)
				break;		

			/* When a client connects create a client handler 
			 * thread and let it handle the request.
			 * Let the client handler run for it self. It will be deleted
			 * when it ends.
			 */
			ch = clienthandler_create(a->_sockio, a->_addr);
			assert(ch);
		}/* Return to the top of the loop and handle the next incomming request*/
	}
}
