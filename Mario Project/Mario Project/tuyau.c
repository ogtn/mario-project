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

tuyau* new_tuyau(SENS_SORTIE sens, char* nom)
{
	tuyau* t = malloc(sizeof(tuyau));

	return init_tuyau(t, sens, nom);
}

tuyau* init_tuyau(tuyau* t, SENS_SORTIE sens, char* nom) {

	if( t != NULL) {
		t->texture = 0;

		t->longueur = 0;
		t->sens_sortie = sens;
		t->etat = FERME;
		t->monstre = NULL;
		t->pipe_dest = 0;
		t->level_dest = NULL;
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


tuyau* charger_tuyau(char* nom, SENS_SORTIE sens)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	tuyau *t = new_tuyau(sens, nom);

	// Chargement de la texture et initiailisation de la taille
	strcpy(nom_texture, "textures/tuyaux/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	t->texture = charger_texture_bis(nom_texture, NULL);

	return t;
}

