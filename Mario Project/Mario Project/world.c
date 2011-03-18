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
			if(((i >= 130 && i <= 133) || (i >= 136 && i <= 139) || (i >= 144 && i <= 148) || (i >= 151 && i <= 154) || (i >= 177 && i <= 181)) && j == 2)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 20, -1, -1); // incassable
			}
			else if(((i >= 131 && i <= 133) || (i >= 136 && i <= 138) || (i >= 145 && i <= 148) || (i >= 151 && i <= 153) || (i >= 178 && i <= 181)) && j == 3)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 20, -1, -1); // incassable
			}
			else if(((i >= 132 && i <= 133) || (i >= 136 && i <= 137) || (i >= 146 && i <= 148) || (i >= 151 && i <= 152) || (i >= 179 && i <= 181)) && j == 4)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 20, -1, -1); // incassable
			}
			else if(((i == 133) || (i == 136) || (i >= 147 && i <= 148) || i == 151 || (i >= 180 && i <= 181)) && j == 5)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 20, -1, -1); // incassable
			}
			else if ((i == 63 || i == 81) && j == 1)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 4, -1, -1); // coin haut à droite
			}
			else if ((i == 63 || i == 81) && j == 0)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 2, -1, -1); // mur à gauche
			}
			else if ((i == 66 || i == 85) && j == 1)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 5, -1, -1); // coin haut à gauche
			}
			else if ((i == 66 || i == 85) && j == 0)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 3, -1, -1); // mur à droite
			}
			else if((i < 63 || (i >= 66 && i <= 80) || (i >= 86 && i <= 148) || (i >= 151 && i <= 200)) && j == 0)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 8, -1, -1); // bloc plein
			}
			else if((i < 63 || (i >= 66 && i <= 80) || (i >= 86 && i <= 148) || (i >= 151 && i <= 200)) && j == 1)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 0, -1, -1); // sol
			}
			else if ((i == 14 || i == 70) && j == 5)
			{
				// ? avec item en fonction de transformation actuelle
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 20, -1);
			}
			else if (i == 106 && j == 9)
			{
				// ? avec item en fonction de transformation actuelle
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 20, -1);
			}
			else if ((i == 10 || i == 14 || i == 16 || i == 103 || i == 106 || i == 109 || i == 166) && j == 5)
			{
				// ? avec piece
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 20, 0);
			}
			else if ((i == 15 || i == 90 || i == 126 || i == 127) && j == 9)
			{
				// ? avec piece
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 20, 0);
			}
			else if ((i == 13 || i == 15 || i == 17 || i == 69 || i == 71 || i == 97 || i == 115 || i == 126 || i == 127 || i == 164 || i == 165 || i == 167) && j == 5)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 18, -1, -1);// bloc cassable
			}
			else if (i == 90 && j == 5)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 19, 20, 0);// bloc cassable  distributeur de piece
			}
			else if (i == 98 && j == 5)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 20, 3); // bloc avec étoile
			}
			else if (((i >= 72 && i <= 83) || (i >= 87 && i <= 89) || (i >= 118 && i <= 120) || i == 125 || i == 128) && j == 9)
			{
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 18, -1, -1); // bloc cassable
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
	//sauver_niveau("smb2.xml", w->niveau);

}




void begin_level(world *w, int *persos_tous_morts)
{
	int i;

	/* Désignation du niveau de départ ou recharger le niveau si tu meurs */
	if(w->num_niveau > 0 || (*persos_tous_morts))
    {
        w->niveau = new_niveau();
        charger_niveau(w->liste_niveaux[w->num_niveau], w->niveau);
		*persos_tous_morts = FAUX;
    }

	for(i = 0; i < w->nb_persos; i++)
	{
		transforme_perso(SMALL_MARIO, w->persos[i]);

		/* Initialisation de la position des persos */
		if(persos_tous_morts && w->persos[i]->checkpoint >= 0)
		{
			w->persos[i]->position.x = (float)(w->niveau->checkpoints[w->persos[i]->checkpoint]->position.x + 3 * w->niveau->taille_blocs.x);
			w->persos[i]->position.y = (float)(w->niveau->checkpoints[w->persos[i]->checkpoint]->position.y);
		}
		else
		{
			w->persos[i]->position.x = (float)w->niveau->spawn.x;
			w->persos[i]->position.y = (float)w->niveau->spawn.y;
		}

		w->persos[i]->position_prec = w->persos[i]->position;
		w->persos[i]->etat = DEBOUT;

		/* Initialisation du nom et du temps dans le HUD pour chaque niveau chargé */
		w->persos[i]->hud->nom_niveau = w->niveau->nom;
		w->persos[i]->hud->time = 3000;
	}
	
	/* Lancement de la musique */
	FSOUND_Stream_Play(1, w->niveau->musique); 
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
		if(w->persos[i]->etat != FINISH && w->persos[i]->etat != FINISH_CHATEAU && w->persos[i]->etat != MORT)
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

void persos_morts(world* w, int *persos_morts)
{
	int i;

	for(i = 0; i < w->nb_persos; i++)
	{
		*persos_morts = *persos_morts || (w->persos[i]->etat == MORT && w->persos[i]->position.y < 0);
	}
}
