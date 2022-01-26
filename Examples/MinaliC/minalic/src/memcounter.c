/*
 * Definition of the Memcounter class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "memcounter.h"


/*
 * Constructor
 */
Memcounter *memcounter_create()
{
	Memcounter	*new_obj;
	int        i;

	new_obj = malloc(sizeof(Memcounter));
	if (new_obj)
	{
        for (i = 0; i < MAX_MEMOBJECTS; i++)
        {
		  new_obj->_mem_objects[i] = 0;
        }
		InitializeCriticalSection(&new_obj->_critical_section);
	}
	return new_obj;
}

/*
 * Destructor
 */
void memcounter_delete(Memcounter *self)
{
	if (self)
	{
		DeleteCriticalSection(&self->_critical_section);
		free(self);
	}
}

/*
 * This method counts up the usage count of an object
 */
void memcounter_inc(Memcounter *self, Memobject obj)
{
    /* Take the mutex*/
    EnterCriticalSection(&self->_critical_section);
    	
    (self->_mem_objects[obj])++;
    	
    /* Give the mutex*/
    LeaveCriticalSection(&self->_critical_section);
}

/*
 * This method counts down the usage count of an object
 */
void memcounter_dec(Memcounter *self, Memobject obj)
{
   	/* Take the mutex*/
   	EnterCriticalSection(&self->_critical_section);
    	
    (self->_mem_objects[obj])--;
    	
    /* Give the mutex*/
    LeaveCriticalSection(&self->_critical_section);
}

void memcounter_report(Memcounter *self, char *report)
{
   	/* Take the mutex*/
   	EnterCriticalSection(&self->_critical_section);
    	
    sprintf(report, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d=%d=%d=%d:%d.%d;%d",
    self->_mem_objects[0], self->_mem_objects[1], self->_mem_objects[2], self->_mem_objects[3],
    self->_mem_objects[4], self->_mem_objects[5], self->_mem_objects[6], self->_mem_objects[7],
    self->_mem_objects[8], self->_mem_objects[9], self->_mem_objects[10], self->_mem_objects[11],
    self->_mem_objects[12], self->_mem_objects[13], self->_mem_objects[14], self->_mem_objects[15],
    self->_mem_objects[16], self->_mem_objects[17], self->_mem_objects[18], self->_mem_objects[19],
    self->_mem_objects[20]);
   
    /* Give the mutex*/
    LeaveCriticalSection(&self->_critical_section);
}

