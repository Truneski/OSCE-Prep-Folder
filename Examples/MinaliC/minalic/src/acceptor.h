/*
 * Declaration of the Acceptor class.
 * This class is responsible for connecting the
 * clients to the sever.
 */

#ifndef _ACCEPTOR
#define _ACCEPTOR

#include "thread.h"

/* 
 * The Acceptor data type.
 * Acceptor is a thread class that includes
 * a Thread instance.
 */
typedef struct Acceptor
{
	/* Acceptor is a thread so include a thread instance*/
	Thread				_base_thread;
	SOCKET				_srv_socket;
	SOCKET				_sockio;
	struct sockaddr_in	_addr;
	int					_addr_len;	
} Acceptor;

/* The Acceptor methods*/
Acceptor *acceptor_create(SOCKET s);
Acceptor *acceptor_create_plugin(SOCKET s);
void acceptor_delete(Acceptor *self);
static void acceptor_run(void* self);
static void acceptor_run_plugin(void* self);

#endif /*_ACCEPTOR*/
