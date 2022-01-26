/*
 * Definition of the Thread class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include "thread.h"
#include "logger.h"
#include "memcounter.h"

/* Declaration of the thread function to run for all threads*/
static unsigned int thread_func (void *self);

/* Declaration of lib function for creating win32 threads*/
HANDLE _beginthreadex(  
   void		*security, 
   unsigned stack_size, 
   unsigned (*start_address)(void*), 
   void		*arglist, 
   unsigned initflag, 
   unsigned *thrdaddr  
); 

/*
 * Constructor
 */
Thread *thread_create()
{
	Thread	*new_obj;
	new_obj = malloc(sizeof(Thread));
	meminc(THREAD);
	assert(new_obj);
	if (new_obj)
	{
		/* Set the function to run*/
		new_obj->_run = thread_run;
		/* Set the destructor to run after execution*/
		new_obj->_delete = (void (*)(void*))thread_delete;
		/* Start the threads execution*/
		thread_start(new_obj);
	}
	return new_obj;
}

/*
 * Destructor
 */
void thread_delete(Thread *self)
{
	assert(self);
	assert(self->_handle);
	CloseHandle(self->_handle);
	free(self);
	memdec(THREAD);
}

/*
 * Method that starts thread execution.
 */
void thread_start(Thread *self)
{
	assert(self);
	self->_handle = (HANDLE) _beginthreadex(NULL, 0, thread_func, self, 0, &self->_id);
}

/*
 * Method that suspends thread execution.
 */
void thread_suspend(Thread *self)
{
	assert(self);
	SuspendThread(self->_handle);
}

/*
 * Method that resumes thread execution after suspend.
 */
void thread_resume(Thread *self)
{
	assert(self);
	ResumeThread(self->_handle);
}

/*
 * Method that returns if a thread is still running.
 */
int thread_isRunning(Thread *self)
{
	DWORD exit_code;

	assert(self);
	GetExitCodeThread(self->_handle, &exit_code);
	return (exit_code == STILL_ACTIVE);
}

/*
 * Method that forses termination of thread execution.
 */
void thread_terminate(Thread *self)
{
	assert(self);
	TerminateThread(self->_handle,0);
}

/*
 * Static thread function for all thread instances.
 * The function takes as parameter a reference to a thread instance.
 */
static unsigned int thread_func (void *self)
{
	Thread *t;
	t = (Thread*)self;
	assert(t);
	/* Run the threads run function*/
	t->_run(self);
	/*
	 * Here the treads job is done and we
	 * call the virtual destructor to automaticly
	 * clean up memory.
	 */
	t->_delete(self);
	
	return 0;
}
/*
 * Private method that has no implementation due to
 * that the actual instances of the thread class has
 * its own thread functions.
 */
static void thread_run(void* self)
{
	
}







