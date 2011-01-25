/*============================================================================*/
/*== [bouton.h] [Version: 0.028a]                                           ==*/
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

#ifndef BOUTON_H
#define BOUTON_H

#define TAILLE_AIDE_BOUTON				70
#define TAILLE_LABEL_BOUTON				30


/* Differents etats possibles d'un bouton */
enum{
	ENFONCE =			                0x1,
	RELEVE =			                0x2,
	JUSTE_ENFONCE =		                0x4,
	JUSTE_RELEVE =		                0x8
};

#define ON_RELEASE	                    JUSTE_RELEVE
#define ON_CLICK	                    JUSTE_ENFONCE
#define ON_CHANGE	                    JUSTE_ENFONCE | JUSTE_RELEVE


/* Differents types de boutons */
enum{
	ON_OFF =			                0x1,
	CLASSIQUE =			                0x2,
	RADIO =				                0x3
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "sprite.h"
#include "keystate.h"
#include "texte_interface.h"
#include "action.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

struct bouton
{
	coordi taille;                      /* taille en pixels */
	coordi pos;                         /* position en pixels */
	int type;                           /* type de bouton (voir enum plus haut) */
    int enabled;                        /* indique si le bouton est en fonctionnement */
	int etat_actuel;                    /* état actuel du bouton (voir enum plus haut) */
	int etat_precedent;                 /* l'état précédent du bouton */
    int survole;                        /* la souris est-elle au dessus? */

	int nb_sous_boutons;                /* nombre de "sous-boutons" (cas des checkbox, radio...) */
	struct bouton **sous_boutons;       /* tableau de pointeur des sous-boutons (cas des checkbox, radio...) */
    action *action;

	sprite sprite_enfonce;              /* sprite du bouton quand il est enfonce */
	sprite sprite_releve;               /* sprite du bouton quand il est releve */
	sprite sprite_inactif;              /* sprite du bouton quand il est inactif */
	sprite sprite_survole;              /* sprite du bouton quand il est survolé */

    char aide[TAILLE_AIDE_BOUTON];      /* aide/infobulle associée */
    char label[TAILLE_LABEL_BOUTON];    /* label du bouton, pratique pour les menus */
	coordi label_pos;                   /* position du label */
};


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation */
bouton *new_bouton(int type, int nb_sous_boutons);
bouton *new_bouton_classique(void);
bouton *new_bouton_radio(int nb_sous_boutons);
bouton *new_bouton_on_off(void);

/* Desallocation */
void free_bouton(bouton **b);

/* Copie */
bouton *copy_rec_btn(bouton *src);

/* Initialisation */
bouton *init_bouton(bouton *b);

/* Donne sa position au bouton */
bouton *set_pos(bouton *b, int x, int y);

/* Deplace le bouton (idem à set_pos, mais en position relative) */
bouton *move_btn(bouton *b, int x, int y);

/* Donne sa taille au bouton */
bouton *set_size(bouton *b, int x, int y);

/* Donne au bouton la texture correspondant à l'état enfoncé */
bouton *set_text_enfonce(bouton *b, char *name);

/* Idem pour l'etat relevé */
bouton *set_text_releve(bouton *b, char *name);

/* Donne au bouton une texture utilisée à l'identique pour tous les états */
bouton *set_text_unique(bouton *b, char *name);

/* Donne au bouton une texture qui sert aux quatre états, la texture doit comporter, de
haut en bas: bouton enfoncé, bouton relevé, bouton survolé, bouton inactif (grisé).
Cette fonction initialise également les tailles des boutons, ainsi que les coordonnées 
de textures. Elle utilise une fonction auxilliaire à ne pas utiliser en dehors de bouton.c.
Dans le cas de bouton radio, la meme texture va etre attribuée à chaque sous-bouton,
si ce n'est pas l'effet recherché, il faut appeller la fonction pour chaque
sous-bouton avec la texture voulue. */
bouton *set_text_globale(bouton *b, char *name);
bouton *set_text_globale_aux(bouton *b, GLuint text, coordi taille);

/* Associe un texte au bouton, pour l'aide */
bouton *set_help(bouton *b, char *help);

/* Associe un texte au bouton */
bouton *set_label(bouton *b, char *label);

/* S'il te plait, dessine moi un bouton
pos indique ou ecrire l'aide associée au bouton */
void draw_bouton(bouton *b, coordi pos);

/* Modifie l'etat du bouton en fonction de la position de la souris */
void maj_bouton(bouton *b, keystate *k);

/* Simule un clic */
void click(bouton *b);

/* Active/desactive le bouton, pour en empecher le clic */
bouton *enable(bouton *b);
bouton *disable(bouton *b);


#endif 
