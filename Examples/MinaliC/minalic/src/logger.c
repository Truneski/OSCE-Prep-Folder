/*
 * Definition of the Logger class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "logger.h"
#include "memcounter.h"

/*
 * Constructor
 */
Logger *logger_create(char *file_name,Runmode mode, int log_level)
{
	Logger	*new_obj;
	new_obj = malloc(sizeof(Logger));
	meminc(LOGGER);
	assert(new_obj);
	if (new_obj)
	{
		strcpy(new_obj->_file, file_name);
		new_obj->_log_level = log_level;
		new_obj->_Runmode = mode;
		InitializeCriticalSection(&new_obj->_critical_section);
	}
	return new_obj;
}

/*
 * Destructor
 */
void logger_delete(Logger *self)
{
	assert(self);
	DeleteCriticalSection(&self->_critical_section);
	free(self);
	memdec(LOGGER);
}

/*
 * Method that prints to the log.
 * The log can be the log file or standad output.
 * The method takes a format string and a variable number of parameters.
 */
void logger_print(Logger *self, int level, char *format,...)
{
	FILE		*out;
	time_t		rawtime;
	struct tm	*timeinfo;
	char		log_time[50];
	char		log_format[LOG_FORMAT_SIZE];
	va_list		args; 
	
	/*
	 * If the level of the message is higher than the current
	 * loglevel don´t log.
	 */
	if (level > self->_log_level)
		return;
		
	if (self == NULL)
		return;

	/*
	 * Protect this section by a CriticalSection object so it not will be used
	 * by several threads at the same time.
	 */
	EnterCriticalSection(&self->_critical_section);
	out = NULL;
	if (self->_Runmode == SERVICE)
		out = fopen(self->_file, "a");
	else if (self->_Runmode == APPLICATION)
		out = stdout;

	if (!out)
	{
		LeaveCriticalSection(&self->_critical_section);
		return;	
	}

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(log_time, sizeof(log_time), "%Y-%b-%d %H:%M:%S", timeinfo);
	va_start(args, format); 
	sprintf(log_format, "%s | %s\n", log_time,format);
	vfprintf(out, log_format, args);
	
	if (self->_Runmode == SERVICE)
		fclose(out);

	va_end(args); 
	LeaveCriticalSection(&self->_critical_section);
	/* End of critical section*/
}

