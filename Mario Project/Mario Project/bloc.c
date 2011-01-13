/*============================================================================*/
/*== [bloc.c] [Version: 0.028a]                                             ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "bloc.h"

occ_bloc* new_occ_bloc(int pos_x, int pos_y, int bloc_actuel, int bloc_alternatif, int id_item)
{
	occ_bloc* occ = malloc(sizeof(occ_bloc));

	return init_occ_bloc(occ, pos_x, pos_y, bloc_actuel, bloc_alternatif, id_item);
}

occ_bloc* init_occ_bloc(occ_bloc* occ, int pos_x, int pos_y, int bloc_actuel, int bloc_alternatif, int id_item)
{
	if(occ != NULL)
	{
		occ->position.x = pos_x;
		occ->position.y = pos_y;
		occ->position_prec = occ->position;
		occ->bloc_actuel = bloc_actuel;
		occ->bloc_alternatif = bloc_alternatif;
		occ->etat = IMMOBILE;
		occ->compteur_etat = 0;
		occ->item = id_item;
	}
	return occ;
}

liste_bloc *new_liste_bloc()
{
    liste_bloc *liste = malloc(sizeof(liste_bloc));
    liste->debut = NULL;
    liste->nb = 0;

    return liste;
}


void free_liste_bloc(liste_bloc **liste)
{
    if(liste != NULL && *liste != NULL)
    {
        elt_liste_bloc *actuel = (*liste)->debut;
        elt_liste_bloc *precedent = NULL;

        while(actuel != NULL)
        {
            precedent = actuel;
            actuel = actuel->suivant;
            free_elt_liste_bloc(&actuel);
        }

        free(*liste);
        *liste = NULL;
    }
}


elt_liste_bloc *new_elt_liste_bloc()
{
    elt_liste_bloc *elt = malloc(sizeof(elt_liste_bloc));
    elt->suivant = NULL;

    return elt;
}


void free_elt_liste_bloc(elt_liste_bloc **elt)
{
    if(elt != NULL && *elt != NULL)
    {
        free(*elt);
        *elt = NULL;
    }
}


void ajouter_bloc_liste(liste_bloc *liste, bloc elt)
{
    if(liste != NULL)
    {
        elt_liste_bloc *actuel = liste->debut;
        elt_liste_bloc *precedent = NULL;

        while(actuel != NULL)
        {
			/* Si un doublon est trouvé, on sort de la fonction */
            if(actuel->elt.coord_sprite.x == elt.coord_sprite.x
                && actuel->elt.coord_sprite.y == elt.coord_sprite.y
                && actuel->elt.texture == elt.texture)
            {
                return;
            }

            precedent = actuel;
            actuel = actuel->suivant;
        }

        precedent->suivant = new_elt_liste_bloc();
        precedent->elt = elt;
        liste->nb++;
    }
}


void supprimer_bloc_liste(liste_bloc *liste, bloc elt)
{
    if(liste != NULL)
    {
        elt_liste_bloc *actuel = liste->debut;
        elt_liste_bloc *precedent = NULL;

        while(actuel != NULL)
        {
            if(actuel->elt.coord_sprite.x == elt.coord_sprite.x
                && actuel->elt.coord_sprite.y == elt.coord_sprite.y
                && actuel->elt.texture == elt.texture)
            {
                if(actuel == liste->debut)
                {
                    liste->debut = actuel->suivant;
                }
                else
                {
                    precedent->suivant = actuel->suivant;
                }

                free_elt_liste_bloc(&actuel);
                liste->nb--;
            }

            precedent = actuel;
            actuel = actuel->suivant;
        }
    }
}


void bloc_liste_a_tableau(liste_bloc *liste, int *taille, bloc **tab)
{
    if(liste != NULL)
    {       
        int i;
        elt_liste_bloc *actuel = liste->debut;

        if(tab != NULL)
        {
            free(tab);
        }

        *tab = malloc(sizeof(bloc) * liste->nb);
        *taille = liste->nb;

        for(i = 0; i < liste->nb; i++)
        {
            *tab[i] = actuel->elt;
            actuel = actuel->suivant;
        }
    }
}

