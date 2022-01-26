/*
 * Declaration of the Config class.
 * This class is a helper class to read from
 * the configuration file.
 */
#define _CRT_SECURE_NO_WARNINGS
#ifndef _CONFIG
#define _CONFIG

#include "common.h"
#include "map.h"

/* 
 * The Config data type.
 */
typedef struct Config
{
	char _file[FILE_SIZE];
	Map *_values;
	Map *_plugin_values;
} Config;

/* The Config methods*/
Config *config_create(char *file_name);
void config_delete(Config *self);
char *config_getval(Config *self, char *varname);
Map *config_get_plugins(Config *self);

#endif /*_CONFIG*/
