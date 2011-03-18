/*============================================================================*/
/*== [tuyau.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "tuyau.h"

tuyau* new_tuyau(char* nom, SENS_SORTIE sens, int longueur, int position_x, int position_y, ETAT_TUYAU etat, int pipe_dest, char* level_dest, int index_monstre)
{
	tuyau* t = malloc(sizeof(tuyau));

	return init_tuyau(t, nom, sens, longueur, position_x, position_y, etat, pipe_dest, level_dest, index_monstre);
}

tuyau* init_tuyau(tuyau* t, char* nom, SENS_SORTIE sens, int longueur, int position_x, int position_y, ETAT_TUYAU etat, int pipe_dest, char* level_dest, int index_monstre) {

	if( t != NULL)
	{
		t->texture = 0;
		t->position.x = position_x;
		t->position.y = position_y;

		t->longueur = longueur;
		t->sens_sortie = sens;
		t->etat = etat;
		t->index_monstre = index_monstre;
		t->pipe_dest = pipe_dest;
		strcpy(t->level_dest, level_dest);
		t->tps_sortie_monstre = TPS_SORTIE_MONSTRE;

		strcpy(t->nom_text, nom);
	}
	return t;
}

tuyau* free_tuyau(tuyau* t)
{
	if(t != NULL)
	{
		free(t);
	}

	return NULL;
}


tuyau* charger_tuyau(char* nom, SENS_SORTIE sens, int longueur, int position_x, int position_y, ETAT_TUYAU etat, int pipe_dest, char* level_dest, int index_monstre)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	tuyau *t = new_tuyau(nom, sens, longueur, position_x, position_y, etat, pipe_dest, level_dest, index_monstre);

	// Chargement de la texture et initiailisation de la taille
	strcpy(nom_texture, "textures/tuyaux/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	t->texture = charger_texture_bis(nom_texture, NULL);

	return t;
}

