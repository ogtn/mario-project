/*============================================================================*/
/*== [tuyau.h] [Version: 0.028a]                                           ==*/
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

#ifndef TUYAU_H_
#define TUYAU_H_

#define TPS_SORTIE_MONSTRE			4000

typedef enum {
	OUVERT,
	FERME
}ETAT_TUYAU;

typedef enum {
	VERS_LE_HAUT,
	VERS_LA_GAUCHE,
	VERS_LA_DROITE,
	VERS_LE_BAS
}SENS_SORTIE;

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "monstre.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure Tuyau */
typedef struct tuyau
{
    GLuint texture;							/* Identifiant de la texture */
	coordi position;
    int longueur;							/* Longueur du tuyau, en pixels ou en blocs comme pour au dessus */
    SENS_SORTIE sens_sortie;				/* D�finit le sens de la sortie du tuyau: si j'ai bien compris �a serait TUYAU_GAUCHE, TUYAU_DROITE, TUYAU_HAUT ou TUYAU_BAS? */
    ETAT_TUYAU etat;                        /* Ouvert ou ferm� ? */
    monstre* monstre;						/* Des monstres sortiront-ils de l� ? ok, avec NULL si pas de monstres qui sortent, on peut compl�ter avec un champs qui donne le temps qui s�pare chaque sortie :p */
	int tps_sortie_monstre;
    int pipe_dest;							/* L'index du tuyau dans le tableau des tuyaux */
    char *level_dest;						/* Le nom du niveau de sortie ou NULL */
} tuyau;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

tuyau* new_tuyau(SENS_SORTIE sens);

tuyau* init_tuyau(tuyau* t, SENS_SORTIE sens);

tuyau* charger_tuyau(char* nom, SENS_SORTIE sens);

tuyau* free_tuyau(tuyau* t);

#endif