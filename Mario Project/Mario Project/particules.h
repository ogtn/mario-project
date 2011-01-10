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

typedef struct particule_generator 
{
	particule* particules;			/* Tableau dynamique de particules */
	int nb_particules;				/* Nombre actuel de particules */
	int max_particules;				/* Nombre maximum de particules */
	int indice_debut;				/* Indice de la premi�re particule en vie */
	int indice_fin;					/* Indice de la derni�re particule en vie */
	Uint32 debit;
	Uint32 temps_ecoule;
	Uint32 vie_particules;			/* Vie maximum des particules */
	coordf position;				/* Position du g�n�rateur */
	coordi taille;					/* Taille du g�n�rateur */
	coordi taille_particule;		/* Taille des particules du g�n�rateur */
	GLuint texture_id;				/* Identifiant de texture */
	unsigned int couleur_debut;		/* Couleur en d�but de vie de la particule */
	unsigned int couleur_fin;		/* Couleur en fin de vie de la particule */
	int gravity;
	char nom_text[TAILLE_NOM_TEXTURE];
} particule_generator;

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Cr�e un g�nrateur de particules */
particule_generator* new_particule_generator(coordf position, coordi taille, Uint32 vie_particules, Uint32 debit, char* nom_texture, int gravity, int couleur_debut, int couleur_fin);

/* Lib�re le g�n�rateur en m�moire */
void free_particule_generator(particule_generator **generator);

/* MAJ des particules pr�sentes dans le g�n�rateur */
void update_particule_genenerator(particule_generator *generator, Uint32 duration);

#endif