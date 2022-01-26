/*
 * Definition of the Response class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "response.h"
#include "request.h"
#include "cgiprocess.h"
#include "cgihandler.h"
#include "logger.h"
#include "sessionhandler.h"
#include "directorybrowser.h"
#include "memcounter.h"

/*
 * System global objects used by this class.
 */
/* Global path to the server executable*/
extern char g_wwwroot_path[256];
/* Global Sessionhandler object*/
extern Sessionhandler *g_sessionhandler;
/* Global variable showing if directory browsing is allowed*/
extern int g_conf_brows_default;
/* Global Cgihandler object*/
extern Cgihandler *g_cgihandler;

/* Define number of content types*/
#define MAX_CONTENT_TYPES 46

/* Declaration of content types that are
 * supported by the server.
 */
char *(content_types[MAX_CONTENT_TYPES][2]) = {
	{"htm","text/html"},
	{"html","text/html"},
	{"shtm","text/html"},
	{"shtml","text/html"},
	{"js","application/x-javascript"},
	{"ico","image/x-icon"},
	{"xml","text/xml"},
	{"pdf","application/pdf"},
	{"txt","text/plain"},
	{"jpg","image/jpeg"},
	{"jpeg","image/jpeg"},
	{"png","image/png"},
	{"bmp","image/bmp"},
	{"svg","image/svg+xml"},
	{"torrent","application/x-bittorrent"},
	{"wav","audio/x-wav"},
	{"mp3","audio/x-mp3"},
	{"mid","audio/mid"},
	{"m3u","audio/x-mpegurl"},
	{"ram","audio/x-pn-realaudio"},
	{"ra","audio/x-pn-realaudio"},
	{"xls","application/excel"},
	{"c","text/plain"},
	{"h","text/plain"},
	{"gif","image/gif"},
	{"exe","application/octet-stream"},
	{"zip","application/zip"},
	{"rtf","application/rtf"},
	{"css","text/css"},
	{"wav","audio/x-wav"},
	{"doc","application/msword"},
	{"docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{"xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{"ppt","application/ms-powerpoint"},
	{"bin","application/octet-stream"},
	{"tgz","application/x-tar-gz"},
	{"tar","application/x-tar"},
	{"gz","application/x-gunzip"},
	{"arj","application/x-arj-compressed"},
	{"rar","application/x-arj-compressed"},
	{"swf","application/x-shockwave-flash"},
	{"mpg","video/mpeg"},
	{"mpeg","video/mpeg"},
	{"asf","video/x-ms-asf"},
	{"avi","video/x-msvideo"},
	{"default","text/plain"}};

/*
 * Month names
 */
static const char *month_names[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/*
 * Constructor
 */
Response *response_create(SOCKET s)
{
	Response	*new_obj;

	new_obj = malloc(sizeof(Response));
	meminc(RESPONSE);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_socket = s;
		new_obj->_headers = map_create();
		new_obj->_recursiveCalls = 0;
	}
	return new_obj;
}

/*
 * Destructor
 */
void response_delete(Response *self)
{
	assert(self);
	assert(self->_headers);
	map_delete(self->_headers);
	free(self);
	memdec(RESPONSE);
}

/*
 * Private method used to append a buffer.
 * The method returns the adress to to the byte after 
 * the last character that was appended.
 * It is used to build a response header.
 */
static char *add_to_header(char *dest, char *src)
{
	char	*d;
	char	*s;
	
	assert(dest);
	assert(src);

	for (s = src, d = dest;*s != '\0';s++, d++)
	{
		*d = *s;
	}
	return d;
}

/*
 * Private method used to build a response header
 * and send it with the response socket.
 */
static Result write_response_header(Response *self)
{
	char		header[HEADER_SIZE];
	char		*next_pos;
	Mapelement	*e;

	assert(self);

	next_pos = header;
	/* HTTP Version*/
	next_pos = add_to_header(next_pos, self->_version);
	next_pos = add_to_header(next_pos, " "); 

	/* HTTP Status*/
	next_pos = add_to_header(next_pos, self->_status);
	next_pos = add_to_header(next_pos, " ");

	/* HTTP Phrase*/
	next_pos = add_to_header(next_pos, self->_phrase);

	/* Add CRLF*/
	next_pos = add_to_header(next_pos, "\r\n");
	
	/* Add all header lines that response object includes*/
	e = map_findfirst(self->_headers);
	if (e != NULL)
	{
		do
		{
			/* Add header line name*/
			next_pos = add_to_header(next_pos,e->key);
			next_pos = add_to_header(next_pos, " "); 

			/* Add header line vaulue*/
			next_pos = add_to_header(next_pos, e->value);
			
			/* Add CRLF*/
			next_pos = add_to_header(next_pos, "\r\n"); 
			e = map_findnext(self->_headers);
		}while(e != NULL);

	}
	/* Add terminating CRLF*/
	next_pos = add_to_header(next_pos, "\r\n");
	/* NULL terminate string*/
	*next_pos = '\0';

	/* Log header to send if loglevel is 2*/
	logger_print(g_logger, 2, "%s", header);
	
	/* Send the complete response header to the client*/
	send(self->_socket, header, (int)(next_pos-header), 0);

	return OK;
}

/*
 * Method that sends a file as a http response content.
 * The method takes an opened file as parameter.
 */
static Result send_file_content(Response *self, FILE *f)
{
	char	file_buf[SEND_FILE_CONTENT_CHUNK_SIZE];
	size_t	size;

	assert(self);
	assert(f);
	
	/* Loop through the file in chunk-steps and send the chunks*/
	do
	{
		/* Read file data*/
		size = fread(file_buf, 1, sizeof(file_buf), f);
		/* Send file data to the socket*/
		send(self->_socket, file_buf, (int)size, 0);
	}while(!feof(f));

	return OK;
}

/*
 * Method that sends a buffer as a http response content io the open socket.
 */
static Result send_buffer_content(Response *self, char *data)
{
	char	buf[SEND_FILE_CONTENT_CHUNK_SIZE];
	char	*s;
	char	*send_start, *send_end;
	int		i;
	int		res;

	assert(self);
	assert(data);
		
	s = data;
	send_start = buf;
	while(*s != 0)
	{
		send_end = buf;
		/*Fill send buffer with a chunk of data*/
		for(i = 0;i < SEND_FILE_CONTENT_CHUNK_SIZE && *s != 0;i++)
		{
			*send_end++ = *s++;
		}
		/* Send file data to the socket*/
		res = send(self->_socket, send_start, i, 0);
	}
	return OK;
}

/*
 * Private method that returns a files extension.
 */
static char *get_file_extension(char *file, char *ext)
{
	char	*p;
	char	cnt;
	
	/* Pre NULL terminate result*/
	*ext = 0;

	assert(file);
	assert(ext);

	/* Loop through the file name*/
	for(p = file;*p != 0;p++)
	{
		/* Look for a dot*/
		if (*p == '.') 
		{
			cnt = 0;
			/* If a dot is found assume that the comming text is the extension*/
			while (*(++p) && (cnt < FILE_EXTENSION - 1 ))
			{
				/* Copy the comming text to the result buffer*/
				cnt++;
				*(ext++) = *p;
			}
			/* NULL terminate string*/
			*ext = 0;
			break;
		}
	}
	/* Return the extension*/
	return ext;
}

/*
 * Private method that maps a file extension
 * to a http content-type by looking in a table.
 */
static char *get_content_type(char *ext, char *type)
{
	int	index;

	assert(ext);
	assert(type);

	/* Loop through the content type table*/
	for	(index = 0;index < MAX_CONTENT_TYPES;index++)
	{
		/* Search for the extension*/
		if (_stricmp(content_types[index][0], ext) == 0)
		{
			/* Extension found so return the corresponding content type*/
			strcpy(type,content_types[index][1]);
			return type;
		}
	}
	/*
	 * No match in the table so return the default type
	 * which is the last element in the table.
	 */
	strcpy(type, content_types[MAX_CONTENT_TYPES - 1][1]);
	return type;
}

/*
 * Private method that searches for a cookie in
 * a string and returns its value.
 */
static char *get_cookie_value(char *cookie_str, char *cookie_name, char *cookie_value)
{
	char	*p;
	char	*s;

	assert(cookie_str);
	assert(cookie_name);
	assert(cookie_value);

	/* Find first occurance of the cookie name in the string*/
	p = strstr(cookie_str, cookie_name);
	s = cookie_value;

	/* If the name was found*/
	if (p != NULL)
	{
		/* Search forward to next '='*/
		while(*p != '=' && *p)
			p++;
		/* If we found a '=' before the end of the string*/
		if (*p == '=')
		{
			/* Collect the string as the value until a ';' is found*/
			p++;
			while(*p != ';' && *p)
				*s++ = *p++;
			/* NULL terminate the result string*/
			*s = 0;
			/* Return the result string*/
			return cookie_value;
		}
	}
	/* Not found so return NULL*/
	return NULL;
}

/*
 * Private method that determines if a path is a directory.
 */
static int is_directory(char *path)
{
	struct stat	info;

	assert(path);
	/* Use the stat file system function*/
	if (!stat(path, &info))
	{
		/* Check if the directory flag is set*/
		if (info.st_mode &_S_IFDIR )
		{
			return 1;/* Is directory*/
		}
		return 0;
	}
	return 0;
}

/*
 * Private method that determines if a file exists on disk.
 */
static int file_exist(char *file_path)
{
	FILE	*f;

	assert(file_path);

	f = NULL;
	/* Try to open the file*/
	f = fopen(file_path, "rb");
	if (!f)
		return 0; /* Could not open file*/
	else
	{
		/* Could open file so close it*/
		fclose(f);
		return 1;
	}
}

/*
 * Private method that appends the right default file to a path.
 * It returns 1 if a default file was found and added else 0.
 * The default file is one of:
 * index.cgi (converts to index.exe)
 * index.php
 * index.pl
 * index.htm
 */
static int add_default_file(char *path)
{
	char file[FILE_SIZE];

	assert(path);

	strcpy(file, path);
	strcat(file, BINARY_DEFAULT_FILE);
	/* If index.exe exists index.cgi will be the default file*/
	if (file_exist(file))
	{
		strcat(path, CGI_DEFAULT_FILE);
		return 1;
	}

	strcpy(file, path);
	strcat(file, PHP_DEFAULT_FILE);
	/* If index.php exists it will be the default file*/
	if (file_exist(file))
	{
		strcat(path, PHP_DEFAULT_FILE);
		return 1;
	}

	strcpy(file, path);
	strcat(file, PERL_DEFAULT_FILE);
	/* If index.pl exists it will be the default file*/
	if (file_exist(file))
	{
		strcat(path, PERL_DEFAULT_FILE);
		return 1;
	}

	/* Else the default file will be index.htm*/
	strcpy(file, path);
	strcat(file, HTM_DEFAULT_FILE);
	if (file_exist(file))
	{
		strcat(path, HTM_DEFAULT_FILE);
		return 1;
	}

	return 0;
	
}

 

/*
 * Private method that tries to build a physial file path on disk from
 * the file in the request object.
 * If a file name is missing it appends a default file name.
 * If directory browsing is allowed it sends the directory content.
 */
static Result retrieve_physical_file_name_or_brows(Response *self, Request *req, char *physical)
{
	char				file[FILE_SIZE];
	size_t				last;
	char *				host;
	Directorybrowser	*dir_brows = NULL;
	int					can_brows_dir = 0;
	int					has_default_file;
	char				*brows_data = 0;
	Result				res;
	int					is_root = 0;
	int                 i;
	
	assert(self);
	assert(req);
	assert(physical);

	/* Add the server web path to the filename*/
	strcpy(file, g_wwwroot_path);
  	strcat(file, req->_file);

    /* Convert webslashes to fileslashes*/
    for (i = 0; file[i] != '\0'; i++)
    {
        if (file[i] == '/')
            file[i] = '\\';
    }


	/* Get the last character in the file name*/
	last = strlen(file) - 1;
	assert(last > 0);

	/* If the filename ends with a slash it means that a default file is requested*/
	if (file[last] == '\\')
	{
		/* Create a directory browser object that can decide if to browse the directory*/
		dir_brows = directorybrowser_create(file, g_conf_brows_default, req->_file);
		can_brows_dir = directorybrowser_can_brows(dir_brows);
	
		has_default_file = add_default_file(file);

		if (can_brows_dir && !has_default_file) 
		{
			/*Perform the directory browsing*/
			directorybrowser_get_brows_data(dir_brows, &brows_data);
			assert(brows_data);
			res = response_send_data(self, req,brows_data, "text/html");
			directorybrowser_delete(dir_brows);
			if (OK == res)
			{
				return ENDED;
			}
		}
		else
		{
            directorybrowser_delete(dir_brows);      
        }
	}
	/*
	 * If no default file is requested go on and try to figure out
	 * if the requested file is a physical file or a directory.
	 */
	else if (is_directory(file))
	{
		/* If the requested file is a directory not ending with a slash (default file)
		 * tell the browser that it should request the directory with a slash.
		 * This is done by send the message MOVED PERMANENTLY to the browser
		 * with the new file name including a slash.
		 */
		/* Get the host name that is needed when building new URL*/
		host = map_getval(req->_headers, "Host:");
		if (host == NULL)
			return ERR;
		/* Start build a new URL*/
		strcpy(file, "http://");
		strcat(file, host);
		strcat(file, req->_file);
		/* Add a slash (default file)*/
		strcat(file, "/");
		/* Send the response*/
		response_send_moved_permanently(self, file);
		/* Return that the response is already taken care of*/
		return ENDED;
	}
	/* Return the physical file name*/
	strcpy(physical, file);
	return OK;
}

/*
 * Private method that determines if a file name is a CGI script file.
 */
static int is_cgi(char *ext)
{
	assert(ext);
	return !_stricmp(ext, "cgi") || !_stricmp(ext, "php") || !_stricmp(ext, "pl");
}

/*
 * Private method that replaces the cgi extension to executable.
 */
static void convert_to_executable(char *cgi_file)
{
	char *p; 

	assert(cgi_file);
	
	/* Loops to the '.' in the file name*/
	for(p = cgi_file;*p != '.';p++)
	{
		/* Emty loop just looking for '.'*/
	}
	p++;
	/* Replaces the extension with exe*/
	strcpy(p, "exe");
}

/*
 * Private method that builds the session id cookie string
 * from the session id in the request object.
 */
static char *build_sessionid_cookie_string(Request *req, char *create_cookie)
{
	char *host;
	
	assert(req);
	assert(create_cookie);
	
	/* Start building string*/
	strcpy(create_cookie, SESSION_ID_COOKIE_NAME);
	strcat(create_cookie, "=");
	/* Append the session id*/
	strcat(create_cookie, req->_session_id);
	/* Build the host part of the cookie by getting the host name*/
	host = map_getval(req->_headers, "Host:");
	/* Add the host part*/
	strcat(create_cookie, "; ");
	strcat(create_cookie, host);
	/* Return the new cookie string*/
	return create_cookie;
}

/*
 * Private method that creates a valid session id for the request
 */
static void create_session_id_from_cookie(Request *req)
{
	char	*cookie;
		
	assert(req);

	/* Get the cookie string from the request*/
	cookie = map_getval(req->_headers, "Cookie:");
	/* If any cookie exists*/
	if (cookie != NULL)
	{	
		/* Get the cookie value for cookie session id*/
		req->_session_id[0] = '\0'; /*Null terminate if we don´t get a value*/
		get_cookie_value(cookie, SESSION_ID_COOKIE_NAME, req->_session_id);
	}
	else
	{
		/* No cookie was sent from the client so just NULL terminate string*/
		req->_session_id[0] = 0;
	}
	
	/*
	 * Now decide if the session id from the client still is
	 * a valid id, else generate a new one with help of the session handler.
	 */
	sessionhandler_get_valid_id(g_sessionhandler, req->_session_id);
}

/*
 * Private method that parse the headers sent from a cgi script.
 */
static Result parse_cgi_response(Response *self, char *content, char **after_header, int *header_count)
{
	char	*p;
	int		cnt;
	int		at_new_line;
	int		collect;
	int		tooken;
	char	*t;
	char	*s;
	int		found_any_header = 0;

	assert(self);
	assert(content);

	/* Assume we are at the beginning of a script output line*/
	at_new_line = 1;
	/* Dont' collect header value yet*/
	collect = 0;
	/* Loop through the output in maximum CGI_HEADER_MAX number of characters*/
	for (p = content, cnt = 0;(cnt < CGI_HEADER_MAX) && (*p != 0);p++, cnt++)
	{
		/* If beginning of line*/
		if (at_new_line)
		{
			/* No tooken found yet*/
			tooken = 0;
			/* If header is Location:*/
			if (!strncmp("Location:", p, 9))
			{
				t = p;
				/* Signal header found*/
				tooken = 1;	
				/* We are not in the state of beginning of a line*/
				at_new_line = 0;
				found_any_header = 1;
			}
			/* If header is Content-Type:*/
			else if (!strncmp("Content-Type:", p, 13))
			{
				t = p;
				/* Signal header found*/
				tooken = 2;	
				/* We are not in the state of beginning of a line*/
				at_new_line = 0;
				found_any_header = 1;
			}
			/* If header is Set-Cookie:*/
			else if (!strncmp("Set-Cookie:", p, 11))
			{
				t = p;
				/* Signal header found*/
				tooken = 3;	
				/* We are not in the state of beginning of a line*/
				at_new_line = 0;
				found_any_header = 1;
			}
			/* If we are at beginning of a line dont´ collect header value*/
			collect = 0;
		}
	  
		/* If state is collect*/
		if (collect)
		{
			/* Collect value by saving pointer to value*/
			s = p;
			/* Dont´ collect any more*/
			collect = 0;		
		}

		/* If header found and we have reached the end of the header name*/
		if (*p == ' ' && tooken > 0)
		{
			/* Set state collect*/
			collect = 1;
			/* NULL terminate header name string*/
			*p = 0;
		}

		/* If new line is found*/
		if (*p == '\r')
		{
			/* If already at beginning of a line*/
			if (at_new_line)
			{
				/* We have found the end of the header*/
				/* Was any header found from the script*/
				if (found_any_header)
				{
					p++;
					p++;
					cnt += 2;
					/* Return the position comming after header*/
					*after_header = p;
					/* Return the size of the header*/
					*header_count = cnt;
					/* Return that we are done with success*/
					return OK;
				}
				else
				{
					/*
					 * If no header was returned from the script return that
					 * header size was 0 and return that the next position is 
					 * the start of the incomming content.
					 */
					*header_count = 0;
					*after_header = content;
					/* Return that we are done with success*/
					return OK;
				}
			}
			/* New line was found but it was not the end of the header*/
			*p = 0;
			p++;
			cnt++;
			/* Set state to be beginning of line*/
			at_new_line = 1;
			/* Add the header name and value to the header map of the Response object*/
			map_add(self->_headers, t, s);
		}
	}
	/*
	 * No data was returned from the script so return that
	 * header size was 0 and return that the next position is 
	 * the start of the incomming content.
	 */
	*header_count = 0;
	*after_header = content;
	/* Return that we are done with success*/
	return OK;
}

/*
 * Private method that takes the content from a cgi script
 * and sends a response of it.
 */
static Result send_script_response(Response *self, Request *req, char *content, int content_len)
{	
	time_t		rawtime;
	struct tm	*timeinfo;
	char		http_time[80];
	char		size_buf[10];
	char		create_cookie[COOKIE_SIZE];
	int			has_content = 0;
	char		*parsed_content;
	int			header_count;
	Mapelement	*e;

	assert(self);
	assert(req);

	/* If there is any content*/
	if (content != NULL)
	{
		/* Parse the content for returned headers*/
		parse_cgi_response(self, content, &parsed_content, &header_count);
		/* Adjust the content size for headers found*/
		content_len -= header_count;
	}
	else
	{
		/* Signal no parsed content*/
		parsed_content = NULL;
	}

	/* Handle script headers found specific*/
	if ((e = map_find(self->_headers, "Location:")) != NULL) 
	{
		/* Handle the Location: header which means that the
		 * cgi script wants to change location.
		 */
		/* Check for absolut URI*/
		if (_strnicmp(e->value, "http:/", 6) == 0)
		{
			/* it is an absolute URI so make a 302 Redirect*/
			return response_send_redirect(self);
		}
		else
		{
			/* Else make an ordinary file request internaly*/
			/* Take the file that was requested*/
			strcpy(req->_file, e->value);
			/* Remove the Location header from the script*/
			map_remove(self->_headers, "Location:");
			/* Remove the Content-Length header from the script*/
			map_remove(req->_headers, "Content-Length:");
			self->_recursiveCalls++;
			/* Check number of recurcive location changes*/
			if (self->_recursiveCalls < RECURSIVE_CALLS_ALLOWED)
			{
				/* Make a file response*/
				return response_send_file(self, req);
			}
			else
			{
				/* To many recursive calls so return bad request*/
				return response_send_bad_request(self);
			}

		}
	}
	/* Set the http response status line to 200 OK*/
	strcpy(self->_version, "HTTP/1.1");
	strcpy(self->_status, "200");
	strcpy(self->_phrase, "OK");

	/* Set the Date header by retrieving the system time*/
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(http_time, sizeof(http_time), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	map_add(self->_headers, "Date:", http_time); 
	/* Set server header*/
	map_add(self->_headers, "Server:","MinaliC");
	/* Set Connection header (always close)*/
	map_add(self->_headers, "Connection:", "Close");
	/* Set the session cookie*/
	map_add(self->_headers, "Set-Cookie:", build_sessionid_cookie_string(req, create_cookie));

	/* If the response includes any content*/
	if (content_len > 0)
	{
		/* And if we got any content from the script*/
		if (parsed_content != NULL)
		{
			/* Set the content-length header*/
			_ltoa(content_len, size_buf, 10);
			map_add(self->_headers, "Content-Length:", size_buf);
			has_content = 1;
		}
		/*
		 * Set the content-type header.
		 * If the script returned a content-type header use that one,
		 * else assume that the content from a script is text/html.
		 */
		if (map_find(self->_headers, "Content-Type:") == NULL)
		{
			map_add(self->_headers, "Content-Type:", "text/html");
		}
	}

	/* If we could send the header with success*/
	if (write_response_header(self) == OK)
	{
		/* If there was any content to send...*/
		if (has_content)
		{	/* ...send that content*/
			if (send(self->_socket, parsed_content, content_len, 0) == content_len)
			{
				/* Return sent OK*/
				return OK;
			}
		}
		else
		{
			/* Return sent OK (no content sent, just a header)*/
			return OK;
		}
	}
	/* If we got here we could not send the response*/
	return ERR;
}

/*
 * Private method that starts a cgi script response.
 */
static Result start_cgi_response(Response *self, Request *req, char *web_file, char *ext)
{
	Cgiprocess	*cgih;
	HANDLE		cgi_out = NULL;
	char		*cgi_buffer = NULL;
	long		index = 0;
	long		cnt = 0;
	int			pages = 4;
	long		len;
	Result		res;
	int			read_result;
	char		tmp[256];
	int			timed_out;

	assert(self);
	assert(req);
	assert(web_file);
	assert(ext);
	
	/* Check so the extension of the script file not is php or pl(perl)*/
	if (_stricmp(ext, "php") && _stricmp(ext, "pl")) 
	{
		/* Convert *.cgi to correct extension*/
		convert_to_executable(web_file); 
	}
	
	/* Physical script file must exist*/
	if (file_exist(web_file)) 
	{
		/* If the extension of the script file is php*/
		if (!_stricmp(ext, "php")) 
		{
			/*
			 * Build a path to the php cli binary and
			 * let that binary be the cgi script to start.
			 */
			strcpy(tmp, "php.exe ");
			strcat(tmp, " \"");
			strcat(tmp, web_file);
			strcat(tmp, " \"");
			strcpy(web_file, tmp);
		}
		else if (!_stricmp(ext, "pl")) 
		{
			/*
			 * Build a path to the php cli binary and
			 * let that binary be the cgi script to start.
			 */
			strcpy(tmp, "perl.exe ");
			strcat(tmp, " \"");
			strcat(tmp, web_file);
			strcat(tmp, " \"");
			strcpy(web_file, tmp);
		}


		/* Create an instance of a Cgiprocess*/
		cgih = cgiprocess_create(req, web_file);
		assert(cgih);
		/*
		 * Let the Cgiprocess start the script and
		 * handle all communication with it. The call will
		 * return a stdout handle from the script.
		*/
		cgi_out = cgiprocess_invoke(cgih);
		assert(cgih);
		/* If we got a valid stdout handle*/
		if (cgi_out != NULL)
		{
			/* Register activity for the cgi process*/
			cgihandler_set_active(g_cgihandler, cgih->_pid);
			/* Allocate initial size of recive buffer*/
			cgi_buffer = malloc(CGI_BUFFER_SIZE * pages);
			meminc(RESPONSE2);
			/* Loop until we have read all content from the script*/
			do
			{
				/* Read from scrpts stdout into the buffer*/
				read_result = ReadFile(cgi_out, &cgi_buffer[index], CGI_BUFFER_SIZE, &len, NULL);
				if (read_result)
				{
					/* Register activity for the cgi process*/
					cgihandler_set_active(g_cgihandler, cgih->_pid);
					/* Increase index in buffer*/
					index += len;
					/* count the number of bytes read*/
					cnt += len;
					
					/* If number of bytes read is on its way to reach the buffer limit*/
					if (cnt >= CGI_BUFFER_SIZE * (pages - 1))
					{
						/* Count up the number of allocated pages*/
						pages++;
						/* Allocate more memory for the recive buffer*/
						cgi_buffer = realloc(cgi_buffer, CGI_BUFFER_SIZE * pages);
					}
				}
				/* Loop until no more to read*/
			}while(read_result && len);

			/* NULL terminate the receive buffer*/
			cgi_buffer[cnt] = 0;
		
			/* Unregister the cgi process and read timeout result*/
			timed_out = cgihandler_remove(g_cgihandler, cgih->_pid);
			if (!timed_out)
			{
				/* Send the content from the script in a response to the client*/
				res = send_script_response(self, req, cgi_buffer, cnt);
			}
			else
			{
				/* Send cgi timeout error as not found*/
				res = response_send_not_found(self);
			}
			assert(cgi_buffer);
			/* Free memory for the receive buffer*/
			free(cgi_buffer);
			memdec(RESPONSE2);
			/* Delete the Cgi handler instance*/
			cgiprocess_delete(cgih);
			/* Return the result from the send operation*/
			return res;
		}
		else
		{
			return ERR;
		}
	}
	/* If the script file doesn´t exist send NOT_FOUND*/
	return NOT_FOUND;
}

/*
 * Private function that convert month to the month number. Return -1 on error.
 */
static int monnum(const char *s)
{
	size_t	i;

	for (i = 0; i < (sizeof(month_names) / sizeof(month_names[0])); i++)
		if (!strcmp(s, month_names[i]))
			return ((int) i);

	return (-1);
}

/*
 * Private function that return the corresponding time_t value for a webdate.
 */
static time_t webdate_to_time(const char *s)
{
	time_t		current_time;
	struct tm	tm, *tmp;
	char		mon[32];
	int			sec, min, hour, mday, month, year;

	memset(&tm, 0, sizeof(tm));
	sec = min = hour = mday = month = year = 0;

	if (((sscanf(s, "%d/%3s/%d %d:%d:%d",
	    &mday, mon, &year, &hour, &min, &sec) == 6) ||
	    (sscanf(s, "%d %3s %d %d:%d:%d",
	    &mday, mon, &year, &hour, &min, &sec) == 6) ||
	    (sscanf(s, "%*3s, %d %3s %d %d:%d:%d",
	    &mday, mon, &year, &hour, &min, &sec) == 6) ||
	    (sscanf(s, "%d-%3s-%d %d:%d:%d",
	    &mday, mon, &year, &hour, &min, &sec) == 6)) &&
	    (month = monnum(mon)) != -1) 
	{
		tm.tm_mday	= mday;
		tm.tm_mon	= month;
		tm.tm_year	= year;
		tm.tm_hour	= hour;
		tm.tm_min	= min;
		tm.tm_sec	= sec;
	}

	if (tm.tm_year > 1900)
		tm.tm_year -= 1900;
	else if (tm.tm_year < 70)
		tm.tm_year += 100;

	/* Set Daylight Saving Time field */
	current_time = time(NULL);
	tmp = localtime(&current_time);
	tm.tm_isdst = tmp->tm_isdst;

	return (mktime(&tm));
}

/*
 * Main function that handle all requests of type GET and POST.
 * The function looks to se if it is a cgi request or
 * a static file request.
 */
Result response_send_file(Response *self, Request *req)
{
	FILE		*f;
	struct		stat info;
	time_t		rawtime;
	struct tm	*timeinfo;
	struct tm	*file_timeinfo;
	char		http_time[HTTP_TIME_SIZE];
	char		size_buf[20];
	Result		res;
	char		web_file[2*FILE_SIZE];
	char		ext[FILE_EXTENSION];
	char		content_type[CONTENT_TYPE_SIZE];
	char		create_cookie[COOKIE_SIZE];
	char		*if_modified_since;

	assert(self);
	assert(req);

	/*
	 * Translate the url request to a physical file
	 * If the function returns ENDED the request was redirected.
	 */
	if (retrieve_physical_file_name_or_brows(self, req, web_file) == ENDED)
		return OK; /* User was redirected*/

	/* Get the file extension of the requested file or cgi module*/
	get_file_extension(web_file, ext);

	/* Create or reuse the session id in the request*/
	create_session_id_from_cookie(req);

	/* Figure out if the requested file will generate a cgi call*/
	if (is_cgi(ext))
	{	
		/* The file is a cgi module to invoke the cgi module requested
		 * instead of sending a static file response.
		 */
		return start_cgi_response(self, req, web_file, ext);
	}
	
	/* Else perform ordinary static file response*/
	/* Try to open the requested file*/
	f = fopen(web_file, "rb");
	if (!f)
		return NOT_FOUND; /* File doesnt´ exist*/
	
	/* Get timestamp and size of the file*/
	stat(web_file,&info);
	_ltoa(info.st_size, size_buf, 10);
	file_timeinfo = localtime((const time_t*)(&(info.st_atime)));
	
	/* Check if we should use the If-Modified-since header*/
	if_modified_since = map_getval(req->_headers, "If-Modified-Since:");
	if (if_modified_since)
	{
		/* Check if file is newer then if_modified_since*/
		if (info.st_atime <= webdate_to_time(if_modified_since))
		{
			/* Close file and send not_modified*/
			fclose(f);
			return response_send_not_modified(self);
		}
	}
	/* The file exist so build a response with the file*/
	/* Create HTTP status line*/
	strcpy(self->_version, "HTTP/1.1");
	strcpy(self->_status, "200");
	strcpy(self->_phrase, "OK");

	/* Create a Date header*/
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(http_time, sizeof(http_time), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	map_add(self->_headers, "Date:", http_time); 

	/* Create a content-type header*/
	map_add(self->_headers, "Content-Length:", size_buf);
	get_content_type(ext, content_type);
	map_add(self->_headers, "Content-Type:", content_type);

	/* Create a last-modified header with the files date and time*/
	strftime(http_time, sizeof(http_time), "%a, %d %b %Y %H:%M:%S GMT", file_timeinfo);
	map_add(self->_headers, "Last-Modified:", http_time);

	/* Create the server header*/
	map_add(self->_headers, "Server:","MinaliC");

	/* Create the connection header. Always use the close type*/
	map_add(self->_headers, "Connection:", "Close");

	/* Create a set-cookie header for the session id*/
	map_add(self->_headers, "Set-Cookie:", build_sessionid_cookie_string(req, create_cookie));

	/* Send the status line and the headers*/
	if (write_response_header(self) == OK)
	{
		/* Then send the requested file content*/
		res = send_file_content(self, f);
		fclose(f);
		return res; 
	}
	fclose(f);

	/* If we get here no successfull response could be made*/
	return ERR;
}

/*
 * Private method that sends a simple response without any content.
 */
static Result response_send_msg(Response *self, char *status, char *phrase)
{
	time_t		rawtime;
	struct tm	*timeinfo;
	char		http_time[180];
	
	assert(self);
	/* Set the status line*/
	strcpy(self->_version, "HTTP/1.1");
	strcpy(self->_status, status);
	strcpy(self->_phrase, phrase);
	/* Set the date header*/
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(http_time, sizeof(http_time), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	map_add(self->_headers, "Date:", http_time); 
	/* Send the response header and return*/
	return write_response_header(self);
}

/*
 * Private method that sends a response with content from a buffer.
 * The content buffer should be null terminated
 */
Result response_send_data(Response *self, Request *req, char *data, char *content_type)
{
	time_t		rawtime;
	struct tm	*timeinfo;
	char		http_time[HTTP_TIME_SIZE];
	Result		res;
	char		size_buf[20];
	char		create_cookie[COOKIE_SIZE];
	
	assert(self);
	assert(req);
	assert(data);

	/* Create or reuse the session id in the request*/
	create_session_id_from_cookie(req);
		
	/* Create HTTP status line*/
	strcpy(self->_version, "HTTP/1.1");
	strcpy(self->_status, "200");
	strcpy(self->_phrase, "OK");

	/* Create a Date header*/
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(http_time, sizeof(http_time), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	map_add(self->_headers, "Date:", http_time); 

	/* Create a content-type header*/
	_ltoa((unsigned long)strlen(data), size_buf,10);
	map_add(self->_headers, "Content-Length:", size_buf);
	map_add(self->_headers, "Content-Type:", content_type);

	/* Create the server header*/
	map_add(self->_headers, "Server:", "MinaliC");

	/* Create the connection header. Always use the close type*/
	map_add(self->_headers, "Connection:", "Close");

	/* Create a set-cookie header for the session id*/
	map_add(self->_headers, "Set-Cookie:", build_sessionid_cookie_string(req, create_cookie));

	/* Send the status line and the headers*/
	if (write_response_header(self) == OK)
	{
		/* Then send the requested file content*/
		res = send_buffer_content(self, data);
		return res; 
	}
	
	/* If we get here no successfull response could be made*/
	return ERR;
}

/*
 * Method that sends a simple response of type 404.
 */
Result response_send_not_found(Response *self)
{
	return response_send_msg(self, "404", "Not Found");
}

/* 
 * Method that sends a simple response of type 501.
 */
Result response_send_not_implemented(Response *self)
{
	return response_send_msg(self, "501", "Not Implemented");
}

/*
 * Method that sends a simple response of type 500.
 */
Result response_send_internal_server_error(Response *self)
{
	return response_send_msg(self, "500", "Internal server error");
}


/*
 * Method that sends a simple response of type 400.
 */
Result response_send_bad_request(Response *self)
{
	return response_send_msg(self, "400", "Bad Request");
}

/*
 * Method that sends a simple response of type 300.
 */
Result response_send_moved_permanently(Response *self, char *location)
{
	/* Add to the response the new location*/
	map_add(self->_headers, "Location:", location); 
	return response_send_msg(self, "300", "Moved Permanently");
}

/*
 * Method that sends a simple response of type 302.
 */
Result response_send_redirect(Response *self)
{
	return response_send_msg(self, "302", "Redirect");
}

/*
 * Method that sends a simple response of type 304.
 */
Result response_send_not_modified(Response *self)
{
	return response_send_msg(self, "304", "Not Modified");
}
