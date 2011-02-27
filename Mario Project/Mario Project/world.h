/*============================================================================*/
/*== [world.h] [Version: 0.028a]                                            ==*/
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

#ifndef WORLD_H
#define WORLD_H


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "perso.h"
#include "niveau.h"
#include "keystate.h"
#include "texture.h"
#include "sprite.h"
#include "bouton.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* La structure world est la structure maitresse du programme, elle contient
absolument tout ce qu'il faut pour lancer une partie
_nb_niveaux: nombre de niveaux du mode en cours
_liste_niveaux: liste des chemins relatifs de ces niveaux
_num_niveau: identificateur du niveau actuel
_niveau: niveau actuellement chargé
_nb_persos: nombre de joueurs
_persos: tableau des joueurs
_keystate: etat des touches
_temps_fps: utilisé pour calculer les FPS
_temps_actuel: tout est dans le nom...
_temps_prec: temps à l'image precedente
_temps_ecoule: difference entre les deux images
_temps_ecoule_prec: différence calculée entre les deux dernières images
_fps: fps...
_frames_calculees: nombre d'images calculees depuis le dernier calcul de fps
_ecran: structure ecran pour la gestion du scrolling notament
_taille_fenetre: la taille de la fenetre, en pixels*/
typedef struct world
{
    int nb_niveaux;
    char **liste_niveaux;
    int num_niveau;
    niveau *niveau;
    int nb_persos;
    perso **persos;
    keystate *keystate;
    Uint32 temps_fps;
    Uint32 temps_actuel;
    Uint32 temps_prec;
    Uint32 temps_ecoule;
	Uint32 temps_ecoule_prec;
    int fps;
    int frames_calculees;
    ecran ecran;
	coordi taille_fenetre;
} world;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* alloue de la memoire pour un world, et renvoie un pointeur valide ou NULL */
world* new_world(void);

/* initialise les champs de world avec les valeurs par defaut */
world* init_world(world *w);

/* desalloue proprement le world pointé par l'argument, elle retourne NULL.
Fonctionne également si le pointeur fourni est NULL */
world *free_world(world* w);

/* charge en memoire une partie, avec son niveau, ses monstres, le
personnage du joueur, les sprites, les blocs etc... */
void load_world(world *w);

/* fonction qui prépare le début d'un niveau */
void begin_level(world *w, int persos_tous_morts);

/* mise à jour des champs qui stockent notament le temps écoulé entre 
deux images */
void update_time(world *w);

/* mise à jour du champs ecran de la structure world, qui permet de gerer le
scrolling des differents plans du niveau */
void update_screen(world *w);

/* Mise a jour de la taille de la fenetre */
void update_taille_fenetre(world *w);

/* Vérification du nombre de vies */
void check_finish(world *w, int* gagne);

/* Teste si un des personnages se transforme ou est en train de mourir */
int perso_transforme_ou_meurt(world* w);

/* Teste si un des personnages meurt */
void persos_morts(world* w, int *persos_morts);

/* Fonctions de main.c, à placer ailleurs peut etre */
void init_SDL(int x, int y);
void init_OpenGL(int x, int y);


#endif
