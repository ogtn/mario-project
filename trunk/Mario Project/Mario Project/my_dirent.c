/*============================================================================*/
/*== [my_dirent.c] [Version: 0.028a]                                        ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "my_dirent.h"


int closedir(DIR *dir)
{
	int res = -1;

	if(dir != NULL)
	{
		if(FindClose(dir->search_handle) != 0)
			res = 0;

		free(dir);
	}

	return res;
}


DIR *opendir(const char *path)
{
	DIR *dir = NULL;
	HANDLE hSearch;
	dirent first;
	char pattern[MAX_PATH + 2];

	strcpy(pattern, path);
	strcat(pattern, "\\*");

	hSearch = FindFirstFileA(pattern, &first.data);

	if(hSearch != INVALID_HANDLE_VALUE)
	{
		dir = malloc(sizeof(DIR));
		dir->current = first;
		dir->search_handle = hSearch;
		dir->pos = 0;
	}

	return dir;
}


dirent *readdir(DIR *dir)
{
	dirent *res = NULL;

	if(dir != NULL)
	{
		int last = 0;

		if(dir->pos != 0)
			last = !FindNextFileA(dir->search_handle, &dir->current.data);

		if(!last)
		{
			strcpy(dir->current.d_name, dir->current.data.cFileName);
			res = &dir->current;
			dir->pos++;
		}
	}

	return res;
}
