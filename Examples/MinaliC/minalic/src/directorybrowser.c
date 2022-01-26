/*
 * Definition of the Directorybrowser class
 */
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "directorybrowser.h"
#include "logger.h"
#include "memcounter.h"


/*
 * Constructor
 */
Directorybrowser *directorybrowser_create(char * directory, int dir_brows_default, char *web_file)
{
	Directorybrowser	*new_obj;

	new_obj = malloc(sizeof(Directorybrowser));
	meminc(DIRECTORYBROWSER);
	assert(new_obj);
	if (new_obj)
	{
		strcpy(new_obj->_directory,directory);
		new_obj->_dir_brows_default = dir_brows_default;
		/* Null terminate result buffer*/
		new_obj->_output = 0;
		new_obj->_pos = 0;
		/* Set initial size of result buffer to 2 pages*/
		new_obj->_pages = 2;
		new_obj->_web_file = web_file;
		/* Decide if this directory is the web root*/
		if (!strcmp(web_file,"/") || !strcmp(web_file,"\\"))
		{
			new_obj->_is_root = 1;
		}
		else
		{
			new_obj->_is_root = 0;
		}
	}
	return new_obj;
}

/*
 * Destructor
 */
void directorybrowser_delete(Directorybrowser *self)
{
	assert(self);
	if (self)
	{
		/* Check if we should free memory for result buffer*/
		if (self->_output != 0)
		{
			free(self->_output);
			memdec(DIRBROWS2);
        }
		/* Free object data*/
		free(self);
		memdec(DIRECTORYBROWSER);
	}
}

/*
 * Method that decides if a directory is allowed to be browsed
 */
int directorybrowser_can_brows(Directorybrowser *self)
{
	FILE	*f;
	char	dir_file[FILE_SIZE];

	/* First check if default value for browsing is not set*/
	if (!self->_dir_brows_default)
	{
		/* Default brows state is not allowed so look for brows file*/
		strcpy(dir_file, self->_directory);
		strcat(dir_file, DIRECTORY_BROWSING_FILE);
		f = NULL;
		/* Try to open the file*/
		f = fopen(dir_file, "rb");
		if (!f)
		{	/* Brows file did not exist so return not allowed*/
			return 0; 
		}
		else
		{
			/* Could open file so close it*/
			fclose(f);
			/* Brows file existed so return allowed*/
			return 1;
		}
	}
	return 1;
}

/*
 * Private method that writes data to result buffer.
 * It allocates new memory if needed.
 */
static void write_data(Directorybrowser *self, char *data)
{
	char *p;

	/* Check if new memory is needed for this writing*/
	if( ((self->_pages * DIR_BUFFER_SIZE) - (unsigned int)(self->_pos - self->_output)) < (unsigned int)strlen(data) + 1)
	{
		/* Allocate a new page of memory*/
		self->_pages++;
		self->_output = realloc(self->_output, DIR_BUFFER_SIZE * self->_pages);
	}

	/* Write to the result buffer*/
	for (p = data; *p != 0; p++)
	{
		*(self->_pos)++ = *p;
	}
	/* Null terminate written data*/
	*(self->_pos) = 0;
}

static void url_encode(const char *src, char *dst, size_t dst_len) 
{
  char *dont_escape = "._-$,;~()/";
  char *hex = "0123456789abcdef";
  char *end = dst + dst_len - 1;

  for (; *src != '\0' && dst < end; src++, dst++) 
  {
    if (isalnum(*(unsigned char *) src) || strchr(dont_escape, * (unsigned char *) src) != NULL) 
    {
      *dst = *src;
    } 
    else if (dst + 2 < end) 
    {
      dst[0] = '%';
      dst[1] = hex[(* (unsigned char *) src) >> 4];
      dst[2] = hex[(* (unsigned char *) src) & 0xf];
      dst += 2;
    }
  }
  *dst = '\0';
}

/*
 * Private method that print a directory name to the result.
 * It generates HTML to present in the browser.
 */
static void print_dir(Directorybrowser *self, char *name, int is_root)
{
	char urlenc[FILE_SIZE*2];

	/* If we are in the root directory dont´ print '..' */
	if (self->_is_root)
		if(!strcmp(name, ".."))
			return;

	/* Never print the '.' directory*/
	if (strcmp(name, "."))
	{
		url_encode(name,urlenc,sizeof(urlenc));	
		write_data(self, "<a href='");
		write_data(self, urlenc);
		write_data(self, "/'>");
		write_data(self, name);
		write_data(self, "</a>");
		write_data(self, "<BR>");
	}
}

/*
 * Private method that print a file name to the result.
 * It generates HTML to present in the browser.
 */
static void print_file(Directorybrowser *self, char *name)
{
	char urlenc[FILE_SIZE*2];

	/* Check that we dont´print the browsing file*/
	if (strcmp(name, DIRECTORY_BROWSING_FILE))
	{
		url_encode(name,urlenc,sizeof(urlenc));	
		write_data(self, "<a href='");
		write_data(self, urlenc);
		write_data(self, "'>");
		write_data(self, name);
		write_data(self, "</a>");
		write_data(self, "<BR>");
	}
}

/*
 * Method that reads the content of a directory and prints it.
 */
Result directorybrowser_get_brows_data(Directorybrowser *self, char **output)
{
	WIN32_FIND_DATA file_data;
	HANDLE			files;
	int				is_root = 0;

	/*Initialize write buffer*/
	if (self->_output == 0)
	{
		self->_output = malloc(DIR_BUFFER_SIZE * self->_pages);
		meminc(DIRBROWS2);
		self->_pos = self->_output;
		self->_pos = self->_pos;
	}

	/* Generate start HTML tags for the page*/
	write_data(self, "<html><head></head><body>");
	write_data(self, "<h2>");
	/* Print the directory name*/
	write_data(self, self->_web_file);
	write_data(self, "</h2>");

	/* Print subdirectories*/
	strcat(self->_directory, "*.*");
	files = FindFirstFileA(self->_directory, (LPWIN32_FIND_DATAA)&file_data);
	if (!files)
		return ERR;
	if (files != INVALID_HANDLE_VALUE)
	{
		/* Check that this is a directory*/
		if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			print_dir(self, (char*)file_data.cFileName, is_root);
		/* Print the rest of the subdirectories*/
		while (FindNextFileA(files, (LPWIN32_FIND_DATAA)&file_data))
		{
			if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				print_dir(self, (char*)file_data.cFileName, is_root);
		}
	}
	FindClose(files);

	/* Print files in the directory*/
	files = FindFirstFileA(self->_directory, (LPWIN32_FIND_DATAA)&file_data);
	if (!files)
		return ERR;
	if (files != INVALID_HANDLE_VALUE)
	{
		/* Check that this is a file*/
		if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			print_file(self, (char*)file_data.cFileName);
		/* Print the rest of the files*/
		while (FindNextFileA(files, (LPWIN32_FIND_DATAA)&file_data))
		{
			if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				print_file(self, (char*)file_data.cFileName);
		}
	}
	FindClose(files);

	/* Print HTML end tags*/
	write_data(self, "</body></html>");
	*output = self->_output;
	return OK;

}
