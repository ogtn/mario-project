/*============================================================================*/
/*== [sprite.h] [Version: 0.028a]                                           ==*/
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

#ifndef SPRITE_H
#define SPRITE_H

typedef enum layer
{
	LAYER_BACKGROUND,
	LAYER_BACKGROUND_PARTICLES,
	LAYER_OBJECTS,
	LAYER_BLOC_1,
	LAYER_ITEM,
	LAYER_BLOC_2,
	LAYER_FOREGROUND,
	LAYER_FOREGROUND_PARTICLES
} layer;


typedef enum angle
{
    ANGLE_0,
    ANGLE_90,
    ANGLE_180,
    ANGLE_270
} angle;

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "perso.h"
#include "monstre.h"
#include "projectile.h"
#include "tuyau.h"
#include "particules.h"
#include "checkpoint.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure sprite
_position: position du sprite
_taille: sa taille (en pixels)
_text_id: l'identificateur OpenGL de sa texture
_point_bg: coordonnées du point bas gauche du sprite
_point_hd: coordonnées du point haut droit du sprite */
typedef struct sprite
{
	coordi position;
	vecti taille;
	GLuint text_id;
	coordf point_bg;
	coordf point_hd;
} sprite;


/* pas d'autre idée de ou ranger ça, alors en attendant... */
/* Structure ecran
_origine: le point bas gauche de la zone de dessin
_scroll: le decalage de la camera
_taille: taille de la zone de dessin */
typedef struct ecran
{
	coordi origine;
	coordi scroll;
	coordi taille;
} ecran;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Dessin d'un sprite */
void draw_sprite_dr(sprite *s, layer depth, angle a);
void draw_sprite_d(sprite *s, layer depth);
void draw_sprite_r(sprite *s, angle a);
void draw_sprite(sprite *s);

/* ecrit à l'ecran les positions de la souris (debug) */
void draw_position_souris(void);

/* dessine les différents sprites du perso selon son etat actuel */
void draw_perso(perso *perso, Uint32 duree);

/* dessine le HUD du personnage */
void draw_HUD(perso* p);

/* dessine les items */
void draw_item(item* model, occ_item* item, Uint32 duree);

/* dessine les différents sprites du perso selon son etat actuel */
void draw_monstre(occ_monstre * monstre, Uint32 duree);

/* Dessine les différents sprites d'un projectile */
void draw_projectile(occ_projectile* proj, Uint32 duree);

/* Dessine les tuyaux présents dans le niveau */
void draw_pipe(tuyau* t);

/* Dessine les tuyaux présents dans le niveau */
void draw_particules(particule_generator* generator);

/* Dessine les checkpoints */
void draw_checkpoint(checkpoint *check, Uint32 duree);

/* dessine un cadre noir autour de l'ecran */
void draw_contours(ecran e);

/* dessine un cadre avec la texture fournie */
void draw_cadre(coordi pos, coordi taille, GLuint texture, int taille_text);

#endif
