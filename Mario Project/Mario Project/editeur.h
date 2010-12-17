/*============================================================================*/
/*== [editeur.h] [Version: 0.028a]                                          ==*/
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

#ifndef EDITEUR_H
#define EDITEUR_H


/* Regions de l'editeur */
enum regions_editeur
{
    ONGLET_BLOCS,
    ONGLET_ENNEMIS,
    ONGLET_BACKGROUND,
    REGION_STATUS_BAR,
    NB_REGIONS_EDITEUR
};


enum region_onglet_bloc
{
    REGION_TOOLBOX_BLOC,
    REGION_FAVORIS_BLOC,
    REGION_PREVIEW_BLOC,
    REGION_OUTIL_MAGIQUE_BLOC,
    REGION_TEXTURE_BROWSER_BLOC,
    NB_REGIONS_BLOC
};


enum region_onglet_ennemi
{
    REGION_TOOLBOX_ENNEMI,
    REGION_FAVORIS_ENNEMI,
    REGION_PREVIEW_ENNEMI,
    REGION_OUTIL_SPECIAL_ENNEMI,
    REGION_ENNEMI_BROWSER_ENNEMI,
    NB_REGIONS_ENNEMI
};


/* Liste de toutes les actions de l'éditeur, les noms sont les mêmes
que les fonctions associées, mais en majuscules */
enum actions_editeur
{
    /* Actions globales */
    AC_MODE_SWITCH,
    AC_MASQUE_FOND,
    AC_MASQUE_ENNEMIS,
    AC_MASQUE_SPECIALS,
    AC_MASQUE_OBJETS,
    AC_ANNULER,
    AC_RETAURER,
    AC_TEST_LEVEL,
    AC_ENREGISTRER,
    AC_ENREGISTRER_SOUS,
    AC_OUVRIR,

    /* Actions propres au mode bloc */
    AC_SWITCH_EDITION_BROWSER,
    AC_SWITCH_SELECT_REMPLI,
    AC_COUPER,
    AC_COPIER,
    AC_COLLER,
    AC_PRECEDENT,
    AC_SUIVANT,
    AC_DOSSIER_PARENT,
    AC_RACINE,
    AC_SUPPRIMER,
    AC_REMPLISSAGE,
    AC_ADD_TO_FAVORITES,
    AC_SELECTIONNE_FAVORI,

    /* Actions propres au mode ennemi */
    AC_ADD_ENNEMI_TO_FAVORITES,

    NB_ACTIONS
};


/* Liste des actions accessibles en mode bloc */
enum actions_bloc
{
    AC_BLOC_SWITCH_EDITION_BROWSER,
    AC_BLOC_SELECTIONNE_FAVORI,
    AC_BLOC_MASQUE_FOND,
    AC_BLOC_MASQUE_ENNEMIS,
    AC_BLOC_MASQUE_SPECIALS,
    AC_BLOC_MASQUE_OBJETS,
    AC_BLOC_ANNULER,
    AC_BLOC_RETAURER,
    AC_BLOC_SWITCH_SELECT_REMPLI,
    AC_BLOC_COUPER,
    AC_BLOC_COPIER,
    AC_BLOC_COLLER,
    AC_BLOC_PRECEDENT,
    AC_BLOC_SUIVANT,
    AC_BLOC_SUPPRIMER,
    AC_BLOC_REMPLISSAGE,
    AC_BLOC_DOSSIER_PARENT,
    AC_BLOC_RACINE,
    AC_BLOC_ADD_TO_FAVORITES,
    AC_BLOC_ENREGISTRER,
    AC_BLOC_ENREGISTRER_SOUS,
    AC_BLOC_OUVRIR,
    AC_BLOC_TEST_LEVEL,
    NB_ACTIONS_BLOC
};


/* Liste des actions accessibles en mode enemis */
enum actions_ennemi
{
    AC_ENEMIS_SWITCH_EDITION_BROWSER,
    AC_ENNEMI_SELECTIONNE_FAVORI,
    AC_ENNEMI_MASQUE_FOND,
    AC_ENNEMI_MASQUE_ENNEMI,
    AC_ENNEMI_MASQUE_SPECIALS,
    AC_ENNEMI_MASQUE_OBJETS,
    AC_ENNEMI_ANNULER,
    AC_ENNEMI_RETAURER,
    AC_ENNEMI_COUPER,
    AC_ENNEMI_COPIER,
    AC_ENNEMI_COLLER,
    AC_ENNEMI_PRECEDENT,
    AC_ENNEMI_SUIVANT,
    AC_ENNEMI_SUPPRIMER,
    AC_ENNEMI_DOSSIER_PARENT,
    AC_ENNEMI_RACINE,
    AC_ENNEMI_ADD_TO_FAVORITES,
    AC_ENNEMI_ENREGISTRER,
    AC_ENNEMI_ENREGISTRER_SOUS,
    AC_ENNEMI_OUVRIR,
    AC_ENNEMI_TEST_LEVEL,
    NB_ACTIONS_ENNEMI
};


enum regions_favoris
{
    REGION_FAV_0,						    /* premier favoris, et ainsi de suite */
    REGION_FAV_1,
    REGION_FAV_2,
    REGION_FAV_3,
    REGION_FAV_4,
    REGION_FAV_5,						    /* dernier favoris */
    REGION_FAV_MAX
};


enum regions_texture_browser
{
    REGION_TEXTURE_BROWSER_BLOC_DOSSIERS,	/* boutons des sous dossier du dossier courant */
    REGION_TEXTURE_BROWSER_BLOC_IMG,		/* images du dossier courant */
    NB_REGION_TEXTURE_BROWSER_BLOC
};


enum regions_ennemis_browser
{
    REGION_ENNEMI_BROWSER_DOSSIERS,         /* boutons des sous dossier du dossier courant */
    REGION_ENNEMI_BROWSER_IMG,              /* images du dossier courant */
    NB_REGION_ENNEMI_BROWSER
};


/* Sous-boutons de l'outil 3x3 */
enum boutons_outil_3x3
{
    SELECT_LINE_3,
    SELECT_BG,
    SELECT_BM,
    SELECT_BD,

    SELECT_LINE_2,
    SELECT_MG,
    SELECT_MM,
    SELECT_MD,

    SELECT_LINE_1,
    SELECT_HG,
    SELECT_HM,
    SELECT_HD,

    SELECT_ALL,
    SELECT_COL_1,
    SELECT_COL_2,
    SELECT_COL_3,

    OUTIL_3x3_TAILLE
};


/* Modes de l'editeur */
enum modes_editeur
{
    MODE_BLOC,
    MODE_ENNEMIS,
    MODE_BACKGROUND,
    NB_MODES
};


/* Modes de l'éditeur de blocs */
enum modes_editeur_bloc
{
    MODE_BLOC_BROWSER,
    MODE_BLOC_EDITOR
};


/* Modes de l'éditeur d'ennemis */
enum modes_editeur_ennemi
{
    MODE_ENNEMI_BROWSER,
    MODE_ENEMI_EDITOR
};


/******************************************************************************/
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "world.h"
#include "bouton.h"
#include "browser.h"
#include "texte_interface.h"
#include "group_box.h"


/******************************************************************************/
/*================================[STRUCTURES]================================*/
/******************************************************************************/


/* Types de selection */
enum select_types
{
    TYPE_FILL =		COLOR_NAVY,
    TYPE_DELETE =	COLOR_RED,
    TYPE_SELECT =	COLOR_YELLOW
};


/* Structure destinee a contenir les tableaux de la structure niveau sous forme
de listes chainees, pour rendre l'editeur plus souple */
typedef struct niveau_editeur
{
    liste_texture textures;		    /* Liste des textures */
    liste_bloc blocs;				/* liste des blocs */
} niveau_editeur;


/* L'outil magique, destiné au remplissage d'ensembles de blocs */
typedef struct outil_3x3
{
    bouton *bouton;
    sprite sprite;
} outil_3x3;


/* Stockage des favoris */
typedef struct favori
{
    char texture_name[MAX_PATH];
    GLuint texture_id;
    int blocs_offset;
} favori;


/* Presse papier des blocs */
typedef struct blocs_clipboard
{
    coordi taille;
    int **blocs;
} blocs_clipboard;


/* Un tableau permet de générer les actions et les boutons via des boucles,
plutot que de tout faire manuelement, cf debut de editeur.c */
typedef struct table_actions_editeur
{
    int type;
    char *texture;
    char* aide;
    void (*func)(action*);
    int shortcut;
    int btn_flag;
    int enable_flag;
} table_actions_editeur;


/* Contient l'ensemble des données necessaires dans le mode bloc */
typedef struct editeur_bloc
{
    action *actions[NB_ACTIONS_BLOC];           /* L'ensemble des actions disponibles depuis le mode bloc */
    int mode;                                   /* Le mode de l'éditeur de blocs */
    int selected_favorite;                      /* Le favoris selectionné */
    favori favoris[REGION_FAV_MAX];             /* Les favoris disponibles */
    dossier *dossier_texture;                   /* Le repertoire du browser */

    /* Tout ce qui concerne la selection des blocs */
    int selection_terminee;		                /* Une selection valide est-elle en cours */
    blocs_clipboard blocs_cb;                   /* Le presse papier */
    int selection;                              /* Une selection est-elle en cours? */
    int select_type;                            /* Type de la selection courante */
    int prev_select_type;                       /* Type de la selection précédente */
    coordi debut_selection;		                /* Le bloc par lequel la selection à commencée */
    coordi fin_selection;		                /* Le bloc qui a mis fin à la selecton */
} editeur_bloc;


/* Contient l'ensemble des données necessaires dans le mode enemis */
typedef struct editeur_enemis
{
    action *actions[NB_ACTIONS_BLOC];           /* L'ensemble des actions disponibles depuis le mode enemis */
    int mode;                                   /* Le mode de l'éditeur d'enemis */
    int selected_favorite;                      /* Le favoris selectionné */
    favori favoris[REGION_FAV_MAX];             /* Les favoris disponibles */
    dossier *dossier_texture;                   /* Le repertoire du browser */
} editeur_enemis;


/* Contient l'ensemble des données necessaires dans l'éditeur de niveaux */
typedef struct editeur
{
    world *world;                               /* La structure world indispensable */
    int mode;                                   /* Le mode dans lequel se trouve l'éditeur */
    GLuint texture_cadre;                       /* Texture utilisée pour les cadres de l'interface */
    int survole_apercu;			                /* Booléen qui permet de savoir si on survole l'aperçu */
    coordi bloc_survole;		                /* Le bloc actuellement survolé par la souris */
    group_box *onglets[NB_REGIONS_EDITEUR];     /* L'ensemble des group_boxes qui composent l'interface */
    bouton *bouton_onglet;                      /* Le bouton radio utilisé pour représenter les onglets */
    action *actions[NB_ACTIONS];                /* L'ensemble des actions disponibles depuis l'éditeur */
    editeur_bloc mode_bloc;                     /* Tout ce qui concerne le mode bloc */
    editeur_enemis mode_enemis;                 /* Tout ce qui concerne le mode enemis */

    /* Options de dessin */
    int dessiner_blocs;
    int dessiner_fonds;
    int dessiner_objets;
    int dessiner_enemis;
    int dessiner_specials;

    /* Positions et tailles des group_boxes de l'interface */
    coordi pos_barre_outils;
    coordi pos_browser;
    coordi pos_aide;
    coordi taille_barre_outils;
    coordi taille_browser;
} editeur;


/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Fonction qui contient la boucle principale de l'editeur de niveau */
void main_editeur(void);


void draw_main_editeur(editeur *e);
void draw_favoris(editeur *e);
void draw_outil(editeur *e);
void draw_onglets(editeur *e);

/* initialisent les champs de l'editeur */
void init_editeur(editeur *e);
void init_onglets(editeur *e);
void init_onglet_bloc(editeur *e);
void init_onglet_ennemi(editeur *e);
void init_toolbox_bloc(editeur *e);
void init_toolbox_ennemi(editeur *e);
void init_region_edition(editeur *e);
void init_region_aide(editeur *e);


/* Fonctions qui allouent de la mémoire pour les boutons des browser */
void charge_boutons_browser(editeur *e);
void charge_boutons_browser_ennemis(editeur *e);

/* Desallocation des boutons du browser */
void supprime_boutons_browser(editeur *e);

/* Gestion des fleches directionnelles pour le deplacement de la "camera" en
fonction du temps */
void deplace_ecran(editeur *e);

/* mise a jour de l'editeur (etats des boutons, modes etc.) */
void maj_editeur(editeur *e);
void maj_editeur_bloc(editeur *e);
void maj_options_dessin(editeur *e);
void maj_selection(editeur *e);

/* fonction merdique provisoire */
void debug_mode(editeur *e);

/* Inscrit le bloc survolé par la souris dans e et renvoie vrai si la souris survole l'écran */
int find_bloc_survole(editeur *e);

/* Dessine le viseur du mode bloc */
void dessine_viseur(editeur *e);

/* Dessine le retangle englobant les blocs selectionnés */
void dessine_selection(editeur *e);

/* Bouton radio prévu pour la manipulation de textures 3*3 blocs */
bouton *new_outil_3x3(void);

/* Mise en place des positions des sous-boutons */
bouton *set_pos_outil_3x3(bouton *b, int x, int y);

/* Chargement d'une texture dans l'outil */
bouton *set_text_outil_3x3(bouton *b, char *nom);

/* Dessin de l'outil */
void draw_outil_3x3(bouton *b);

/* Dessine un rectangle de la couleur voulue, avec un contour opaque et
un remplissage translucide */
void draw_rectangle(coordi pos, coordi taille, GLfloat r, GLfloat g, GLfloat b);

/* Actions globales */
void ac_mode_switch(action *a);
void ac_masque_fond(action *a);
void ac_masque_enemis(action *a);
void ac_masque_specials(action *a);
void ac_masque_objets(action *a);
void ac_annuler(action *a);
void ac_retaurer(action *a);
void ac_test_level(action *a);
void ac_enregistrer(action *a);
void ac_enregistrer_sous(action *a);
void ac_ouvrir(action *a);

/* Actions propres au mode bloc */
void ac_switch_edition_browser(action *a);
void ac_switch_select_rempli(action *a);
void ac_couper(action *a);
void ac_copier(action *a);
void ac_coller(action *a);
void ac_precedent(action *a);
void ac_suivant(action *a);
void ac_dossier_parent(action *a);
void ac_racine(action *a);
void ac_supprimer(action *a);
void ac_remplissage(action *a);
void ac_add_to_favorites(action *a);
void ac_selectionne_favori(action *a);

/* Declaration ici en attendant mieux... La fonction devrait être dans 
world.c, mais elle a besoin de collision.h, ce qui crée un probleme 
d'inclusion circulaire */
void jouer(world *w);

#endif
