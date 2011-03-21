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
		/* Lib�ration du niveau */
		w->niveau = free_niveau(w->niveau);
		
		/* Lib�ration du tableau des touches */
		w->keystate = free_keystate(w->keystate);

		/* Lib�ration des persos */
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

		/* Lib�ration de la liste des niveaux */
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
			if(((i == 35 || i == 41) && j == 12)
				|| (i == 168 && j == 5))
			{
				// bord � droite
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 12, -1, -1);
			}
			else if(((i == 39) && j == 12)
				|| (i == 47 && j == 0))
			{
				// bord � gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 10, -1, -1);
			}
			else if((i == 35 || i == 41) && j <= 11)
			{
				// mur � droite passable
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 11, -1, -1);
			}
			else if((i == 39 && j <= 11)
				|| (i == 168 && j <= 4))
			{
				// mur � gauche passable
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 9, -1, -1);
			}
			else if((i == 20 && j == 10)
				|| (i == 58 && j == 0))
			{
				// rattachement sol/mur haut gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 13, -1, -1); 
			}
			else if(i == 58 && j == 3)
			{
				// rattachement sol/mur bas gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 14, -1, -1); 
			}
			else if((i == 20 && j == 12)
				|| (i == 48 && j == 13)
				|| (i == 106 && j == 5)
				|| (i == 124 && j == 8))
			{
				// coin haut � gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 2, -1, -1);
			}
			else if((i == 8 && j == 39)
				|| (i == 76 && j == 13)
				|| (i == 134 && j == 8))
			{
				// coin haut � droite
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 8, -1, -1);
			}
			else if(i == 8 && j == 38)
			{
				// coin bas � droite
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 6, -1, -1);
			}
			else if(i == 48 && j == 3)
			{
				// coin bas � gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 0, -1, -1);
			}
			else if((i == 20 && j == 11)
				|| (i == 48 && j <= 12 && j >= 4)
				|| (i == 58 && j >= 1 && j <= 2)
				|| (i == 106 && j <= 4)
				|| (i == 124 && j <= 7 && j >= 6))
			{
				// mur � droite
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 1, -1, -1);
			}
			else if((i == 76 && j <= 13)
				|| (i == 134 && j <= 7 && j >= 6))
			{
				// mur � gauche
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 7, -1, -1);
			}
			else if((i < 35 && j <= 9)
				|| ((i >= 20 && i < 35) && j <= 11)
				|| (i == 40 && j <= 11)
				|| (i >= 49 && i <= 58 && j <= 12 && j >= 4)
				|| (i >= 59 && i <= 75 && j <= 12))
			{
				// plein
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 4, -1, -1);
			}
			else if((i >= 49 && i <= 57 && j == 3)
				|| (i >= 0 && i <= 7 && j == 38))
			{
				// plafond
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 3, -1, -1);
			}
			else if((i >= 48 && i <= 57 && j == 0)
				|| (i < 20 && j == 10)
				|| (((i >= 21 && i < 35) || i == 40) && j == 12)
				|| ((i >= 49 && i <= 75) && j == 13)
				|| (i >= 0 && i <= 7 && j == 39)
				|| (i >= 107 && j == 5)
				|| (i >= 125 && i <= 133 && j == 8))
			{
				// sol
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 5, -1, -1);
			}
			else if(i >= 5 && i <= 6 && j == 43)
			{
				// Bloc ? avec champi vie
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 18, 3); 
			}
			else if((i == 52 && j == 18)
				|| (i == 129 && j == 11))
			{
				// Bloc ? avec item en fonction de la transformation
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 18, -1); 
			}
			else if((i == 53 && j == 18)
				|| (i == 72 && j == 17)
				|| (i == 159 && j == 9)
				|| (i == 161 && j == 9)
				|| (i == 163 && j == 9))
			{
				// Bloc ? avec piece
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 17, 18, 0); 
			}
			else if((i == 69 && j == 17)
				|| (i == 127 && j == 11))
			{
				// Bloc distributeur de piece
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 16, 18, 0); 
			}
			else if((i == 60 && j >= 14 && j <= 15)
				|| (i >= 81 && i <= 84 && j == 15)
				|| (i >= 89 && i <= 91 && j == 12)
				|| (i >= 94 && i <= 95 && j == 13)
				|| (i == 99 && j == 11)
				|| (i == 103 && j == 9)
				|| (i == 150 && j == 6)
				|| (i == 151 && j <= 7 && j >= 6))
			{
				// Bloc incassable
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 18, -1, -1); 
			}
			else if((i == 128 && j == 11)
				|| (i == 130 && j == 11)
				|| (i == 158 && j == 9)
				|| (i == 160 && j == 9)
				|| (i == 162 && j == 9)
				|| (i == 164 && j == 9)
				|| (i == 159 && j == 12)
				|| (i == 161 && j == 12)
				|| (i == 163 && j == 12))
			{
				// Bloc cassable
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, 15, -1, -1); 
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
	//modifier_niveau_SMB(w->niveau);
	//sauver_niveau("smb.xml2", w->niveau);

}




void begin_level(world *w, int *persos_tous_morts)
{
	int i;

	/* D�signation du niveau de d�part ou recharger le niveau si tu meurs */
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

		/* Initialisation du nom et du temps dans le HUD pour chaque niveau charg� */
		w->persos[i]->hud->nom_niveau = w->niveau->nom;
		w->persos[i]->hud->time = 5000;
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

	/* Si la diff�rence de temps entre deux images est sup�rieure � 0.5s
		alors on remet le temps pr�c�dent en temps actuel
		pour qu'il n'y ait pas de collisions (il n'y a pas eu de d�placement) */
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
	/* cas g�n�ral */
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

	/* cas particuliers des extremit�es du niveau et des niveaux plus petits que l'ecran */
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
		*persos_morts = *persos_morts || (w->persos[i]->etat == MORT && w->persos[i]->position.y < 0);
		total_vies += w->persos[i]->hud->nb_vies;
	}

	if(total_vies == 0)
		*continuer = 0;
}
