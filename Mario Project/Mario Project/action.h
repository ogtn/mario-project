/*============================================================================*/
/*== [action.h] [Version: 0.028a]                                           ==*/
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

#ifndef ACTION_H
#define ACTION_H


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "keystate.h"
#include "bouton.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

struct action
{
    int enabled;					    /* L'action est-elle active? */
    void (*func)(struct action *);	    /* Fonction associ�e */
    void *data;						    /* Donn�es � transmettre � func */

    int nb_boutons;                     /* nombre de boutons associ�s */
    bouton **boutons;				    /* Boutons associ�s */
    int *btn_flag;					    /* Flags pour savoir lors de quelle action sur les boutons lancer l'action */
    int shortcut;					    /* Racourci clavier */
};


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Creer une nouvelle action en fournissant:
_l'adresse de la fonction correspondante
_un bouton associ� (ou NULL)
_un racourci associ� (ou AUCUN)
_un boul�en pour donner l'etat de l'action
_les donn�es � transmettre � la fonction
_le flag associ� au bouton */
action *new_action(void(*f)(action *), bouton *b, int shortcut, int enabled, void *data, int flag);

/* Memes parametres, mais pour une action existante */
action *init_action(action *a, void(*f)(action *), bouton *b, int shortcut, int enabled, void *data, int flag);

/* Ajouter un bouton � l'action, avec son flag associ� */
void link_action_btn(action *a, bouton *b, int flag);

/* Coupe les liens entre une action et un bouton */
void unlink_action_btn(action *a, bouton *b);

/* Liberer une action */
void free_action(action **a);

/* activer/desactiver */
void enable_action(action *a);
void disable_action(action *a);
void toggle_action_state(action *a);

/* Appeler si necessaire la fonction associ�e */
void execute_action(action *a, keystate *k);

/* Idem mais en for�ant l'execution */
void force_execute_action(action *a, keystate *k);

/* En attendant mieux, la version racourci? */
void execute_action_shortcut(action *a, keystate *k);


#endif 
