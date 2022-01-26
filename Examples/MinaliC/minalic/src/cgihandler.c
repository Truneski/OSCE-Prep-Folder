/*
 * Definition of the Cgihandler class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "cgihandler.h"
#include "map.h"
#include "logger.h"
#include "memcounter.h"

/*
 * Constructor
 */
Cgihandler *cgihandler_create(int seconds)
{
	Cgihandler *new_obj;
	new_obj = malloc(sizeof(Cgihandler));
	meminc(CGIHANDLER);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_timeout = seconds;
		new_obj->_processes = map_create();
		/* Initialize the critical section object*/
		InitializeCriticalSection(&new_obj->_critical_section);
	}
	return new_obj;
}

/*
 * Destructor
 */
void cgihandler_delete(Cgihandler *self)
{
	assert(self);
	assert(self->_processes);
	map_delete(self->_processes);
	DeleteCriticalSection(&self->_critical_section);
	free(self);
	memdec(CGIHANDLER);
}

/*
 * Method used to set or update that a cgi process is alive.
 * The method can be called repeatedly to set that the cgi
 * process is active.
 */
void cgihandler_set_active(Cgihandler *self, unsigned long pid)
{
	time_t	now;
	time_t	then;
	char	then_buf[20];
	char	pid_buf[20];
	char	*timeout;

	
	assert(self);

	/*
	 * This is a common function called by several server threads so
	 * it needs to be protected by a system critcal section.
	 */
	EnterCriticalSection(&self->_critical_section);

	/* Get current time*/
	time ( &now );
	then = now + self->_timeout;
	_ltoa((long)then, then_buf,10);
	_ltoa(pid, pid_buf,10);
	/* Read current timeout value for the process from the process map*/
	timeout = map_getval(self->_processes, pid_buf);
	/* If timeout value exist we must check it for 0*/
	if (timeout != NULL)
	{
		/* If the process already has be terminated don´t update timeout value*/
		if (strcmp(timeout, "0") == 0)
		{
			LeaveCriticalSection(&self->_critical_section);
			return;
		}
	}
	/* Update/create process id with current time*/
	map_set(self->_processes, pid_buf, then_buf);

	LeaveCriticalSection(&self->_critical_section);
}

/*
 * Method used to remove cgi process from process map.
 * The method return whether the process was timed out or not.
 */
int cgihandler_remove(Cgihandler *self, unsigned long pid)
{
	char	*timeout;
	char	pid_buf[20];
	int		was_timedout;

	/*
	 * This is a common function called by several server threads so
	 * it needs to be protected by a system critcal section.
	 */
	EnterCriticalSection(&self->_critical_section);

	_ltoa(pid, pid_buf, 10);
	timeout = map_getval(self->_processes,pid_buf);
	assert(timeout);
	/* If the process was timed out the process´ timeout value
	 * is set to 0 by the scheduler and here we check that value.
	 */
	was_timedout = (timeout[0] == '0'); 
	/* Remove from the map*/
	map_remove(self->_processes, pid_buf);

	LeaveCriticalSection(&self->_critical_section);	

	return was_timedout;
}

/*
 * Method that terminates all cgi processes that has timed out.
 * This method is called repeatedly by the server main function.
 */
void cgihandler_delete_old(Cgihandler *self)
{
	Mapelement		*e = NULL;
	time_t			now = 0;
	time_t			then = 0;
	unsigned long	pid;
	unsigned long	exitcode;
	HANDLE			hprocess;


	/* 
	 * This is a common function called by several threads so
	 * it needs to be protected by a system critcal section.
	 */
	EnterCriticalSection(&self->_critical_section);
	assert(self);
	/* Get current time*/
	time (&now);
	assert(now);
	/* Get first process id from map*/
	e = map_findfirst(self->_processes);
	if (e != NULL)
	{
		/* Loop through all processes to se if they have timed out*/
		do
		{
			/* Get current process timeout limit*/
			then = atol(e->value);
			/* Only check not terminated processes*/
			if (then > 0)
			{
				/* Compare the process timeout limit to current time*/
				if ( (then - now) < 0)
				{
					/* Set timeout value to 0, which means that
					 * the process has been terminated.
					 */
					map_set_element_value(e, "0");
					pid = atol(e->key);

					/* Now terminate the cgi process*/
					if (hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid))
					{
						GetExitCodeProcess(hprocess, &exitcode);
						if (exitcode == STILL_ACTIVE)
							TerminateProcess(hprocess, 1);
						CloseHandle(hprocess);	
					}
				}
			}
			/* Get next session id*/
			e = map_findnext(self->_processes);
		}while(e != NULL);
	}
	/* End of critical section*/
	LeaveCriticalSection(&self->_critical_section);
}
