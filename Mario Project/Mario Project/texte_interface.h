/******************************************************************************/
/*	texte_interface.h   v0.8	05/02/2010  	Guittonneau '[ZBM]' Olivier   */
/*******************************************************************************
Ce petit module permet d'�crire simplement du texte � l'�cran, dans une 
application qui utilise OpenGL, sans avoir � se soucier de l'endroit ou l'on
doit placer les appels: le texte est effectivement affich� � l'�cran au moment
d�sir�, et non pas lors de l'appel aux fonctions d'�criture.
*******************************************************************************/

#ifndef TEXTE_INTERFACE_H
#define TEXTE_INTERFACE_H

/*			=======================[Defines/Enums]====================		  */

/* Quelques couleurs de base, format: 0xAARRGGBB */
typedef enum colors
{
    COLOR_AQUA =      0xFF00FFFF,
    COLOR_BLACK =     0xFF000000,
    COLOR_BLUE =      0xFF0000FF,
    COLOR_FUCHSIA =   0xFFFF00FF,
    COLOR_GRAY =      0xFF808080,
    COLOR_GREEN =     0xFF008000,
    COLOR_LIME =      0xFF00FF00,
    COLOR_MAROON =    0xFF800000,
    COLOR_NAVY =      0xFF000080,
    COLOR_OLIVE =     0xFF808000,
    COLOR_PURPLE =    0xFF800080,
    COLOR_RED =       0xFFFF0000,
    COLOR_SILVER =    0xFFC0C0C0,
    COLOR_TEAL =      0xFF008080,
    COLOR_WHITE =     0xFFFFFFFF,
    COLOR_YELLOW =    0xFFFFFF00
} couleur;


/*			=======================[Includes]=========================		  */

#include "alloc_free.h"
#include "vectors.h"


/*			=======================[Structures]=======================		  */

typedef struct font_2d font_2d;

/*			=======================[Prototypes]=======================		  */

/* Envoie dans le buffer la chaine formatt�e, le textue sera plac� automatiquement
sur l'�cran, comme dans la console (sauf que le retour � la ligne n'est pas auto.
La police et la couleur sont celles pass�es � init_text() */
void screen_printf_dbg(char *format, ...);

/* Identique � screen_printf_dbg(), sauf qu'on pr�cise la position et la police.
Si on lui passe NULL comme police, elle utilise la police par defaut, donn�e
dans le init_text() */
void screen_printf(vecti pos, font_2d *font, couleur c, char *format, ...);

/* Provoque l'affichage de tout ce qui se trouve dans le buffer. A appeler en fin
de boucle, pour que le texte soit au premier plan. */
void screen_flush(void);

/* Initialise le module. On lui passe le nom de la police, la taille du buffer d�sir�
et la couleur. Cette police et cette couleur deviendrons la police par defaut, 
utilis�e par screen_printf_dbg() */
void init_text(char *font_name, size_t buff_size, couleur c);

/* Charge une police � partir de l'emplacement du fichier .fnt */
font_2d *load_font_2d(char *nom);

/* Quitte le module proprement (ou pas :p) */
void quit_text(void);

#endif
