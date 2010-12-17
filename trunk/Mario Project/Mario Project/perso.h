/*============================================================================*/
/*== [personnage.h] [Version: 0.028a]                                       ==*/
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

#ifndef PERSO_H
#define PERSO_H

/* Constante correspondant à la longueur du tableau de points */
#define TAB_POINTS_LENGTH				8

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "texte_interface.h"
#include "texture.h"
#include "sound.h"
#include "monstre.h"
#include "items.h"
#include "tuyau.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Element d'une file */
typedef struct elem_file_pts {
	int points;
	int tps_affichage;
	coordi pos_points;
}elem_file_pts;


/* File pour l'affichage des points */
typedef struct file_pts {
	elem_file_pts* element;
	struct file_pts* suivant;
	struct file_pts* precedent;
}file_pts;


/* Structure HUD */
typedef struct HUD 
{
	unsigned int personnage;				/* identifiant du personnage */
	unsigned int nb_vies;					/* nombre de vies dont disposent le personnage */
	unsigned int nb_pieces;					/* nombre de pièces dont disposent le personnage */
	unsigned int score;						/* score atteint par le personnage */
	unsigned int time;						/* temps restant au personnage pour terminer le niveau */
	char *nom_niveau;						/* nom du niveau dans lequel se trouve le niveau */
	item* reserve_item;						/* item que le personnage possède en réserve */	
	unsigned int nb_monstres_tues;			/* variable permettant de calculer le score du personnage en sautant sur les monstres*/
	unsigned int nb_monstres_tues_carapace;	/* variable permettant de calculer le score du personnage en lançant une carapace */
	int tab_points[TAB_POINTS_LENGTH];		/* tableau correspondant à la suite des points lorsque le personnage tue des monstres */
	file_pts* file_points;					/* file correspondant à l'affichage des points dans le jeu */
    font_2d *font;                          /* police du HUD lors de l'affichage */
}HUD;


/* Structure data_texture */
typedef struct data_texture 
{	
	GLuint texture;							/* l'identificateur OpenGL de la texture du personnage */
	int abscisse_bas;						/* contient l'abscisse du point d'où l'on calculera les collisions */
	int ordonnee_haut;						/* contient l'ordonnee du point d'où l'on calculera les collisions */
	int nb_sprites[NB_ETATS_TEXTURE];		/* tableau contenant le nombre de sprites de chacun des états du personnage */
	int nb_sprites_max;						/* indique le nombre maximum de sprites en largeur dans la texture du personnage */
	int v_anim[NB_ETATS_TEXTURE];			/* tableau contenant la vitesse d'animation de chacun des états du personnage */
}data_texture;



/* Structure Personnage */
typedef struct perso
{
	coordf position;						/* la position absolue du personnage dans le niveau */
	coordf position_prec;					/* la position absolue du personnage dans le niveau à l'image precedente */
	coordi taille;							/* la taille du perso (en pixels) */
	vectf vitesse;							/* la vitesse du personnage */
	id personnage;							/* l'identificateur du personnage (inutile?) */
	int environnement;						/* environnment dans lequel se trouve le personnage */
	int cote;								/* indique vers quel coté est tourné le personnage (gauche ou droite) */
	int etat;								/* l'etat actuel du personnage */
	int transformation;						/* la transformation actuelle du personnage (FIRE_MARIO, MINI_MARIO,...) */
	float accel;							/* accélération actuelle du personnage */
	occ_monstre* monstre_porte;				/* pointeur sur l'occurrence du monstre porté par le personnage */
	Uint32 est_invincible;					/* indique le temps qu'il reste au personnage avant qu'il ne redevienne vulnérable */
	Uint32 est_invincible_etoile;			/* indique le temps qu'il reste au personnage avant qu'il ne redevienne vulnérable */
	Uint32 tps_transformation;				/* indique le temps restant au personnage pour se transformer */
	Uint32 tps_attaque;						/* indique le temps restant au personnage pour qu'il reste en état d'attaque */
	Uint32 tps_attaque_speciale;			/* indique le temps restant au personnage pour qu'il reste en état d'attaque spéciale */
	Uint32 tps_pousse_carapace;				/* indique le temps restant au personnage pour qu'il reste en état où il pousse qqch */
	Uint32 tps_mort;						/* indique le temps restant avant la fin d'animation de mort */
	Uint32 tps_finish;						/* indique le temps restant au personnage avant la fin d'animation de fin de niveau */
	data_texture* texture_act;				/* indique les infos sur l'affichage du personnage par apport à la texture actuelle */
	data_texture* texture_prec;				/* indique les infos sur l'affichage du personnage par apport à la texture précédente */
	FSOUND_SAMPLE* sons[NB_SONS];			/* contient les sons des actions du personnage */
	HUD* hud;								/* informations sur le personnage */
	tuyau* tuyau;							/* pointeur sur un tuyau si mario se trouve dedans */
	int passe_checkpoint;					/* indique si le personnage a passé le checkpoint */
} perso;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation d'un nouveau personnage */
perso* new_perso(void);

/* Initilisation des valeurs par defaut */
perso* init_perso(perso *p);

/* Libère le perso en mémoire */
perso* free_perso(perso *p);

/* Fait une copie du personnage */
perso* copy_perso(perso *p);

/* Libere le perso uniquement (sans liberer les pointeurs) */
perso* free_copy_perso(perso *p);

/* Copie les données de textures de 'data' dans 'copy' */
void copy_data_texture(data_texture* data, data_texture* copy);

/* Compte le nombre d'états que le personnage ne possède pas dans la texture */
int nb_etats_absents(data_texture *d);

/* Change la texture de mario  en fonction de son état actuel et de l'état dans le quel il va être */
void transforme_perso(int transformation_future, perso* p);

/* Charge un monstre à partir du fichier .perso */
void charger_perso(char *nom, perso* p);

/* Initialisation du HUD du personnage */
void init_HUD(perso* p);

/* Initialisation d'une file */
file_pts* new_file_pts();

/* Ajoute un nouvel élément dans la file */
file_pts* add_file_pts(file_pts* file, int points, coordi position);

/* Libération d'éléments */
file_pts* free_file_pts(file_pts* file);

#endif
