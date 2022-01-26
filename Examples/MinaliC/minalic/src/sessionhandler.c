/*
 * Definition of the Sessionhandler class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sessionhandler.h"
#include "common.h"
#include "map.h"
#include "logger.h"
#include "memcounter.h"

/*
 * Constructor
 */
Sessionhandler *sessionhandler_create(int seconds)
{
	Sessionhandler *new_obj;
	new_obj = malloc(sizeof(Sessionhandler));
	meminc(SESSIONHANDLER);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->_timeout = seconds;
		new_obj->_sessions = map_create();
		new_obj->_differ = 0;
		/* Initialize the critical section object*/
		InitializeCriticalSection(&new_obj->_critical_section);
	}
	return new_obj;
}

/*
 * Destructor
 */
void sessionhandler_delete(Sessionhandler *self)
{
	assert(self);
	assert(self->_sessions);
	map_delete(self->_sessions);
	DeleteCriticalSection(&self->_critical_section);
	free(self);
	memdec(SESSIONHANDLER);
}

/*
 * Private method that generates a new session id.
 */
static char *generate_id(char *session_id, int differ )
{
	int		i;
	/* Buffer to pick random characters from*/
	char	*character = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789";

	assert(session_id);
	/* Initialize the random generator with the time*/
	srand((unsigned) clock() + (int)time(NULL) + differ);

	/* Loop through the buffer and get a random value for each character*/
	for ( i = 0; i < SESSION_ID_SIZE-1; i++ )
		/* Map the random value to a character in the buffer*/
		session_id[i] = character[rand() % 62];

	/* NULL terminate the result*/
	session_id[i] = '\0';
	return session_id;
}

/*
 * Private method that tries to generate a unique session id
 * by calling generate_id() one or several times.
 */
static void get_new_session_id(Sessionhandler *self, char *session_id)
{
	time_t	now;
	int		cnt = 0;
	char	now_buf[20];
	
	assert(self);
	assert(session_id);
	/* Start the try loop of generating unique id*/
	do
	{
		/* Count the tries*/
		cnt++;
		/* Get the current time of the generation*/
		time(&now);
		/* Create a timeout limit for the new id by adding the timeout constant*/
		now += self->_timeout;
		self->_differ++;
		/* Generate a id*/
		generate_id(session_id, self->_differ);
		/* Look in the session Map to se if the id is unique*/
	}while(map_find(self->_sessions, session_id) != NULL && cnt < SESSION_MAX_TRY);
	/* When we get here we probably have a unique id or
	 * else we accept the id anyway.
	 */
	/* Put the id in the session Map*/
	_ltoa((long)now, now_buf, 10);
	map_add(self->_sessions, session_id, now_buf);
}

/*
 * Private method that removes the session id if it has timed out.
 */
static int remove_invalid_session_id(Sessionhandler *self, char *session_id)
{
	Mapelement	*e;
	time_t		now;
	char		now_buf[20];
	int			exist = 0;

	assert(self);
	assert(session_id);

	/* Get the current time to compare the sessions time with*/
	time (&now);
	_ltoa((long)now, now_buf, 10);
	/* Se if the session id exist*/
	e = map_find(self->_sessions, session_id);
	if (e != NULL)
	{
		exist = 1;
		/* Test if the session has timed out*/
		if (strcmp(e->value, now_buf) < 0)
		{
			/* Remove the timed out session id*/
			map_remove(self->_sessions, e->key);
			/* Return removed*/
			return 1;
		}
	}
	/* se if session id still is alive*/
	if (exist)
	{
		/* Return not removed*/
		return 0;
	}
	else
	{
		/* Return removed*/
		return 1;
	}
}

/*
 * Private method that updates a session id with the current time.
 */
static void update_session_id_time(Sessionhandler *self, char *session_id)
{
	time_t	now;
	time_t	then;
	char	then_buf[20];
	
	assert(self);
	assert(session_id);

	/* Get current time*/
	time (&now);
	then = now + self->_timeout;
	_ltoa((long)then, then_buf, 10);
	/* Update or create session id with current time*/
	map_set(self->_sessions, session_id, then_buf);
}

/*
 * Method that removes all sessions that has timed out.
 */
void sessionhandler_delete_old(Sessionhandler *self)
{
	Mapelement	*e = NULL;
	time_t		now = 0;
	time_t		then = 0;

	/* 
	 * This is a common function called by several threads so
	 * it needs to be protected by a system critcal section.
	 */
	EnterCriticalSection(&self->_critical_section);
	assert(self);
	/* Get current time*/
	time ( &now );
	assert(now);
	/* Get first session id from map*/
	e = map_findfirst(self->_sessions);
	if (e != NULL)
	{
		/* Loop through all sessions to se if thay have timed out*/
		do
		{
			/* Get current sessions timeout limit*/
			then = atol(e->value);
			assert(then);
			/* Compare the sessions timeout limit to current time*/
			if (then - now < 0)
			{
				/* Remove the timed out session*/
				if (e = map_remove(self->_sessions, e->key))
				{
					/*
					 * If element was removed next element will be pointed to automaticly
					 * so don´t do find_next, just continue to beginning of loop
					*/
					continue;
				}
			}
			/* Get next session id*/
			e = map_findnext(self->_sessions);
		}while(e != NULL);
				
	}
	/* End of critical section*/
	LeaveCriticalSection(&self->_critical_section);
}

/*
 * Method that fully generates or updates a session id.
 * The function may generate a new id if the old has timed out.
 */
int sessionhandler_get_valid_id(Sessionhandler *self, char *session_id)
{
	int removed;

	assert(self);
	assert(session_id);

	/*
	 * This is a common function called by several server threads so
	 * it needs to be protected by a system critcal section.
	 */
	EnterCriticalSection(&self->_critical_section);
	
	/* If incomming session id is empty...*/
	if (strlen(session_id) == 0)
	{
		/* ...we genearate a new one*/
		get_new_session_id(self, session_id);
		LeaveCriticalSection(&self->_critical_section);
		/* Return that a new id is created*/
		return 1;
	}
	/* Else we have to check the incomming id*/
	else
	{
		/* Test if session id is timed out*/
		removed = remove_invalid_session_id(self, session_id);
		/* Test if it was removed from list*/
		if (removed)
		{
			/* Generate a new session id*/
			get_new_session_id(self, session_id);
			LeaveCriticalSection(&self->_critical_section);
			/* Return that old id was timed out*/
			return 2; 
		}
		else
		{
			/* Update incomming id with a new timestamp*/
			update_session_id_time(self, session_id);
			LeaveCriticalSection(&self->_critical_section);
			/* Resturn that incomming is was valid*/
			return 0; 
		}
	}
}
