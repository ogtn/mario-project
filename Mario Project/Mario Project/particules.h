/*============================================================================*/
/*== [particules.h] [Version: 0.028a]                                           ==*/
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

#ifndef PARTICULES_H
#define PARTICULES_H

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

typedef struct particule {
	Uint32 vie;						/* Vie restante de la particule */
	coordf position;				/* Position actuelle */
	coordf vitesse;					/* Vitesse actuelle */
} particule;

typedef struct particule_generator {
	particule* particules;			/* Tableau dynamique de particules */
	int nb_particules;				/* Nombre actuel de particules */
	int max_particules;				/* Nombre maximum de particules */
	int indice_debut;				/* Indice de la première particule en vie */
	int indice_fin;					/* Indice de la dernière particule en vie */
	Uint32 debit;
	Uint32 temps_ecoule;
	Uint32 vie_particules;			/* Vie maximum des particules */
	coordf position;				/* Position du générateur */
	coordi taille;					/* Taille du générateur */
	coordi taille_particule;		/* Taille des particules du générateur */
	GLuint texture_id;				/* Identifiant de texture */
	unsigned int couleur_debut;		/* Couleur en début de vie de la particule */
	unsigned int couleur_fin;		/* Couleur en fin de vie de la particule */
	int gravity;
} particule_generator;

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Crée un génrateur de particules fumée */
particule_generator* new_particule_generator_smoke(coordf position, Uint32 vie_particules, Uint32 debit);

/* Crée un génrateur de particules feu */
particule_generator* new_particule_generator_fire(coordf position, Uint32 vie_particules, Uint32 debit);

/* Crée un générateur de particules pluie */
particule_generator* new_particule_generator_rain(coordf position, Uint32 vie_particules, Uint32 debit);

/* Crée un générateur de particules neige */
particule_generator* new_particule_generator_snow(coordf position, Uint32 vie_particules, Uint32 debit);

/* Libère le générateur en mémoire */
void free_particule_generator(particule_generator **generator);

/* MAJ des particules présentes dans le générateur */
void update_particule_genenerator(particule_generator *generator, Uint32 duration);

#endif