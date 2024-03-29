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

/* Constante correspondant � la longueur du tableau de points */
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
	unsigned int nb_pieces;					/* nombre de pi�ces dont disposent le personnage */
	unsigned int score;						/* score atteint par le personnage */
	unsigned int time;						/* temps restant au personnage pour terminer le niveau */
	char *nom_niveau;						/* nom du niveau dans lequel se trouve le niveau */
	struct item* reserve_item;				/* item que le personnage poss�de en r�serve */	
	unsigned int nb_monstres_tues;			/* variable permettant de calculer le score du personnage en sautant sur les monstres*/
	unsigned int nb_monstres_tues_carapace;	/* variable permettant de calculer le score du personnage en lan�ant une carapace */
	int tab_points[TAB_POINTS_LENGTH];		/* tableau correspondant � la suite des points lorsque le personnage tue des monstres */
	file_pts* file_points;					/* file correspondant � l'affichage des points dans le jeu */
}HUD;


/* Structure data_texture */
typedef struct data_texture 
{	
	GLuint texture;							/* l'identificateur OpenGL de la texture du personnage */
	int abscisse_bas;						/* contient l'abscisse du point d'o� l'on calculera les collisions */
	int ordonnee_haut;						/* contient l'ordonnee du point d'o� l'on calculera les collisions */
	int nb_sprites[NB_ETATS_TEXTURE];		/* tableau contenant le nombre de sprites de chacun des �tats du personnage */
	int nb_sprites_max;						/* indique le nombre maximum de sprites en largeur dans la texture du personnage */
	int v_anim[NB_ETATS_TEXTURE];			/* tableau contenant la vitesse d'animation de chacun des �tats du personnage */
	int phase_prec;							/* indique la phase pr�c�ndente dans la texture */
	int use_phase_prec;						/* indique s'il daut utiliser la phase pr�c�dente plut�t que la phase actuelle */
}data_texture;



/* Structure Personnage */
typedef struct perso
{
	coordf position;						/* la position absolue du personnage dans le niveau */
	coordf position_prec;					/* la position absolue du personnage dans le niveau � l'image precedente */
	coordi taille;							/* la taille du perso (en pixels) */
	vectf vitesse;							/* la vitesse du personnage */
	id personnage;							/* l'identificateur du personnage (inutile?) */
	int environnement;						/* environnment dans lequel se trouve le personnage */
	int cote;								/* indique vers quel cot� est tourn� le personnage (gauche ou droite) */
	int etat;								/* l'etat actuel du personnage */
	int transformation;						/* la transformation actuelle du personnage (FIRE_MARIO, MINI_MARIO,...) */
	int peut_grimper;						/* indique si le perso peut grimper � une �chelle */
	float accel;							/* acc�l�ration actuelle du personnage */
	occ_monstre* monstre_porte;				/* pointeur sur l'occurrence du monstre port� par le personnage */
	Uint32 tps_invincible;					/* indique le temps qu'il reste au personnage avant qu'il ne redevienne vuln�rable */
	Uint32 tps_invincible_etoile;			/* indique le temps qu'il reste au personnage avant qu'il ne redevienne vuln�rable */
	Uint32 tps_transformation;				/* indique le temps restant au personnage pour se transformer */
	Uint32 tps_attaque;						/* indique le temps restant au personnage pour qu'il reste en �tat d'attaque */
	Uint32 tps_attaque_speciale;			/* indique le temps restant au personnage pour qu'il reste en �tat d'attaque sp�ciale */
	Uint32 tps_pousse_carapace;				/* indique le temps restant au personnage pour qu'il reste en �tat o� il pousse qqch */
	Uint32 tps_mort;						/* indique le temps restant avant la fin d'animation de mort */
	Uint32 tps_finish;						/* indique le temps restant au personnage avant la fin d'animation de fin de niveau */
    Uint32 tps_saut;                        /* temps depuis lequel le saut � commenc� */
	data_texture* texture_act;				/* indique les infos sur l'affichage du personnage par apport � la texture actuelle */
	data_texture* texture_prec;				/* indique les infos sur l'affichage du personnage par apport � la texture pr�c�dente */
	FSOUND_SAMPLE* sons[NB_SONS];			/* contient les sons des actions du personnage */
	HUD* hud;								/* informations sur le personnage */
	tuyau* tuyau;							/* pointeur sur un tuyau si mario se trouve dedans */
	int checkpoint;							/* indique si le personnage a pass� le checkpoint */
} perso;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation d'un nouveau personnage */
perso* new_perso(void);

/* Initilisation des valeurs par defaut */
perso* init_perso(perso *p);

/* Initialisation du perso au d�but de chaque niveau */
void init_perso_niveau(perso* p);

/* Lib�re le perso en m�moire */
perso* free_perso(perso *p);

/* Fait une copie du personnage */
perso* copy_perso(perso *p);

/* Libere le perso uniquement (sans liberer les pointeurs) */
perso* free_copy_perso(perso *p);

/* Copie les donn�es de textures de 'data' dans 'copy' */
void copy_data_texture(data_texture* data, data_texture* copy);

/* Compte le nombre d'�tats que le personnage ne poss�de pas dans la texture */
int nb_etats_absents(data_texture *d);

/* Change la texture de mario  en fonction de son �tat actuel et de l'�tat dans le quel il va �tre */
void transforme_perso(int transformation_future, perso* p);

/* Charge un monstre � partir du fichier .perso */
void charger_perso(char *nom, perso* p);

/* Initialisation du HUD du personnage */
void init_HUD(perso* p);

/* Initialisation d'une file */
file_pts* new_file_pts();

/* Ajoute un nouvel �l�ment dans la file */
file_pts* add_file_pts(file_pts* file, int points, coordi position);

/* Lib�ration d'�l�ments */
file_pts* free_file_pts(file_pts* file);

/* Calcul des poins � la fin d'un niveau */
int calc_finish_points(perso** persos, int nb_persos);

#endif
