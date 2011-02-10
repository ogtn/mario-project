/*============================================================================*/
/*== [checkpoint.c] [Version: 0.028a]                                       ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "checkpoint.h"

checkpoint* new_checkpoint()
{
	return	init_checkpoint(malloc(sizeof(checkpoint)));
}

checkpoint* init_checkpoint(checkpoint* check)
{
	if(check != NULL)
	{
		check->position.x = 0;
		check->position.y = 0;
		check->taille.x = 0;
		check->taille.y = 0;
		check->nb_sprites_actif = 0;
		check->nb_sprites_inactif = 0;
		check->v_anim = 0;
		check->id_text = 0;
		check->etat = NOT_PASSED;
	}
	return check;
}

checkpoint* charger_checkpoint(char* nom)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	FILE* check_file;
	int nb_1;
	checkpoint* check = new_checkpoint();

	/* Chargement des infos */
	strcpy(nom_texture, "textures/objects/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".obj");

	check_file = fopen(nom_texture, "r");

	if(check_file == NULL)
	{
		printf("Le fichier %s est introuvable.\n", nom_texture);
		return NULL;
	}

	fscanf(check_file, "nb_sprites_actif : %d\n", &nb_1);
	check->nb_sprites_actif = nb_1;

	fscanf(check_file, "nb_sprites_inactif : %d\n", &nb_1);
	check->nb_sprites_inactif = nb_1;


	if(max(check->nb_sprites_actif, check->nb_sprites_inactif) > 1)
	{
		fscanf(check_file, "v_anim : %d\n", &nb_1);
		check->v_anim = nb_1;
	}
	else
	{
		check->v_anim = 0;
	}

	fclose(check_file);

	/* Chargement des infos */
	strcpy(nom_texture, "textures/objects/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	check->id_text = charger_texture_bis(nom_texture, &check->taille);

	/* Calcul des infos */
	check->taille.x = check->taille.x / max(check->nb_sprites_actif, check->nb_sprites_inactif);
	check->taille.y /= 2;
	strcpy(check->nom_text, nom);

	return check;
}