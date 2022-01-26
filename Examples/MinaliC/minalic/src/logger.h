/*
 * Declaration of the Logger class.
 * This class handles all logging to file
 * and stdout.
 */

#ifndef _LOGGER
#define _LOGGER

#include <windows.h>
#include "common.h"

/*Definition of the Logger type*/
typedef struct Logger
{
	char	_file[FILE_SIZE];
	Runmode	_Runmode;
	int		_log_level;
	/* Logger is used by several threads and need a critical section*/
	CRITICAL_SECTION _critical_section;
} Logger;

/* The Logger methods*/
Logger *logger_create(char *file_name, Runmode mode, int log_level);
void logger_delete(Logger *self);
void logger_print(Logger *self, int level, char *format,...);

/*Declaration of global logger instance*/
extern Logger *g_logger;

/* The assert declaration*/
#undef  assert
#ifdef  USE_ASSERT /* USE_ASSERT is defined in common.h*/
/* Declaration of the assert macro used to log failure in debug mode*/
#define assert(Expression) (!(Expression) ? logger_print(g_logger,1,"Fatal error %s %s %d",#Expression, __FILE__ , __LINE__ ):((void)0))
#else
/*Declaration of assert in relese mode*/
#define assert(Expression)     ((void)0)

#endif  /* USE_ASSERT */

#endif /*_LOGGER*/
