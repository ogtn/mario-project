/*============================================================================*/
/*== [projectile.h] [Version: 0.028a]                                       ==*/
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

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "texture.h"
#include "constantes.h"

/* Enum pour caractériser l'envoyeur */
enum {
	NIVEAU,
	GENTIL,
	MECHANT
};

/* Enum pour caractériser les projectiles */
enum {
	DEBRIS,
	FIREBALL,
	SPECIAL_FIREBALL,
	ICEBALL,
	THUNDER_BALL,
	SUPER_FIREBALL,
	HYPER_FIREBALL,
	SHURIKEN
};


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure occ_projectile */
typedef struct occ_projectile {
	coordf position;						/* la position absolue du projectile dans le niveau */
	coordf position_prec;					/* la position absolue du projectile dans le niveau à l'image precedente */
	coordf vitesse;							/* la vitesse actuelle du projectile */
	int envoyeur;							/* indique le type d'envoyeur du projectile */
	int cote;								/* indique le cote d'où est tourné le projectile */
	Uint32 tps_apparition;					/* indique le temps restant au projectile avant d'apparaitre */
	Uint32 tps_vie;							/* indique le temps qui reste au projectile avant de mourir */
	Uint32 tps_disparition;					/* indique le temps restant au projectile avant de disparaitre */
	int actif;								/* indique si le projectile est actif (dans l'écran ou non) */
	struct projectile* type_projectile;		/* pointeur sur le projectile général de l'occurence */
}occ_projectile;

/* Structure elem_projectile */
typedef struct elem_projectile
{
	occ_projectile *occ_projectile;			/* l'occurence du projectile */
	struct elem_projectile *suivant;		/* pointeur sur le suivant dans la liste de projectile */

}elem_projectile;


/* Structure liste_projectile */
typedef struct liste_projectile {
	elem_projectile* projectile;			/* représente la liste de projectiles */ 
	nb nb_elements;							/* indique le nombre d'éléments dans la liste */
}liste_projectile;


/* Structure projectile */
typedef struct projectile {
	coordi taille;							/* la taille du projectile (en pixels) */
	GLuint texture;							/* l'id de la texture du projectile */
	coordf vitesse;							/* vitesse du projectile (c'est la même pour tous les projectiles du même type) */
	int abscisse_bas;						/* contient l'abscisse du point d'où l'on calculera les collisions */
	int ordonnee_haut;						/* contient l'ordonnee du point d'où l'on calculera les collisions */
	int v_anim_marche;						/* vitesse d'animation du projectile lorsqu'il est encore en vie */
	int v_anim_mort;						/* vitesse d'animation du projectile lorsqu'il est en train de mourir */
	Uint32 tps_apparition;					/* indique le temps restant au projectile avant d'apparaitre */
	Uint32 tps_vie;							/* indique le temps qui reste au projectile avant de mourir */
	Uint32 tps_disparition;					/* indique le temps restant au projectile avant de disparaitre  */
	int nb_sprites_marche;					/* indique le nombre de sprites contenus dans la 'marche' du projectile */
	int nb_sprites_mort;					/* indique le nombre de sprites contenus dans la 'mort' du projectile */
	int soumission;							/* indique le type de soumission dont est victime le projectile */
	liste_projectile* occ_projectiles;
	char nom_text[TAILLE_NOM_TEXTURE];		/* nom de la texture */
}projectile;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation d'un nouveau projectile */
projectile* new_projectile(char* nom);

/* Initialisation d'un nouveau projectile */
projectile* init_projectile(projectile* p, char* nom);

/* Allocation d'une occurence d'un projectile */
occ_projectile* new_occ_projectile(int tps_vie, int tps_apparition, int tps_disparition);

/* Initialisation d'une occurence d'un projectile */
occ_projectile* init_occ_projectile(occ_projectile *p, int tps_vie, int tps_apparition, int tps_disparition);

/* Crée une copie du projectile */
occ_projectile* copy_projectile(occ_projectile* projectile);

/* Créer un élément de la liste de projectile */
elem_projectile* creer_element_projectile(occ_projectile* proj);

/* Alloue une liste de projectiles */
liste_projectile* new_liste_projectile();

/* Ajout d'un nouveau projectile dans la liste */
liste_projectile* ajout_projectile(liste_projectile* liste, occ_projectile* proj);

/* Supprime un projectile de la liste */
liste_projectile* supprime_projectile(liste_projectile* liste, occ_projectile *p);

/* Charge un projectile à partir d'un fichier */
projectile* charger_projectile(char* nom);

/* Créer un debris de bloc */
occ_projectile* create_debris(projectile* proj, int x, int y, float vitesse_x, float vitesse_y);

#endif