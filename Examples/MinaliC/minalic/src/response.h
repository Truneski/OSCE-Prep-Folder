/*
 * Declaration of the Response class.
 * This class is responsible for forming a correct
 * http response message and sending it.
 */

#ifndef _RESPONSE
#define _RESPONSE

#include "common.h"
#include "map.h"
#include "request.h"

/* 
 * The Response data type.
 */
typedef struct Response
{
	SOCKET	_socket;
	char	_version[VERSION_SIZE];
	char	_phrase[PHRASE_SIZE];
	char	_status[STATUS_SIZE];
	Map		*_headers;
	int		_recursiveCalls;
} Response;

/* The Response methods*/
Response *response_create(SOCKET s);
void response_delete(Response *self);
Result response_send_file(Response *self, Request *req);
Result response_send_data(Response *self ,Request *req, char *data, char *content_type);
Result response_send_not_found(Response *self);
Result response_send_not_implemented(Response *self);
Result response_send_internal_server_error(Response *self);
Result response_send_bad_request(Response *self);
Result response_send_moved_permanently(Response *self, char *location);
Result response_send_redirect(Response *self);
Result response_send_not_modified(Response *self);


#endif /*_RESPONSE*/
