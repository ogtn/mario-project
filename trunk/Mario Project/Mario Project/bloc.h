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

/* Définition des flags pour les blocs de base */
#define VIDE					0x000000
#define PLEIN					0x000001
#define SOL						0x000002
#define PLAFOND					0x000004
#define MUR_A_GAUCHE			0x000008
#define MUR_A_DROITE			0x000010
#define BORD_A_GAUCHE			SOL
#define BORD_A_DROITE			SOL

/* Numérotation par rapport à la hauteur du pixel en haut à gauche :
on enlève une certaine hauteur de l'ordonnée du point le plus à gauche de la pente par rapport au coin en haut à gauche */
#define PENTE_30_GAUCHE_8		0x000100 // Ici 8
#define PENTE_30_GAUCHE_16		0x000200 // Ici 16
#define PENTE_30_DROITE_0		0x000300 
#define PENTE_30_DROITE_8		0x000400
#define PENTE_45_GAUCHE			0x000800 // 16 pxls
#define PENTE_45_DROITE			0x001000 // 0 pxls

#define PENTE_15_DROITE_16		0x002000
#define PENTE_15_GAUCHE_16		0x004000
#define PENTE_15_DROITE_12		0x008000
#define PENTE_15_GAUCHE_12		0x010000
#define PENTE_15_DROITE_8		0x020000
#define PENTE_15_GAUCHE_8		0x040000
#define PENTE_15_DROITE_4		0x080000
#define PENTE_15_GAUCHE_4		0x100000

/* Définition des flags pour les blocs plus compliqués */
#define COIN_HAUT_A_GAUCHE		(SOL | MUR_A_GAUCHE)
#define COIN_HAUT_A_DROITE		(SOL | MUR_A_DROITE)
#define COIN_BAS_A_GAUCHE		(PLAFOND | MUR_A_GAUCHE)
#define COIN_BAS_A_DROITE		(PLAFOND | MUR_A_DROITE)
#define TUYAU_HAUT_GAUCHE		COIN_HAUT_A_DROITE
#define TUYAU_HAUT_DROITE		COIN_HAUT_A_GAUCHE
#define TUYAU_BAS_GAUCHE		MUR_A_DROITE
#define TUYAU_BAS_DROITE		MUR_A_GAUCHE
#define EAU						VIDE


/* Blocs avc lesquels Mario peut intéragir */
#define CASSABLE				0x200000
#define DISTRIBUTEUR_PIECE		0x400000
#define EST_VIDE				0x800000
#define BLOC_SPEC				(SOL | PLAFOND | MUR_A_DROITE | MUR_A_GAUCHE )

typedef enum ETAT_BLOC
{
	IMMOBILE,
	POUSSE_PAR_LA_GAUCHE,
	POUSSE_PAR_LA_DROITE,
	POUSSE_PAR_LE_HAUT,
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
	coordi coord_sprite;					/* les coordonnées du sprite dans la texture */
	int phys;								/* Contient la physique du bloc sous la forme d'un flag */
	int type_bloc;
	int tps_piece;							/* Temps restant avant qu'il ne reste qu'une pièce */
} bloc;

typedef struct occ_bloc
{
	coordi position;						/* Position actuelle du bloc */
	coordi position_prec;					/* POsition du bloc à l'image précédente */
	ETAT_BLOC etat;							/* Contient l'etat du bloc */
	int compteur_etat;						/* Définit l'étape de dessin d'un mouvement de bloc */
	int id_perso;							/* Identifiant du perso qui a frappé dans le bloc */
	int bloc_actuel;						/* Pointeur sur le bloc modèle actuel */
	int bloc_alternatif;					/* Pointeur sur le bloc modèle alternatif */
	int item;								/* Contient l'identifiant de l'item */
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

occ_bloc* new_occ_bloc(int pos_x, int pos_y, int bloc_actuel, int bloc_alternatif, int id_item);

occ_bloc* init_occ_bloc(occ_bloc* occ, int pos_x, int pos_y, int bloc_actuel, int bloc_alternatif, int id_item);

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