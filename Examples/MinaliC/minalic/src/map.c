/*
 * Definition of the Map class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "logger.h"
#include "memcounter.h"

/*
 * Constructor
 */
Map *map_create()
{
	Map *new_obj;
	new_obj = malloc(sizeof(Map));
	meminc(MAP);
	assert(new_obj);
	if (new_obj)
	{
		new_obj->findPrev = NULL;
		new_obj->list = NULL;
	}
	return new_obj;
}

/*
 * Destructor
 */
void map_delete(Map *self)
{
	Mapelement *e = self->list;
	Mapelement *f;

	assert(self);
	if (self)
	{
		while(e != NULL)
		{
			f = e->next;
			assert(e->key);
			free(e->key);
			memdec(MAPELEMP1);
			assert(e->value);
			free(e->value);
			memdec(MAPELEMP2);
			assert(e);
			free(e);
			memdec(MAPELEMENT);
			e = f;
		}
		free(self);
		memdec(MAP);
	}
}

/*
 * Adds a new element to the table, no check if it allready exists in the table.
 */
Mapelement *map_add(Map *self, char *key, char *value)
{
	Mapelement *e = NULL,*f;
	
	assert(self);

	e = malloc(sizeof(Mapelement));
	meminc(MAPELEMENT);
	assert(e);
	if (e == NULL)
		return NULL;

	e->key = malloc(strlen(key) + 1);
	meminc(MAPELEMP1);
	assert(e->key);
	if (e->key == NULL)
	{
		free(e);
		memdec(MAPELEMENT);
		return NULL;
	}
	
	e->value = malloc(strlen(value) + 1);
	meminc(MAPELEMP2);
	assert(e->value);
	if (e->value == NULL)
	{
		free(e);
		memdec(MAPELEMENT);
		free(e->key);
		memdec(MAPELEMP1);
		return NULL;
	}

	strcpy(e->key,key);
	strcpy(e->value,value);
	e->next = NULL;

	if (self->list == NULL)
	{
		self->list = e;
	}
	else
	{
		f = self->list;
		while(f->next != NULL)
		{
			f = f->next;
		}
		f->next = e;
	}
	
	return e;
}

/*
 * Removes an element from the map.
 */
Mapelement *map_remove(Map *self, char *key)
{
	Mapelement *e;
	Mapelement *f;

	assert(self);
	assert(key);

	e = map_find(self, key);

	if (e != NULL)
	{
		/* If first element in list*/
		if (e == self->list) 
		{
			f = e->next;
			free(e->key);
			memdec(MAPELEMP1);
			free(e->value);
			memdec(MAPELEMP2);
			free(e);
			memdec(MAPELEMENT);
			self->list = f;
			if (e == self->current)
				self->current = f;
		}
		else
		{
			f = e->next;
			free(e->key);
			memdec(MAPELEMP1);
			free(e->value);
			memdec(MAPELEMP2);
			free(e);
			memdec(MAPELEMENT);
			self->findPrev->next = f;
			if (e == self->current)
				self->current = f;
		}

		return self->current;
	}

	return e;

}

/*
 * Finds an element in the map.
 */
Mapelement *map_find(Map *self, char *key)
{
	Mapelement *e;

	assert(self);
	assert(key);

	e = self->findPrev = self->list;
	
	if (e == NULL)
		return NULL;


	while(e != NULL)
	{
		if (_stricmp(e->key, key) == 0)
		{
			return e;
		}
		self->findPrev = e;
		e = e->next;
	}
	return NULL;
}

/*
 * Finds an element in the map and return its value.
 */
char *map_getval(Map *self, char *key)
{
	Mapelement *e;

	assert(self);
	
	e = map_find(self, key);
	if (e != NULL)
		return e->value;
	else
		return NULL;
}

/*
 * Sets a value to an mapelement.
 * If it exists it is overwritten else updated.
 */
Mapelement *map_set(Map *self, char *key, char *value)
{
	assert(self);

	if (map_find(self, key) != NULL)
	{
		map_remove(self, key);
	}
	return map_add(self, key, value);
}

/*
 * Gets first element in map. Should be celled before map_findnext.
 */
Mapelement *map_findfirst(Map *self)
{
	assert(self);
	self->current = self->list;
	return self->current;
}

/*
 * Gets the next element in map. Used to iterate through the maps elements.
 */
Mapelement *map_findnext(Map *self)
{
	assert(self);
	if(self->list != NULL)
	{
		if (self->current->next != NULL)
		{
			self->current = self->current->next;
			return self->current;
		}
	}
	return NULL;
}


void map_set_element_value(Mapelement *e,char *value)
{
	e->value = realloc(e->value, strlen(value)*sizeof(char) + 1);
	strcpy(e->value, value);
}




