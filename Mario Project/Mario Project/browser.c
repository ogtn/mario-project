/*============================================================================*/
/*== [browser.c] [Version: 0.028a]	                                        ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "browser.h"


dossier *new_dossier(char *path, char *extension)
{
    dossier *dos = NULL;
    DIR *dir;

    dir = opendir(path);

    if(dir != NULL)
    {
        size_t i, j = 0;
        dirent *entity;
        dos = malloc(sizeof(dossier));

        dos->nb_dossiers = 0;
        dos->nb_fichiers = 0;
        strcpy(dos->path, path);
        dos->dossier_parent[0] = '\0';
        dos->racine[0] = '\0';
        strcpy(dos->extension, extension);

        while((entity = readdir(dir)) != NULL)
        {
            /* On ignore les fichiers/dossiers cachés, et les fichiers n'ayants pas la bonne extension */
            if(entity->d_name[0] == '.' || strstr(entity->d_name, extension) == NULL)
                continue;
                
            /* Si c'est un fichier */
            if(strchr(entity->d_name, '.'))
                dos->nb_fichiers++;
            else
                dos->nb_dossiers++;
        }

        dos->dossiers = malloc(sizeof(char *) * dos->nb_dossiers);
        for(i = 0; i < dos->nb_dossiers; i++)
            dos->dossiers[i] = malloc(MAX_PATH);

        dos->fichiers = malloc(sizeof(char *) * dos->nb_fichiers);
        for(i = 0; i < dos->nb_fichiers; i++)
            dos->fichiers[i] = malloc(MAX_PATH);

        /* Vu que pas de rewinddir(), on va faire comme ça pour le moment */
        closedir(dir);
        dir = opendir(path);
        i = 0;

        while((entity = readdir(dir)) != NULL)
        {
            if(entity->d_name[0] == '.' || strstr(entity->d_name, extension) == NULL)
                continue;
                
             /* Si c'est un fichier */
            if(strchr(entity->d_name, '.'))
            {
                strcpy(dos->fichiers[i], entity->d_name);
                i++;
            }
            else
            {
                strcpy(dos->dossiers[j], entity->d_name);
                j++;
            }
        }

        closedir(dir);
    }

    return dos;
}


void free_dossier(dossier **dos)
{
    if(dos != NULL && *dos != NULL)
    {
        size_t i;

        for(i = 0; i < (*dos)->nb_dossiers; i++)
            free((*dos)->dossiers[i]);

        if((*dos)->dossiers != NULL)
            free((*dos)->dossiers);

        for(i = 0; i < (*dos)->nb_fichiers; i++)
            free((*dos)->fichiers[i]);

        if((*dos)->fichiers != NULL)
            free((*dos)->fichiers);

        free(*dos);
        *dos = NULL;
    }
}


void ouvre_sous_dossier(dossier **dos, size_t n)
{
    if(dos != NULL && *dos != NULL && (*dos)->nb_dossiers > n)
    {
        char path_enfant[MAX_PATH];
        char path_parent[MAX_PATH];
        char racine[MAX_PATH];

        dossier *sous_dossier;

        strcpy(racine, (*dos)->racine);
        strcpy(path_parent, (*dos)->path);
        strcpy(path_enfant, (*dos)->path);
        strcat(path_enfant, "/");
        strcat(path_enfant, (*dos)->dossiers[n]);

        sous_dossier = new_dossier(path_enfant, (*dos)->extension);

        if(sous_dossier != NULL)
        {
            free_dossier(dos);
            *dos = sous_dossier;
            strcpy((*dos)->dossier_parent, path_parent);
            strcpy((*dos)->racine, racine);
        }
    }
}


void ouvre_dossier_parent(dossier **dos)
{
    /* On verifie que le dossier soit valide */
    if(dos != NULL && *dos != NULL && 
        strcmp((*dos)->dossier_parent, ""))
    {
        char *end_path;
        dossier *dossier_parent = new_dossier((*dos)->dossier_parent, (*dos)->extension);

        if(dossier_parent != NULL)
        {
            strcpy(dossier_parent->racine, (*dos)->racine);
            free_dossier(dos);
            *dos = dossier_parent;

            /* On determine le dossier parent */
            if(!strcmp((*dos)->path, (*dos)->racine))
                (*dos)->dossier_parent[0] = '\0'; //empeche de remonter plus haut que la racine
            else
            {
                strcpy((*dos)->dossier_parent, (*dos)->path);
                end_path = strrchr((*dos)->dossier_parent, '/');

                if(end_path != NULL)
                    *end_path = '\0';
            }
        }
    }
}


void ouvre_racine(dossier **dos)
{
    /* On verifie que le dossier soit valide */
    if(dos != NULL && *dos != NULL && strcmp((*dos)->racine, ""))
    {
        char *end_path;
        dossier *racine = new_dossier((*dos)->racine, (*dos)->extension);

        if(racine != NULL)
        {
            strcpy(racine->racine, (*dos)->racine);
            free_dossier(dos);
            *dos = racine;

            /* On determine le dossier parent */
            if(!strcmp((*dos)->path, (*dos)->racine))
                (*dos)->dossier_parent[0] = '\0'; //empeche de remonter plus haut que la racine
            else
            {
                strcpy((*dos)->dossier_parent, (*dos)->path);
                end_path = strrchr((*dos)->dossier_parent, '/');

                if(end_path != NULL)
                    *end_path = '\0';
            }
        }
    }
}


void afficher_dossier(dossier *dos)
{
    size_t i;

    printf("Dossier: %s\n", dos->path);
    printf("Dossier parent: %s\n", dos->dossier_parent);

    printf("%d fichier(s):\n", dos->nb_fichiers);
    for(i = 0; i < dos->nb_fichiers; i++)
        puts(dos->fichiers[i]);

    printf("%d dossier(s):\n", dos->nb_dossiers);
    for(i = 0; i < dos->nb_dossiers; i++)
        puts(dos->dossiers[i]);
}


void get_file_path(dossier *dos, size_t i, char *dest)
{
    if(dos != NULL && dos->nb_fichiers >= i && dest != NULL)
    {
        strcpy(dest, dos->path);
        strcat(dest, "/");
        strcat(dest, dos->fichiers[i]);
    }
}


void get_file_name(dossier *dos, size_t i, char *dest)
{
    if(dos != NULL && dos->nb_fichiers >= i && dest != NULL)
    {
        char *c;

        strcpy(dest, dos->fichiers[i]);
        c = strrchr(dest, '.');
        *c = '\0';
    }
}


void get_file_full_name(dossier *dos, size_t i, char *dest)
{
    if(dos != NULL && dos->nb_fichiers >= i && dest != NULL)
    {
        strcpy(dest, dos->fichiers[i]);
    }
}
