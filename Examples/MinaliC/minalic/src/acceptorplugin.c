/*
 * Definition of the Acceptorpluginplugin class
 */
#define _CRT_SECURE_NO_WARNINGS
#include "acceptorplugin.h"
#include "clienthandlerplugin.h"
#include "logger.h"
#include "memcounter.h"

/*Server shut down flag*/
extern int g_shut_down;

/*
 * Constructor
 */
Acceptorplugin *acceptorplugin_create(SOCKET s, char *plugin_name)
{
	Acceptorplugin	*new_obj = NULL;
		
	if (plugin_name != NULL)
	{
		new_obj = malloc(sizeof(Acceptorplugin));
		meminc(ACCEPTORPLUGIN);
		assert(new_obj);
		if (new_obj)
		{
			new_obj->_plugin_module = malloc(strlen(plugin_name) + 10);
			strcpy(new_obj->_plugin_module, plugin_name);
			strcat(new_obj->_plugin_module, ".DLL");
			new_obj->_base_thread._run = acceptorplugin_run;
			new_obj->_base_thread._delete = (void (*)(void*))acceptorplugin_delete;
			new_obj->_srv_socket = s;
			new_obj->_addr_len = sizeof(new_obj->_addr);
			/*Load the plugin dll into memory*/
			new_obj->_module_instance = LoadLibraryA(new_obj->_plugin_module);
			if (new_obj->_module_instance == NULL)
			{
				logger_print(g_logger,1, "Failed to load plugin module %s", new_obj->_plugin_module);
				return NULL;
			}
			new_obj->_plugin_get_version = (GET_VERSION)GetProcAddress(new_obj->_module_instance, "get_version");
			if (new_obj->_plugin_get_version == NULL)
			{
				logger_print(g_logger, 1, "Invalid plugin module %s", new_obj->_plugin_module);
				return NULL;
			}
			new_obj->_plugin_get_build = (GET_BUILD)GetProcAddress(new_obj->_module_instance, "get_build");
			if (new_obj->_plugin_get_build == NULL)
			{
				logger_print(g_logger, 1, "Invalid plugin module %s", new_obj->_plugin_module);
				return NULL;
			}
			new_obj->_plugin_handle_request = (HANDLE_REQUEST)GetProcAddress(new_obj->_module_instance, "handle_request");
			if (new_obj->_plugin_handle_request == NULL)
			{
				logger_print(g_logger, 1, "Invalid plugin module %s", new_obj->_plugin_module);
				return NULL;
			}
			logger_print(g_logger, 1, "%s (%s build %s) loaded", new_obj->_plugin_module, new_obj->_plugin_get_version(), new_obj->_plugin_get_build());
			thread_start(&new_obj->_base_thread);
		}
	}
	return new_obj;
}

/*
 * Destructor
 */
void acceptorplugin_delete(Acceptorplugin *self)
{
	assert(self);
	if (self)
	{
		if (self->_module_instance)
		{
			FreeLibrary(self->_module_instance);
		}
		shutdown(self->_sockio, 2);
		closesocket(self->_sockio);
		CloseHandle(self->_base_thread._handle);
		free(self->_plugin_module);
		free(self);
		memdec(ACCEPTORPLUGIN);
	}
}

/* 
 * Thread function of Acceptorplugin that performs
 * the real work.
 */
static void acceptorplugin_run(void* self)
{
	Clienthandlerplugin	*ch;
	Acceptorplugin		*a = (Acceptorplugin*)self;

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
			
			/* When a client connects create a client handler plugin 
			 * thread and let it handle the request.
			 * Let the client handler run for it self. It will be deleted
			 * when it ends.
			 */
			ch = clienthandlerplugin_create(a->_sockio, a->_addr, a->_plugin_handle_request);
			if (!ch)
				logger_print(g_logger, 1, "Error creating clienthandlerplugin");
		}/* Return to the top of the loop and handle the next incomming request*/
	}
}
