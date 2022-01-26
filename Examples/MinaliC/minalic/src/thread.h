/*
 * Declaration of the Thread class.
 * This class is a base class for all classes
 * that runs in a separate thread.
 */

#ifndef _THREAD
#define _THREAD

#include <windows.h>

/* 
 * The Thread data type.
 */
typedef struct Thread
{
	HANDLE	_handle;
	unsigned _id;
	/* Virtual run method*/
	void	(*_run)(void* self);	
	/* Virtual destructor*/
	void	(*_delete)(void* self);	
} Thread;

/* The Thread methods*/
Thread *thread_create();
void thread_delete(Thread *self);
static void thread_run(void* self);
void thread_start(Thread *self);
int thread_isRunning(Thread *self);
void thread_terminate(Thread *self);
void thread_suspend(Thread *self);
void thread_resume(Thread *self);

#endif /*_THREAD*/
