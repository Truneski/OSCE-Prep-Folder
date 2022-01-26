/*
 * Declaration of the Acceptorpluginplugin class.
 * This class is responsible for connecting the
 * clients to the sever for plugin implemetations.
 */

#ifndef _ACCEPTORPLUGIN
#define _ACCEPTORPLUGIN

#include "thread.h"
#include "plugin.h"

/* 
 * The Acceptorpluginplugin data type.
 * Acceptorplugin is a thread class that includes
 * a Thread instance.
 */
typedef struct Acceptorplugin
{
	/* Acceptorplugin is a thread so include a thread instance*/
	Thread				_base_thread;
	SOCKET				_srv_socket;
	SOCKET				_sockio;
	struct sockaddr_in	_addr;
	int					_addr_len;	
	char				*_plugin_module;
	GET_VERSION			_plugin_get_version;
	GET_BUILD			_plugin_get_build;
	HANDLE_REQUEST		_plugin_handle_request;
	HINSTANCE			_module_instance;
} Acceptorplugin;


/* The Acceptorplugin methods*/
Acceptorplugin *acceptorplugin_create(SOCKET s, char *plugin_name);
void acceptorplugin_delete(Acceptorplugin *self);
static void acceptorplugin_run(void* self);


#endif /*_ACCEPTORPLUGIN*/
