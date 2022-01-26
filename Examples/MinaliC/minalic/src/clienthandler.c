/*
 * Definition of the Clienthandler class
 */
#define _CRT_SECURE_NO_WARNINGS
#include "clienthandler.h"
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
Clienthandler *clienthandler_create(SOCKET s, struct sockaddr_in addr)
{
	Clienthandler	*new_obj = NULL;

	/*Check that we dont' create to many simultaneous threads*/
	if (clientcounter_hold_client(g_clientcounter))
	{
		/* Allocate memory for this object*/
		new_obj = malloc(sizeof(Clienthandler));
		meminc(CLIENTHANDLER);
		assert(new_obj);
		if (new_obj)
		{
			new_obj->_base_thread._run = clienthandler_run;
			/*Set virtual destructor to the destructor of this class*/
			new_obj->_base_thread._delete = (void (*)(void*))clienthandler_delete;
			new_obj->_sockio = s;
			new_obj->_addr = addr;
			/*Create a Request object for the client request*/
			new_obj->_request = request_create(new_obj->_sockio, addr);	
			/*Create a Response object for the client response*/
			new_obj->_response = response_create(new_obj->_sockio);
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
void clienthandler_delete(Clienthandler *self)
{
	assert(self);
	if (self)
	{
		/* Close the socket for this object*/
		shutdown(self->_sockio, 2);
		closesocket(self->_sockio);
		/* Close the handle to the thread*/
		CloseHandle(self->_base_thread._handle);
		/* Delete the referensing objects*/
		request_delete(self->_request);
		response_delete(self->_response);
		/* Free memory for this object*/
		free(self);
		memdec(CLIENTHANDLER);
		/*Count down number of simultanous clienthandler threads*/
		clientcounter_release_client(g_clientcounter);
	}
}

/* 
 * Thread function of Clienthandler
 */
static void clienthandler_run(void* self)
{
	Clienthandler	*me = (Clienthandler*)self;
	Result			res;
	
	assert(me);
	/* Try to read a http header from the request*/
	if (request_read_header(me->_request) == OK)
	{
		/*
		 * Check if http method is GET or POST
		 * which are the only two methods implemented by the server.
		 */
		if ( (_stricmp(me->_request->_method, "GET") == 0) ||
			 (_stricmp(me->_request->_method, "POST") == 0) )
		{
			/*
			 * Handle GET and POST the same way by asuming
			 * that some kind of file is requested.
			 */
			res = response_send_file(me->_response, me->_request);
			
			/* Check the result from the Response object*/
			if ( res != OK)
			{
				switch(res)
				{
				case NOT_FOUND:
					/* Requested file doesn´t exist*/
					response_send_not_found(me->_response);
				break;
				default:
					/*
					 * Send 500 'Internal server error'
					 * for all other errors.
					 */
					response_send_internal_server_error(me->_response);
				}
			}
		}
		else
		{
			/*
			 * Send 501 'Not Implemented' if request method
			 * was not GET or POST.
			 */
			response_send_not_implemented(me->_response);
		}
	}
	else
	{
		/*
		 * Send  400 'Bad Request' if failed to
		 * read a proper http header.
		 */
		response_send_bad_request(me->_response);
	}
}









