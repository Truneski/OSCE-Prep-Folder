/*
 * Constant values used in the server.
 */

#ifndef _COMMON
#define _COMMON

//define for debugmode with assertions 
//#define DEBUG

/* Server version presented at startup*/
#define VERSION "2.0.0" 
/* Server build version presented at startup*/
#define BUILD	"A001"
/* Max size of a http request header*/
#define HEADER_SIZE 2048
/* Maximum size of the http request method (GET,POST)*/
#define METHOD_SIZE 20
/* Maximum size of a phisical file and path name*/
#define FILE_SIZE 256
/* Maximum size of the querystring in a http request*/
#define QUERY_SIZE 256
/* Maximum size of the version part in a http request*/
#define VERSION_SIZE 20
/* Maximum size of the phrase part in a http request*/
#define	PHRASE_SIZE 40
/* Max size of the status code part in a http response*/
#define	STATUS_SIZE 5
/* Maximun size of host header*/
#define HOST_SIZE 256
/* Maximum size of the cookie header in a http response*/
#define COOKIE_SIZE (HOST_SIZE + 50)
/* The size of a session id string*/
#define SESSION_ID_SIZE 31
/* Maximum number of retries to create a unique session id*/
#define SESSION_MAX_TRY 50
/* The size of the chunk to increase the cgi response buffer with*/
#define CGI_BUFFER_SIZE 1024
/* The size of the chunk to increase the directory browsing response buffer with*/
#define DIR_BUFFER_SIZE 1024
/* The maximum size of the headers returned from a cgi script*/
#define CGI_HEADER_MAX 512
/* The maximum size of a file extension*/
#define FILE_EXTENSION 10
/* The maxumum number of internal http redirections*/
#define RECURSIVE_CALLS_ALLOWED 10
/* The size of the log buffer*/
#define LOG_FORMAT_SIZE 200
/* The size of the http Date header*/
#define HTTP_TIME_SIZE 80
/* Maximum size of the Content-Type header*/
#define CONTENT_TYPE_SIZE 100
/* The size of the chunk to send a file with*/
#define SEND_FILE_CONTENT_CHUNK_SIZE 256

/* Server script default file strings*/
#define BINARY_DEFAULT_FILE "index.exe"
#define CGI_DEFAULT_FILE "index.cgi"
#define PHP_DEFAULT_FILE "index.php"
#define PERL_DEFAULT_FILE "index.pl"
#define HTM_DEFAULT_FILE "index.htm"

/* The cookie name of the session id cookie*/
#define SESSION_ID_COOKIE_NAME "session_id"

/* Name of directory browsing file*/
#define DIRECTORY_BROWSING_FILE "dir.ini"

/* Enum constants used as return value from functions*/
typedef enum Result
{
	OK = 0,        /* Success*/
	ERR = 1,       /* Error*/
	ENDED = 2,     /* Operation is finished, don´t proceed*/
	NOT_FOUND = 3  /* Page is not found*/
} Result;

/* Enum constants used to specify run mode for the server*/
typedef enum Runmode
{
	SERVICE = 0,       /* Run as service*/
	APPLICATION = 1    /* Run as application*/
} Runmode;


/* Build variables*/
//#define USE_ASSERT
//#define USE_MEMCOUNTER


#endif /*_COMMON*/
