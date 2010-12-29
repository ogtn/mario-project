/*============================================================================*/
/*== [bloc.h] [Version: 0.028a]                                             ==*/
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

#ifndef BLOC_H_
#define BLOC_H_

typedef enum ETAT_BLOC
{
	IMMOBILE,
	POUSSE_PAR_LA_GAUCHE,
	POUSSE_PAR_LA_DROITE,
	POUSSE_PAR_LE_HAUT
}ETAT_BLOC;


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "vectors.h"
#include "items.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Un bloc est au niveau ce qu'un pixel est à une image: le plus petit element existant */
typedef struct bloc
{
	id texture;								/* l'id de la texture dans la structure niveau */
	int est_vide;							/* indique si le bloc contient est vide ou contient qqch */
	int est_cassable;						/* indique si le bloc est cassable */
	coordi coord_sprite;					/* les coordonnées du sprite dans la texture */
	int phys;								/* Contient la physique du bloc sous la forme d'un flag */
	item* item;								/* Contient l'item */
} bloc;

typedef struct occ_bloc
{
	coordi position;						/* Position actuelle du bloc */
	ETAT_BLOC etat;							/* Contient l'etat du bloc */
	bloc* bloc_actuel;						/* Pointeur sur le bloc modèle actuel */
	bloc* bloc_alternatif;					/* Pointeur sur le bloc modèle alternatif */
} occ_bloc;

/* Liste simplement chainee de blocs */
typedef struct elt_liste_bloc
{
    struct elt_liste_bloc *suivant;			/* Element suivant */
    bloc elt;								/* Contenu */
} elt_liste_bloc;


typedef struct liste_bloc
{
    elt_liste_bloc *debut;					/* Premier element */
    int nb;									/* Taille de la liste */
} liste_bloc;

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

occ_bloc* new_occ_bloc(int pos_x, int pos_y, bloc* bloc_actuel, bloc* bloc_alternatif);

occ_bloc* init_occ_bloc(occ_bloc* occ, int pos_x, int pos_y, bloc* bloc_actuel, bloc* bloc_alternatif);

/*********** Gestion des listes chainees ***********/

/* Allocation et desallocation d'une liste de blocs */
liste_bloc *new_liste_bloc();
void free_liste_bloc(liste_bloc **liste);

/* Allocation et desallocation d'un élément de la liste de blocs */
elt_liste_bloc *new_elt_liste_bloc();
void free_elt_liste_bloc(elt_liste_bloc **elt);

/* Ajoute un element a une liste sans doublon */
void ajouter_bloc_liste(liste_bloc *liste, bloc elt);

/* Retire l'element de la liste */
void supprimer_bloc_liste(liste_bloc *liste, bloc elt);

/* Genere un tableau a partir de la liste */
void bloc_liste_a_tableau(liste_bloc *liste, int *taille, bloc **tab);

#endif