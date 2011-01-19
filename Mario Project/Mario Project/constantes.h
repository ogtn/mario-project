/*============================================================================*/
/*== [constantes.h] [Version: 0.028a]                                       ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

/* Contient toutes les constantes definies dans le programme. Ces constantes
pourraient, après quelques modifications, etre remplacées par des variables
contenues dans une meme structure (definie dans ce fichier, et le fichier serait
chargé par un constantes.c). Cela permettrait de pouvoir personnaliser les
parametres du jeu tels que sa vitesse, la limite de fps, ou encore les touches
utilisées. Au lieu qu'ils soient codés en dur dans le programme.

Dans ce cas, les valeurs des variables doivent etre stockées dans un fichier chargé au
demarrage, pour sauvegarder les valeurs d'une execution à l'autre. Les anciennes
constantes resteraient les valeurs par defaut de ces variables, dans le cas ou
le fichier de config est manquant, ou dans le cas ou l'utilisateur desire
réinitialiser les parametres à leur valeur par defaut. */

/******************************************************************************/
/*==============================[DEFINES/ENUMS/]==============================*/
/******************************************************************************/

#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Types */
typedef unsigned int ID, NB;
typedef unsigned char id, nb;

/* Constantes générales */
#define VRAI						1
#define FAUX						0
#define TAILLE_NOM_NIVEAU			40
#define TAILLE_TITRE_MENUS			40
#define TAILLE_TITRE_MUSIQUE		40
#define TAILLE_NOM_TEXTURE          70
#define BLOC_VIDE					255
#define OBJET_VIDE					255
#define TPS_PROTEC_COLLISIONS		500
#define TPS_ITEM_SORT_BLOC			650
#define LARGEUR_BLOC				32.0F

/* constantes d'animation */
#define V_ANIM_MARCHE				400
#define V_ANIM_COURSE				V_ANIM_MARCHE / 2
#define V_ANIM_ATTAQUE				1000
#define V_ANIM_BLOC_SPEC			750

/* Constantes de l'ecran */
#define LARGEUR_FENETRE				1024
#define HAUTEUR_FENETRE				768
#define SCROLLING_AV				(float)0.5
#define SCROLLING_AR				(float)0.2
#define SCROLLING_HAUT				(float)0.5
#define SCROLLING_BAS				(float)0.2

/* Constantes du jeu */
#define MARIO						0
#define LUIGI						1

/* Pause portable */
#ifdef WIN32
#define my_sleep(x) Sleep(x)
#endif

#ifdef linux
#define my_sleep(x) usleep(x * 1000)
#endif

/* Pour gcc qui ne fournit pas cette fonction non standard */
#ifdef linux
char* itoa(int value, char* result, int base);
#endif

/* Touches utilisées */
enum{
	AUCUN = -1,
	GAUCHE,
	DROITE,
	HAUT,
	BAS,
	SAUTER,
	RUN,
	CLIC_G,
	CLIC_D,
	ENTRER,
	ECHAP,
	CTRL_G,
    SUPPR,
    CUT,
    COPY,
    PASTE,
    NB_TOUCHES,
};


/* Etats du personnage */
/* /!\ Toute modification doit etre repercutée sur le tableau de chaines qui suit /!\ 
On peut toutefois faire mieux, voir ici:
http://www.bien-programmer.fr/item_list.htm */
enum ETATS_PERSO {
	MARCHE,
	COURSE_2,
	SAUT,
	DEBOUT,
	MARCHE_CARAPACE,
	POUSSE_CARAPACE,
	ATTAQUE,
	ATTAQUE_SPECIALE,
	NB_ETATS_TEXTURE, // Nombre d'états max en hauteur pour la texture du personnage
	DEBOUT_CARAPACE,
	SAUT_CARAPACE,
	BAISSE_CARAPACE,
	SAUT_BAISSE_CARAPACE,
	REGARDE_HAUT,
	REGARDE_HAUT_CARAPACE,
	BAISSE,
	SAUT_BAISSE,
	ASSIS,
	SAUT_DESCENDANT,
	COTE_DROIT,
	COTE_GAUCHE,
	COURSE_1,
	DERAPE,
	GLISSE,
	MORT,
	SORT_TUYAU_HORIZONTAL,
	RENTRE_TUYAU_HORIZONTAL,
	SORT_TUYAU_VERTICAL,
	RENTRE_TUYAU_VERTICAL,
	FINISH,
	FINISH_CHATEAU,
	NB_ETATS
};


static char const *chaines_etats_perso[] =
{
    "MARCHE",
	"COURSE_2",
	"SAUT",
	"DEBOUT",
	"MARCHE_CARAPACE",
	"POUSSE_CARAPACE",
	"ATTAQUE",
	"ATTAQUE_SPECIALE",
	"NB_ETATS_TEXTURE",
	"DEBOUT_CARAPACE",
	"SAUT_CARAPACE",
	"BAISSE_CARAPACE",
	"SAUT_BAISSE_CARAPACE",
	"REGARDE_HAUT",
	"REGARDE_HAUT_CARAPACE",
	"BAISSE",
	"SAUT_BAISSE",
	"ASSIS",
	"SAUT_DESCENDANT",
	"COTE_DROIT",
	"COTE_GAUCHE",
	"COURSE_1",
	"DERAPE",
	"GLISSE",
	"MORT",
	"NB_ETATS"
};


/* Transformations des personnages */
/* /!\ Toute modification doit etre repercutée sur le tableau de chaines qui suit /!\ 
On peut toutefois faire mieux, voir ici:
http://www.bien-programmer.fr/item_list.htm */
enum {
	MINI_MARIO,
	SMALL_MARIO,
	SUPER_MARIO,
	FIRE_MARIO,
	ICE_MARIO,
	ELECTRIC_MARIO,
	WAR_MARIO,
	SHELL_MARIO,
	SUPER_FIRE_MARIO,
	NINJA_MARIO,
	NB_TRANSFORMATIONS
};


static char const *chaines_transformations[] =
{
    "MINI_MARIO",
	"SMALL_MARIO",
	"SUPER_MARIO",
	"FIRE_MARIO",
	"ICE_MARIO",
	"ELECTRIC_MARIO",
	"WAR_MARIO",
	"SHELL_MARIO",
	"SUPER_FIRE_MARIO",
	"NINJA_MARIO",
	"NB_TRANSFORMATIONS"
};

/* Environnements */
/* /!\ Toute modification doit etre repercutée sur le tableau de chaines qui suit /!\ 
On peut toutefois faire mieux, voir ici:
http://www.bien-programmer.fr/item_list.htm */
enum{
	SOL_DUR,
	AIR,
	AQUA
};


static char const *chaines_environement[] =
{
    "SOL_DUR",
	"AIR",
	"AQUA"
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#ifdef WIN32
#define  _USE_MATH_DEFINES
#include <windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "vectors.h"
#include "alloc_free.h"

/* Macros pratiques */
#ifndef min
#define min(a, b) (((a)<(b))?(a):(b))
#endif

#ifndef max
#define max(a, b) (((a)>(b))?(a):(b))
#endif


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

void print_time(void);
typedef struct bouton bouton;
typedef struct action action;


#endif
