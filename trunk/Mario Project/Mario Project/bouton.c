/*============================================================================*/
/*== [bouton.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "bouton.h"


bouton *new_bouton(int type, int nb_sous_boutons)
{
    int i;

    bouton *b = malloc(sizeof(bouton));
    b = init_bouton(b);
    b->type = type;
    b->action = NULL;

    if(type == RADIO)
    {
        b->sous_boutons = malloc(sizeof(bouton) * nb_sous_boutons);
        b->nb_sous_boutons = nb_sous_boutons;

        for(i = 0; i < nb_sous_boutons; i++)
            b->sous_boutons[i] = new_bouton_on_off();
    }

    return b;
}


bouton *new_bouton_classique(void)
{
    return new_bouton(CLASSIQUE, 0);
}


bouton *new_bouton_radio(int nb_sous_boutons)
{
    return new_bouton(RADIO, nb_sous_boutons);
}


bouton *new_bouton_on_off(void)
{
    return new_bouton(ON_OFF, 0);
}


void free_bouton(bouton **b)
{
    if(b != NULL && *b != NULL)
    {
        /* Liberation des sous boutons dans le cas des radios */
        if((*b)->type == RADIO)
        {
            int i;

            for(i = 0; i < (*b)->nb_sous_boutons; i++)
                free_bouton(&(*b)->sous_boutons[i]);

            free((*b)->sous_boutons);
        }

        /* On retire le lien avec l'action avant de liberer */
        unlink_action_btn((*b)->action, *b);

        free((*b));
        *b = NULL;
    }
}


bouton *copy_rec_btn(bouton *src)
{
    bouton *res = NULL;

    if(src != NULL)
    {
        int i;

        res = malloc(sizeof *res);
        *res = *src;                     /* Copie même les chaines de caractere, car elles sont statiques */

        res->sous_boutons = malloc(sizeof(bouton *) * src->nb_sous_boutons);

        for(i = 0; i < src->nb_sous_boutons; i++)
            res->sous_boutons[i] = copy_rec_btn(src->sous_boutons[i]);
    }

    return res;
}


bouton *init_bouton(bouton *b)
{
    sprite s = {{0, 0}, {0, 0}, 0, {0, 0}, {0, 0}};

    b->etat_actuel = RELEVE;
    b->etat_precedent = RELEVE;
    b->pos.x = 0;
    b->pos.y = 0;
    b->taille.x = 0;
    b->taille.y = 0;
    b->survole = 0;
    b->nb_sous_boutons = 0;
    b->sous_boutons = NULL;
    b->sprite_enfonce = s;
    b->sprite_releve = s;
    b->sprite_inactif = s;
    b->sprite_survole = s;
    b->enabled = 1;
    b->label_pos.x = b->label_pos.y = 8;

    b->aide[0] = '\0';
    b->label[0] = '\0';

    return b;
}


bouton *set_pos(bouton *b, int x, int y)
{
    if(b == NULL)
        return NULL;

    b->pos.x = x;
    b->pos.y = y;

    b->sprite_enfonce.position = b->pos;
    b->sprite_releve.position = b->pos;
    b->sprite_inactif.position = b->pos;
    b->sprite_survole.position = b->pos;

    return b;
}


bouton *move_btn(bouton *b, int x, int y)
{
    if(b != NULL)
    {
        set_pos(b, b->pos.x + x, b->pos.y + y);
    }

    return b;
}


bouton *set_size(bouton *b, int x, int y)
{
    if(b == NULL)
        return NULL;

    b->taille.x = x;
    b->taille.y = y;

    b->sprite_enfonce.taille =  b->taille;
    b->sprite_inactif.taille =  b->taille;
    b->sprite_releve.taille =  b->taille;
    b->sprite_survole.taille =  b->taille;

    return b;
}


bouton *set_text_enfonce(bouton *b, char *name)
{
    coordi taille = {0, 0};

    if(b == NULL)
        return NULL;

    b->sprite_enfonce.text_id = charger_texture_bis(name, &taille);
    b->sprite_enfonce.taille.x = taille.x;
    b->sprite_enfonce.taille.y = taille.y;

    return b;
}


bouton *set_text_releve(bouton *b, char *name)
{
    coordi taille = {0, 0};

    if(b == NULL)
        return NULL;

    b->sprite_releve.text_id = charger_texture_bis(name, &taille);
    b->sprite_releve.taille.x = taille.x;
    b->sprite_releve.taille.y = taille.y;

    return b;
}


bouton *set_text_unique(bouton *b, char *name)
{
    if(b != NULL)
    {
        coordi taille = {0, 0};
        coordf bg = {0, 0};
        coordf hd = {1, 1};
        int text = charger_texture_bis(name, &taille);

        set_size(b, taille.x, taille.y);

        /* id de texture */
        b->sprite_enfonce.text_id = text;
        b->sprite_inactif.text_id = text;
        b->sprite_releve.text_id = text;
        b->sprite_survole.text_id = text;

        /* coordonées de texture */
        b->sprite_enfonce.point_bg = bg;
        b->sprite_enfonce.point_hd = hd;
        b->sprite_inactif.point_bg = bg;
        b->sprite_inactif.point_hd = hd;
        b->sprite_releve.point_bg = bg;
        b->sprite_releve.point_hd = hd;
        b->sprite_survole.point_bg = bg;
        b->sprite_survole.point_hd = hd;
    }

    return b;
}


bouton *set_text_globale(bouton *b, char *name)
{
    coordi taille = {0, 0};
    int i;
    GLuint text;

    if(b == NULL)
        return NULL;

    text = charger_texture_bis(name, &taille);
    taille.y /= 4; /* Car la texture contient 4 boutons en hauteur */

    if(b->type == RADIO)
        for(i = 0; i < b->nb_sous_boutons; i++)
            set_text_globale_aux(b->sous_boutons[i], text, taille);
    else
        set_text_globale_aux(b, text, taille);

    return b;
}


bouton *set_text_globale_aux(bouton *b, GLuint text, coordi taille)
{
    /* Sprite enfoncé */
    b->sprite_enfonce.text_id = text;
    b->sprite_enfonce.taille = taille;
    b->sprite_enfonce.point_bg.y = 0.75F;
    b->sprite_enfonce.point_hd.x = 1;
    b->sprite_enfonce.point_hd.y = 1;

    /* Sprite relevé */
    b->sprite_releve.text_id = text;
    b->sprite_releve.taille = taille;
    b->sprite_releve.point_bg.y = 0.5F;
    b->sprite_releve.point_hd.x = 1;
    b->sprite_releve.point_hd.y = 0.75;

    /* Sprite survole */
    b->sprite_survole.text_id = text;
    b->sprite_survole.taille = taille;
    b->sprite_survole.point_bg.y = 0.25F;
    b->sprite_survole.point_hd.x = 1;
    b->sprite_survole.point_hd.y = 0.5;

    /* Sprite inactif */
    b->sprite_inactif.text_id = text;
    b->sprite_inactif.taille = taille;
    b->sprite_inactif.point_hd.x = 1;
    b->sprite_inactif.point_hd.y = 0.25;

    ///* Sprite enfoncé */
    //b->sprite_enfonce.text_id = text;
    //b->sprite_enfonce.taille.x = taille.x;
    //b->sprite_enfonce.taille.y = taille.y;
    //b->sprite_enfonce.point_hd.x = 1;
    //b->sprite_enfonce.point_hd.y = 0.5;

    ///* Sprite relevé */
    //b->sprite_releve.text_id = text;
    //b->sprite_releve.taille.x = taille.x;
    //b->sprite_releve.taille.y = taille.y;
    //b->sprite_releve.point_bg.y = 0.5;
    //b->sprite_releve.point_hd.x = 1;
    //b->sprite_releve.point_hd.y = 1;

    /* Bouton */
    b->taille.x = taille.x;
    b->taille.y = taille.y;

    return b;
}


bouton *set_help(bouton *b, char *help)
{
    int i;

    if(b == NULL)
        return NULL;

    for(i = 0; help[i] != '\0' && i < (TAILLE_AIDE_BOUTON - 1); i++)
        b->aide[i] = help[i];

    b->aide[i] = '\0';

    return b;
}


bouton *set_label(bouton *b, char *label)
{
    int i;

    if(b == NULL)
        return NULL;

    for(i = 0; label[i] != '\0' && i < (TAILLE_LABEL_BOUTON - 1); i++)
        b->label[i] = label[i];

    b->label[i] = '\0';

    return b;
}


void draw_bouton(bouton *b, GLuint last, coordi pos)
{
    int i;

    if(b == NULL)
        return;

    /* Type radio: on ne dessine que les sous boutons */
    if(b->type == RADIO)
    {
        for(i = 0; i < b->nb_sous_boutons; i++)
        {
            draw_bouton(b->sous_boutons[i], last, pos);
        }
    }
    /* Pour les autres on dessine le bouton */
    else
    {
        /* On affiche le label si il existe */
        if(b->label[0] != '\0')
        {
            coordi label_pos;
            label_pos.x = b->pos.x + b->label_pos.x;
            label_pos.y = b->pos.y + b->label_pos.y;

            screen_printf(label_pos, NULL, COLOR_WHITE, b->label);
        }

        glBegin(GL_QUADS);

        if(b->enabled == 0)
        {
            draw_sprite_(&b->sprite_inactif, last);

            /* On affiche l'aide grisée si elle existe */
            if(b->survole && b->aide[0] != '\0')
                screen_printf(pos, NULL, COLOR_GRAY, "[Inactif]%s", b->aide);
        }
        else if(b->survole)
        {
            /* On affiche l'aide si elle existe */
            if(b->aide[0] != '\0')
                screen_printf(pos, NULL, COLOR_WHITE, b->aide);

            if(b->etat_actuel == RELEVE)
                draw_sprite_(&b->sprite_survole, last);
            else
                draw_sprite_(&b->sprite_enfonce, last);
        }
        else
        {
            if(b->etat_actuel == RELEVE)
                draw_sprite_(&b->sprite_releve, last);
            else
                draw_sprite_(&b->sprite_enfonce, last);
        }

        glEnd();
    }
}


void maj_bouton(bouton *b, keystate *k)
{
    int i;
    int mx, my, nouveau = -1;

    if(b == NULL || k == NULL)
        return;

    SDL_GetMouseState(&mx, &my);
    my = HAUTEUR_FENETRE - my;

    if(b->type != RADIO)
    {
        if(b->etat_actuel == JUSTE_ENFONCE)
            b->etat_actuel = ENFONCE;
        else if(b->etat_actuel == JUSTE_RELEVE)
            b->etat_actuel = RELEVE;
    }

    /* Si la souris est dessus ou que le bouton est de type radio */
    if(((mx > b->pos.x && mx < (b->pos.x + b->taille.x)) && 
        (my > b->pos.y && my < (b->pos.y + b->taille.y))) || 
        b->type == RADIO)
    {
        if(b->type != RADIO)
            b->survole = 1;

        if(!b->enabled)
            return;

        b->etat_precedent = b->etat_actuel;

        switch(b->type)
        {
        case ON_OFF:
            if(k->actuel[CLIC_G] && !k->precedent[CLIC_G])
            {
                click(b);
                execute_action(b->action, k);
            }
            break;

        case CLASSIQUE:
            if(k->actuel[CLIC_G] && !k->precedent[CLIC_G])
            {
                click(b);
                execute_action(b->action, k);
            }
            else if(k->actuel[CLIC_G])
                b->etat_actuel = ENFONCE;
            else
                b->etat_actuel = RELEVE;
            break;

        case RADIO:
            /* Mise a jour de l'état de chaque sous-bouton */
            for(i = 0; i < b->nb_sous_boutons; i++)
            {
                maj_bouton(b->sous_boutons[i], k);
                if(b->sous_boutons[i]->etat_actuel == JUSTE_ENFONCE)
                    nouveau = i;
            }

            /* Si un nouveau a été pressé, on releve les autres */
            if(nouveau != -1)
            {
                b->etat_actuel = nouveau;
                force_execute_action(b->action, k);
            }

            for(i = 0; i < b->nb_sous_boutons; i++)
            {
                if(i == b->etat_actuel)
                    b->sous_boutons[i]->etat_actuel = ENFONCE;
                else
                    b->sous_boutons[i]->etat_actuel = RELEVE;

            }
            break;
        }
    }

    /* Eviter que les boutons ne restent enfoncés si on sort
    du bouton avec le clic maintenu et masquer l'aide */
    else
    {
        b->survole = 0;
        if(b->type == CLASSIQUE)
            b->etat_actuel = RELEVE;
    }
}


void click(bouton *b)
{
    if(b != NULL)
    {
        if(!b->enabled)
            return;

        b->survole = 1;
        b->etat_precedent = b->etat_actuel;

        switch(b->type)
        {
        case ON_OFF:
            if(b->etat_actuel == ENFONCE)
                b->etat_actuel = JUSTE_RELEVE;
            else if(b->etat_actuel == RELEVE)
                b->etat_actuel = JUSTE_ENFONCE;
            break;

        case CLASSIQUE:
            b->etat_actuel = JUSTE_ENFONCE;
            break;

        default:
            break;
        }
    }
}


bouton *enable(bouton *b)
{
    if(b != NULL)
    {
        int i;

        if(b->type == RADIO)
            for(i = 0; i < b->nb_sous_boutons; i++)
                enable(b->sous_boutons[i]);

        b->enabled = 1;
    }

    return b;
}


bouton *disable(bouton *b)
{
    if(b != NULL)
    {
        int i;

        if(b->type == RADIO)
            for(i = 0; i < b->nb_sous_boutons; i++)
                disable(b->sous_boutons[i]);

        b->enabled = 0;
    }

    return b;
}
