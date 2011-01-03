/*============================================================================*/
/*== [texture.h] [Version: 0.028a]                                          ==*/
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
/*==============================[DEFINES/ENUMS/]==============================*/
/******************************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#define	NB_TEXTURES_BASE	26
#define TAILLE_NOM_TEXTURE  50





/* Textures de base du jeu (boutons, polices...) */
enum {
	RIEN,
	FONT_1,
	TITLE_SCREEN,
	BUTTON_BLU,
	BUTTON_RED,
	BUTTON_BLK
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#ifdef WIN32
#include <windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "vectors.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Types */
typedef unsigned int ID, NB;
typedef unsigned char id, nb;


/* Structure texture
_nom: chemin relatif du fichier image
_id_text: identificateur OpenGL de la texture
_taille_sprite: taille des sprites que contient la texture (en pixels)
_taille: taille de la texture (en pixels)
_phys: tableau des types de pieces que contient la texture */
typedef struct texture
{
	char nom[TAILLE_NOM_TEXTURE];
	GLuint id_text;
	coordi taille_sprite;
	coordi taille;
	id *phys;
} texture;


/* Liste simplement chainee de textures */
typedef struct elt_liste_texture
{
    struct elt_liste_texture *suivant;      /* Element suivant */
    texture elt;                            /* Contenu */
} elt_liste_texture;


typedef struct liste_texture
{
    elt_liste_texture *debut;               /* Premier element */
    int nb;                                 /* Taille de la liste */
} liste_texture;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Charge la texture dont le nom est fournit en argument, et renvoie l'ID
correspondant. On peu également lui passer un pointeur sur un coordi dans lequel
la fonction va stocker la taille de la texture. On fournit NULL si pas besoin */
GLuint charger_texture_bis(char *nom, coordi *taille);

/* Charge les infos relatives à une texture depuis un fichier .txtr dans la 
texture pointée par t */
void charger_infos_texture(texture *t);

/* Sauvegarde dans un fichier .txtr les infos relatives à la texture pointée 
par t*/
void sauver_infos_texture(texture *t);

/* Remplace les caracteres suivants '.' par '\0' */
char *supprime_extension(char *nom);

/* Calcule la dimension de la nouvelle texture */
coordi calcul_nouvelle_taille(SDL_Surface *t);

/* Retourne le pixel qui se trouve en (x, y) dans la surface */
Uint32 getpixel(SDL_Surface *surface, int x, int y);


/*********** Gestion des listes chainees ***********/
liste_texture *new_liste_texture();
void free_liste_texture(liste_texture **liste);

elt_liste_texture *new_elt_liste_texture();
void free_elt_liste_texture(elt_liste_texture **elt);

/* Ajoute un element a une liste sans doublon */
void ajouter_text_liste(liste_texture *liste, texture elt);

/* Retire l'element de la liste */
void supprimer_text_liste(liste_texture *liste, texture elt);

/* Genere un tableau a partir de la liste */
void text_liste_a_tableau(liste_texture *liste, int *taille, texture **tab);


#endif
