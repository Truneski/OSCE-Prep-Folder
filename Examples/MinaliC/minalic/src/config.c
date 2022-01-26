/*
 * Definition of the Config class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "logger.h"
#include "memcounter.h"

/* Define maximum length of a row in the config file*/
#define CONFIG_LINE 400

/*
 * Private method that parses a config line.
 * If it finds a proper config variable it will 
 * be added to the Map that holds all variables.
 */
static void parseLine(Config *self, char *line)
{
	char *p;
	char *key;
	char *value;
	char *value2;

	p = line;
	while(*p == ' ' || *p == '\t')
	{
		p++;
		if (!*p)
			return;
	}
	
	key = p;

	while(*p != ' ' && *p != '=')
	{
		p++;
		if (!*p)
			return;
	}
	
	*p = 0; 

	p++;
	if (*p)
	{
		while(*p == ' ' || *p == '=' || *p == '\t')
		{
			p++;
			if (!*p)
				return;
		}
	}
	
	value = p;

	while(*p != ' ' && *p != '\n' && *p != 0)
	{
		p++;
	}

	*p = 0;
	
	if (!_stricmp(key, "SERVER_PLUGIN"))
	{
		for (value2 = value; *value2 != ':'; value2++);
		*value2 = 0;
		value2++;
		/* Add plugin name and portnumber as a pair into the plugin map*/
		map_set(self->_plugin_values, value, value2);
	}
	else
	{
		/* Add found variable and its value to the Map*/
		map_set(self->_values, key, value);
	}
}

/*
 * Private method that reads the config file.
 */
static void readFile(Config *self)
{
	FILE   *f = NULL;
	char   line[CONFIG_LINE + 1];

	f = fopen(self->_file, "r");
	if (!f)
		return;
	
	while(!feof(f))
	{
		fgets(line, CONFIG_LINE, f);
		if (line[0] != '*')
		{
			parseLine(self, line);
		}
	}
	fclose(f);
}

/*
 * Constructor
 */
Config *config_create(char *file_name)
{
	Config *new_obj;

	new_obj = malloc(sizeof(Config));
	meminc(CONFIG);
	assert(new_obj);
	if (new_obj)
	{
		strcpy(new_obj->_file, file_name);
		new_obj->_values = map_create();
		new_obj->_plugin_values = map_create();
		readFile(new_obj);
	}
	return new_obj;
}

/*
 * Destructor
 */
void config_delete(Config *self)
{
	assert(self);
	if (self)
	{
		map_delete(self->_values);
		map_delete(self->_plugin_values);
		free(self);
		memdec(CONFIG);
	}
}

/*
 * Method that returns the value of a config variable.
 */
char *config_getval(Config *self, char *varname)
{
	assert(self);
	assert(varname);
	return map_getval(self->_values, varname);
}

/*
 * Method that returns the plugin map.
 */
Map *config_get_plugins(Config *self)
{
	assert(self);
	return self->_plugin_values;
}
