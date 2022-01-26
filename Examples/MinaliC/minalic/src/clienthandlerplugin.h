/*
 * Declaration of the Clienthandlerplugin class.
 * This class handles a client request from start
 * to the end. It runs in a separate thred.
 */

#ifndef _CLIENTHANDLERPLUGIN
#define _CLIENTHANDLERPLUGIN

#include "thread.h"
#include "common.h"
#include "request.h"
#include "response.h"
#include "plugin.h"

/* 
 * The Clienthandlerpluginplugin data type.
 * Clienthandlerplugin is a thread class that includes
 * a Thread instance.
 */
typedef struct Clienthandlerplugin
{
	/* Clienthandlerplugin is a thread so include a thread instance*/
	Thread	_base_thread;
	SOCKET	_sockio;
	struct	sockaddr_in _addr;
	HANDLE_REQUEST _handle_request;
} Clienthandlerplugin;

/* The Clienthandlerplugin methods*/
Clienthandlerplugin *clienthandlerplugin_create(SOCKET s, struct sockaddr_in addr, HANDLE_REQUEST func);
void clienthandlerplugin_delete(Clienthandlerplugin *self);
static void clienthandlerplugin_run(void* self);

#endif /*_CLIENTHANDLERPLUGIN*/

