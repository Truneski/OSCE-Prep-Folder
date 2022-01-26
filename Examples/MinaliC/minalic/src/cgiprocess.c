/*
 * Definition of the Cgiprocess class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "cgiprocess.h"
#include "request.h"
#include "map.h"
#include "logger.h"
#include "memcounter.h"

/* Size of chunk buffer to read http content data in*/
#define CONTENT_CHUNK_BUFSIZE 4096
/* Size of buffer to create environment variables for cgi*/
#define ENVIRONMENT_AREA_SIZE 2048


/* Declaration of global server module path*/
extern char g_module_path[256];
/* Declaration of global wwwroot path*/
extern char g_wwwroot_path[256];
/*The start time of the server*/
extern time_t g_uptime;

/*
 * Constructor
 */
Cgiprocess *cgiprocess_create(Request *req, char *module)
{
	Cgiprocess	*new_obj;
	
	assert(req);
	assert(module);
	new_obj = malloc(sizeof(Cgiprocess));
	meminc(CGIPROCESS);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_request = req;
		new_obj->_module = malloc(strlen(module) + 1);
		meminc(CGIPROC2);
		strcpy(new_obj->_module, module);
		new_obj->_child_stdin_read = NULL;
		new_obj->_child_stdin_write = NULL;
		new_obj->_child_stdout_read = NULL;
		new_obj->_child_stdout_write = NULL;
	}
	return new_obj;
}

/*
 * Destructor
 */
void cgiprocess_delete(Cgiprocess *self)
{
	assert(self);
	if (self)
	{
		CloseHandle(self->_child_stdin_read);
		CloseHandle(self->_child_stdout_read);
		free(self->_module);
		memdec(CGIPROC2);
		free(self);
		memdec(CGIPROCESS);
	}
}

/*
 * Function that creates the process of the cgi module
 * and send the http content to its standard input.
 * The function returns a standard output handle to read
 * the module output content from.
 */
HANDLE cgiprocess_invoke(Cgiprocess *self)
{
	SECURITY_ATTRIBUTES		sa_attr; 
	PROCESS_INFORMATION		proc_info; 
	STARTUPINFOA			start_info;
	BOOL					success = FALSE; 
	DWORD					written; 
	char					chBuf[CONTENT_CHUNK_BUFSIZE];
	DWORD					flags = 0;
	HANDLE					input_file = NULL;
	int						res;
	char					new_env[ENVIRONMENT_AREA_SIZE];
	char					*current_variable;
	char					system_root[100];
	char					system_root_value[100];
	char					comspec[100];
	char					comspec_value[100];
	Mapelement				*e;
	int						content_length = 0;
	int						read_content = 0;
	time_t		            rawtime;
	int                     diff, sec, min, hour;
    char                    up_time[50];
    
#ifdef USE_MEMCOUNTER
    char mem_report[100];
#endif

	sa_attr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa_attr.bInheritHandle = TRUE; 
	sa_attr.lpSecurityDescriptor = NULL; 

	/* Child process's STDOUT*/
    if (!CreatePipe(&self->_child_stdout_read, &self->_child_stdout_write, &sa_attr,0)) 
      logger_print(g_logger, 1, "StdoutRd CreatePipe"); 

	/*Ensure the read handle to the pipe for STDOUT is not inherited*/
	if (!SetHandleInformation(self->_child_stdout_read, HANDLE_FLAG_INHERIT,0))
      logger_print(g_logger, 1, "Stdout SetHandleInformation"); 

	/* Child process's STDIN*/
    if (!CreatePipe(&self->_child_stdin_read, &self->_child_stdin_write, &sa_attr,0)) 
      logger_print(g_logger, 1, "Stdin CreatePipe"); 

	/* Ensure the write handle to the pipe for STDIN is not inherited*/
    if (!SetHandleInformation(self->_child_stdin_write, HANDLE_FLAG_INHERIT,0))
      logger_print(g_logger, 1, "Stdin SetHandleInformation"); 

	/* Set up members of the PROCESS_INFORMATION structure*/
	ZeroMemory(&proc_info, sizeof(PROCESS_INFORMATION));
	
	
	/*Copy environment strings into an environment block*/
	/* First create the system root variable that all processes
	 * must have to work.
	 */
	GetEnvironmentVariableA("systemRoot", system_root, 99);
	strcpy(system_root_value, "systemRoot=");
	strcat(system_root_value, system_root);

	GetEnvironmentVariableA("COMSPEC", comspec,99);
	strcpy(comspec_value, "COMSPEC=");
	strcat(comspec_value, comspec);
		
	/* Add environment variable REQUEST_METHOD*/
    current_variable = new_env;
	strcpy(current_variable, "REQUEST_METHOD=");
	strcat(current_variable, self->_request->_method);
    current_variable += strlen(current_variable) + 1; 
	
	/* Add environment variable QUERY_STRING*/
	strcpy(current_variable, "QUERY_STRING=");
	strcat(current_variable, self->_request->_query);
    current_variable += strlen(current_variable) + 1; 

	/* Add environment variable SERVER_SOFTWARE*/
	strcpy(current_variable,"SERVER_SOFTWARE=MinaliC ");
	strcat(current_variable,VERSION);
    current_variable += strlen(current_variable) + 1; 
	
	/* Add environment variable SERVER_NAME*/
	strcpy(current_variable, "SERVER_NAME=MinaliC"); 
	current_variable += strlen(current_variable) + 1; 
	
	/* Add environment variable SCRIPT_NAME*/
	strcpy(current_variable, "SCRIPT_NAME=");
	strcat(current_variable, self->_request->_file);
    current_variable += strlen(current_variable) + 1; 

	/* Add environment variable SCRIPT_FILENAME for PHP*/
	strcpy(current_variable, "SCRIPT_FILENAME=");
	strcat(current_variable, self->_request->_file);
    current_variable += strlen(current_variable) + 1; 

	/* Add environment variable GATEWAY_INTERFACE*/
	strcpy(current_variable, "GATEWAY_INTERFACE=CGI/1.1");
    current_variable += strlen(current_variable) + 1; 

	/* Add environment variable SERVER_PROTOCOL*/
	strcpy(current_variable, "SERVER_PROTOCOL=HTTP/1.1");
    current_variable += strlen(current_variable) + 1; 
		
	/* Add environment variable REMOTE_ADDR*/
	strcpy(current_variable, "REMOTE_ADDR=");
	strcat(current_variable, inet_ntoa(self->_request->_addr.sin_addr));
    current_variable += strlen(current_variable) + 1; 

	/* Add environment variable SESSION_ID*/
	strcpy(current_variable, "SESSION_ID=");
	strcat(current_variable, self->_request->_session_id);
    current_variable += strlen(current_variable) + 1; 
	
	/* Add environment variable WWWROOT*/
	strcpy(current_variable, "WWWROOT=");
	strcat(current_variable, g_wwwroot_path);
    current_variable += strlen(current_variable) + 1; 
    
    /* Add environment variable SERVER_ROOT*/
	strcpy(current_variable, "SERVER_ROOT=");
	strcat(current_variable, g_module_path);
    current_variable += strlen(current_variable) + 1; 
    
    /* Add environment variable SERVER_UP_TIME*/
	strcpy(current_variable, "SERVER_UP_TIME=");
	rawtime = time(NULL);
	diff = (int)difftime(rawtime, g_uptime);
	hour = diff / 3600;
	diff -= hour * 3600;
	min = diff / 60;
	diff -= min * 60;
	sec = diff;
	sprintf(up_time, "%d:%d:%d", hour, min, sec);;
	strcat(current_variable, up_time);
    current_variable += strlen(current_variable) + 1; 
    
    #ifdef USE_MEMCOUNTER
    /* Add environment variable SERVER_MEM*/
	strcpy(current_variable, "SERVER_MEM=");
	memcounter_report(g_memcounter, mem_report);
	strcat(current_variable, mem_report);
    current_variable += strlen(current_variable) + 1; 
    #endif
	
	/* Loop through the request headers and add some selected headers
	 * to the cgi modules´environment variables.
	 */
	e = map_findfirst(self->_request->_headers);
	if (e != NULL)
	{
		do
		{
			if (_stricmp(e->key, "Accept:") == 0)
			{
				/* Add environment variable HTTP_ACCEPT*/
				strcpy(current_variable, "HTTP_ACCEPT="); 
				strcat(current_variable, e->value);
				current_variable += strlen(current_variable) + 1; 
			}
			else if (_stricmp(e->key, "Content-Type:") == 0)
			{
				/* Add environment variable CONTENT_TYPE*/
				strcpy(current_variable, "CONTENT_TYPE="); 
				strcat(current_variable, e->value);
				current_variable += strlen(current_variable) + 1; 
			}
			else if (_stricmp(e->key, "Content-Length:") == 0)
			{
				/* Add environment variable CONTENT_LENGTH*/
				strcpy(current_variable, "CONTENT_LENGTH=");
				strcat(current_variable, e->value);
				/* Save the comming content length to compare with later*/
				content_length = atoi(e->value);
				current_variable += strlen(current_variable) + 1; 
			}
			else if (_stricmp(e->key, "Cookie:") == 0)
			{
				/* Add environment variable HTTP_COOKIE*/
				strcpy(current_variable, "HTTP_COOKIE="); 
				strcat(current_variable, e->value);
				current_variable += strlen(current_variable) + 1; 
			}
			e = map_findnext(self->_request->_headers);
		}while(e != NULL);
	}

	/* Add environment variable COMSPEC*/
	strcpy(current_variable, comspec_value);	
	current_variable += strlen(current_variable) + 1; 
	/* Add environment variable SystemRoot*/
	strcpy(current_variable, system_root_value);
	/* Terminate the block with a NULL byte*/
     current_variable += strlen(current_variable) + 1; 
    *current_variable = 0; 

	/* Set up members of the STARTUPINFO structure. 
	 * This structure specifies the STDIN and STDOUT handles for redirection.
	 */
	ZeroMemory(&start_info,sizeof(STARTUPINFO));
	start_info.cb = sizeof(STARTUPINFO); 
	start_info.hStdError = self->_child_stdout_write;
	start_info.hStdOutput = self->_child_stdout_write;
	start_info.hStdInput = self->_child_stdin_read;
	start_info.dwFlags |= STARTF_USESTDHANDLES;
  
	success = CreateProcessA(NULL, 
		self->_module,		/*Command line*/
		NULL,				/*Process security attributes*/ 
		NULL,				/*Primary thread security attributes*/ 
		TRUE,				/*Handles are inherited*/ 
		flags,				/*Creation flags*/ 
		(LPVOID) new_env,	/*New environmentvariables from parent*/
		NULL,				/*Use parent's current directory*/ 
		&start_info,		/*STARTUPINFO pointer*/ 
		&proc_info);		/*Receives PROCESS_INFORMATION*/

	if (!success) 
	{	
		/* If an error occurs, send it to the log*/
		logger_print(g_logger, 1, "Error creating CGI process %s", self->_module);
		/* Return a null handle*/
		return NULL;
	}
	else 
	{
		/* If process created successfully close the handles*/	
		CloseHandle(proc_info.hProcess);
		CloseHandle(proc_info.hThread);
		/* Save the process id*/
		self->_pid = proc_info.dwProcessId;
	}

	/* If there is any content to read*/
	if (content_length > 0)
	{
		/* Read content from socket and send it to process stdin*/
		while (res = request_read_content(self->_request, chBuf,CONTENT_CHUNK_BUFSIZE))
		{	
			/* Count the content read*/
			read_content += res;
			success = WriteFile(self->_child_stdin_write, chBuf, res, &written, NULL);
			/* If we have reached the expected content break*/
			if (read_content >= content_length)
				break;
		}
	}

	/* Close the pipe handles*/
	if (!CloseHandle(self->_child_stdin_write)) 
		logger_print(g_logger, 1, "StdInWr CloseHandle"); 
	
	if (!CloseHandle(self->_child_stdout_write)) 
		logger_print(g_logger, 1, "StdOutWr CloseHandle"); 

	/* Return to the caller the cgi process´ standard output handle*/
	return self->_child_stdout_read;
}

