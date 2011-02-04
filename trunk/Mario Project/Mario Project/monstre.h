/*============================================================================*/
/*== [monstre.h] [Version: 0.028a]                                          ==*/
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

#ifndef MONSTRE_H
#define MONSTRE_H

#define M_V_MARCHE				(float)0.05
#define M_V_MIN					(float)0.008
#define M_TPS_DISPARITION		500
#define M_TPS_RETRACTE			5000


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "texture.h"
#include "sound.h"

enum ETATS_MONSTRES {
	M_MARCHE,
	M_RETRACTE,
	M_SORT_CARAPACE,
	M_NB_ETATS,
	M_MORT_PAR_PROJ,
	M_MORT,
	M_MORT_PAR_SAUT,
	M_RETRACTE_RETOURNE,
	M_RETRACTE_PORTED,
	M_SORT_DU_TUYAU
};


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/

/* Structure  Occ_monstre (Occurence du monstre) */
typedef struct occ_monstre {
	coordf position;						/* la position absolue du monstre dans le niveau */
	coordf position_ini;					/* la position absolue du monstre dans le niveau */
	coordf position_prec;					/* la position absolue du monstre dans le niveau à l'image precedente */
	coordf vitesse;							/* la vitesse du monstre */
	int etat;								/* l'etat actuel du monstre */
	int cote;								/* indique le cote vers lequel est tourné le monstre */
	struct monstre* type_monstre;			/* pointeur sur le monstre général */
	int tps_disparition;					/* Temps restant au monstre avant de disparaitre */
	int tps_sortie_tuyau;					/* Temps restant au monstre avant de sortir du tuyau */
	int tps_retracte;						/* Indique le temps avant que l'ennemi retracté ne sorte de la carapace */
	int actif;								/* indique si le monstre actif (dans l'écran ou non) */
}occ_monstre;

/* Structure elem_monstre */
typedef struct elem_monstre {
	occ_monstre *occ_monstre;				/* l'occurence du monstre */
	struct elem_monstre *suivant;			/* pointeur sur le suivant dans la liste */
}elem_monstre;

/* Structure liste_monstre (liste chainée d'occurrences de monstre) */
typedef struct liste_monstre {
	elem_monstre* monstre;					/* représente la liste de monstres */
	nb nb_elements;							/* indique le nombre de monstres dans la liste */
}liste_monstre;


/* Structure Monstre */
typedef struct monstre {
	coordi taille;							/* la taille du monstre (en pixels) */
	int abscisse_bas;						/* contient l'abscisse du point d'où l'on calculera les collisions  */
	int nb_sprites[M_NB_ETATS];				/* Nombre de sprites du monstre selon ses etats */
	int v_anim[M_NB_ETATS];					/* vitesse d'animation du monstre selon ses etats */
	int nb_sprites_max;						/* donne le nombre de sprites max en longueur dans la texture */
	int est_tuable_par_boule_feu;			/* indique si le monstre peut etre tué par une boule de feu */
	int est_tuable_par_saut;				/* indique si le monstre peut etre tué par un saut */
	int peut_sauter_dessus;					/* indique si le personnage peut sauter sur le monstre, sinon il est touché */
	int reste_sur_plateforme;				/* indique si le personnage doit rester sur la plateforme sur laquelle il est */
	GLuint texture;							/* l'identificateur OpenGL de la texture du monstre */
	FSOUND_SAMPLE* sons[NB_SONS];			/* tableau des sons correspondant au monstre */
	int points;								/* points remportés par la mort de ce monstre par un projectile */
	int tps_disparition;					/* Temps qu' un monstre met à disparaitre après avoir été tué par un saut du personnage */
	int tps_sortie_tuyau;					/* Intervalle de temps de sortie depuis un tuyau */
    char nom[255];                          /* Bah, le nom de la bestiole quoi */
	liste_monstre* occ_monstres;			/* Liste de ses occurences dans le niveau */
}monstre;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Allocation d'un monstre */
monstre* new_monstre(void);

/* Initialisation */
monstre* init_monstre(monstre *m);

/* Libération d'un monstre */
monstre* free_monstre(monstre* m);

/* Allocation d'une occurence de monstre */
occ_monstre* new_occ_monstre(float position_x, float position_y, monstre* type_monstre);

/* Initialisation d'une occurrence de monstre */
occ_monstre*  init_occ_monstre(occ_monstre *m, float position_x, float position_y, monstre* type_monstre);

/* Allocation d'une nouvelle liste de monstres */
liste_monstre* new_liste_monstre();

/* Copie l'occurence d'un monstre */
occ_monstre* copy_monstre(occ_monstre *m);

/* Libère la copie d'une coccurence d'un monstre */
occ_monstre* free_copy_monstre(occ_monstre *m);

/* Créer un élément de la liste */
elem_monstre* creer_element_occ_monstre(occ_monstre* m);

/* Ajoute une occurence d'un monstre dans la liste */
liste_monstre* ajout_monstre(liste_monstre* liste, occ_monstre *m);

/* Supprime une occurence d'un monstre de la liste */
liste_monstre *supprime_monstre(liste_monstre* liste, occ_monstre *m);

/* Chargement d'un monstre à partir d'un nom */
monstre * charger_monstre(char* nom);

/* Compte le nombre d'états absents dans la texture du monstre */
int m_nb_etats_absents(monstre* m);


#endif
