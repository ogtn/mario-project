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

	w->niveau = new_niveau();

	w->persos = NULL;
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
	w->nb_niveaux = 1;

	/* Initialisation du perso */
	w->persos = malloc(sizeof(perso*) * w->nb_persos);
    w->persos[0] = new_perso();
	charger_perso("small_mario", w->persos[0]);
	transforme_perso(SMALL_MARIO, w->persos[0]);

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

void modifier_niveau_SMB(niveau* n)
{
	int i, j;
	
	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			if((j == 1))
			{
				// sol
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 5, -1, -1);
			}
			else if((j == 0))
			{
				// plein
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 4, -1, -1);
			}
		}
	}
}


void load_world(world *w)
{
	int i;

	FILE* wldFile = fopen("world1.wld", "r");
	if(wldFile == NULL)
	{
		printf("Le fichier est world1.wld introuvable !\n");
	}

	fscanf(wldFile, "nb_niveaux : %d", &w->nb_niveaux);
	w->liste_niveaux = malloc(sizeof(char*) * w->nb_niveaux);

	/* Enregistrement de tous les niveaux compris dans le monde */
	for(i = 0; i < w->nb_niveaux; i++)
	{
		w->liste_niveaux[i] = malloc(sizeof(char) * TAILLE_NOM_NIVEAU);
		fscanf(wldFile, "%s", w->liste_niveaux[i]);
	}

    w->niveau = new_niveau();
    charger_niveau(w->liste_niveaux[w->num_niveau], w->niveau);
	modifier_niveau_SMB(w->niveau);
	//sauver_niveau("smb4.xml", w->niveau);

}

void begin_level(world *w, int *persos_tous_morts, int* continuer)
{
	int i;

	/* Désignation du niveau de départ ou recharger le niveau si tu meurs */
	if(w->num_niveau > 0 || (*persos_tous_morts))
    {
        w->niveau = new_niveau();
        charger_niveau(w->liste_niveaux[w->num_niveau], w->niveau);
		*persos_tous_morts = FAUX;
    }

	/* Présentation du niveau */
	presentation_niveau(w, continuer, *persos_tous_morts);

	/* initialisations (temporaires?) de l'ecran */
    w->ecran.taille.x = LARGEUR_FENETRE;
    w->ecran.taille.y = HAUTEUR_FENETRE;
    w->ecran.origine.x = 0;
    w->ecran.origine.y = 0;

	/* Initialisation de chaque position des persos */
	for(i = 0; i < w->nb_persos; i++)
	{
		/* Initialisation de la position des persos en fonction de s'il a passé un checkpoint ou non */
		if(persos_tous_morts && w->persos[i]->checkpoint >= 0)
		{
			w->persos[i]->position.x = (float)(w->niveau->checkpoints[w->persos[i]->checkpoint]->position.x + (i + 3) * LARGEUR_BLOC);
			w->persos[i]->position.y = (float)(w->niveau->checkpoints[w->persos[i]->checkpoint]->position.y);

			/* Activation du checkpoint passé auparavant */
			w->niveau->checkpoints[w->persos[i]->checkpoint]->etat = PASSED;
		}
		else
		{
			w->persos[i]->position.x = (float)w->niveau->spawn.x + i * LARGEUR_BLOC;
			w->persos[i]->position.y = (float)w->niveau->spawn.y;
		}

		w->persos[i]->position_prec = w->persos[i]->position;
	}

	/* Lancement de la musique */
	FSOUND_Stream_Play(1, w->niveau->musique); 
}

void presentation_niveau(world *w, int *continuer, int persos_tous_morts)
{
	int i;
	vecti pos_text_vie, pos_text_nom_niveau;
	w->ecran.scroll.x = w->ecran.scroll.y = 0;

	while(!w->keystate->actuel[ENTRER] && !w->keystate->actuel[ECHAP])
	{
		glEnd();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_QUADS);

		maj_keystate(w->keystate, continuer);		

		/* Affiche l'aperçu du niveau */
		w->ecran.origine.x = LARGEUR_FENETRE / 4;
		w->ecran.origine.y = HAUTEUR_FENETRE / 4;

		w->ecran.taille.x = LARGEUR_FENETRE / 2;
		w->ecran.taille.y = HAUTEUR_FENETRE / 2;

		draw_main_options(w->niveau, w->ecran, w->temps_actuel, 1, 1, 1);

		/* Affiche le cadre autour de l'aperçu */
		w->ecran.origine.x -= 5;
		w->ecran.origine.y -= 5;
		w->ecran.taille.x += 10;
		w->ecran.taille.y += 10;
		draw_cadre(w->ecran.origine, w->ecran.taille, charger_texture_bis("textures/cadre3.png", NULL), 5);

		/* Affiche le nom du niveau */
		pos_text_nom_niveau.x = w->ecran.origine.x;
		pos_text_nom_niveau.y =  w->ecran.origine.y - LARGEUR_BLOC;

		screen_printf(pos_text_nom_niveau, NULL, COLOR_WHITE, "%s", w->niveau->nom);

		/* Affiche chaque perso et leur nombre de vies restantes */
		for(i = 0; i < w->nb_persos; i++)
		{
			/* Si c'est le premier niveau ou si les persos sont tous morts, on remet tout le monde petit */
			if(w->num_niveau == 0 || persos_tous_morts)
			{
				transforme_perso(SMALL_MARIO, w->persos[i]);
			}

			/* Réinitialisation de ceraines valeurs utilisées pour chaque niveau */
			init_perso_niveau(w->persos[i]);

			/* Initialisation du nom et du temps dans le HUD pour chaque niveau chargé */
			w->persos[i]->hud->nom_niveau = w->niveau->nom;
			w->persos[i]->hud->time = 5000;

			w->persos[i]->position.x = LARGEUR_FENETRE / 4 + i * LARGEUR_BLOC - w->persos[i]->texture_act->abscisse_bas;
			w->persos[i]->position.y = HAUTEUR_FENETRE / 4 * 3 + LARGEUR_BLOC / 2;

			pos_text_vie.x = LARGEUR_FENETRE / 4 + (i + 1) * LARGEUR_BLOC;
			pos_text_vie.y = w->persos[i]->position.y;

			screen_printf(pos_text_vie, NULL, COLOR_WHITE, "x%d", w->persos[i]->hud->nb_vies);

			draw_perso(w->persos[i], 0);
		}

		screen_flush();
		SDL_GL_SwapBuffers();
		my_sleep(2);
	}
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
		if(w->persos[i]->etat != FINISH && w->persos[i]->etat != FINISH_CHATEAU && w->persos[i]->etat != MORT && w->persos[i]->hud->time > 0)
		{
			w->persos[i]->hud->time -= w->temps_ecoule / 10;
			if(w->persos[i]->hud->time < 0)
				w->persos[i]->hud->time = 0;
		}
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

void check_finish(world *w, int* gagne)
{
	int i;

	for(i = 0; i < w->nb_persos; i++)
	{
		if(w->persos[i]->etat == FINISH_CHATEAU && w->persos[i]->tps_finish == 0)
		{
			*gagne = 1;
			return;
		}
	}
}

int perso_transforme_ou_meurt(world* w)
{
	int i, res = 0;

	for(i = 0; i < w->nb_persos; i++)
	{
		if(w->persos[i]->tps_transformation || w->persos[i]->tps_mort)
		{
			res = 1;
			break;
		}
	}

	return res;
}

void persos_morts(world* w, int *persos_morts, int* continuer)
{
	int i, total_vies = 0;

	for(i = 0; i < w->nb_persos; i++)
	{
		*persos_morts = *persos_morts 
			|| (w->persos[i]->etat == MORT && w->persos[i]->position.y + w->persos[i]->texture_act->ordonnee_haut  < 0);
		total_vies += w->persos[i]->hud->nb_vies;
	}

	if(total_vies == 0)
		*continuer = 0;
}
