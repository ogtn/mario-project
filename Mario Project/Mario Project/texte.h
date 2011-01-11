/******************************************************************************/
/*	texte.h		v0.8		05/02/2010			Guittonneau '[ZBM]' Olivier   */
/******************************************************************************/

#ifndef TEXTE_H
#define TEXTE_H

/*			=======================[Defines/Enums]====================		  */

#define NB_CARACTERES       256
#define STR_MAX_LEN         500
#define NB_LIGNES_MAX       50
#define DEBUT_LIGNE_X       8
#define DEBUT_LIGNE_Y       5


/*			=======================[Includes]=========================		  */

#include "texte_interface.h"
#include <stdarg.h>
#include "sprite.h"
#include "texture.h"


/*			=======================[Structures]=======================		  */

/* Caractere d'une police  genere avec Bitmap Font Generator:
http://www.angelcode.com/products/bmfont/ */
typedef struct caractere
{
	vecti position;     /* Position du pixel bas-gauche dans la texture */
	vecti taille;       /* Taille du caractere en pixels */
	vecti offset;       /* Offset en pixels */
	int	avance_x;       /* Avancement en pixel */
	vectf coords_bg;    /* Coordonnées de texture du point bas gauche */
	vectf coords_hd;    /* Coordonnées de texture du point haut droit */
} caractere;


/* Police en 2 dimensions, genere avec Bitmap Font Generator:
http://www.angelcode.com/products/bmfont/ */
struct font_2d
{
	caractere carac[NB_CARACTERES];			/* Tableau des caracteres */
	GLuint id_texture;						/* Identifiant OpenGL de la texture */
	char nom_texture[TAILLE_NOM_TEXTURE];	/* Nom du fichier de texture correspondant */
	vecti resolution;						/* Resolution de l'image, en pixels */
	int hauteur_ligne;						/* Hauteur d'une ligne, en pixels */
	int outline;							/* Largeur du liseret autour des caracteres */
};


/* Chaine de caractere en 2 dimensions */
typedef struct string_2d
{
    vecti pos;          /* Position de la chaine en pixels */
    font_2d *font;      /* Police en 2 dimension */
    couleur c;          /* La couleur au format 0xRRGGBB, voir enum couleur plus haut */
    char *str;          /* Contenu de la chaine */
} string_2d;


/* Element de liste chainee de string_2d */
typedef struct str_2d_elt
{
    string_2d str;
    struct str_2d_elt *next;
    Uint32 time_left;
} str_2d_elt;


/* Liste chainee de string_2d */
typedef struct str_2d_list
{
    str_2d_elt *first;
    size_t nb_elts;
} str_2d_list;


/* Variables globales du module */
typedef struct g_vars
{
    string_2d *buffer_2d;       /* Buffer des chaines 2d en attente de rendu */
    size_t nb_msg_2d;           /* Nombre de messages dans le buffer 2d */
    size_t nb_msg_max;          /* Nombre maximum de messages dans le buffer 2d */
    int ligne_actuelle;         /* Ligne courante de l'affichage debug */
    int colonne_actuelle;       /* Colonne courante de l'affichage debug */
    font_2d *debug_font;        /* Police 2d de l'affichage debug */
    couleur couleur;            /* Couleur de l'affichage debug */
} g_vars;


/*			=======================[Prototypes]=======================		  */

font_2d *new_font_2d(void);

font_2d *init_font_2d(font_2d *f);

void free_font_2d(font_2d **f);

font_2d *lire_font_2d(FILE *flux, font_2d *f);

char *chaine_apres_char(const char *chaine, char delim);

void draw_str_2d(string_2d str_2d);

int determine_colonne(string_2d str_2d);

void screen_vprintf(vecti pos, font_2d *font, couleur c, char *format, va_list ap);

void add_str_2d_to_list(str_2d_list *list, vecti pos, font_2d *font, couleur c, Uint32 time_left, char *format, ...);

void rem_str_2d_from_list(str_2d_list *list, str_2d_elt *elt);


#endif
