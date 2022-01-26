/*
 * Declaration of the Plugin API functions.
 */

#ifndef _PLUGIN
#define _PLUGIN

#include <windows.h>

/* 
 * The function typedefinitions.
 */

typedef char *(*GET_VERSION)();
typedef char *(*GET_BUILD)();
typedef void (*HANDLE_REQUEST)(SOCKET s, struct sockaddr_in addr);

#endif /*_PLUGIN*/
