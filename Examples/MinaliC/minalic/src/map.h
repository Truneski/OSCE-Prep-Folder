/*
 * Declaration of the Map class.
 * Map is a class to handle lists of a key and
 * a value pairs.
 */

#ifndef _MAP
#define _MAP

/* 
 * The Mapelement data type that represents
 * an element in the Map.
 */
//typedef struct Mapelement Mapelement;
struct Mapelement;
typedef struct Mapelement
{
	char *key;
	char *value;
	struct Mapelement *next;
} Mapelement;

/* 
 * The Map data type.
 */
typedef struct Map
{
	Mapelement *findPrev;
	Mapelement *current;
	Mapelement *list;
} Map;

/* The Map methods*/
Map *map_create();
void map_delete(Map *self);
Mapelement *map_add(Map *self, char *key, char *value);
Mapelement *map_remove(Map *self, char *key);
Mapelement *map_find(Map *self, char *key);
Mapelement *map_set(Map *self, char *key, char *value);
char *map_getval(Map *self, char *key);
Mapelement *map_findfirst(Map *self);
Mapelement *map_findnext(Map *self);
void map_set_element_value(Mapelement *e, char *value);


#endif /*_MAP*/
