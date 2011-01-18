/*============================================================================*/
/*== [browser.h] [Version: 0.028a]	                                        ==*/
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

#ifndef BROWSER_H
#define BROWSER_H

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"

#ifdef _MSC_VER
#include "my_dirent.h"
#else
#include <dirent.h>
#include <sys/types.h>
typedef struct dirent dirent;
#define MAX_PATH 260
#endif


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/


typedef struct dossier dossier;
struct dossier
{
	char path[MAX_PATH];

	char **fichiers;
	char **dossiers;
	char dossier_parent[MAX_PATH];
	char racine[MAX_PATH];
    char extension[MAX_PATH];

	size_t nb_dossiers; 
	size_t nb_fichiers;
};


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

dossier *new_dossier(char *path, char *extension);

void free_dossier(dossier **dos);

void ouvre_sous_dossier(dossier **dos, size_t n);

void ouvre_dossier_parent(dossier **dos);

void ouvre_racine(dossier **dos);

void afficher_dossier(dossier *dos);

/* Inscrit le chemin du fichier i du dossier dos dans la chaine dest (de la racine à l'extension) */
void get_file_path(dossier *dos, size_t i, char *dest);

/* Inscrit le nom du fichier i du dossier dos dans la chaine dest (sans l'extension) */
void get_file_name(dossier *dos, size_t i, char *dest);

/* Inscrit le nom complet du fichier i du dossier dos dans la chaine dest (avec l'extension) */
void get_file_full_name(dossier *dos, size_t i, char *dest);

#endif
