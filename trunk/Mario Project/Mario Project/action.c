/*============================================================================*/
/*== [action.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "action.h"


action *new_action(void(*f)(action *), bouton *b, int shortcut, int enabled, void *data, int flag)
{
    return init_action(malloc(sizeof(action)), f, b, shortcut, enabled, data, flag);
}


action *init_action(action *a, void(*f)(action *), bouton *b, int shortcut, int enabled, void *data, int flag)
{
    if(a != NULL)
    {
        a->func = f;
        a->shortcut = shortcut;
        a->enabled = enabled;
        a->data = data;
        a->nb_boutons = 0;
        a->boutons = NULL;
        a->btn_flag = NULL;

        link_action_btn(a, b, flag);
    }

    return a;
}


void link_action_btn(action *a, bouton *b, int flag)
{
    if(a != NULL && b != NULL)
    {
        b->action = a;

        /* Puis le contraire */
        a->nb_boutons++;
        a->boutons = realloc(a->boutons, sizeof(bouton *) * a->nb_boutons);
        a->boutons[a->nb_boutons - 1] = b;
        a->btn_flag = realloc(a->btn_flag, sizeof(int) * a->nb_boutons);
        a->btn_flag[a->nb_boutons - 1] = flag;

        /* Etat du bouton cohérent avec celui de l'action */
        if(a->enabled)
            enable(b);
        else
            disable(b);
    }
}


void unlink_action_btn(action *a, bouton *b)
{
    if(a!= NULL && b != NULL && b->action == a)
    {
        int i, j;

        for(i = j = 0; j < a->nb_boutons; i++, j++)
        {
            if(a->boutons[i] == b)
            {
                i--;
            }
            else
            {
                a->boutons[i] = a->boutons[j];
                a->btn_flag[i] = a->btn_flag[j];
            }
        }

        /* Le bouton était bien lié à l'action */
        if(i != j)
        {
            a->nb_boutons--;
            a->boutons = realloc(a->boutons, sizeof(bouton *) * a->nb_boutons);
            a->btn_flag = realloc(a->btn_flag, sizeof(int) * a->nb_boutons);
        }
    }
}


void free_action(action **a)
{
    if(a != NULL && *a != NULL)
    {
        free((*a)->boutons);
        free((*a)->btn_flag);

        free(*a);
        *a = NULL;
    }
}


void enable_action(action *a)
{
    if(a != NULL)
    {
        int i;

        a->enabled = 1;

        for(i = 0; i < a->nb_boutons; i++)
            enable(a->boutons[i]);
    }
}


void disable_action(action *a)
{
    if(a != NULL)
    {
        int i;

        a->enabled = 0;

        for(i = 0; i < a->nb_boutons; i++)
            disable(a->boutons[i]);
    }
}


void execute_action(action *a, keystate *k)
{
    if(a != NULL && a->func != NULL && a->enabled && k != NULL)
    {
        int i;
        int shortcut = 0;
        bouton *b = NULL;

        /* On regarde si un racourci à été definit, et si c'est le cas, si il a été pressé */
        if(a->shortcut != AUCUN)
            shortcut = k->actuel[a->shortcut] && !k->precedent[a->shortcut];

        /* Si des boutons ont étés attribués, on regarde si leur etat correspond a celui associé à l'action */
        for(i = 0; i < a->nb_boutons; i++)
        {
            if(a->boutons[i]->etat_actuel & a->btn_flag[i])
                b = a->boutons[i];
        }

        /* Si on est dans un des deux cas, on execute */
        if(shortcut || b != NULL)
        {
            a->func(a);

            /* et on simule un clic pour que l'état des boutons soit cohérent */
            for(i = 0; i < a->nb_boutons; i++)
            {
                if(a->boutons[i] != b)
                    click(a->boutons[i]);
            }
        }

    }
}


void force_execute_action(action *a, keystate *k)
{
    if(a != NULL && a->func != NULL && a->enabled && k != NULL)
    {
        int i;

        /* Quoi qu'il arrive, on execute */
        a->func(a);

        /* On simule un clic pour que l'état des boutons soit cohérent */
        for(i = 0; i < a->nb_boutons; i++)
        {
            click(a->boutons[i]);
        }
    }
}


void execute_action_shortcut(action *a, keystate *k)
{
    if(a != NULL && a->func != NULL && a->enabled && k != NULL)
    {
        if(a->shortcut != AUCUN && k->actuel[a->shortcut] && !k->precedent[a->shortcut])
            force_execute_action(a, k);
    }
}


void toggle_action_state(action *a)
{
    if(a != NULL)
    {
        if(a->enabled)
            disable_action(a);
        else
            enable_action(a);
    }
}
