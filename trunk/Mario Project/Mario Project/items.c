/*============================================================================*/
/*== [items.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "items.h"

item* new_item(char* nom)
{
	item* i = malloc(sizeof(item));

	return init_item(i, nom);
}

item* init_item(item* i, char* nom)
{
	if( i != NULL)
	{
		i->taille.x = 0;
		i->taille.y = 0;

		i->texture = 0;

		i->vitesse.x = 0;
		i->vitesse.y = 0;

		i->soumission = 0;
		i->occ_items = new_liste_item();

		i->nom = 0;
		i->nb_sprites = 0;
		strcpy(i->nom_text, nom);
	}

	return i;
}

occ_item* new_occ_item(float position_x, float position_y, int type_item, coordf vitesse, ETAT_ITEM etat)
{
	occ_item* i = malloc(sizeof(occ_item));

	return init_occ_item(position_x, position_y, type_item, i, vitesse, etat);
}

occ_item* init_occ_item(float position_x, float position_y, int type_item, occ_item* i, coordf vitesse, ETAT_ITEM etat)
{
	if(i != NULL)
	{
		i->position.x = position_x;
		i->position.y = position_y;

		i->position_prec.x = i->position.x;
		i->position_prec.y = i->position.y;

		i->type_item = type_item;
		i->etat = etat;
		i->actif = 1;

		i->vitesse.x = vitesse.x;
		i->vitesse.y = vitesse.y;
	}

	return i;
}

occ_item* copy_item(occ_item* i) {

	occ_item *i_bis = NULL;

    i_bis = malloc(sizeof(occ_item));
    *i_bis = *i;

    return i_bis;
}

elem_item* creer_element_item(occ_item* i)
{
	elem_item* elt = malloc(sizeof(elem_item));

	if(elt != NULL)
	{		
		elt->occ_item = i;
		elt->suivant = NULL;
	}

	return elt;
}

liste_item* new_liste_item(){

	liste_item* li = malloc(sizeof(liste_item));
	
	if(li != NULL) 
	{
		li->item = NULL;
		li->nb_elements = 0;
	}
	return li;
}


liste_item* ajout_item(liste_item* liste, occ_item* i)
{
	elem_item* nouveau = creer_element_item(i);

	/* Ajoute un item en début de liste */
	if(liste->item != NULL)
		nouveau->suivant = liste->item;

	liste->item = nouveau;
	liste->nb_elements++;


	return liste;
}
liste_item* supprime_item(liste_item* liste, occ_item *i){

	elem_item *actuel = liste->item;
	elem_item *precedent = NULL;

	/* Parcours de la liste des items */
	while(actuel->occ_item != i
		&& actuel->suivant != NULL)
	{
		precedent = actuel;
		actuel = actuel->suivant;
	}

	/* Si l'item a été trouvé */
	if(actuel->occ_item == i)
	{
		/* Si c'était le premier élément de la liste */
		if(precedent == NULL)
			liste->item = actuel->suivant;
		else
			precedent->suivant = actuel->suivant;

		free(actuel->occ_item);
		free(actuel);
	}
	
	liste->nb_elements--;

	if(liste->item == NULL)
		return NULL;

	return liste;
}

item* charger_item(char* nom, int type_item)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	item* i = new_item(nom);
	FILE * item_file;
	int nb = 0;
	float nb1 = 0;

	// Chargement de la texture et initiailisation de la taille
	strcpy(nom_texture, "textures/items/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	i->texture = charger_texture_bis(nom_texture, &i->taille);

	// Chargement des infos
	strcpy(nom_texture, "textures/items/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".item");

	item_file = fopen(nom_texture, "r");

	i->nom = type_item;

	fscanf(item_file, "vitesse_x : %f\n", &nb1);
	i->vitesse.x = nb1;

	fscanf(item_file, "vitesse_y : %f\n", &nb1);
	i->vitesse.y = nb1;

	fscanf(item_file, "soumission : %x\n", &nb);
	i->soumission = nb;

	fscanf(item_file, "nb_sprites : %d\n", &nb);
	i->nb_sprites = nb;

	i->taille.x = i->taille.x / i->nb_sprites;

	
	fclose(item_file);
	return i;
}