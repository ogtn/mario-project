/*============================================================================*/
/*== [group_box.c] [Version: 0.028a]                                        ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "group_box.h"


group_box *new_group_box(int pos_x, int pos_y, int taille_x, int taille_y)
{
    group_box *g = malloc(sizeof *g);

    g->boutons = NULL;
    g->nb_boutons = 0;
    g->visible = 1;

    g->group_boxes = NULL;
    g->nb_group_boxes = 0;

    g->position.x = pos_x;
    g->position.y = pos_y;

    g->taille.x = taille_x;
    g->taille.y = taille_y;

    g->texture = 0;
    g->epaisseur = 0;

    return g;
}


void free_group_box(group_box **g)
{
    if(g != NULL && *g != NULL)
    {
        int i;

        free_btn(*g);

        if((*g)->group_boxes != NULL)
        {
            for(i = 0; i < (*g)->nb_group_boxes; i++)
                free_group_box(&(*g)->group_boxes[i]);
        }

        free(*g);
        *g = NULL;
    }
}


void clean_group_box(group_box *g)
{
    if(g != NULL)
    {
        int i;

        if(g->group_boxes != NULL)
        {
            for(i = 0; i < g->nb_group_boxes; i++)
                free_group_box(&g->group_boxes[i]);

            g->nb_group_boxes = 0;
        }

        free_btn(g);
    }
}


group_box *copy_rec_group_box(group_box *src)
{
    group_box *res = NULL;

    if(src != NULL)
    {
        int i;
        res = malloc(sizeof *res);

        /* Copie de tous les champs */
        *res = *src;

        /* Réallocation des champs qui contiennent des pointeurs */
        res->boutons = malloc(sizeof(bouton*) * src->nb_boutons);
        res->group_boxes = malloc(sizeof(group_box *) * src->nb_group_boxes);

        /* Copie des champs qui le demandent */
        for(i = 0; i < src->nb_boutons; i++)
            res->boutons[i] = copy_rec_btn(src->boutons[i]);

        for(i = 0; i < src->nb_group_boxes; i++)
            res->group_boxes[i] = copy_rec_group_box(src->group_boxes[i]);
    }

    return res;
}


void add_btn(group_box *g, bouton *b)
{
    int i = 0;
    int j;

    if(g->boutons != NULL)
    {
        bouton **save = malloc(sizeof(bouton *) * g->nb_boutons);

        for(i = 0; i < g->nb_boutons; i++)
            save[i] = g->boutons[i];

        free(g->boutons);

        g->nb_boutons++;
        g->boutons = malloc(sizeof(bouton *) * g->nb_boutons);

        for(i = 0; i < (g->nb_boutons - 1); i++)
            g->boutons[i] = save[i];
    }
    else
    {
        g->nb_boutons++;
        g->boutons = malloc(sizeof(bouton *) * g->nb_boutons);
    }

    /* On utilise la position comme étant relative au conteneur, et pas à l'ecran */
    if(b->type == RADIO)
    {
        for(j = 0; j < b->nb_sous_boutons; j++)
        {
            set_pos(b->sous_boutons[j], b->sous_boutons[j]->pos.x + g->position.x + g->epaisseur,
                b->sous_boutons[j]->pos.y + g->position.y + g->epaisseur);
        }
    }
    else
    {
        set_pos(b, b->pos.x + g->position.x + g->epaisseur,
            b->pos.y + g->position.y + g->epaisseur);
    }

    g->boutons[i] = b;
}


void free_btn(group_box *g)
{
    if(g != NULL)
    {
        int i;

        for(i = 0; i < g->nb_boutons; i++)
            free_bouton(&g->boutons[i]);

        g->nb_boutons = 0;
    }
}


void add_group_box(group_box *g, group_box *fils)
{
    int i = 0;

    if(g->group_boxes != NULL)
    {
        group_box **save = malloc(sizeof(group_box *) * g->nb_group_boxes);

        for(i = 0; i < g->nb_group_boxes; i++)
            save[i] = g->group_boxes[i];

        free(g->group_boxes);

        g->nb_group_boxes++;
        g->group_boxes = malloc(sizeof(group_box *) * g->nb_group_boxes);

        for(i = 0; i < (g->nb_group_boxes - 1); i++)
            g->group_boxes[i] = save[i];
    }
    else
    {
        g->nb_group_boxes++;
        g->group_boxes = malloc(sizeof(group_box *) * g->nb_group_boxes);
    }

    /* On utilise la position comme étant relative au conteneur, et pas à l'ecran */
    if(fils != NULL)
    {
        fils->position.x += g->position.x + g->epaisseur;
        fils->position.y += g->position.y + g->epaisseur;
    }

    g->group_boxes[i] = fils;
}


void add_group_box_to_pos(group_box *g, group_box *fils, int pos)
{
    if(g != NULL && pos < g->nb_group_boxes)
    {
        move_group_box(fils, g->position.x, g->position.y);
        g->group_boxes[pos] = fils;
    }
}


void draw_group_box(group_box *g)
{
    if(g != NULL && g->visible)
    {
        int i;
        coordi pos = {8, 4}; /******************************************************************************************* a virer, mettre pos aide dans la structure bouton? */

        draw_cadre(g->position, g->taille, g->texture, g->epaisseur);

        for(i = 0; i < g->nb_boutons; i++)
            draw_bouton(g->boutons[i], pos);

        for(i = 0; i < g->nb_group_boxes; i++)
            draw_group_box(g->group_boxes[i]);
    }
}


void maj_group_box(group_box *g, keystate *k)
{
    if(g != NULL && g->visible)
    {
        int i;

        for(i = 0; i < g->nb_boutons; i++)
            maj_bouton(g->boutons[i], k);

        for(i = 0; i < g->nb_group_boxes; i++)
            maj_group_box(g->group_boxes[i], k);
    }
}


void show(group_box *g)
{
    if(g != NULL)
        g->visible = 1;
}


void hide(group_box *g)
{
    if(g != NULL)
        g->visible = 0;
}


void set_group_box_pos(group_box *g, int x, int y)
{
    if(g != NULL)
    {
        int i;
        int diff_x = x - g->position.x;
        int diff_y = y - g->position.y;

        for(i = 0; i < g->nb_group_boxes; i++)
            move_group_box(g->group_boxes[i], diff_x, diff_y);

        for(i = 0; i < g->nb_boutons; i++)
            move_btn(g->boutons[i], diff_x, diff_y);

        g->position.x = x;
        g->position.y = y;
    }
}


void move_group_box(group_box *g, int x, int y)
{
    if(g != NULL)
    {
        int i;

        for(i = 0; i < g->nb_group_boxes; i++)
            move_group_box(g->group_boxes[i], x, y);

        for(i = 0; i < g->nb_boutons; i++)
            move_btn(g->boutons[i], x, y);

        g->position.x += x;
        g->position.y += y;
    }
}
