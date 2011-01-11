/*============================================================================*/
/*== [editeur.c] [Version: 0.028a]                                          ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "editeur.h"


/* Ce super tableau permet de générer via une boucle les boutons et les actions, et de
les associer, sans avoir à le faire manuelement. Sortez vos écrans wide les enfants. */
static const table_actions_editeur action_master_table[NB_ACTIONS] = 
{
    /*
    [type]		[texture]						[aide]											[fonction associée]			[Racourci]	[action sur le bouton]  [activation] */
    /* Actions générales */
    {0, 	    NULL,	                        NULL,								            ac_mode_switch,				AUCUN,		0,                      1},
    {ON_OFF,	"textures/boutons/test.png",	"Masque ou affiche les fonds",					ac_masque_fond,				AUCUN,		ON_CHANGE,              1},
    {ON_OFF,	"textures/boutons/test.png",	"Masque ou affiche les ennemis",				ac_masque_enemis,			AUCUN,		ON_CHANGE,              1},
    {ON_OFF,	"textures/boutons/test.png",	"Masque ou affiche les objets spéciaux",		ac_masque_specials,			AUCUN,		ON_CHANGE,              1},
    {ON_OFF,	"textures/boutons/test.png",	"Masque ou affiche les objets",					ac_masque_objets,			AUCUN,		ON_CHANGE,              1},
    {0, 	    "textures/boutons/test.png",	"Non implémenté",								ac_annuler,					AUCUN,		0,                      0},
    {0, 	    "textures/boutons/test.png",	"Non implémenté",								ac_retaurer,				AUCUN,		0,                      0},
    {CLASSIQUE,	"textures/boutons/test.png",	"Teste le niveau",              				ac_test_level,				AUCUN,		ON_CHANGE,              1},
    {CLASSIQUE, "textures/boutons/test.png",	"Non implémenté",								ac_enregistrer,				AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE, "textures/boutons/test.png",	"Non implémenté",								ac_enregistrer_sous,		AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE, "textures/boutons/test.png",	"Non implémenté",								ac_ouvrir,					AUCUN,		ON_CHANGE,              0},

    /* Actions propres au mode bloc */
    {ON_OFF,	"textures/boutons/test.png",	"Passe du browser de bloc au mode d'édition",	ac_switch_edition_browser,	SAUTER,		ON_CHANGE,              1},
    {ON_OFF,	"textures/boutons/test.png",	"Passe du mode sélection au mode remplissage",	ac_switch_select_rempli,	AUCUN,		ON_CHANGE,              1},
    {CLASSIQUE,	"textures/boutons/test.png",	"Coupe la sélection (X)",  						ac_couper,  				CUT,		ON_CHANGE,              1},
    {CLASSIQUE,	"textures/boutons/test.png",	"Copie la sélection (C)",			    		ac_copier,				    COPY,		ON_CHANGE,              1},
    {CLASSIQUE,	"textures/boutons/test.png",	"Colle le contenu du presse papier (V)",		ac_coller,  				PASTE,		ON_CHANGE,              1},
    {CLASSIQUE,	"textures/boutons/test.png",	"Non implémenté",								ac_precedent,				AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE,	"textures/boutons/test.png",	"Non implémenté",								ac_suivant,					AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE,	"textures/boutons/test.png",	"Aller au dossier parent",						ac_dossier_parent,			AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE,	"textures/boutons/test.png",	"Aller à la racine",							ac_racine,					AUCUN,		ON_CHANGE,              0},
    {CLASSIQUE,	"textures/boutons/test.png",	"Supprime la séléction",						ac_supprimer,				SUPPR,		ON_CHANGE,              1},
    {0, 		NULL,							NULL,											ac_remplissage,				AUCUN,		0,                      1},
    {0, 		NULL,							NULL,											ac_add_to_favorites,		ENTRER,		0,                      0},
    {0, 		NULL,							NULL,											ac_selectionne_favori,    	AUCUN,		0,                      1}   
};


void main_editeur(void)
{
    int continuer = VRAI;
    editeur e;

    /* Initialisation de l'editeur */
    init_editeur(&e);

    /* Boucle principale du programme à modifier pour qu'elle soit plus "intelligente"
    il faut que quand on presse echap le menu pause s'ouvre, et qu'on retourne dans
    la boucle en quittant le menu (sauf si on decide d'arreter de jouer) */
    while(continuer)
    {
        glClearColor(0x31 / 256., 0x49 / 256., 0x6A / 256., 0);
        glClear(GL_COLOR_BUFFER_BIT);
   
        /* Mise à jour */
        maj_keystate(e.world->keystate, &continuer); 
        update_taille_fenetre(e.world);
        maj_editeur(&e);
       
        /* Rendu */
        draw_main_editeur(&e);

        /* Impression du texte */
        screen_flush();
        SDL_GL_SwapBuffers();
        my_sleep(1);
    }

    //liberer_textures_niveau(e.world->niveau);
}


void draw_main_editeur(editeur *e)
{
    coordi pos = {0};
    group_box *gb;
    int i;

    for(i = 0; i < NB_REGIONS_EDITEUR; i++)
        draw_group_box(e->onglets[i]);

    draw_bouton(e->bouton_onglet, 0, pos);

    /* Regions dépendantes du mode */
    switch(e->mode)
    {
    case MODE_BLOC:
        if(e->mode_bloc.mode == MODE_BLOC_EDITOR)
        {
            draw_main_options(e->world->niveau, e->world->ecran, e->world->temps_actuel, e->dessiner_fonds, e->dessiner_blocs, e->dessiner_objets);

            if(e->mode_bloc.selection)
                dessine_selection(e);

            if(e->survole_apercu)
                dessine_viseur(e);
        }

        gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_OUTIL_MAGIQUE_BLOC];
        draw_outil_3x3(gb->boutons[0]);

        break;

    case MODE_ENNEMIS:
        if(e->mode_enemis.mode == MODE_ENEMI_EDITOR)
        {
            draw_main_options(e->world->niveau, e->world->ecran, e->world->temps_actuel, e->dessiner_fonds, e->dessiner_blocs, e->dessiner_objets);

            if(e->survole_apercu)
                dessine_viseur(e);
        }
        break;

    case MODE_BACKGROUND:
        screen_printf_dbg("\n\nRien à faire ici pour le moment!");
        break;

    default:
        screen_printf_dbg("Pas cool mec, t'as inventé un mode!");
        break;
    }
}


void init_editeur(editeur *e)
{
    int i;

    /* Trucs généraux en vrac */
    SDL_ShowCursor(1);
    e->mode = MODE_BLOC;
    e->texture_cadre = charger_texture_bis("textures/cadre3.png", NULL);
    e->world = new_world();
    load_world(e->world);

    /* Actions: on associe juste une function et un racourci */
    for(i = 0; i < NB_ACTIONS; i++)
        e->actions[i] = new_action(action_master_table[i].func, NULL,
        action_master_table[i].shortcut, action_master_table[i].enable_flag, e, 0);

    init_onglets(e);

    hide(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_TEXTURE_BROWSER_BLOC]);
    show(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_PREVIEW_BLOC]);
}


void init_onglets(editeur *e)
{
    int i;
    bouton *b;

    /* Onglet bloc */
    init_onglet_bloc(e);

    /* Onglet ennemis */
    init_onglet_ennemi(e);

    /* Onglet background */
    e->onglets[ONGLET_BACKGROUND] = new_group_box(0, 24, e->world->taille_fenetre.x, e->world->taille_fenetre.y - 24 - 28);

    /* Status bar */
    e->onglets[REGION_STATUS_BAR] = new_group_box(0, 0, e->world->taille_fenetre.x, 24);

    for(i = 0; i < NB_REGIONS_EDITEUR; i++)
    {
        e->onglets[i]->epaisseur = 4;
        e->onglets[i]->texture = e->texture_cadre;
    }

    /* Bouton des onglets */
    e->bouton_onglet = new_bouton_radio(NB_MODES);

    for(i = 0; i < NB_MODES; i++)
    {
        b = new_bouton_on_off();
        set_text_globale(b, "textures/boutons/onglet.png");
        set_pos(b, 5 + i * b->taille.x, e->world->taille_fenetre.y - b->taille.y);

        e->bouton_onglet->sous_boutons[i] = b;

        if(i == 0)
            b->etat_actuel = ENFONCE;
    }

    set_label(e->bouton_onglet->sous_boutons[MODE_BLOC], "Blocs");
    set_label(e->bouton_onglet->sous_boutons[MODE_ENNEMIS], "Ennemis");
    set_label(e->bouton_onglet->sous_boutons[MODE_BACKGROUND], "Backgrounds");
    e->bouton_onglet->etat_actuel = e->bouton_onglet->etat_precedent = 0;
    link_action_btn(e->actions[AC_MODE_SWITCH], e->bouton_onglet, ON_CHANGE);

    /* On masque tous les onglets, sauf le premier */
    for(i = 1; i < NB_MODES; i++)
            hide(e->onglets[i]);
}


void init_onglet_bloc(editeur *e)
{
    int i;
    group_box *gb, *main, *sub;
    editeur_bloc *eb = &e->mode_bloc;
    bouton *b;
    main = new_group_box(0, 24, e->world->taille_fenetre.x, e->world->taille_fenetre.y - 24 - 28);
    main->epaisseur = 4;
    e->onglets[ONGLET_BLOCS] = main;

    eb->mode = MODE_BLOC_EDITOR;

    /* Toolbox */
    init_toolbox_bloc(e);

    /* Selection */
    e->bloc_survole.x = e->bloc_survole.y = 0;
    eb->select_type = eb->prev_select_type = TYPE_FILL;
    eb->selection = 0;

    /* Presse-papier des blocs */
    eb->blocs_cb.taille.x = eb->blocs_cb.taille.y = 0;
    eb->blocs_cb.blocs = NULL;

    // favoris
    eb->selected_favorite = 0;
    gb = new_group_box(main->taille.x - 250 - 2 * main->epaisseur, 250, 250, main->taille.y - 250 - 2 * main->epaisseur);
    add_group_box(main, gb);
    // On lui colle des gb nulles pour en fixer le nombre
    for(i = 0; i < REGION_FAV_MAX; i++)
        add_group_box(gb, NULL);

    // preview
    gb = new_group_box(0, 0, main->taille.x - 250 - 2 * main->epaisseur, main->taille.y - main->group_boxes[REGION_TOOLBOX_BLOC]->taille.y - 2 * main->epaisseur);
    add_group_box(main, gb);

    // outil magique
    gb = new_group_box(main->taille.x - 250 - 2 * main->epaisseur, 0, 250, 250);
    add_group_box(main, gb);

    // texture browser
    eb->dossier_texture = new_dossier("./textures/blocs");
    strcpy(eb->dossier_texture->racine, eb->dossier_texture->path);

    gb = new_group_box(0, 0, main->taille.x - 250 - 2 * main->epaisseur, main->taille.y - main->group_boxes[REGION_TOOLBOX_BLOC]->taille.y - 2 * main->epaisseur);
    add_group_box(main, gb);

    sub = new_group_box(0, 0, 180, gb->taille.y);
    sub->epaisseur = 2;
    sub->texture = e->texture_cadre;
    add_group_box(gb, sub);

    sub = new_group_box(180, 0, gb->taille.x - sub->taille.x, gb->taille.y);
    sub->epaisseur = 2;
    sub->texture = e->texture_cadre;
    add_group_box(gb, sub);

    for(i = 0; i < main->nb_group_boxes; i++)
    {
        if(i != REGION_TEXTURE_BROWSER_BLOC)
        {
            main->group_boxes[i]->epaisseur = 2;
            main->group_boxes[i]->texture = e->texture_cadre;
        }
    }

    charge_boutons_browser(e);

    /* Outil magique */
    gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_OUTIL_MAGIQUE_BLOC];
    b = new_outil_3x3();
    set_pos_outil_3x3(b, 61, 61);
    set_text_outil_3x3(b, "");
    add_btn(gb, b);

    /* Initialisation des favoris des blocs */
    gb = e->onglets[ONGLET_BLOCS];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC_IMG];

    for(i = 0; i < REGION_FAV_MAX; i++)
    {
        click(gb->group_boxes[0]->boutons[0]);
        execute_action(e->actions[AC_ADD_TO_FAVORITES], e->world->keystate);
        maj_bouton(gb->group_boxes[0]->boutons[0], e->world->keystate);
    }

    /* C'est moche ce truc non? */
    gb = e->onglets[ONGLET_BLOCS];
    gb = gb->group_boxes[REGION_TOOLBOX_BLOC];

    click(gb->boutons[0]);
    execute_action(e->actions[AC_BLOC_SWITCH_EDITION_BROWSER], e->world->keystate);
    maj_bouton(gb->boutons[0], e->world->keystate);
}


void init_onglet_ennemi(editeur *e)
{
    editeur_enemis *en = &e->mode_enemis;
    group_box *main, *gb, *sub;
    int i;

    main = new_group_box(0, 24, e->world->taille_fenetre.x, e->world->taille_fenetre.y - 24 - 28);
    main->epaisseur = 4;
    e->onglets[ONGLET_ENNEMIS] = main;

    init_toolbox_ennemi(e);

    // favoris
    en->selected_favorite = 0;
    gb = new_group_box(main->taille.x - 250 - 2 * main->epaisseur, 250, 250, main->taille.y - 250 - 2 * main->epaisseur);
    add_group_box(main, gb);
    // On lui colle des gb nulles pour en fixer le nombre
    for(i = 0; i < REGION_FAV_MAX; i++)
        add_group_box(gb, NULL);

    // preview
    gb = new_group_box(0, 0, main->taille.x - 250 - 2 * main->epaisseur, main->taille.y - main->group_boxes[REGION_TOOLBOX_ENNEMI]->taille.y - 2 * main->epaisseur);
    add_group_box(main, gb);

    // outils special
    gb = new_group_box(main->taille.x - 250 - 2 * main->epaisseur, 0, 250, 250);
    add_group_box(main, gb);

    // texture browser
    en->dossier_texture = new_dossier("./textures/monstres");
    strcpy(en->dossier_texture->racine, en->dossier_texture->path);

    gb = new_group_box(0, 0, main->taille.x - 250 - 2 * main->epaisseur, main->taille.y - main->group_boxes[REGION_TOOLBOX_ENNEMI]->taille.y - 2 * main->epaisseur);
    add_group_box(main, gb);

    sub = new_group_box(0, 0, 180, gb->taille.y);
    sub->epaisseur = 2;
    sub->texture = e->texture_cadre;
    add_group_box(gb, sub);

    sub = new_group_box(180, 0, gb->taille.x - sub->taille.x, gb->taille.y);
    sub->epaisseur = 2;
    sub->texture = e->texture_cadre;
    add_group_box(gb, sub);

    for(i = 0; i < main->nb_group_boxes; i++)
    {
        if(i != REGION_ENNEMI_BROWSER_ENNEMI)
        {
            main->group_boxes[i]->epaisseur = 2;
            main->group_boxes[i]->texture = e->texture_cadre;
        }
    }

    charge_boutons_browser_ennemis(e);

    ///* Outil magique */
    //gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_OUTIL_MAGIQUE_BLOC];
    //b = new_outil_3x3();
    //set_pos_outil_3x3(b, 61, 61);
    //set_text_outil_3x3(b, "");
    //add_btn(gb, b);

    ///* Initialisation des favoris des blocs */
    //gb = e->onglets[ONGLET_BLOCS];
    //gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC];
    //gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC_IMG];

    //for(i = 0; i < REGION_FAV_MAX; i++)
    //{
    //    click(gb->group_boxes[0]->boutons[0]);
    //    execute_action(e->actions[AC_ADD_TO_FAVORITES], e->world->keystate);
    //    maj_bouton(gb->group_boxes[0]->boutons[0], e->world->keystate);
    //}
}


void init_toolbox_bloc(editeur *e)
{
    int i;
    int x, y, pas;
    group_box *main, *g;
    bouton *b;
    editeur_bloc *eb = &e->mode_bloc;
    const int correspondance[NB_ACTIONS_BLOC] = 
    {
        /* Association actions editeur <-> actions editeur bloc */
        AC_SWITCH_EDITION_BROWSER,
        AC_SELECTIONNE_FAVORI,
        AC_MASQUE_FOND,
        AC_MASQUE_ENNEMIS,
        AC_MASQUE_SPECIALS,
        AC_MASQUE_OBJETS,
        AC_ANNULER,
        AC_RETAURER,
        AC_SWITCH_SELECT_REMPLI,
        AC_COUPER,
        AC_COPIER,
        AC_COLLER,
        AC_PRECEDENT,
        AC_SUIVANT,
        AC_SUPPRIMER,
        AC_REMPLISSAGE,
        AC_DOSSIER_PARENT,
        AC_RACINE,
        AC_ADD_TO_FAVORITES,
        AC_ENREGISTRER,
        AC_ENREGISTRER_SOUS,
        AC_OUVRIR,
        AC_TEST_LEVEL
    };

    main = e->onglets[ONGLET_BLOCS];
    g = new_group_box(0, main->taille.y - 90 - 2 * main->epaisseur, main->taille.x - 250 - 2 * main->epaisseur, 90);
    add_group_box(main, g);
    main->epaisseur = 4;
    main->texture = e->texture_cadre;
    g->epaisseur = 2;
    g->texture = e->texture_cadre;

    /* Boutons et actions associées, attention les yeux */
    pas = 82;
    x = 2;
    y = 42 + g->epaisseur;

    for(i = 0; i < NB_ACTIONS_BLOC; i++)
    {
        int ind_ed = correspondance[i];
        eb->actions[i] = e->actions[ind_ed];

        if(action_master_table[ind_ed].type != 0)
        {
            /* Allocation bouton */
            if(action_master_table[ind_ed].type == ON_OFF)
                b = new_bouton_on_off();
            else
                b = new_bouton_classique();

            /* Initialisations du bouton, et placement dans la toolbar */
            set_text_globale(b, action_master_table[ind_ed].texture);
            set_help(b, action_master_table[ind_ed].aide);
            set_pos(b, x, y);
            add_btn(g, b);

            /* Lien du bouton avec l'action */
            link_action_btn(eb->actions[i], b, action_master_table[ind_ed].btn_flag);

            /* On se decalle */
            if((b->pos.x + b->taille.x) > (g->taille.x - pas))
            {
                x = 2;
                y -= 42;
            }
            else
                x += pas;
        }
    }
}


void init_toolbox_ennemi(editeur *e)
{
    group_box *main, *g;
//    editeur_enemis *en = &e->mode_enemis;

    main = e->onglets[ONGLET_ENNEMIS];
    g = new_group_box(0, main->taille.y - 90 - 2 * main->epaisseur, main->taille.x - 250 - 2 * main->epaisseur, 90);
    add_group_box(main, g);
    main->epaisseur = 4;
    main->texture = e->texture_cadre;
    g->epaisseur = 2;
    g->texture = e->texture_cadre;
}


void charge_boutons_browser(editeur *e)
{
    char aide[TAILLE_AIDE_BOUTON];
    unsigned int i;
    int pos_ini;
    coordi pos;
    editeur_bloc *eb = &e->mode_bloc;
    group_box *gb = e->onglets[ONGLET_BLOCS];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC_DOSSIERS];

    pos_ini = gb->taille.y;
    pos_ini -= 5;							/* pour pas coller le bouton aux bords */
    pos_ini -= 2 * gb->epaisseur;			/* pour tenir compte de la bordure */

    free_btn(gb);

    /* Boutons des dossiers */
    for(i = 0; i < eb->dossier_texture->nb_dossiers; i++)
    {
        bouton *b = new_bouton_classique();
        set_text_globale(b, "textures/boutons/repertoire.png");
        strcpy(aide, "Ouvre le repertoire ");
        strcat(aide, eb->dossier_texture->dossiers[i]);
        set_help(b, aide);
        set_label(b, eb->dossier_texture->dossiers[i]);
        pos_ini -= b->taille.y + 5;			/* On se decalle d'un bouton, et de 5 pixels pour qu'ils ne soient pas collés */
        set_pos(b, 5, pos_ini);				/* pour pas coller le bouton aux bords */

        add_btn(gb, b);
    }

    /* Miniatures des textures */
    gb = e->onglets[ONGLET_BLOCS];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC];
    gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC_IMG];
    pos.x = 5;
    pos.y = gb->taille.y - 5 - gb->epaisseur - 130;
    clean_group_box(gb);

    for(i = 0; i < eb->dossier_texture->nb_fichiers; i++)
    {
        bouton *b;
        char tex_path[MAX_PATH];
        group_box *mini;

        /* Retour à la ligne si besoin */
        if((pos.x + 110 + 5) > gb->taille.x)
        {
            pos.x = 5;
            pos.y -= 130 + 5;
        }

        /* Cadre de la miniature */
        mini = new_group_box(pos.x, pos.y, 110, 130);
        mini->epaisseur = 4;
        mini->texture = e->texture_cadre;
        add_group_box(gb, mini);
        pos.x += mini->taille.x + 5; // decalage

        /* Bouton */
        b = new_bouton_classique();
        get_file_path(eb->dossier_texture, i, tex_path);
        set_text_unique(b, tex_path);

        /* Label du bouton */
        get_file_name(eb->dossier_texture, i, tex_path);
        set_label(b, tex_path);

        /* Label trop long */
        if(strlen(b->label) > 10)
        {
            b->label[7] = b->label[8] = b->label[9] = '.';
            b->label[10] = '\0';
        }


        set_pos(b, 3, 23);
        set_help(b, "tu survoles un bloc, clique pour l'ajouter aux favoris");
        b->label_pos.x = 3;
        b->label_pos.y = -17;
        add_btn(mini, b);

        link_action_btn(e->actions[AC_ADD_TO_FAVORITES], b, ON_CLICK);
    }
}


void charge_boutons_browser_ennemis(editeur *e)
{
    char aide[TAILLE_AIDE_BOUTON];
    unsigned int i;
    int pos_ini;
    coordi pos;
    editeur_enemis *ee = &e->mode_enemis;
    group_box *gb = e->onglets[ONGLET_ENNEMIS];
    gb = gb->group_boxes[REGION_ENNEMI_BROWSER_ENNEMI];
    gb = gb->group_boxes[REGION_ENNEMI_BROWSER_DOSSIERS];

    pos_ini = gb->taille.y;
    pos_ini -= 5;							/* pour pas coller le bouton aux bords */
    pos_ini -= 2 * gb->epaisseur;			/* pour tenir compte de la bordure */

    free_btn(gb);

    /* Boutons des dossiers */
    for(i = 0; i < ee->dossier_texture->nb_dossiers; i++)
    {
        bouton *b = new_bouton_classique();
        set_text_globale(b, "textures/boutons/repertoire.png");
        strcpy(aide, "Ouvre le repertoire ");
        strcat(aide, ee->dossier_texture->dossiers[i]);
        set_help(b, aide);
        set_label(b, ee->dossier_texture->dossiers[i]);
        pos_ini -= b->taille.y + 5;			/* On se decalle d'un bouton, et de 5 pixels pour qu'ils ne soient pas collés */
        set_pos(b, 5, pos_ini);				/* pour pas coller le bouton aux bords */

        add_btn(gb, b);
    }

    /* Miniatures des textures */
    gb = e->onglets[ONGLET_ENNEMIS];
    gb = gb->group_boxes[REGION_ENNEMI_BROWSER_ENNEMI];
    gb = gb->group_boxes[REGION_ENNEMI_BROWSER_IMG];
    pos.x = 5;
    pos.y = gb->taille.y - 5 - gb->epaisseur - 130;
    clean_group_box(gb);

    for(i = 0; i < ee->dossier_texture->nb_fichiers; i++)
    {
        bouton *b;
        char tex_path[MAX_PATH];
        group_box *mini;

        /* Retour à la ligne si besoin */
        if((pos.x + 110 + 5) > gb->taille.x)
        {
            pos.x = 5;
            pos.y -= 130 + 5;
        }

        /* Cadre de la miniature */
        mini = new_group_box(pos.x, pos.y, 110, 130);
        mini->epaisseur = 4;
        mini->texture = e->texture_cadre;
        add_group_box(gb, mini);
        pos.x += mini->taille.x + 5; // decalage

        /* Bouton */
        b = new_bouton_classique();
        get_file_path(ee->dossier_texture, i, tex_path);
        set_text_unique(b, tex_path);

        /* Label du bouton */
        get_file_name(ee->dossier_texture, i, tex_path);
        set_label(b, tex_path);

        /* Label trop long */
        if(strlen(b->label) > 10)
        {
            b->label[7] = b->label[8] = b->label[9] = '.';
            b->label[10] = '\0';
        }

        set_pos(b, 3, 23);
        set_help(b, "tu survoles un monstre, clique pour l'ajouter aux favoris");
        b->label_pos.x = 3;
        b->label_pos.y = -17;
        add_btn(mini, b);

        link_action_btn(e->actions[AC_ADD_ENNEMI_TO_FAVORITES], b, ON_CLICK);
    }
}


void maj_editeur(editeur *e)
{
    int i;
    group_box *gb;

    /* Taille de l'aperçu du niveau */
    gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_PREVIEW_BLOC];
    e->world->ecran.origine.x = gb->position.x + gb->epaisseur;
    e->world->ecran.origine.y = gb->position.y + gb->epaisseur;
    e->world->ecran.taille.x =  gb->taille.x - gb->epaisseur * 2;
    e->world->ecran.taille.y =  gb->taille.y - gb->epaisseur * 2;

    /* Regions */
    for(i = 0; i < NB_REGIONS_EDITEUR; i++)
        maj_group_box(e->onglets[i], e->world->keystate);

    /* Onglets et mode courant */
    maj_bouton(e->bouton_onglet, e->world->keystate);
    e->mode = e->bouton_onglet->etat_actuel;

    switch(e->mode)
    {
    case MODE_BLOC:
        maj_editeur_bloc(e);
        break;

    case MODE_ENNEMIS:
        break;

    case MODE_BACKGROUND:
        break;

    default:
        break;
    }

    update_time(e->world);
}


void maj_editeur_bloc(editeur *e)
{
    group_box *gb;
    int i;

    /* Dossiers */
    gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_TEXTURE_BROWSER_BLOC]->group_boxes[REGION_TEXTURE_BROWSER_BLOC_DOSSIERS];
    for(i = 0; i < gb->nb_boutons; i++)
    {
        if(gb->boutons[i]->etat_actuel == JUSTE_ENFONCE)
        {
            ouvre_sous_dossier(&e->mode_bloc.dossier_texture, i);
            charge_boutons_browser(e);
            break;
        }
    }

    /* On s'occupe des selections si besoin */
    if(e->mode_bloc.mode == MODE_BLOC_EDITOR)
        maj_selection(e);

    /* Actions */
    for(i = 0; i < NB_ACTIONS_BLOC; i++)
        execute_action_shortcut(e->mode_bloc.actions[i], e->world->keystate);
}


void maj_selection(editeur *e)
{
    editeur_bloc *eb = &e->mode_bloc;

    /* Attention les yeux, on verifie juste qu'un clic ou un relachement
    de clic vient de se produire, à gauche et à droite */
    int debut_clic_g = e->world->keystate->actuel[CLIC_G] && 
        !e->world->keystate->precedent[CLIC_G];
    int fin_clic_g = !e->world->keystate->actuel[CLIC_G] && 
        e->world->keystate->precedent[CLIC_G];
    int debut_clic_d = e->world->keystate->actuel[CLIC_D] && 
        !e->world->keystate->precedent[CLIC_D];
    int fin_clic_d = !e->world->keystate->actuel[CLIC_D] && 
        e->world->keystate->precedent[CLIC_D];

    e->survole_apercu = find_bloc_survole(e);
    SDL_ShowCursor(!e->survole_apercu);

    if(e->survole_apercu)
    {
        deplace_ecran(e);

        /* Un clic provoque un debut de selection */
        if(debut_clic_g || debut_clic_d)
        {
            eb->selection = 1;
            eb->selection_terminee = 0;
            eb->debut_selection = e->bloc_survole;
            eb->fin_selection = e->bloc_survole;

            if(debut_clic_d)
                eb->select_type = TYPE_DELETE;
            else
                eb->select_type = eb->prev_select_type;
        }
        else if(!eb->selection_terminee)
        {
            eb->fin_selection = e->bloc_survole;

            /* Un relachement provoque une fin de selection entamée */
            if(fin_clic_g || fin_clic_d)
            {
                eb->selection_terminee = 1;

                /* En cas de clic droit, on effectue la suppression */
                if(fin_clic_d)
                    force_execute_action(e->actions[AC_BLOC_SUPPRIMER], e->world->keystate);

                /* En cas de clic gauche, si on est en mode remplissage, on remplie */
                else if(fin_clic_g && eb->select_type == TYPE_FILL)
                    force_execute_action(e->actions[AC_BLOC_REMPLISSAGE], e->world->keystate);
            }
        }

        /* Affichage status bar */
        {
            coordi pos = e->onglets[REGION_STATUS_BAR]->position;
            pos.x += 6;
            pos.y += 6;

            if(eb->selection)
            {
                screen_printf(pos, NULL, COLOR_WHITE, "[%d;%d]   Selection: %dx%d (%d blocs)",
                    e->bloc_survole.x, e->bloc_survole.y,
                    abs(eb->debut_selection.x - eb->fin_selection.x) + 1, abs(eb->debut_selection.y - eb->fin_selection.y) + 1,
                    (abs(eb->debut_selection.x - eb->fin_selection.x) + 1) * (abs(eb->debut_selection.y - eb->fin_selection.y) + 1));
            }
            else
            {
                int id_bloc = e->world->niveau->occ_blocs[e->bloc_survole.x][e->bloc_survole.y];

                if(id_bloc != BLOC_VIDE)
                    screen_printf(pos, NULL, COLOR_WHITE, "[%d;%d] physique: [%d]", e->bloc_survole.x, e->bloc_survole.y, e->world->niveau->blocs[id_bloc].phys);
                else
                    screen_printf(pos, NULL, COLOR_WHITE, "[%d;%d] physique: VIDE", e->bloc_survole.x, e->bloc_survole.y);
            }
        }
    }
    else
    {
        /* La selection se termine si on sort du niveau */
        if(eb->selection && !eb->selection_terminee)
            eb->selection = 0;
    }
}


void deplace_ecran(editeur *e)
{
    /* Mise à jour de la position de l'ecran */
    if(e->world->keystate->actuel[GAUCHE])
        e->world->ecran.scroll.x -= e->world->temps_ecoule;
    else if(e->world->keystate->actuel[DROITE])
        e->world->ecran.scroll.x += e->world->temps_ecoule;
    if(e->world->keystate->actuel[BAS])
        e->world->ecran.scroll.y -= e->world->temps_ecoule;
    else if(e->world->keystate->actuel[HAUT])
        e->world->ecran.scroll.y += e->world->temps_ecoule;

    /* On empeche que ça depasse des limites du niveau */
    if(e->world->ecran.scroll.x > e->world->niveau->taille.x * e->world->niveau->taille_blocs.x - e->world->ecran.taille.x)
        e->world->ecran.scroll.x = e->world->niveau->taille.x * e->world->niveau->taille_blocs.x - e->world->ecran.taille.x;
    if(e->world->ecran.scroll.y > e->world->niveau->taille.y * e->world->niveau->taille_blocs.y - e->world->ecran.taille.y)
        e->world->ecran.scroll.y = e->world->niveau->taille.y * e->world->niveau->taille_blocs.y - e->world->ecran.taille.y;
    if(e->world->ecran.scroll.y < 0)
        e->world->ecran.scroll.y = 0;
    if(e->world->ecran.scroll.x < 0)
        e->world->ecran.scroll.x = 0;
}


void debug_mode(editeur *e)
{
    screen_printf_dbg(" ===MODE DEBUG===\n");
    screen_printf_dbg("FPS:%4d\n", e->world->fps);

    if(e->bloc_survole.x >= 0 && e->bloc_survole.x < e->world->niveau->taille.x &&
        e->bloc_survole.y >= 0 && e->bloc_survole.y < e->world->niveau->taille.y)
        screen_printf_dbg("id du bloc (%d;%d): %d\n", e->bloc_survole.x, e->bloc_survole.y, e->world->niveau->occ_blocs[e->bloc_survole.x][e->bloc_survole.y]);
}


int find_bloc_survole(editeur *e)
{
    int mx, my;

    SDL_GetMouseState(&mx, &my);
    my = HAUTEUR_FENETRE - my;

    /* Si la souris se trouve sur l'ecran */
    if(mx > e->world->ecran.origine.x
        && mx < (e->world->ecran.origine.x + e->world->ecran.taille.x)
        && my > e->world->ecran.origine.y
        && my < (e->world->ecran.origine.y + e->world->ecran.taille.y))
    {
        /* On determine les coordonnées de la souris dans le niveau */
        mx += e->world->ecran.scroll.x - e->world->ecran.origine.x;
        my = my + e->world->ecran.scroll.y - e->world->ecran.origine.y;

        /* On en deduit les coordonées du bloc */
        e->bloc_survole.x = mx / e->world->niveau->taille_blocs.x;
        e->bloc_survole.y = my / e->world->niveau->taille_blocs.y;

        return 1;
    }
    else
        return 0;
}


void dessine_viseur(editeur *e)
{
    int mx, my;

    /* On empeche le viseur de depasser de la zone d'aperçu */
    glScissor(e->world->ecran.origine.x, e->world->ecran.origine.y,
        e->world->ecran.taille.x, e->world->ecran.taille.y);
    glEnable(GL_SCISSOR_TEST);

    /* On determine les coordonnées (dans la fenetre) du point bas gauche du bloc survolé */
    mx = e->world->ecran.origine.x + (e->bloc_survole.x * e->world->niveau->taille_blocs.x) - e->world->ecran.scroll.x;
    my = e->world->ecran.origine.y + (e->bloc_survole.y * e->world->niveau->taille_blocs.y) - e->world->ecran.scroll.y;

    /* Coloration clignotante (de façon fluide) de la colone et de la ligne active */
    glDisable(GL_TEXTURE_2D);
    glColor4f(0, 0, 0, (float)(sin((float)e->world->temps_actuel / 200)/10 + 0.3));
    glBegin(GL_QUADS);

    /* Ligne */
    glVertex2i(e->world->ecran.origine.x, my);
    glVertex2i(e->world->ecran.origine.x + e->world->ecran.taille.x, my);
    glVertex2i(e->world->ecran.origine.x + e->world->ecran.taille.x, my + e->world->niveau->taille_blocs.y);
    glVertex2i(e->world->ecran.origine.x, my + e->world->niveau->taille_blocs.y);

    /* Colone */
    glVertex2i(mx, e->world->ecran.origine.y);
    glVertex2i(mx + e->world->niveau->taille_blocs.x, e->world->ecran.origine.y);
    glVertex2i(mx + e->world->niveau->taille_blocs.x, e->world->ecran.origine.y + e->world->ecran.taille.y);
    glVertex2i(mx, e->world->ecran.origine.y + e->world->ecran.taille.y);

    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);	

    glDisable(GL_SCISSOR_TEST);
}


void dessine_selection(editeur *e)
{
    coordi dernier, survole;
    float red, green, blue;

    /* On recupere la couleur */
    red = ((e->mode_bloc.select_type >> 16) & 0xFF) / 255.0f;
    green = ((e->mode_bloc.select_type >> 8) & 0xFF) / 255.0f;
    blue =  (e->mode_bloc.select_type & 0xFF) / 255.0f;

    /* On empeche le viseur de depasser de la zone d'aperçu */
    glScissor(e->world->ecran.origine.x, e->world->ecran.origine.y,
        e->world->ecran.taille.x, e->world->ecran.taille.y);
    glEnable(GL_SCISSOR_TEST);

    /* On determine les coordonnées (dans la fenetre) du point bas gauche du dernier bloc modifié */
    dernier.x = e->world->ecran.origine.x + (e->mode_bloc.debut_selection.x * e->world->niveau->taille_blocs.x) - e->world->ecran.scroll.x;
    dernier.y = e->world->ecran.origine.y + (e->mode_bloc.debut_selection.y * e->world->niveau->taille_blocs.y) - e->world->ecran.scroll.y;

    /* On determine les coordonnées (dans la fenetre) du point bas gauche du bloc survolé */
    survole.x = e->world->ecran.origine.x + (e->mode_bloc.fin_selection.x * e->world->niveau->taille_blocs.x) - e->world->ecran.scroll.x;
    survole.y = e->world->ecran.origine.y + (e->mode_bloc.fin_selection.y * e->world->niveau->taille_blocs.y) - e->world->ecran.scroll.y;

    if(dernier.x <= survole.x)
        survole.x += e->world->niveau->taille_blocs.x;
    else
        dernier.x += e->world->niveau->taille_blocs.x;

    if(dernier.y <= survole.y)
        survole.y += e->world->niveau->taille_blocs.y;
    else
        dernier.y += e->world->niveau->taille_blocs.y;

    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0);

    /* Contour */
    glColor4f(red, green, blue, 1);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2i(dernier.x, dernier.y);
        glVertex2i(survole.x, dernier.y);
        glVertex2i(survole.x, survole.y);
        glVertex2i(dernier.x, survole.y);
    }
    glEnd();

    /* Interieur du rectangle */
    glColor4f(red, green, blue, 0.25F);
    glBegin(GL_QUADS);
    {
        glVertex2i(dernier.x, dernier.y);
        glVertex2i(survole.x, dernier.y);
        glVertex2i(survole.x, survole.y);
        glVertex2i(dernier.x, survole.y);
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);

    glDisable(GL_SCISSOR_TEST);
}


bouton *new_outil_3x3(void)
{
    bouton *b = new_bouton_radio(16);
    b->etat_precedent = b->etat_actuel = SELECT_ALL;

    /******************************* pas optimal car textures chargés plusieurs fois en mémoire *****************/
    set_text_globale(b->sous_boutons[SELECT_ALL], "textures/boutons/fleche_d.png");
    set_text_globale(b->sous_boutons[SELECT_COL_1], "textures/boutons/fleche_v.png");
    set_text_globale(b->sous_boutons[SELECT_COL_2], "textures/boutons/fleche_v.png");
    set_text_globale(b->sous_boutons[SELECT_COL_3], "textures/boutons/fleche_v.png");
    set_text_globale(b->sous_boutons[SELECT_LINE_1], "textures/boutons/fleche_h.png");
    set_text_globale(b->sous_boutons[SELECT_LINE_2], "textures/boutons/fleche_h.png");
    set_text_globale(b->sous_boutons[SELECT_LINE_3], "textures/boutons/fleche_h.png");

    return b;
}


bouton *set_pos_outil_3x3(bouton *b, int x, int y)
{
    int i;
    int p_x = x, p_y = y;

    if(b == NULL)
        return NULL;

    b->pos.x = p_x;
    b->pos.y = p_y;

    for(i = 0; i < OUTIL_3x3_TAILLE; i++)
    {
        if(i % 4 == 0 && i != 0)
        {
            p_x = x;
            p_y += 32;
        }
        else if(i != 0)
            p_x += 32;

        set_pos(b->sous_boutons[i], p_x, p_y);
    }

    return b;
}


/* Bouh que c'est laid, faire une boucle risque d'etre assez chiant, on verra plus tard... */
bouton *set_text_outil_3x3(bouton *b, char *nom)
{
    coordi taille = {0};
    GLuint text;
    float un_tiers = 1/3.F;
    coordf bg = {0}, hd = {0};

    if(b == NULL)
        return NULL;

    text = charger_texture_bis(nom, &taille);
    taille.x /= 3;
    taille.y /= 3;
    hd.x = hd.y = un_tiers;

    /*********************LIGNE DU BAS**********************/
    /* Coin bas gauche */
    b->sous_boutons[SELECT_BG]->taille = taille;
    b->sous_boutons[SELECT_BG]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_BG]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_BG]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_BG]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_BG]->sprite_releve = b->sous_boutons[SELECT_BG]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Coin bas milieu */
    b->sous_boutons[SELECT_BM]->taille = taille;
    b->sous_boutons[SELECT_BM]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_BM]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_BM]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_BM]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_BM]->sprite_releve = b->sous_boutons[SELECT_BM]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Coin bas droite */
    b->sous_boutons[SELECT_BD]->taille = taille;
    b->sous_boutons[SELECT_BD]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_BD]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_BD]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_BD]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_BD]->sprite_releve = b->sous_boutons[SELECT_BD]->sprite_enfonce;

    bg.x = 0;
    bg.y += un_tiers;
    hd.x = un_tiers;
    hd.y += un_tiers;

    /*********************LIGNE DU MILIEU**********************/
    /* Cote gauche */
    b->sous_boutons[SELECT_MG]->taille = taille;
    b->sous_boutons[SELECT_MG]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_MG]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_MG]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_MG]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_MG]->sprite_releve = b->sous_boutons[SELECT_MG]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Centre */
    b->sous_boutons[SELECT_MM]->taille = taille;
    b->sous_boutons[SELECT_MM]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_MM]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_MM]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_MM]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_MM]->sprite_releve = b->sous_boutons[SELECT_MM]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Cote droit */
    b->sous_boutons[SELECT_MD]->taille = taille;
    b->sous_boutons[SELECT_MD]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_MD]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_MD]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_MD]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_MD]->sprite_releve = b->sous_boutons[SELECT_MD]->sprite_enfonce;

    bg.x = 0;
    bg.y += un_tiers;
    hd.x = un_tiers;
    hd.y += un_tiers;

    /*********************LIGNE DU HAUT**********************/
    /* Coin haut gauche */
    b->sous_boutons[SELECT_HG]->taille = taille;
    b->sous_boutons[SELECT_HG]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_HG]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_HG]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_HG]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_HG]->sprite_releve = b->sous_boutons[SELECT_HG]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Coin haut milieu */
    b->sous_boutons[SELECT_HM]->taille = taille;
    b->sous_boutons[SELECT_HM]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_HM]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_HM]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_HM]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_HM]->sprite_releve = b->sous_boutons[SELECT_HM]->sprite_enfonce;

    hd.x += un_tiers;
    bg.x += un_tiers;

    /* Coin haut droite */
    b->sous_boutons[SELECT_HD]->taille = taille;
    b->sous_boutons[SELECT_HD]->sprite_enfonce.taille = taille;
    b->sous_boutons[SELECT_HD]->sprite_enfonce.text_id = text;
    b->sous_boutons[SELECT_HD]->sprite_enfonce.point_bg = bg;
    b->sous_boutons[SELECT_HD]->sprite_enfonce.point_hd = hd;
    b->sous_boutons[SELECT_HD]->sprite_releve = b->sous_boutons[SELECT_HD]->sprite_enfonce;

    return b;
}


void draw_rectangle(coordi pos, coordi taille, GLfloat r, GLfloat g, GLfloat b)
{
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0);

    glColor3f(r, g, b);

    /* Contour du rectangle */
    glBegin(GL_LINE_LOOP);
    {
        glVertex2i(pos.x, pos.y);
        glVertex2i(pos.x + taille.x, pos.y);
        glVertex2i(pos.x + taille.x, pos.y + taille.y);
        glVertex2i(pos.x, pos.y + taille.y);
    }
    glEnd();

    /* Interieur du rectangle */
    glColor4f(r, g, b, (float)(sin((float)SDL_GetTicks() / 200)/10 + 0.3));

    glBegin(GL_QUADS);
    {
        glVertex2i(pos.x, pos.y);
        glVertex2i(pos.x + taille.x, pos.y);
        glVertex2i(pos.x + taille.x, pos.y + taille.y);
        glVertex2i(pos.x, pos.y + taille.y);
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
}


void draw_outil_3x3(bouton *b)
{
    coordi pos, taille;

    pos = b->sous_boutons[b->etat_actuel]->pos;

    /* Cas des lignes */
    if(b->etat_actuel == SELECT_LINE_1 || b->etat_actuel == SELECT_LINE_2 || b->etat_actuel == SELECT_LINE_3)
    {
        taille.x = 32 * 3;
        taille.y = 32;
        pos.x += 32;
    }
    /* Cas des colonnes */
    else if(b->etat_actuel == SELECT_COL_1 || b->etat_actuel == SELECT_COL_2 || b->etat_actuel == SELECT_COL_3)
    {
        taille.x = 32;
        taille.y = 32 * 3;
        pos.y -= 32 * 3;
    }
    /* Cas de l'ensemble */
    else if(b->etat_actuel == SELECT_ALL)
    {
        taille.x = taille.y = 32 * 3;
        pos.x += 32;
        pos.y -= 32 * 3;
    }
    /* Cas des blocs seuls */
    else
    {
        taille.x = taille.y = 32;
    }

    draw_rectangle(pos, taille, 1, 1, 1);
}
