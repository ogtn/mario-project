/*============================================================================*/
/*== [items.h] [Version: 0.028a]                                           ==*/
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

#ifndef ITEMS_H_
#define ITEMS_H_

#define charger_fleur() charger_item("fleur", FLEUR);
#define charger_champignon() charger_item("champignon", CHAMPIGNON );
#define charger_mini_champi() charger_item("mini_champi", MINI_CHAMPI );
#define charger_champivie() charger_item("champivie", CHAMPI_VIE);
#define charger_piece() charger_item("piece", PIECE);
#define charger_champipoison() charger_item("champi_poison", CHAMPI_POISON);
#define charger_etoile() charger_item("etoile", ETOILE);

#define VIT_SORTIE_BLOC		0.05F

enum {
	PIECE,
	CHAMPIGNON,
	FLEUR,
	MINI_CHAMPI,
	CHAMPI_POISON,
	ETOILE,
	CHAMPI_VIE,
};

typedef enum {
	NORMAL,
	SORT_DU_BLOC
} ETAT_ITEM;


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "texture.h"
#include "perso.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure occ_item */
typedef struct occ_item {
	coordf position;						/* la position absolue de l'item dans le niveau */
	coordf position_prec;					/* la position absolue de l'item dans le niveau à l'image precedente */
	coordf vitesse;							/* la vitesse actuelle de l'item */
	ETAT_ITEM etat;							/* etat de l'item */
	int	tps_sortie_bloc;					/* temps qu'il reste avant que l'item sorte d'un élément du décor */
	int actif;								/* indique si l'item est actif (dans l'écran) */
	struct item* type_item;					/* pointeur sur l'item général de l'occurence */
	struct perso** perso_destine;
}occ_item;

/* Structure elem_item */
typedef struct elem_item
{
	occ_item *occ_item;						/* l'occurence de l'item */
	struct elem_item *suivant;				/* pointeur sur le suivant dans la liste */
}elem_item;


/* Structure liste_item */
typedef struct liste_item {
	elem_item* item;						/* représente la liste d éléments items */
	nb nb_elements;							/* indique le nombre d'éléments présents dans la liste */
}liste_item;


/* Structure item */
typedef struct item {
	coordi taille;							/* la taille de l'item (en pixels) */
	GLuint texture;							/* l'id de la texture de l'item */
	int nom;								/* nom de l'item */
	coordf vitesse;							/* indique la vitesse de référende de l'item */
	int soumission;							/* indique le type de soumission dont est victime l'item */
	int nb_sprites;							/* indique le nombre de sprites dont est composé l'item */
	liste_item* occ_items;					/* Liste de toutes les occurences de ce modèle d'item */
}item;

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation d'un item */
item* new_item(void);

/* Initialisation d'un item */
item* init_item(item* i);

/* Allocation d'une occurence d'un item */
occ_item* new_occ_item(float position_x, float position_y,item* type_item, coordf vitesse, ETAT_ITEM etat);

/* Initialisation d'une occurence d'un item */
occ_item* init_occ_item(float position_x, float position_y, item* type_item, occ_item* i, coordf vitesse, ETAT_ITEM etat);

/* Allocation d'une liste d'items */
liste_item* new_liste_item();

/* Copie l'occurence d'un item */
occ_item* copy_item(occ_item* item);

/* Création d'un nouvel élément */
liste_item* creer_element_occ_item(occ_item* i);

/* Ajoute un item dans la liste */
liste_item* ajout_item(liste_item* liste, occ_item* i);

/* Supprime une occurence d'un monstre de la liste */
liste_item* supprime_item(liste_item* liste, occ_item *i);

/* Chargement d'un monstre à partir d'un nom d'item */
item* charger_item(char* nom, int type_item);

#endif
