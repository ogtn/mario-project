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

/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "perso.h"
#include "monstre.h"
#include "projectile.h"
#include "tuyau.h"
#include "particules.h"

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

/* Dessine le sprite ayant pout coordonées du coin bas-gauche (x;y), avec la
taille donnée par largeur et hauteur avec la texture indiquée. Les coordXX
fournissent les coordonées de texture à utiliser */
void draw_sprite(int x, int y, int largeur, int hauteur, GLuint texture,
				 float coordx1,	 float coordx2, float coordy1, float coordy2);

void draw_sprite_90(int x, int y, int largeur, int hauteur, GLuint texture,
				 float coordx1,	 float coordx2, float coordy1, float coordy2);

/* Version optimisée de draw_sprite(), il faut lui fournir le dernier
identificateur de texture OpenGL et un poiteur sur une structure sprite */
void draw_sprite_(sprite *s, GLuint last);

/* Version avec choix de la profonceur */
void draw_sprite_layer(sprite *s, GLuint last, int depth);

void draw_sprite_90_(sprite *s, GLuint last);


/* Idem avec un triangle et on spécifie la couleur en plus */
void draw_triangle_illum(int x1, int y1, int x2, int y2, int x3, int y3, GLuint texture,
						 float coordx1,	 float coordy1,
						 float coordx2, float coordy2,
						 float coordx3,	 float coordy3);

/* dessine la chaine de caractere fournie en argument, à la position voulue et
avec la couleur choisie */
void draw_text(char *msg, coordi pos, float r, float g, float b);

/* ecrit à l'ecran les positions de la souris (debug) */
void draw_position_souris(void);

/* dessine les différents sprites du perso selon son etat actuel */
void draw_perso(perso *perso, Uint32 duree);

/* dessine le HUD du personnage */
void draw_HUD(perso* p);

/* dessine les items */
void draw_item(occ_item* item, Uint32 duree);

/* dessine les différents sprites du perso selon son etat actuel */
void draw_monstre(occ_monstre * monstre, Uint32 duree);

/* Dessine les différents sprites d'un projectile */
void draw_projectile(occ_projectile* proj, Uint32 duree);

/* Dessine les tuyaux présents dans le niveau */
void draw_pipe(tuyau* t);

/* Dessine les tuyaux présents dans le niveau */
void draw_particules(particule_generator* generator);

/* dessine un cadre noir autour de l'ecran */
void draw_contours(ecran e);

/* dessine un cadre avec la texture fournie */
void draw_cadre(coordi pos, coordi taille, GLuint texture, int taille_text);

#endif
