/*
 * Definition of the Request class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include "request.h"
#include "logger.h"
#include "memcounter.h"

/* Use global hexadecimal constants for urlencoding*/
extern int g_hex_value[256];

/* Escape states for urlencoding*/
typedef enum {
	esc_rest,
	esc_first,
	esc_second
} Escstate;

/*
 * Constructor
 */
Request *request_create(SOCKET s, struct sockaddr_in a)
{
	Request	*new_obj;

	new_obj = malloc(sizeof(Request));
	meminc(REQUEST);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_socket = s;
		new_obj->_addr = a;
		new_obj->_headers = map_create();
	}
		
	return new_obj;
}

/*
 * Destructor
 */
void request_delete(Request *self)
{
	assert(self);
	assert(self->_headers);
	map_delete(self->_headers);
	free(self);
	memdec(REQUEST);
}

/*
 * Private helper function that removes double dots and slashes
 */
static void remove_double_dots_and_double_slashes(char *s)
{
	char	*p = s;

	while (*s != '\0') 
	{
		*p++ = *s++;
		if (s[-1] == '/' || s[-1] == '\\') 
		{
			/* Skip all following slashes and backslashes */
			while (*s == '/' || *s == '\\')
			{
				s++;
			}

			/* Skip all double-dots */
			while (*s == '.' && s[1] == '.')
			{
				s += 2;
			}
		}
	}
	*p = '\0';
}


/* Unescape urlnencoded string*/
static int unescape_chars(char *sp, char *cp, int len) 
{
	Escstate escape_state = esc_rest;
	int escaped_value = 0;
	int src_pos = 0;
	int dst_pos = 0;

	while (src_pos < len) 
	{
		int ch = cp[src_pos];
		switch (escape_state) 
		{
			case esc_rest:
				if (ch == '%') 
				{
					escape_state = esc_first;
				} 
				else if (ch == '+') 
				{
					sp[dst_pos++] = ' ';
				} 
				else 
				{
					sp[dst_pos++] = ch;	
				}
			break;
			case esc_first:
				escaped_value = g_hex_value[ch] << 4;	
				escape_state = esc_second;
			break;
			case esc_second:
				escaped_value += g_hex_value[ch];
				sp[dst_pos++] = escaped_value;
				escape_state = esc_rest;
			break;
		}
		src_pos++;
	}
	sp[dst_pos] = '\0';

	return 1;
}


/*
 * Private method that parses the http request header.
 * The method looks for the following parts:
 * HTTP_METHOD, URL, QUERY_STRING, HTTP_VERSION
 * It also looks for all http headers that are found.
 */
static Result parse_header(Request *self, char *header)
{
	char	*p;
	char	*name;
	char	*value;
	int		index;
	int		parsed;
	int		end_of_header;
	int		has_query;
	int		host_found = 0;
	char	tmp_file[FILE_SIZE];
	int		file_len;
		
	assert(self);
	assert(header);

	/* HTTP_METHOD*/
	for(parsed = 0, p = header, index = 0;index < METHOD_SIZE - 1;index++,p++)
	{
		if (*p == ' ')
		{
			parsed = 1;
			break;
		}
		if (*p == '\r')
		{
			break;
		}
		self->_method[index] = *p;
	}
	self->_method[index] = 0;

	if (!parsed)
		return ERR;

	/* URL*/
	p++;
	has_query = 0;
	file_len = 0;
	for(parsed = 0, index = 0;index < FILE_SIZE - 1;index++, p++)
	{
		if (*p == ' ')
		{
			parsed = 1;
			has_query = 0;
			break;
		}
		else if (*p == '?')
		{
			parsed = 1;
			has_query = 1;
			break;
		}
		else if (*p == '\r')
		{
			break;
		}
		tmp_file[index] = *p;
		file_len++;
	}
	tmp_file[index] = 0;
	
	/* Unescape the URL*/
	unescape_chars(self->_file, tmp_file, file_len);
	/* Remove slashes and dots that can create security holes*/
	remove_double_dots_and_double_slashes(self->_file);
 
	if (!parsed)
		return ERR;

	if (has_query)
	{
		/* QUERY_STRING*/
		p++;
		has_query = 0;
		for(parsed = 0, index = 0;index < QUERY_SIZE - 1;index++, p++)
		{
			if (*p == ' ')
			{
				parsed = 1;
				break;
			}
			else if (*p == '\r')
			{
				break;
			}
			self->_query[index] = *p;
		}
		self->_query[index] = 0;

		if (!parsed)
			return ERR;
	}
	else
	{
		self->_query[0] = 0;
	}

	/* HTTP_VERSION*/
	p++;
	for(parsed = 0, index = 0;index < VERSION_SIZE - 1;index++, p++)
	{
		if (*p == '\r')
		{
			parsed = 1;
			break;
		}
		self->_version[index] = *p;
	}
	self->_version[index] = 0;

	/* Parse other headers found*/
	if (parsed)
	{
		end_of_header = 0;
		do
		{
			p++; /* skip '\n'*/
			p++;
			name = p;
			parsed = 0;
			for(;;p++)
			{
				/*Check that we don´t read over header bound*/
				if ( (p - header) >= HEADER_SIZE)
					return ERR;

				if (*p == ' ')
				{
					parsed = 1;
					break;
				}
				if (*p == '\r')
				{
					parsed = 1;
					break;
				}
			}
			*p = 0;
			if (parsed)
			{
				p++;
				value = p;
				for(;*p != '\r';p++)
				{
					/*Check that we don´t read over header bound*/
					if ( (p - header) >= HEADER_SIZE)
						return ERR;
				}
				*p = 0;

				if (*(p+2) == '\r')
					end_of_header = 1;
				
				/*
				 * Header name and value found so add it to the header map.
				 */
				/* Collect headers that must exist in a proper request*/
				if (!_stricmp(name, "host:"))
				{
					/*Check that host is not already found*/
					if (host_found)
					{
						return ERR;
					}
					/*Check size of host value field*/
					if (strlen(value) <= HOST_SIZE)
					{
						host_found = 1;
					}
				}
				map_add(self->_headers, name, value);
			}
		}while(parsed && !end_of_header);
	}
	/*Check that all headers exist*/
	parsed = parsed && host_found;
	return parsed ? OK : ERR;
}

/*
 * Method that reads the content part of a http request header.
 * The method could be called several times to get the whole content.
 * Each call returns a chunk of data deciced by parameter len.
 */
int request_read_content(Request *self, char *buf, int len)
{
	int		res;
	
	assert(self);
	assert(buf);
	
	/* Read data from the socket*/
	res = recv(self->_socket, buf, len, 0);
	
	/* Return 0 when there is no more data*/ 
	if (res <= 0) 
		return 0;
	else
		return res;
}

/*
 * Method that reads the header part of a http request header.
 * The header can be maximum HEADER_SIZE.
 */
Result request_read_header(Request *self)
{
	int		len;
	int		max;
	int		line_end;
	int		header_end;
	char	header[HEADER_SIZE]; /* Input buffer*/

	assert(self);

	max = line_end = header_end = 0;
	while (max < HEADER_SIZE - 1 && header_end != 1)
	{
		/*
		 * Read one byte at the time from the socket and add it to the input buffer
		 * By reading one byte at the time it is easy to check for end characters.
		 */
		len = recv(self->_socket, &header[max], 1, 0);
		if (len == 0)
			return ERR; /* No data found*/

		if (len < 0)
			return ERR; /* Socket closed*/
		
		if (header[max] == '\n' && max > 1 )
		{
			/* Here we have found a new line*/
			if (header[max - 1] == '\r')
			{
				/* Privious character was carrige return*/
				if (line_end)
				{
					/*
					 * Here we have found a second \r\n direcly after the previous
					 * so stop loocking for header.
					 */
					header_end = 1;
				}
				else
				{	
					/* First \r\n found*/
					line_end = 1;
				}
			}
		}
		else
		{
			if (header[max] != '\r')
			{
				/* Charecter is not \r so go back to start state.*/
				line_end = 0;
			}
		}
		/* Move buffer index with number of bytes read.*/
		max += len;
	}
	/* Header is found so terminate buffer*/
	header[max] = 0;
	
	/* Log the header if loglevel is 2*/
	logger_print(g_logger,2, "%s", header);
	
	/* If we found a proper header*/
	if (header_end) 
	{
		/* Parse the header into its parts*/
		return parse_header(self, header);
	}
	else
	{
		/* Else return error*/
		return ERR;
	}
}
