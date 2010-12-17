/*============================================================================*/
/*== [group_box.h] [Version: 0.028a]                                        ==*/
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

#ifndef GROUP_BOX_H
#define GROUP_BOX_H


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "bouton.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

typedef struct group_box
{
	coordi taille;
	coordi position;
	int visible;

	bouton **boutons;
	int nb_boutons;

	struct group_box **group_boxes;
	int nb_group_boxes;

	GLuint texture;
	int epaisseur;
} group_box;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

group_box *new_group_box(int pos_x, int pos_y, int taille_x, int taille_y);
void free_group_box(group_box **g);
void clean_group_box(group_box *g);
group_box *copy_rec_group_box(group_box *src);
void add_btn(group_box *g, bouton *b);
void free_btn(group_box *g);
void add_group_box(group_box *g, group_box *fils);
void add_group_box_to_pos(group_box *g, group_box *fils, int pos);
void draw_group_box(group_box *g);
void maj_group_box(group_box *g, keystate *k);
void reset_group_box(group_box *g);
void show(group_box *g);
void hide(group_box *g);
void set_group_box_pos(group_box *g, int x, int y);
void move_group_box(group_box *g, int x, int y);

#endif 
