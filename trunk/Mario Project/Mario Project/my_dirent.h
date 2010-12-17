/*============================================================================*/
/*== [my_dirent.h] [Version: 0.028a]	                                    ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

/******************************************************************************/
/*==============================[DEFINES/ENUMS]===============================*/
/******************************************************************************/

#ifndef MY_DIRENT_H
#define MY_DIRENT_H


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include <windows.h>
#include <string.h>
#include "constantes.h"
/* #include <sys/types.h> pour ino_t */

/* http://opengroup.org/onlinepubs/007908799/xsh/dirent.h.html
et http://www.softagalleria.net/dirent.php
La gestion des erreurs via errno sera pour plus tard, ou pas */

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

typedef struct dirent 
{
  char d_name[MAX_PATH];
  /* ino_t d_ino; pas trop compris comment recuperer ça sous sous windows pour le moment */
  WIN32_FIND_DATAA data;
} dirent;


typedef struct DIR 
{
  dirent current;
  HANDLE search_handle;
  long pos;
} DIR;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

int closedir(DIR *dir);

DIR *opendir(const char *path);

dirent *readdir(DIR *dir);

int readdir_r(DIR *dir, dirent *, dirent **);

/* Pas implémentées pour le moment, ça semble pas utile pour ce qu'on cherche à faire */
void rewinddir(DIR *dir);
void seekdir(DIR *dir, long pos);
long telldir(DIR *dir);


#endif
