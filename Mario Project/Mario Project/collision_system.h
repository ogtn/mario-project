/*============================================================================*/
/*== [collision_system.h] [Version: 0.028a]                                 ==*/
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

#ifndef COLLISION_SYSTEM_H_
#define COLLISION_SYSTEM_H_

/* Physique */
#define GRAVITY					    0.002F
#define MARGE_ECRAN_ACTIF			5 * LARGEUR_BLOC

/* Constantes determinant les frottements */
#define FROTT_CLASSIQUE			    0.007F
#define FROTT_GLACE				    0.004F
#define FROTT_AIR				    0.001F
#define COEFF_GLISSE			    0.3F

/* Constantes des vitesses et des accélèrations */
#define ACC_MARCHE				    0.05F
#define ACC_COURSE				    0.07F
#define V_MAX   					1.6F
#define V_MIN	    				0.008F
#define V_MARCHE	    			0.3F
#define V_COURSE		    		0.6F
#define VITESSE_SAUT		    	0.5F
#define V_REBOND			    	0.5F
#define V_REBOND_SAUT		    	0.8F
#define V_REBOND_PROJECTILE	    	0.3F
#define VITESSE_Y_EJECTION		    0.2F
#define VITESSE_X_EJECTION		    VITESSE_Y_EJECTION
#define V_CARAPACE				    0.5F
#define V_SORTIE_MONSTRE			0.05F

/* Flags concernant le type de soumission dont sont victimes les projectiles et les items */
#define SOUMIS_COLLISIONS_SOL		0x0000001
#define SOUMIS_GRAVITE				0x0000002
#define SOUMIS_RETOUR_ENVOYEUR		0x0000004

/* Temps correspondant aux états spéciaux de Mario */
#define TPS_POUSSE_CARAPACE			150			
#define TPS_MORT					1000
#define TPS_FINISH					7000

enum TYPES_COLLISIONS {
	PAR_LE_BAS,
	PAR_LA_GAUCHE,
	PAR_LA_DROITE,
	PAR_LE_HAUT,
	SUPERPOSITION,
	PENTE_PAR_LE_BAS,
	PENTE_PAR_LA_GAUCHE,
	PENTE_PAR_LA_DROITE,
	PAS_DE_COLLISION
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "world.h"

/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure carre (pour savoir s'il y a collision ou non) */
typedef struct carre {
	coordf position;						/* la position absolue du carré dans le niveau */
	coordf position_prec;					/* la position absolue du carré dans le niveau à l'image precedente */
	coordi taille;							/* la taille du carré (en pixels) */
	int est_bloc_pente;						/* indique si c'est un bloc pente */
	float angle_pente;						/* indique l'angle de la pente */ 
	int hauteur_a_retirer;					/* hauteur que l'on retire du haut du bloc (pour le calcul des pentes) */
} carre;

/* Structure collision (contient toutes les infos sur la collision en cours) */
typedef struct collision {
	int type_collision;						/* indique le type de collision */
	int carre1_est_touche;					/* indique si le premier carre est touché */
	int carre2_est_touche;					/* indique si le deuxième carré est touché */
} collision;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* fonction principale qui réalise les appels à toutes les sortes de collisions
et de d'interractions avec l'environement */
void main_collisions(world *w);

/* MAJ des éléments contenus dnas les tuyaux, s'il y en a */
void MAJ_tuyau(niveau* n, tuyau *t, Uint32 duree);

/* MAJ des positions et vitesses du perso */
void MAJ_collision_perso(perso *perso, niveau* lvl, keystate* keystate, Uint32 duree);

/* MAJ des positions et vitesses du monstre */
void MAJ_collision_monstre(occ_monstre* monstre, ecran e, Uint32 duree);

/* MAJ des positions et vitesses de l'item */
int MAJ_collision_item(niveau* n, occ_item* item, ecran e, Uint32 duree);

/* MAJ des positions et vitesses des projectiles */
void MAJ_collision_projectile(occ_projectile* projectile, ecran e, Uint32 duree);

/* MAJ du HUD */
void MAJ_HUD(perso *p, Uint32 duree, ecran e);

/* MAJ du perso en fonction de la destination du tuyau */
void MAJ_perso_tuyau(perso* p, niveau* lvl);

/* Gère le lancer de projectile de mario */
void throw_projectile_perso(perso* perso, niveau* lvl, occ_projectile *p);

/* Gère le lancer de projectiles spéciaux du perso */
void throw_special_projectile_perso(perso *perso, niveau* lvl, occ_projectile *p);

/* Detection/resolution de collisions sur le personnage */
void solve_collisions_perso(perso* p, niveau *n, keystate* keystate);

/* Detection/resolution de collisions sur le monstre */
void solve_collisions_monstre(occ_monstre* m, perso* p, niveau* n, Uint32 duree);

/* Detection/resolution de collisions sur l'item */
void solve_collisions_item(occ_item* it, perso** persos, niveau* n, Uint32 duree);

/* Detection/resolution de collisions sur le projectile */
void solve_collisions_projectile(occ_projectile* projectile, niveau *n);

/* Donne toutes les informations sur la collision entre c1 et c2 */
void determinate_collision(carre c1, carre c2, collision* collision);

/* applique la force de gravité sur le vecteur vitesse fournit */
vectf *gravity(vectf *v,  Uint32 duree);

/* modifie e vecteur vitesse fournit pour que le personnage realise un saut */
vectf *jump(vectf *v);

/* applique des forces de frottement pour un perso qui glisse ou derape */
void frottements(perso *p, Uint32 duree);

/* se charge de limiter la vitesse de deplacement d'un personnage à partir de
son vecteur vitesse */
vectf *lim_vitesse(vectf *v);

/* Fais sauter le personnage */
void jump_perso(perso* p);

/* Gére les transformations de Mario quand il est touché par un ennemi */
void touche_perso(perso* p, niveau* n);

/* Gére les transformations de Mario quand il prend un item */
void prend_item(perso* p, int item, niveau* n);

/* Determine l'acceleration du perso en fonction de l'etat des touches et de l'environement */
void solve_acc(perso *p, keystate *k);

/* limite la vitesse de deplacement du personnage */
void limit_speed(perso *p, keystate *k);

/* effectue les deplacements lateraux */
void lateral_move(perso *p, keystate *k, Uint32 t);

/* Calcul l'angle et la hauteur en fonction de la physique du bloc */
void find_angle_height_with_phys(niveau* n, int phys_bloc, carre* bloc);

/* MAJ de l'activité d'un monstre */
void pause_monstre(occ_monstre* monstre, ecran e);

/* MAJ de l'activité d'un projectile */
void pause_projectile(occ_projectile* projectile, ecran e);

/* MAJ de l'activité d'un item */
void pause_item(niveau* n, occ_item* item, ecran e);

/* Compte les points */
void compte_points(perso* p, occ_monstre* montre, int is_finish);

/* S'occupe de tuer les monstres à la fin d'un niveau */
void kill_monster_finish(niveau *n, perso* p);

#endif