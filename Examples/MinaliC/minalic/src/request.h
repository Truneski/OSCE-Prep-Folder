/*
 * Declaration of the Request class.
 * This class is responsible for reading the socket
 * input stream an interpret is as a http message.
 */

#ifndef _REQUEST
#define _REQUEST

#include "common.h"
#include "map.h"

/* 
 * The Request data type.
 */
typedef struct Request
{
	SOCKET	_socket;
	struct	sockaddr_in _addr;
	char	_method[METHOD_SIZE];
	char	_file[FILE_SIZE];
	char	_query[QUERY_SIZE];
	char	_version[VERSION_SIZE];
	int		_contentExist;
	char	_session_id[SESSION_ID_SIZE];
	Map *_headers;
} Request;

/* The Request methods*/
Request *request_create(SOCKET s, struct sockaddr_in);
void request_delete(Request *self);
Result request_read_header(Request *self);
int request_read_content(Request *self, char *buf, int len);

#endif /*_REQUEST*/
