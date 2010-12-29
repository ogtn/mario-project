/*============================================================================*/
/*== [editeur_action.c] [Version: 0.028a]                                   ==*/
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


void ac_switch_edition_browser(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;

        for(i = 0; i < NB_ACTIONS_BLOC; i++)
            toggle_action_state(eb->actions[i]);

        enable_action(eb->actions[AC_BLOC_SWITCH_EDITION_BROWSER]);

        if(eb->mode == MODE_BLOC_BROWSER)
        {
            /* Passage en mode edition */
            eb->mode = MODE_BLOC_EDITOR;
            hide(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_TEXTURE_BROWSER_BLOC]);
            show(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_PREVIEW_BLOC]);
        }
        else
        {
            /* Passage ne mode browser */
            eb->mode = MODE_BLOC_BROWSER;
            hide(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_PREVIEW_BLOC]);
            show(e->onglets[ONGLET_BLOCS]->group_boxes[REGION_TEXTURE_BROWSER_BLOC]);
            SDL_ShowCursor(1);
        }
    }
}


void ac_masque_fond(action *a)
{
    static unsigned int i = 0;

    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        e->dessiner_fonds = i++ % 2;
    }
}


void ac_masque_enemis(action *a)
{
    static unsigned int i = 0;

    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        e->dessiner_enemis = i++ % 2;
    }
}


void ac_masque_specials(action *a)
{
    static unsigned int i = 0;

    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        e->dessiner_specials = i++ % 2;
    }
}


void ac_masque_objets(action *a)
{
    static unsigned int i = 0;

    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        e->dessiner_objets = i++ % 2;
    }
}


void ac_annuler(action *a)
{
    a;
}


void ac_retaurer(action *a)
{
    a;
}


void ac_switch_select_rempli(action *a)
{
    static unsigned int i = 0;

    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;

        if(i++ % 2)
            eb->select_type = TYPE_FILL;
        else
            eb->select_type = TYPE_SELECT;

        eb->prev_select_type = eb->select_type;
        eb->selection_terminee = 0;
        eb->selection = 0;
    }
}


void ac_couper(action *a)
{
    ac_copier(a);
    ac_supprimer(a);
}


void ac_copier(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i, j;
        int si, sj;
        int x1, y1, x2, y2;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc; 

        if(eb->selection)
        {
            x1 = min(eb->debut_selection.x, eb->fin_selection.x);
            y1 = min(eb->debut_selection.y, eb->fin_selection.y);

            x2 = max(eb->debut_selection.x, eb->fin_selection.x);
            y2 = max(eb->debut_selection.y, eb->fin_selection.y);

            /* Reallocation du presse papier précédent */
            for(i = 0; i < eb->blocs_cb.taille.x; i++)
                free(eb->blocs_cb.blocs[i]);

            eb->blocs_cb.taille.x = x2 - x1 + 1;
            eb->blocs_cb.taille.y = y2 - y1 + 1;

            eb->blocs_cb.blocs = realloc(eb->blocs_cb.blocs, sizeof(int *) * eb->blocs_cb.taille.x);

            for(i = 0; i < eb->blocs_cb.taille.x; i++)
                eb->blocs_cb.blocs[i] = malloc(sizeof(int) * eb->blocs_cb.taille.y);

            /* Copie des blocs */
            for(i = x1, si = 0; i <= x2; i++, si++)
                for(j = y1, sj = 0; j <= y2; j++, sj++)
                    eb->blocs_cb.blocs[si][sj] = e->world->niveau->occ_blocs[i][j];
        }
    }
}


void ac_coller(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i, j;
        int si, sj;
        int x1, y1, x2, y2;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;

        if(eb->selection)
        {
            x1 = min(eb->debut_selection.x, eb->fin_selection.x);
            y1 = min(eb->debut_selection.y, eb->fin_selection.y);

            /* On copie le max dispo dans le presse-papier, tant que ça ne sort pas du niveau */
            x2 = min(eb->blocs_cb.taille.x + x1, e->world->niveau->taille.x) - 1;
            y2 = min(eb->blocs_cb.taille.y + y1, e->world->niveau->taille.y) - 1;

            /* Copie des blocs non vides */
            for(i = x1, si = 0; i <= x2; i++, si++)
            {
                for(j = y1, sj = 0; j <= y2; j++, sj++)
                {
                    if(eb->blocs_cb.blocs[si][sj] != BLOC_VIDE)
                    {
                        e->world->niveau->occ_blocs[i][j] = (id)(eb->blocs_cb.blocs[si][sj]);
                    }
                }
            }
        }
    }
}


void ac_precedent(action *a)
{
    a;
}


void ac_suivant(action *a)
{
    a;
}


void ac_dossier_parent(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;

        ouvre_dossier_parent(&e->mode_bloc.dossier_texture);
        charge_boutons_browser(e);
    }
}


void ac_racine(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;

        ouvre_racine(&e->mode_bloc.dossier_texture);
        charge_boutons_browser(e);
    }
}


void ac_enregistrer(action *a)
{
    a;
}


void ac_enregistrer_sous(action *a)
{
    a;
}


void ac_ouvrir(action *a)
{
    a;
}


void ac_supprimer(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i, j;
        int x1, y1, x2, y2;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;

        if(eb->selection)
        {
            x1 = min(eb->debut_selection.x, eb->fin_selection.x);
            y1 = min(eb->debut_selection.y, eb->fin_selection.y);

            x2 = max(eb->debut_selection.x, eb->fin_selection.x);
            y2 = max(eb->debut_selection.y, eb->fin_selection.y);

            for(i = x1; i <= x2; i++)
                for(j = y1; j <= y2; j++)
                    e->world->niveau->occ_blocs[i][j] = BLOC_VIDE;

            eb->selection = 0;
        }
    }
}


/* Bobo les noeils */
void ac_remplissage(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;

        if(eb->selection)
        {
            int i, j;
            int x1, y1, x2, y2;
            int bg, b, bd, g, c, d, hg, h, hd;
            group_box *gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_OUTIL_MAGIQUE_BLOC];
            bouton *outil_magique = gb->boutons[0];
            int offset = eb->favoris[eb->selected_favorite].blocs_offset;

            /* Attribution des id en fonction de l'outil magique */
            /* Je pense pas qu'on puisse simplement factoriser ce bordel en une boucle */
            switch(outil_magique->etat_actuel)
            {
                int unique;

            case SELECT_LINE_1:
                hg = g = bg =   offset + 2;
                h = c = b =     offset + 1;
                hd = d = bd =   offset;
                break;
            case SELECT_LINE_2:
                hg = g = bg =   offset + 5;
                h = c = b =     offset + 4;
                hd = d = bd =   offset + 3;
                break;
            case SELECT_LINE_3:
                hg = g = bg =   offset + 8;
                h = c = b =     offset + 7;
                hd = d = bd =   offset + 6;
                break;
            case SELECT_COL_1:
                hg = h = hd =   offset + 2;
                g = c = d =     offset + 5;
                bg = b = bd =   offset + 8;
                break;
            case SELECT_COL_2:
                hg = h = hd =   offset + 1;
                g = c = d =     offset + 4;
                bg = b = bd =   offset + 7;
                break;
            case SELECT_COL_3:
                hg = h = hd =   offset;
                g = c = d =     offset + 3;
                bg = b = bd =   offset + 6;
                break;
            case SELECT_ALL:
                bg =    offset + 8;
                b =     offset + 7;
                bd =    offset + 6;
                g =     offset + 5;
                c =     offset + 4;
                d =     offset + 3;
                hg =    offset + 2;
                h =     offset + 1;
                hd =    offset;
                break;

            default:
                switch(outil_magique->etat_actuel)
                {
                case SELECT_BG: unique = offset + 8; break;
                case SELECT_BM: unique = offset + 7; break;
                case SELECT_BD: unique = offset + 6; break;
                case SELECT_MG: unique = offset + 5; break;
                case SELECT_MM: unique = offset + 4; break;
                case SELECT_MD: unique = offset + 3; break;
                case SELECT_HG: unique = offset + 2; break;
                case SELECT_HM: unique = offset + 1; break;
                case SELECT_HD: default: unique = offset; break;
                }

                hg = h = hd = g = c = d = bg = b = bd = unique;
            };

            /* Calcul du rectange de selection dans le bon sens */
            x1 = min(eb->debut_selection.x, eb->fin_selection.x);
            y1 = min(eb->debut_selection.y, eb->fin_selection.y);
            x2 = max(eb->debut_selection.x, eb->fin_selection.x);
            y2 = max(eb->debut_selection.y, eb->fin_selection.y);

            for(i = x1; i <= x2; i++)
            {
                for(j = y1; j <= y2; j++)
                {
                    /* Premiere ligne */
                    if(j == y1)
                    {
                        if(i == x1)
                            e->world->niveau->occ_blocs[i][j] = (id)bg;  /* Coin bas gauche */
                        else if(i == x2)
                            e->world->niveau->occ_blocs[i][j] = (id)bd;  /* Coin bas droite */
                        else
                            e->world->niveau->occ_blocs[i][j] = (id)b;   /* Bas */
                    }
                    /* Derniere ligne */
                    else if(j == y2)
                    {
                        if(i == x1)
                            e->world->niveau->occ_blocs[i][j] = (id)hg;  /* Coin haut gauche */
                        else if(i == x2)
                            e->world->niveau->occ_blocs[i][j] = (id)hd;  /* Coin haut droite */
                        else
                            e->world->niveau->occ_blocs[i][j] = (id)h;   /* Haut */
                    }
                    /* Milieu */
                    else
                    {
                        if(i == x1)
                            e->world->niveau->occ_blocs[i][j] = (id)g;   /* Gauche */
                        else if(i == x2)
                            e->world->niveau->occ_blocs[i][j] = (id)d;   /* Droite */
                        else
                            e->world->niveau->occ_blocs[i][j] = (id)c;   /* Centre */
                    }
                }
            }

            eb->selection = 0;
        }
    }
}


void ac_add_to_favorites(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i;
        int x, y;
        bouton *b = NULL;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;
        group_box *g_loc = NULL, *fav, *gb;

        /* On recupere dans gb la group_box du browser de texture */
        gb = e->onglets[ONGLET_BLOCS];
        gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC];
        gb = gb->group_boxes[REGION_TEXTURE_BROWSER_BLOC_IMG];

        /* On cherche le bouton pressé et on recupere sa group_box */
        for(i = 0; i < gb->nb_group_boxes; i++)
        {
            g_loc = gb->group_boxes[i];

            b = g_loc->boutons[0];

            if(b->etat_actuel == JUSTE_ENFONCE)
                break;
        }

        /* On recupere dans gb la group_box des favoris */
        gb = e->onglets[ONGLET_BLOCS];
        gb = gb->group_boxes[REGION_FAVORIS_BLOC];

        /* On vire le favoris selectionné et on copie le nouveau, en prenant soin de le decliquer */
        free_group_box(&gb->group_boxes[eb->selected_favorite]);
        fav = copy_rec_group_box(g_loc);
        fav->boutons[0]->etat_actuel = fav->boutons[0]->etat_precedent;
        fav->boutons[0]->survole = 0;

        /* Placement relatif dans les favoris */
        x = (eb->selected_favorite % 2) * (fav->taille.x + 10) + 10;
        y = gb->taille.y - fav->taille.y - 32;
        y -= (eb->selected_favorite / 2) * (fav->taille.y + 16) - 16;
        set_group_box_pos(fav, x, y);

        /* Ajout dans la groupbox et affectation de l'action et du label qui va bien */
        add_group_box_to_pos(gb, fav, eb->selected_favorite);
        link_action_btn(e->actions[AC_BLOC_ADD_TO_FAVORITES], fav->boutons[0], ON_CLICK);
        set_help(fav->boutons[0], "Selectionner ce slot de favoris");

        /* Remplissage du favoris (chemin de la texture, puis son id, offset de ses blocs) */
        get_file_path(eb->dossier_texture, i, eb->favoris[eb->selected_favorite].texture_name);
        eb->favoris[eb->selected_favorite].texture_id = charger_texture_bis(eb->favoris[eb->selected_favorite].texture_name, NULL);
        eb->favoris[eb->selected_favorite].blocs_offset = e->world->niveau->nb_blocs;

        /* Ajouter le favoris au niveau */                                                                                              // OMGWTFBBQ§§§§
        {
            // ajout de la texture au niveau
            {
                niveau *n = e->world->niveau;
                texture t;

                n->taille_blocs.x = n->taille_blocs.y = 32;
                n->nb_textures++;
                n->textures = realloc(n->textures, sizeof(texture) * n->nb_textures);

                t.taille_sprite.x = t.taille_sprite.y = 32;
                t.taille.x = t.taille.y = 32 * 3;
                t.id_text = eb->favoris[eb->selected_favorite].texture_id;                                // plutot recharger la texture?
                strcpy(t.nom, eb->favoris[eb->selected_favorite].texture_name);
                t.phys = NULL;

                //t.phys = malloc(sizeof(id) * 9);

                //t.phys[0] = COIN_BAS_A_GAUCHE;
                //t.phys[1] = PLAFOND;
                //t.phys[2] = COIN_BAS_A_DROITE;
                //t.phys[3] = MUR_A_GAUCHE;
                //t.phys[4] = VIDE;
                //t.phys[5] = MUR_A_DROITE;
                //t.phys[6] = COIN_HAUT_A_GAUCHE;
                //t.phys[7] = SOL;
                //t.phys[8] = COIN_HAUT_A_DROITE;

                n->textures[n->nb_textures - 1] = t;
            }
            //ajout des blocs
            {
                int i, j;
                int save;
                bloc *save_blocs;
                niveau *n = e->world->niveau;
                bloc b = {0};

                save = n->nb_blocs;
                save_blocs = n->blocs;

                n->nb_blocs += 9;
                n->blocs = malloc(sizeof(bloc) * n->nb_blocs);
                for(i = 0; i < save; i++)
                    n->blocs[i] = save_blocs[i];

                //n->blocs = realloc(n->blocs, sizeof(bloc) * n->nb_blocs);
                b.texture = n->nb_textures - 1;

                for(i = 0; i < 3; i++)
                {
                    b.coord_sprite.y = i;

                    for(j = 0; j < 3; j++)
                    {
                        b.coord_sprite.x = j;
                        n->blocs[n->nb_blocs - 1 - (i*3+j)] = b;
                    }
                }

                n->blocs[n->nb_blocs - 9].phys = COIN_HAUT_A_GAUCHE;
                n->blocs[n->nb_blocs - 8].phys = SOL;
                n->blocs[n->nb_blocs - 7].phys = COIN_HAUT_A_DROITE;
                n->blocs[n->nb_blocs - 6].phys = MUR_A_GAUCHE;
                n->blocs[n->nb_blocs - 5].phys = VIDE;
                n->blocs[n->nb_blocs - 4].phys = MUR_A_DROITE;
                n->blocs[n->nb_blocs - 3].phys = COIN_BAS_A_GAUCHE;
                n->blocs[n->nb_blocs - 2].phys = PLAFOND;
                n->blocs[n->nb_blocs - 1].phys = COIN_BAS_A_DROITE;
            }
        }

        /* On selectionne le suivant */
        ac_selectionne_favori(a);
    }
}


void ac_selectionne_favori(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int fav;
        editeur *e = a->data;
        editeur_bloc *eb = &e->mode_bloc;
        group_box *gb;

        gb = e->onglets[ONGLET_BLOCS];
        gb = gb->group_boxes[REGION_FAVORIS_BLOC];

        /* Recherche du bouton responsable */
        for(fav = 0; fav < gb->nb_group_boxes; fav++)
        {
            if(gb->group_boxes[fav] != NULL && gb->group_boxes[fav]->boutons[0]->etat_actuel == JUSTE_ENFONCE)
                break;
        }

        /* Si on est pas passé par un bouton, on passe au favori suivant */
        if(fav == gb->nb_group_boxes)
        {
            fav = eb->selected_favorite + 1;

            if(fav >= REGION_FAV_MAX)
                fav = 0;
        }

        /* On effectue les changements de texture */
        if(gb->group_boxes[fav] != NULL)
            gb->group_boxes[fav]->texture = charger_texture_bis("textures/cadre5.png", NULL);

        if(gb->group_boxes[eb->selected_favorite] != NULL)
            gb->group_boxes[eb->selected_favorite]->texture = e->texture_cadre;

        eb->selected_favorite = fav;

        gb = e->onglets[ONGLET_BLOCS]->group_boxes[REGION_OUTIL_MAGIQUE_BLOC];
        set_text_outil_3x3(gb->boutons[0], eb->favoris[fav].texture_name);
    }
}


void ac_test_level(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        editeur *e = a->data;

        jouer(e->world);
    }
}


void ac_mode_switch(action *a)
{
    if(a != NULL && a->data != NULL)
    {
        int i;
        editeur *e = a->data;

        for(i = 0; i < NB_MODES; i++)
            hide(e->onglets[i]);

        show(e->onglets[e->bouton_onglet->etat_actuel]);
    }
}
