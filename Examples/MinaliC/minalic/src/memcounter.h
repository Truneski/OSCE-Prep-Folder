/*
 * Declaration of the Memtcounter class.
 * This class is a class conting memory
 * allocation and deallications. It is used
 * as a debug class and doesn´t need to be included
 * in the build.
 */

#ifndef _MEMCOUNTER
#define _MEMCOUNTER

#include <windows.h>
#include "common.h"

#define MAX_MEMOBJECTS 21

/* Definition of the kind of memobjects to count*/
typedef enum Memobject
{
	ACCEPTOR = 0, ACCEPTORPLUGIN = 1, CGIHANDLER = 2, CGIPROCESS = 3, CLIENTCOUNTER = 4,
    CLIENTHANDLER = 5, CLIENTHANDLERPLUGIN = 6, CONFIG = 7, DIRECTORYBROWSER = 8,
    LOGGER = 9, REQUEST = 10, RESPONSE = 11, SESSIONHANDLER = 12, THREAD = 13,
    MAP = 14, MAPELEMENT = 15, MAPELEMP1 = 16, MAPELEMP2 = 17, CGIPROC2 = 18,
    RESPONSE2 = 19, DIRBROWS2 = 20
} Memobject;

/* 
 * The Memcounter data type.
 */
typedef struct Memcounter
{
	long                _mem_objects[MAX_MEMOBJECTS];
	CRITICAL_SECTION	_critical_section;
} Memcounter;

/* The Memcounter methods*/
Memcounter *memcounter_create();
void memcounter_inc(Memcounter *self, Memobject obj);
void memcounter_dec(Memcounter *self, Memobject obj);
void memcounter_report(Memcounter *self, char *report);
void memcounter_delete(Memcounter *self);

/*Declaration of global logger instance*/
extern Memcounter *g_memcounter;

/* The memcounter macro declarations*/
#ifdef  USE_MEMCOUNTER /* USE_MEMCOUNTER is defined in common.h*/
/* Declaration of the meminc macro used to increase usage count of objects*/
#define meminc(object) memcounter_inc(g_memcounter,object) 
/* Declaration of the meminc macro used to increase usage count of objects*/
#define memdec(object) memcounter_dec(g_memcounter,object) 
#else
#define meminc(object)     ((void)0)
#define memdec(object)     ((void)0)
#endif  /* USE_MEMCOUNTER */

#endif /*_MEMCOUNTER*/
