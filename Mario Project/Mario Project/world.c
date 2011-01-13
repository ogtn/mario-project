/*============================================================================*/
/*== [world.c] [Version: 0.028a]                                            ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "world.h"

world *new_world(void)
{
	return	init_world(malloc(sizeof(world)));
}


world *init_world(world *w)
{
	if(w == NULL)
	{
		return w;
	}

	w->nb_persos = 0;
	w->num_niveau = 0;
	w->nb_niveaux = 0;

	w->persos = NULL;
	w->niveau = new_niveau();
	w->liste_niveaux = NULL;
	w->keystate = new_keystate();
	w->ecran.scroll.x = 0;
	w->ecran.scroll.y = 0;
    w->taille_fenetre.x = LARGEUR_FENETRE;
	w->taille_fenetre.y = HAUTEUR_FENETRE;

	w->temps_prec = SDL_GetTicks();
	w->temps_actuel = w->temps_prec;
	w->temps_ecoule = 0;
	w->temps_fps = 0;
	w->fps = 0;
	w->frames_calculees = 0;

	/* ====initialisations temporaires pour tester==== */ 
	/* cela concerne tout ce qui suit jusqu'au return */

	w->nb_persos = 1;
	w->num_niveau = 1;
	w->nb_niveaux = 1;

	/* Initialisation du perso */
	w->persos = malloc(sizeof(perso*) * w->nb_persos);
    w->persos[0] = new_perso();
	charger_perso("small_mario", w->persos[0]);
	transforme_perso(SMALL_MARIO, w->persos[0]);

	w->liste_niveaux = malloc(sizeof(char*));
	w->liste_niveaux[0] = malloc(TAILLE_NOM_NIVEAU);
	strcpy(w->liste_niveaux[0], "niveau1.lvl");

	return w;
}


world *free_world(world *w)
{
	int i;

	if(w != NULL)
	{
		/* Libération du niveau */
		w->niveau = free_niveau(w->niveau);
		
		/* Libération du tableau des touches */
		w->keystate = free_keystate(w->keystate);

		/* Libération des persos */
		if(w->persos != NULL)
		{
			for(i = 0; i < w->nb_persos; i++)
			{
				if(w->persos[i] != NULL)
					free_perso(w->persos[i]);
			}

			free(w->persos);
			w->persos = NULL;
		}

		/* Libération de la liste des niveaux */
		if(w->liste_niveaux != NULL)
		{
			for(i = 0; i < w->nb_niveaux; i++)
			{
				if(w->liste_niveaux[i] != NULL)
				{
					free(w->liste_niveaux[i]);
					w->liste_niveaux[i] = NULL;
				}
			}

			free(w->liste_niveaux);
			w->liste_niveaux = NULL;
		}

		/* free sound */
		/* free ecran */
	}

	free(w);

	return NULL;
}


void load_world(world *w)
{
	/* Chargement du niveau */
    //charger_niveau_test_vide(w->niveau);
	charger_niveau_test(w->niveau);
	//affiche_occ_blocs(w->niveau);
	//charger_niveau("test_xml.xml", w->niveau);
	charger_textures_niveau(w->niveau);
	
	/* Initialisation du nom et du temps dans le HUD pour chaque niveau chargé */
	w->persos[0]->hud->nom_niveau = w->niveau->nom;
	w->persos[0]->hud->time = 4000;

	sauver_niveau("test_xml.xml", w->niveau);
}


void update_time(world *w)
{
	int i;

	w->temps_prec = w->temps_actuel;
	w->temps_actuel = SDL_GetTicks();
	w->temps_ecoule_prec = w->temps_ecoule;
	w->temps_ecoule = (w->temps_actuel - w->temps_prec);

	/* Calcul des fps */
	if(w->temps_actuel - w->temps_fps >= 250)
	{
		w->temps_fps = w->temps_actuel;
		w->fps = w->frames_calculees * 4;
		w->frames_calculees = 0;
	}

	/* Si la différence de temps entre deux images est supérieure à 0.5s
		alors on remet le temps précédent en temps actuel
		pour qu'il n'y ait pas de collisions (il n'y a pas eu de déplacement) */
	if(w->temps_ecoule > TPS_PROTEC_COLLISIONS)
	{
        w->temps_prec = SDL_GetTicks();
	    w->temps_actuel = w->temps_prec;
	    w->temps_ecoule = 0;
	}

	for(i = 0; i < w->nb_persos; i++)
	{
		w->persos[i]->hud->time -= w->temps_ecoule / 10;
	}
	

	w->frames_calculees++;
}


void update_screen(world *w)
{
	/* cas général */
	/* horizontal */
	if((w->persos[0]->position.x - w->ecran.scroll.x) > w->ecran.taille.x * SCROLLING_AV)
		w->ecran.scroll.x = (int)w->persos[0]->position.x - (int)(w->ecran.taille.x * SCROLLING_AV);

	else if((w->persos[0]->position.x - w->ecran.scroll.x) < w->ecran.taille.x * SCROLLING_AR)
		w->ecran.scroll.x = (int)w->persos[0]->position.x - (int)(w->ecran.taille.x * SCROLLING_AR);

	/* vertical */
	if((w->persos[0]->position.y - w->ecran.scroll.y) > w->ecran.taille.y * SCROLLING_HAUT)
		w->ecran.scroll.y = (int)w->persos[0]->position.y - (int)(w->ecran.taille.y * SCROLLING_HAUT);

	else if((w->persos[0]->position.y - w->ecran.scroll.y) < w->ecran.taille.y * SCROLLING_BAS)
		w->ecran.scroll.y = (int)w->persos[0]->position.y - (int)(w->ecran.taille.y * SCROLLING_BAS);

	/* cas particuliers des extremitées du niveau et des niveaux plus petits que l'ecran */
	/* horizontal */
	if(w->ecran.scroll.x > (w->niveau->taille.x * w->niveau->taille_blocs.x - w->ecran.taille.x))
		w->ecran.scroll.x = w->niveau->taille.x * w->niveau->taille_blocs.x - w->ecran.taille.x;
	if(w->ecran.scroll.x < 0)
		w->ecran.scroll.x = 0;

	/* vertical */
	if(w->ecran.scroll.y > (w->niveau->taille.y * w->niveau->taille_blocs.y - w->ecran.taille.y))
		w->ecran.scroll.y = w->niveau->taille.y * w->niveau->taille_blocs.y - w->ecran.taille.y;
	if(w->ecran.scroll.y < 0)
		w->ecran.scroll.y = 0;
}


void update_taille_fenetre(world *w)
{
	SDL_Event event;

	SDL_PollEvent(&event);

	if(event.type == SDL_VIDEORESIZE)
	{
		w->taille_fenetre.x = event.resize.w;
		w->taille_fenetre.y = event.resize.h;

		init_SDL(w->taille_fenetre.x, w->taille_fenetre.y);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
        glOrtho(0, w->taille_fenetre.x, 0, w->taille_fenetre.y, -1000, 1000);
		glMatrixMode(GL_MODELVIEW);
	}
}

void check_lives_finish(world *w, int* continuer)
{
	int i, sont_vivants = 0;

	for(i = 0; i < w->nb_persos; i++)
	{
		sont_vivants |= (w->persos[i]->hud->nb_vies != 0);
		if(w->persos[i]->etat == FINISH_CHATEAU && w->persos[i]->tps_finish == 0)
		{
			*continuer = 0;
			return;
		}
	}

	if(!sont_vivants)
		*continuer = 0;
}

int perso_mort_ou_transforme(world* w)
{
	int i, res = 0;

	for(i = 0; i < w->nb_persos; i++)
	{
		if(!w->persos[i]->tps_transformation && !w->persos[i]->tps_mort)
		{
			res = 1;
			break;
		}
	}

	return res;
}
