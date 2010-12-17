/*============================================================================*/
/*== [menus.h] [Version: 0.028a]                                            ==*/
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
/*==============================[DEFINES/ENUMS]==============================*/
/******************************************************************************/

#ifndef MENUS_H_
#define MENUS_H_

/* Menus à afficher */
enum{
	MENU_AUCUN,
	MENU_PRINCIPAL,
	MENU_PAUSE
};

/* Valeurs de retour de main_menu */
enum{
	RETOUR,
	AVENTURE,
	RETRO,
	COOPERATION,
	DUEL,
	CREDITS,
	QUITTER
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "sprite.h"
#include "keystate.h"
#include "sound.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/*
_titre: le titre du menu, à afficher
_nb_choix: le nombre de sous menus (sous arbres)
_choix: un tableau des sous menus (sous arbres)
_valeur: la valeur de retour du menu (cas des feuilles uniquement) */
typedef struct menu
{
  char titre[TAILLE_TITRE_MENUS];
  int nb_choix;
  struct menu **choix;
  int valeur;
}menu;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* places dans leschamps les valeurs par defaut */
void initialiser_menu(menu *m);

/* genere une arborescence de menus à partir d'un fichier */
menu *charger_menu(FILE *flux);

/* desallocation */
void free_menu(menu *m);

/* affichage d'un menu sur l'ecran (à coder plus propre) */
void afficher_menu(menu *m, int surligne);

/* rend le sous menu choix du menu sournit en argument */
menu *sous_menu(menu *m, int choix);

/* rend le menu parent du menu passé en argument */
menu *menu_parent(menu *m);

/* rend la valeur de la feuille numero choix du menu passé en argument */
int menu_feuille(menu *m, int choix);

/* fonction contenantla boucle principale du menu de l'ecran titre */
int main_menu(keystate *k);


#endif
