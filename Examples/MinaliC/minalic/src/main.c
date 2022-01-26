/************************************************************/
/*															*/
/*				  	MinaliC Web Server 						*/
/*															*/
/*			A minimal web server developed in C			    */
/*			Author: Hans Alshoff,                           */
/*          Sweden 2010-2011			                    */
/*															*/
/*			Features:										*/
/*			CGI, PHP and Perl scripts,						*/
/*			directory browsing,	session managering,			*/
/*			custom plugins									*/
/*															*/
/*			The source code is built in an object			*/
/*			oriented manner.								*/
/*															*/
/************************************************************/

/*
 * Development log:
 * Date			Ver		Build	Desc
 * 2010-08-21	1.0.0	A001	First version.
 * 2010-11-01	1.0.1	A001	Fixed dir. browsing hole.
 * 2010-11-02	1.0.2	A001	Fixed : sending a request with a length greater 
								than or equal to 2048 bytes causes the server to crash.  
 * 2011-03-02   1.1.0   A001    Miner code changes to make the server build with GNU MinGw compiler.
                                Bugg fix on sessionid.
 * 2011-03-03   1.1.1   A001    Acceptor threads did not terminate properly on server shut down.     
 * 2011-03-10   1.1.2   A001    Buggfix CgiProcess environvariable COMSPEC overwritten.
 * 2011-03-11   1.2.0   A001    Buggfix Map and Sessionhandler. Old sessionid´s not deleted correctly.
                                Made the server crash at session timeout.                           
 * 2011-03-15   1.2.1   A001    Added CGI vars SERVER_UP_TIME and SERVER_ROOT.
 * 2011-03-20   1.2.2   A001    Fixed memory leak in response.c.
                                Fixed better ensurance of unique session id generation.
 * 2011-03-31   1.3.0   A001    Can now handle UTF-8 URL:s, changed in Request.c.         
 * 2011-04-11   1.3.1   A001    URL-encoding i Directorybrowser.c, UTF-8 handling removed in Request.c     
 * 2011-04-12   2.0.0   A001    New version release after testing. Stabil version.                  
 */

/*
 * This is the main file of the source code.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "acceptor.h" 
#include "acceptorplugin.h" 
#include "common.h"
#include "logger.h"
#include "sessionhandler.h"
#include "cgihandler.h"
#include "clientcounter.h"
#include "memcounter.h"

/*
 * System global objects and variables.
 */
/* Global path to the server executable*/
char g_module_path[256];
/* Global path to the wwwroot*/
char g_wwwroot_path[256];
/* Global logger object*/
Logger *g_logger;
/* Global memcounter object*/
Memcounter *g_memcounter;
/* Global Sessionhandler object*/
Sessionhandler *g_sessionhandler;
/* Global Cgihandler object*/
Cgihandler *g_cgihandler;
/* Global variable holding TCP port number to listen to*/
int g_conf_port;
/* Global variable showing if directory browsing is allowed*/
int g_conf_brows_default;
/* Config file object*/
Config	*g_conf;
/* Clientcounter*/
Clientcounter *g_clientcounter;
/* Hexadecimal constants for urlencoding*/
int g_hex_value[256];
/*Server shut down flag*/
volatile int g_shut_down = 0;
/*The start time of the server*/
time_t g_uptime = 0;


/*
 * File global define constants.
 */
#define SERVER_UPDATE_TIME 5000
#define CSTIMEOUT 10000

/*
 * File global variables.
 */
SERVICE_STATUS static g_service_status;
SERVICE_STATUS_HANDLE static g_service_status_handle;
char static g_service_name[100];

/*
 * Function predeclarations.
 */
void WINAPI service_main(DWORD argc, LPSTR argv[]);
void WINAPI server_ctrl_handler(DWORD control);
void update_status(int new_status);
int service_specific(int argc, char *argv[]);


/************************************************************
/* Exported API functions for use from plugins
 */
__declspec(dllexport) void echo(char *txt)
{
	logger_print(g_logger, 1, txt);
}

__declspec(dllexport) void echoex(char *txt, int level)
{
	logger_print(g_logger, level, txt);
}

__declspec(dllexport) char *get_config_value(char *key, char *value)
{	
	char *p;
	p = config_getval(g_conf, key);
	if (p)
	{
		strcpy(value, p);
		return value;
	}
	return 0;
	
}


/************************************************************
/* Start of main code definitions, install functions and
 * service handlers.
 */

/* Function that initializes urlencode hex constants*/
static void setup_hex_constants() 
{
	int i;
	for (i = 0; (i < 256); i++) 
    {
		g_hex_value[i] = 0;
	}
	g_hex_value['0'] = 0;	
	g_hex_value['1'] = 1;	
	g_hex_value['2'] = 2;	
	g_hex_value['3'] = 3;	
	g_hex_value['4'] = 4;	
	g_hex_value['5'] = 5;	
	g_hex_value['6'] = 6;	
	g_hex_value['7'] = 7;	
	g_hex_value['8'] = 8;	
	g_hex_value['9'] = 9;
	g_hex_value['A'] = 10;
	g_hex_value['B'] = 11;
	g_hex_value['C'] = 12;
	g_hex_value['D'] = 13;
	g_hex_value['E'] = 14;
	g_hex_value['F'] = 15;
	g_hex_value['a'] = 10;
	g_hex_value['b'] = 11;
	g_hex_value['c'] = 12;
	g_hex_value['d'] = 13;
	g_hex_value['e'] = 14;
	g_hex_value['f'] = 15;
}


/*
 * Function for creating and initializing server objects.
 */
void initialize_server(Runmode run_mode)
{
	char	configfile_name[FILE_SIZE];
	char	logfile_name[FILE_SIZE];
	size_t	len;
	char	*port;
	char	*log_level;
	int		conflog_level;
	char	*timeout;
	int		conf_timeout;
	char	*tmp;
	char	*threads;
	char	*cgi_timeout;
	int		conf_cgi_timeout;
	char	*brows;
	int     conf_threads;

    /* First create a memcounter before all other objects*/
#ifdef USE_MEMCOUNTER
    g_memcounter = memcounter_create();
#endif
	/* Initialize hex constants*/
	setup_hex_constants();

	/* Get the physical path for the server executable*/
	GetModuleFileNameA(NULL, g_module_path, 255);
	len = strlen(g_module_path) - 1;
	for (tmp = &g_module_path[len];*tmp != '\\'; tmp--);
	tmp++;
	*tmp = 0; /* Terminate string*/
	
	/* Create config file path*/
	strcpy(configfile_name, g_module_path);
	strcat(configfile_name, "minalic.ini");
	
	/*Read config file*/
	g_conf = config_create(configfile_name);
		
	/* PORT*/
	port = config_getval(g_conf, "HTTP_PORT");
	if (port == NULL)
		g_conf_port = 80; /* No web server port*/
	else
		g_conf_port = atoi(port);
	
	/* LOGLEVEL*/
	log_level = config_getval(g_conf, "SERVER_LOGLEVEL");
	if (log_level == NULL)
		conflog_level = 1; /* Default value*/
	else
		conflog_level = atoi(log_level);
	
	/* TIMEOUT*/
	timeout = config_getval(g_conf, "HTTP_SESSION_TIMEOUT");
	if (timeout == NULL)
		conf_timeout = 1800; /* Default value*/
	else
		conf_timeout = atoi(timeout);

	/* CGI TIMEOUT*/
	cgi_timeout = config_getval(g_conf, "CGI_TIMEOUT");
	if (cgi_timeout == NULL)
		conf_cgi_timeout = 90; /* Default value*/
	else
		conf_cgi_timeout = atoi(cgi_timeout);

	/* THREADS*/
	threads = config_getval(g_conf, "SERVER_HANDLERS");
	if (threads == NULL)
		conf_threads = 100; /* Default value*/
	else
		conf_threads = atoi(threads);

	/* DIRECTORY_BROWSING*/
	brows = config_getval(g_conf, "DIRECTORY_BROWSING_DEFAULT");
	if (brows == NULL)
		g_conf_brows_default = 0; /* Default value*/
	else
		g_conf_brows_default = atoi(brows);
			
	/* Create log file path*/
	strcpy(logfile_name, g_module_path);
	strcat(logfile_name, "\\minalic.log");

	/* Create global logger object*/
	g_logger = logger_create(logfile_name, run_mode, conflog_level);
	assert(g_logger);
	
	/* Create global Sessionhander object*/
	g_sessionhandler = sessionhandler_create(conf_timeout);
	assert(g_sessionhandler);

	/* Create global Cgihander object*/
	g_cgihandler = cgihandler_create(conf_cgi_timeout);
	assert(g_cgihandler);

	/* Create global Clientcounter object*/
	g_clientcounter = clientcounter_create(conf_threads);
	assert(g_clientcounter);

	/* Create the server wwwroot path by adding wwwroot to the server module path*/
	strcpy(g_wwwroot_path, g_module_path);
	strcat(g_wwwroot_path, "wwwroot");

	/* Do startup loggings*/
	logger_print(g_logger, 1, "MinaliC server %s  build:%s", VERSION,BUILD);
	logger_print(g_logger, 1, "Server loglevel: %d", conflog_level);
	logger_print(g_logger, 1, "Server handlers: %d", conf_threads);
	/*Only show webserver values if we have a web server running*/
	if (g_conf_port != 0)
	{
		logger_print(g_logger, 1, "Web root: %s", g_wwwroot_path);
		logger_print(g_logger, 1, "Http session timeout: %d", conf_timeout);
		logger_print(g_logger, 1, "CGI timeout: %d", conf_cgi_timeout);
		logger_print(g_logger, 1, "Directory browsing: %s", (g_conf_brows_default ? "ON" : "OFF"));
	}

    /*Set server start time*/
    g_uptime = time(NULL);
	
#ifdef USE_ASSERT /* If build with assers*/
	/* Show that assert mode is active*/
	logger_print(g_logger, 1, "Build with assert mode");
#endif /*USE_ASSERT*/

#ifdef USE_MEMCOUNTER /* If build with dynamic memory counting*/
	/* Show that memcount mode is active*/
	logger_print(g_logger, 1, "Build with memory counting mode");
#endif /*USE_MEMCOUNTER*/

}


/*
 * Function for starting a socket listener thread.
 */
void start_bind(int port,char *plugin_name)
{
	struct sockaddr_in	srv_s_addr;
	SOCKET				srv_sock;
	WORD				version_requested;
	WSADATA				data;
	int					err;
	Acceptor			*acceptor = NULL;
	Acceptorplugin		*acceptor_plugin = NULL;
	
	/* Initialize socket api*/
	version_requested = MAKEWORD(2, 0);
 	err = WSAStartup(version_requested, &data);
	if (err != 0) 
	{
		logger_print(g_logger, 1, "Could not find a usable WinSock DLL");
		exit(1);
	}
	srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	srv_s_addr.sin_family = AF_INET;
	/* Accept all adresses*/
	srv_s_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  
	/* Port to listen on*/
	srv_s_addr.sin_port = htons(port);  

	/* Socket bind to port*/
	if (bind(srv_sock, (struct sockaddr*) &srv_s_addr, sizeof(srv_s_addr)))
	{
		logger_print(g_logger, 1, "Could not bind to port %d", port);
		exit(1);
	}
	else
	{
		/*Check if this is a webserver binding or a custom plugin binding*/
		if (plugin_name == NULL)
		{
			logger_print(g_logger, 1, "Web server binding to port: %d, ready for requests", port);
		}
		else
		{
			logger_print(g_logger, 1, "Plugin %s binding to port: %d ", plugin_name, port);
		}
	}

	/* Socket listen on port*/
	listen(srv_sock,5);

	if (plugin_name == NULL)
	{
		/* Create a web acceptor object that performs the socket accepts from clients*/
		acceptor = acceptor_create(srv_sock);
		assert(acceptor);
	}
	else
	{
		/* Create a plugin acceptor object that performs the socket accepts from clients*/
		acceptor_plugin = acceptorplugin_create(srv_sock, plugin_name);
		assert(acceptor_plugin);
	}
}

/*
 * This function starts the server by starting the socket bindings
 */
void start_server()
{
	Map			*plugin_map;
	Mapelement	*e;
	int			plugin_port_int;
	char		*plugin_port;
	char		*plugin_module;

	if (g_conf_port != 0)
	{
		/*Start web server socket*/
		start_bind(g_conf_port, NULL);	
	}

	/*Start other plugin sockets*/
	plugin_map = config_get_plugins(g_conf);
	e = map_findfirst(plugin_map);
	if (e != NULL)
	{
		do
		{	
			plugin_module = e->key;
			plugin_port = e->value;
			plugin_port_int = atoi(plugin_port);
			start_bind(plugin_port_int, plugin_module);
			e = map_findnext(plugin_map);
		}while(e != NULL);
	}

	/* Delete the Config object since we don´t need it anymore*/
	config_delete(g_conf);
}


/*
 * Function that cleans up server objects and close the socket api.
 */
void endServer()
{
	sessionhandler_delete(g_sessionhandler);
	cgihandler_delete(g_cgihandler);
	clientcounter_delete(g_clientcounter);
	logger_delete(g_logger);
	WSACleanup();
	
	/* Finally delete the memcounter after all other objects dealloctions*/
#ifdef USE_MEMCOUNTER
    memcounter_delete(g_memcounter);
#endif
}

/*
 * Function for showing the help text
 */
void ShowUsage()
{
	printf("minalic -install | -stop | -start | -uninstall | -run\n");
}

/*
 * Function that performs the windows service installation.
 */
Result DoInstallService(int argc, char *argv [])
{
	SC_HANDLE   service;
    SC_HANDLE   sc_manager;
	char		executable[256];
	char		exec_path[256]="";

	strcpy(g_service_name, "Minalic webserver");
	if (argc == 1)
	{
		if (!_strnicmp(argv[0],"-name:",6)) 
		{
			strcpy(g_service_name,&argv[0][6]);	
		}
	}
	sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    GetModuleFileNameA(NULL, executable, 256);
	strcat(executable, " -srv");

	if ( sc_manager )
    {
        service = CreateServiceA(sc_manager,
									(LPCSTR)g_service_name,
									(LPCSTR)g_service_name,
									SERVICE_START | SERVICE_STOP,
									SERVICE_WIN32_OWN_PROCESS, 
									SERVICE_AUTO_START,
									SERVICE_ERROR_NORMAL,
									executable,
									NULL, NULL, NULL, NULL, NULL);
        if (service)
        {
			StartService(service, 0, NULL);
			CloseServiceHandle(service);
			return OK;
        }
	}
	return ERR;
}

/*
 * Function that stops and uninstalls the windows service.
 */
Result DoRemoveService(int argc, char *argv [])
{
    SC_HANDLE   service;
    SC_HANDLE   sc_manager;
	Result		res = ERR;

	strcpy(g_service_name, "Minalic webserver");
	if (argc == 1)
	{
		if (!_strnicmp(argv[0], "-name:", 6)) 
		{
			strcpy(g_service_name, &argv[0][6]);
		}
	}

    sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( sc_manager )
    {
        service = OpenServiceA(sc_manager, (LPCSTR)g_service_name, SERVICE_ALL_ACCESS);

        if (service)
        {
            /*Try to stop the service*/
            if (ControlService(service, SERVICE_CONTROL_STOP, &g_service_status))
            {
                printf("Stopping %s.", g_service_name);
                Sleep(1000);

                while(QueryServiceStatus(service, &g_service_status))
                {
                    if (g_service_status.dwCurrentState == SERVICE_STOP_PENDING)
                    {
                        printf(".");
                        Sleep(1000);
                    }
                    else
                        break;
                }

                if (g_service_status.dwCurrentState == SERVICE_STOPPED)
				{
                    printf("\n stopped.\n");	
				}
                else
                    printf("\n failed to stop.\n");
            }

            /* Now remove the service*/
            if(DeleteService(service))
			{
                printf("%s removed.\n", g_service_name);
				res = OK;
			}
            CloseServiceHandle(service);
        }
        CloseServiceHandle(sc_manager);
    }
    return res;
}

/*
 * Function that stops the windows service.
 */
Result DoStopService(int argc, char *argv [])
{
    SC_HANDLE   service;
    SC_HANDLE   sc_manager;
	Result		res = ERR;

	strcpy(g_service_name,"Minalic webserver");
	if (argc == 1)
	{
		if (!_strnicmp(argv[0],"-name:",6)) 
		{
			strcpy(g_service_name,&argv[0][6]);
		}
	}

    sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager)
    {
        service = OpenServiceA(sc_manager, (LPCSTR)g_service_name, SERVICE_ALL_ACCESS);

        if (service)
        {
            /* Try to stop the service*/
            if (ControlService(service, SERVICE_CONTROL_STOP, &g_service_status))
            {
                printf("Stopping %s.", g_service_name);
                Sleep(1000);

                while(QueryServiceStatus(service, &g_service_status))
                {
                    if (g_service_status.dwCurrentState == SERVICE_STOP_PENDING)
                    {
                        printf(".");
                        Sleep(1000);
                    }
                    else
					{
                        break;
					}
                }

                if (g_service_status.dwCurrentState == SERVICE_STOPPED)
				{
                    printf("\n stopped.\n");
					res = OK;
				}
                else
                    printf("\n failed to stop.\n");
            }
            CloseServiceHandle(service);
        }
        CloseServiceHandle(sc_manager);
    }
	return res;
}

/*
 * Function that starts the windows service.
 */
Result DoStartService(int argc, char *argv [])
{
    SC_HANDLE   service;
    SC_HANDLE   sc_manager;
	Result		res = ERR;

	strcpy(g_service_name,"Minalic webserver");
	if (argc == 1)
	{
		if (!_strnicmp(argv[0],"-name:",6)) 
		{
			strcpy(g_service_name,&argv[0][6]);
		}
	}

    sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager)
    {
        service = OpenServiceA(sc_manager, (LPCSTR)g_service_name, SERVICE_ALL_ACCESS);

        if (service)
        {
           	StartService(service, 0, NULL);
		    CloseServiceHandle(service);
			res = OK;
        }
        CloseServiceHandle(sc_manager);
    }
    return res;
}


/*
 * The main function of the server process. 
 * This function acts both as the main install function called as a normal application
 * and as routine that starts the service control dispatcher from SCM (Service Control Manager).
 */
int main(int argc, char *argv [])
{
    if (argc < 2) /* Started as an application*/ 
	{
		/* Initialized server to start as an application*/
		initialize_server(APPLICATION);
		start_server();
		/* Main loop for the server*/
		do
		{	Sleep (SERVER_UPDATE_TIME);
			/* Periodically delete old sessions in the Sessionhandler*/
			sessionhandler_delete_old(g_sessionhandler);
			/* Periodically delete old cgi processes*/
			cgihandler_delete_old(g_cgihandler);
		
		}while(1);
		endServer();
	}
	else 
	{
		printf("MinaliC server %s  build:%s Command Line\n", VERSION,BUILD);
		/*The servers is started as an application*/
		if (!_strnicmp(argv[1], "-uninstall", 10)) 
		{
			if (DoRemoveService(argc - 2, &argv[2]) == OK)
				printf("Command completed successfully");
		}
		else if (!_strnicmp(argv[1], "-stop", 5)) 
		{
			if (DoStopService(argc - 2, &argv[2]) == OK)
				printf("Command completed successfully");
		}
		else if (!_strnicmp(argv[1], "-start", 6)) 
		{
			if (DoStartService(argc - 2, &argv[2]) == OK)
				printf("Command completed successfully");
		}
		else if (!_stricmp(argv[1], "-install"))	
		{
			if (DoInstallService(argc - 2, &argv[2]) == OK)
				printf("Command completed successfully");
		}
		else if (!_strnicmp(argv[1], "-?", 2))
		{
			ShowUsage();	
		}
		else if (!_strnicmp(argv[1], "-srv", 2)) /* Started from service control managed*/
		{
			SERVICE_TABLE_ENTRYA  DispatchTable[] =	{{g_service_name, service_main},{ NULL,	NULL }};
		
			/*The servers is initialized to start as a service*/
			initialize_server(SERVICE);

			if (!StartServiceCtrlDispatcherA(DispatchTable))
			{
				ShowUsage();
			}	
		}
		else
		{
			printf("Error - wrong number of arguments\n\n");
			ShowUsage();
		}
	}
	return 0;
}


/*
 * Service entry point, called when the service is created.
 */
void WINAPI service_main(DWORD argc, LPSTR argv[])
{
	/* The service runs in its own process*/
	g_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;	
	/* The service is starting*/
	g_service_status.dwCurrentState = SERVICE_START_PENDING;	
	/* Can be stopped and be notified when system is shut_down*/
	g_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;  
	/* Running normal (no error)*/	
	g_service_status.dwWin32ExitCode = NO_ERROR;
	/* Running normal (no error)*/
	g_service_status.dwServiceSpecificExitCode = NO_ERROR;
	/* Service incremental checkpoint reset*/ 
	g_service_status.dwCheckPoint = 0;
	/* An estimate of the amount of time nedded in pending modes*/
	g_service_status.dwWaitHint = 2 * CSTIMEOUT;

	/* Register a function to handle the service control requests*/
	g_service_status_handle = RegisterServiceCtrlHandlerA((LPCSTR)g_service_name, server_ctrl_handler);
	if (g_service_status_handle == 0) 
		logger_print(g_logger,1, "Cannot register control handler");

	/* Updates the service control manager's status */
	SetServiceStatus(g_service_status_handle, &g_service_status);

	/* Start the service-specific work, now when the generic work is complete */
	if (service_specific(argc,argv) != 0)
	{
		g_service_status.dwCurrentState = SERVICE_STOPPED;
		g_service_status.dwServiceSpecificExitCode = 1;  /* Server initilization failed */
		SetServiceStatus (g_service_status_handle, &g_service_status);
		return;
	}
	
	/* Here the serverspecific execution is finished, so report status stopped*/
	update_status(SERVICE_STOPPED);
	
	return;
}


/*
 * This is the service specific function called when the service is started.
 */
int service_specific(int argc, char *argv[])
{	
	 /* At this point we are running so change to status running*/
	update_status(SERVICE_RUNNING);
	
	/* Start the webserver*/
	start_server();
			
	/* Main loop for service wating for shut_down*/
	while (!g_shut_down) 
	{ 
		/* Shut_down is set on a shut down control in SCM*/
		Sleep(SERVER_UPDATE_TIME);	
		/* Periodically delete old sessions in the Sessionhandler*/
		sessionhandler_delete_old(g_sessionhandler);
		/* Periodically delete old cgi processes*/
		cgihandler_delete_old(g_cgihandler);
	}
	logger_print(g_logger,1, "Server stoped.");
	endServer();
	return 0;
}


/*
 * Callback control handler called from SCM to change status of the service.
 */
void WINAPI server_ctrl_handler(DWORD control)
{
	switch (control)
	{
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		/* Answer SCM with stop_pending*/
		update_status(SERVICE_STOP_PENDING);	
		/* Set the global shut_down flag */
		g_shut_down = 1;	
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		if (control > 127 && control < 256) 
			break;
	}	
	return;
}


/* 
 * Set a new status and checkpoint for the service. 
 */
void update_status(int new_status)
{
	if (new_status >= 0) g_service_status.dwCurrentState = new_status;
	if (!SetServiceStatus(g_service_status_handle, &g_service_status))
		logger_print(g_logger,1, "Failed to set status for service", g_service_name);
	return;
}
