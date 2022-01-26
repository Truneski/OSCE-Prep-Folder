/*
 * Declaration of the Directorybrowser class.
 * This class is responsible for deciding if a
 * directory content is allowed to be listed. 
 */

#ifndef _DIRECTORYBROWSER
#define _DIRECTORYBROWSER

#include "common.h"

/* 
 * The Directorybrowser data type.
 */
typedef struct Directorybrowser
{
	char	_directory[FILE_SIZE];
	int		_dir_brows_default;
	char	*_output;
	char	*_pos;
	int		_pages;
	int		_is_root;
	char	*_web_file;
} Directorybrowser;

/* The Directorybrowser methods*/
Directorybrowser *directorybrowser_create(char * directory, int dir_brows_default, char *web_file);
int directorybrowser_can_brows(Directorybrowser *self);
Result directorybrowser_get_brows_data(Directorybrowser *self, char **output);
void directorybrowser_delete(Directorybrowser *self);


#endif /*_DIRECTORYBROWSER*/
