/*
 * Declaration of the Cgiprocess class.
 * This class handles all tricky things of
 * creating a cgi script process and comminicating with is.
 */

#ifndef _CGIPROCESS
#define _CGIPROCESS

#include "request.h"
#include "response.h"

/* 
 * The Cgiprocess data type.
 */
typedef struct Cgiprocess
{
	/* Reference to a Request object*/
	Request *_request;
	/* The name of the cgi module to run*/
	char *_module;
	/* Cgi module standard input to read from*/
	HANDLE _child_stdin_read;
	/* Cgi module standard input to write to*/
	HANDLE _child_stdin_write;
	/* Cgi module standard output to read from*/
	HANDLE _child_stdout_read;
	/* Cgi module standard output to write to*/
	HANDLE _child_stdout_write;
	/* Process id of the cgi process*/
	unsigned long _pid;
} Cgiprocess;

/* The Cgiprocess methods*/
Cgiprocess *cgiprocess_create(Request *req, char *module);
void cgiprocess_delete(Cgiprocess *self);
HANDLE cgiprocess_invoke(Cgiprocess *self);


#endif /*_CGIPROCESS*/
