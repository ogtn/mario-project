/*============================================================================*/
/*== [niveau.h] [Version: 0.028a]                                           ==*/
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

#ifndef NIVEAU_H
#define NIVEAU_H

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "sprite.h"
#include "monstre.h"
#include "items.h"
#include "projectile.h"
#include "tuyau.h"
#include "particules.h"
#include "checkpoint.h"
#include "bloc.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure objet */
typedef struct objet
{
	char nom_text[TAILLE_NOM_TEXTURE];		/* nom de la texture */
	GLuint* id_text;						/* l'id de la texture */
	coordi taille;							/* taille de l'objet (en pixel) */
	int nb_sprites;							/* nombre de sprites */
	int v_anim;								/* vitesse d'animation */
} objet, background, foreground;

/* Structure finish */
typedef struct finish
{
	char nom_text[TAILLE_NOM_TEXTURE];
	GLuint id_text;
	coordi taille;
	coordf position;
}finish;

/* Structure niveau */
typedef struct niveau
{
	char nom[TAILLE_NOM_NIVEAU];					/* le nom du niveau, il est affich� au chargement de celui-ci */
	char titre_zik[TAILLE_TITRE_MUSIQUE];			/* nom du fichier audio � lire pendant le niveau */
	coordi taille;									/* la taille du niveau (en blocs) */
	coordi spawn;									/* les coordonn�es du spawn point (en bloc) */
	GLuint last_texture;							/* dernier identificateur OpenGL ayant �t� utilis� */

	/* Layer checkpoint */
	int nb_checkpoints;
	checkpoint** checkpoints;

	/* Layer finish */
	nb nb_finish;									/* Nombre d'arriv�es dans le niveau */
	finish* finishes;								/* Tableau des arriv�es */
	
	/* Layer background */
	nb nb_backgrounds;								/* nombre d'arriere-plans */
	background *backgrounds;						/* tableau � une dimension d'arriere-plans */
	nb nb_background_generators;					/* nombre de g�n�rateurs de particules */
	particule_generator** background_generators;	/* tableau de g�n�rateurs pr�sents en arri�re plan */

	/* Layer foreground */
	nb nb_foregrounds;								/* nombre d'arriere-plans */
	background *foregrounds;						/* tableau � une dimension d'avant-plans */
	nb nb_foreground_generators;					/* nombre de g�n�rateurs de particules */
	particule_generator** foreground_generators;	/* tableau de g�n�rateurs pr�sents en avant plan */

	/* Layer projectiles */
	nb nb_projectiles;								/* indique le nombre total de projectiles dans le niveau */
	projectile** projectiles;						/* tableau de pointeurs pointant sur les projectiles g�n�raux qui font partie du niveau */			

	/* Layer monstres */
	nb nb_monstres;									/* indique le nombre total de monstres dans le niveau */
	monstre** monstres;								/* tableau de pointeurs pointant sur les monstres g�n�raux qui font partie du niveau */

	/* Layer tuyaux */
	nb nb_tuyaux;									/* indique le nombre total de tuyaux dans le niveau */
	tuyau** tuyaux;									/* tableau de pointeurs pointant sur les tuyaux g�n�raux qui font partie du niveau */

	/* Layer items */
	nb nb_items;									/* indique le nombre total d'items dans le niveau */
	item** items;									/* tableau de pointeurs pointant sur les items g�n�raux qui font partie du niveau */
	
	/* Layer objets */
	nb nb_objets;									/* le nombre d'objets diff�rents pr�sents dans le niveau */
	objet *objets;									/* un tableau � une dimension qui contient tous les objets */
	id **id_objets;									/* tableau � deux dimensions dont chaque case contient l'identificateur d'un objet */

	/* Layer blocs */
	nb nb_blocs;									/* le nombre de blocs diff�rents qui constituent le niveau */
	bloc *blocs;									/* un tableau � une dimension qui contient tous les blocs */
	occ_bloc ***occ_blocs;							/* tableau � deux dimensions dont chaque case contient l'occurence d'un bloc */
	coordi taille_blocs;							/* vecteur 2D qui contient la taille des blocs (en pixels) */

	/* Layer textures */
	nb nb_textures;									/* nombre de textures utilis�es pour les blocs */
	texture *textures;								/* tableau � 1 dimension contenant les textures des blocs */
} niveau;

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* alloue de la memoire pour un niveau, et retourne un pointeur sur le niveau,
ou NULL si l'allocation echoue */
niveau* new_niveau(void);

/* initialise les champs du niveau avec les valeurs par defaut */
niveau* init_niveau(niveau *n);

/* libere proprement la m�moire allou�e pour le niveau, et retourne NULL,
fonctionne meme si le pointeur donn� en argument est NULL */
niveau *free_niveau(niveau *n);

/* a partir d'un flux sur un fichier .lvl, la fonction lis le debut du fichier
(header et ID des blocs) et remplis les champs du niveau fournit */
void charger_niveau(char *nom, niveau *n);

/* enregistre le header et les ID des blocs dans le fichier associ� au pointeur
sur FILE donn� */
void sauver_niveau(char *nom, niveau *n);

/* charge dans la memoire video toutes les textures necessaire � l'affichage 
d'un niveau */
void charger_textures_niveau(niveau *n);

/* Charge un arri�re-plan */
void charger_objet_background(background* b, int is_object);

/* Charge une arriv�e */
void charger_finish(finish* f);

/* Charge "� la main" un niveau de texte, c'est crade mais c'est comme �a o) */
void charger_niveau_test(niveau *n);

/* Charge "� la main" un niveau de test pour le chargement du XML */
void charger_niveau_test_xml(niveau *n);

/* Charge un niveau vide pour l'�diteur */
void charger_niveau_test_vide(niveau *n);

/* Libere les textures du niveau de la memoire video */
void liberer_textures_niveau(niveau *n);

/* fonction principale d'affichage, elle dessine TOUTE la scene */
void draw_main(niveau *lvl, perso **persos, ecran e, Uint32 duree);

/* tr�s proche, mais permet dessiner ce que l'on souhaite seulement */
void draw_main_options(niveau *lvl, ecran e, Uint32 duree, int bck, int blocs, int objets);

/* dessine les arriere plans du niveau (en tenant compte du scrolling 
vectical et horizontal) */
void draw_background(niveau *n, ecran e, Uint32 duree);

/* dessine les avants plans du niveau (en tenant compte du scrolling 
vectical et horizontal) */
void draw_foreground(niveau *n, ecran e, Uint32 duree);

/* dessine les blocs qui contituent la partie du niveau situ�e dans le meme
plan que les personnages */
void draw_blocs(niveau *n, ecran e, Uint32 duree);

/* dessine les objets du niveau, situ�s entre les arrieres plans et le plan des
personnages. But uniquement decoratif */
void draw_objects(niveau *n, Uint32 duree);

/* Affiche les id des blocs du niveau dans un fichier */
void affiche_occ_blocs(niveau* n);

/* MAJ des particules pr�sentes dans le niveau */
void MAJ_particules(niveau* n, Uint32 duree);

typedef void (*balise_func)(niveau *, const char **);

/*** UTILITAIRES ***/
float deg_to_rad(float degres);


#endif
