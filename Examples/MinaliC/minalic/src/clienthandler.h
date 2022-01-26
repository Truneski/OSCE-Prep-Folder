/*
 * Declaration of the Clienthandler class.
 * This class handles a client request from start
 * to the end. It runs in a separate thred.
 */

#ifndef _CLIENTHANDLER
#define _CLIENTHANDLER

#include "thread.h"
#include "common.h"
#include "request.h"
#include "response.h"

/* 
 * The Clienthandler data type.
 * Clienthandler is a thread class that includes
 * a Thread instance.
 */
typedef struct Clienthandler
{
	/* Clienthandler is a thread so include a thread instance*/
	Thread	_base_thread;
	SOCKET	_sockio;
	struct	sockaddr_in _addr;
	Request	*_request;
	Response *_response;	
} Clienthandler;

/* The Clienthandler methods*/
Clienthandler *clienthandler_create(SOCKET s, struct	sockaddr_in addr);
void clienthandler_delete(Clienthandler *self);
static void clienthandler_run(void* self);

#endif /*_CLIENTHANDLER*/

