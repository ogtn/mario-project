/*============================================================================*/
/*== [menus.c] [Version: 0.028a]                                            ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "menus.h"


void initialiser_menu(menu *m)
{
	int i;

	for(i = 1; i < m->nb_choix; i++)
	{
		if(m->choix[i]->choix != NULL)
		{
			m->choix[i]->choix[0] = m;
			initialiser_menu(m->choix[i]);
		}
	}
}


menu *charger_menu(FILE *flux){
	menu* m;
	int i;

	m = malloc(sizeof(menu));

	for(i = 0; i < TAILLE_TITRE_MENUS; i++)
	{
		m->titre[i] = '\0';
	}

	fscanf(flux, "%s", m->titre);
	fscanf(flux, "%d", &m->nb_choix);
	fscanf(flux, "%d", &m->valeur);


	m->choix = malloc(sizeof(menu**) * (m->nb_choix + 1));

	m->choix[0] = NULL;
	for(i = 1; i <= m->nb_choix; i++){
		m->choix[i] = charger_menu(flux);
	}

	initialiser_menu(m);
	return m;
}


void free_menu(menu *m)
{
	int i;

	for(i = 1; i <= m->nb_choix; i++)
		free_menu(m->choix[i]);

	free(m->choix);
	free(m);
}


void afficher_menu(menu *m, int surligne){
	int i;
	unsigned int j;
	coordi pos;
	size_t taille;
	GLuint texture = 6;

	/* Centrage des boutons */
	taille = strlen(m->titre);
	pos.x = LARGEUR_FENETRE / 2 - (((int)taille + 2) * 4);
	pos.y = HAUTEUR_FENETRE / 2;

	/* Dessin du titre du menu */
	draw_sprite(pos.x - 8, pos.y - 5, 16, 26, texture, 0, (float)1/3, 0, 1);
	for(j = 1; j < taille; j++)
	{
		draw_sprite(pos.x + j * 8, pos.y -5, 16, 26, texture, (float)1/3, (float)2/3, 0, 1); 
	}
	draw_sprite(pos.x + (int)taille * 8, pos.y - 5, 16, 26, texture, (float)2/3, 1, 0, 1); 
	
	draw_text(m->titre, pos, 1, 1, 1);

	/* Dessin des titres des sous-menus */
	for(i = 1; i <= m->nb_choix; i++){
		pos.y -= 30;

		if(i == surligne)
			texture = 5;
		else
			texture = 4;

		taille = strlen(m->choix[i]->titre);
		pos.x = LARGEUR_FENETRE / 2 - (((int)taille + 2) * 4);

		draw_sprite(pos.x - 8, pos.y - 5, 16, 26, texture, 0, (float)1/3, 0, 1);
		for(j = 1; j < taille; j++)
		{
			draw_sprite(pos.x + j * 8, pos.y - 5, 16, 26, texture, (float)1/3, (float)2/3, 0, 1); 
		}
		draw_sprite(pos.x + (int)taille * 8, pos.y - 5, 16, 26, texture, (float)2/3, 1, 0, 1); 

		draw_text(m->choix[i]->titre, pos, 1, 1, 1);
	}
}


menu *sous_menu(menu *m, int choix)
{
	menu *res = NULL;

	if(choix <= 0 || choix > m->nb_choix)
		res = m;
	else if(choix == m->nb_choix)
		res = menu_parent(m);
	else
		res = m->choix[choix];

	return res;
}


menu *menu_parent(menu *m)
{
	if(m->choix[0] != NULL)
		return m->choix[0];
	else
		return m;
}


int menu_feuille(menu *m, int choix)
{
	if(m->choix[choix]->nb_choix == 0)
		return m->choix[choix]->valeur;
	else
		return -1;
}


/* coder une fonction qui prend le menu actuel et les coordonées de la souris et un
pointeur sur choix. La fonction modifie choix si la souris est sur un menu, et renvoie 1.
Sinon, elle renvoie 0 
Permet de mettre à jour choix et de savoir si la souris est sur un choix */

int main_menu(keystate *k)
{
	int continuer = 1;
	int choix = 1;
	int mx = 0, my = 0;
	menu *m;
	FILE *flux_menu = NULL;
	FSOUND_STREAM *zik;

	/* On cache le curseur, ni vu ni connu j'tembrouille */
	SDL_ShowCursor(0);

	k->precedent[ECHAP] = 1;
	k->precedent[QUITTER] = 1;

	flux_menu = fopen("main.menu", "r");
	m = charger_menu(flux_menu);
	fclose(flux_menu);

	zik = charger_musique("musics/space_mario.wav", 100, -1);
	//FSOUND_Stream_Play(FSOUND_FREE, zik);
	while(continuer)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		/* Dessin du fond, du menu et recuperation de la position du pointeur souris
		et de l'etat des touches */
		draw_sprite(0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 3, 0, 1, 0, 1);
		SDL_GetMouseState(&mx, &my);
		my = HAUTEUR_FENETRE - my;
		maj_keystate(k, &continuer);
		afficher_menu(m, choix);

		/* L'utilisateur clique sur la croix ou presse echap */
		if(continuer == 0)
		{
			//FSOUND_Stream_Stop(zik);
			//FSOUND_Stream_Close(zik);
			return QUITTER;
		}

		/* Feuille de l'arbre */
		else if(m->nb_choix == 0)
		{
			//FSOUND_Stream_Stop(zik);
			//FSOUND_Stream_Close(zik);
			return m->valeur;
		}

		else
		{
			if((k->actuel[ENTRER] && k->precedent[ENTRER] == 0) || 
				(k->actuel[DROITE] && k->precedent[DROITE] == 0))
			{
				m = sous_menu(m, choix);
				choix = 1;
			}

			else if(k->actuel[GAUCHE] && k->precedent[GAUCHE] == 0)
			{
				m = menu_parent(m);
				choix = 1;
			}

			else if(k->actuel[BAS] && k->precedent[BAS] == 0)
				choix++;

			else if(k->actuel[HAUT] && k->precedent[HAUT] == 0)
				choix--;

			else if(k->actuel[CLIC_G] && k->precedent[CLIC_G] == 0)
			{
				/*************** si la souris est dessu ******************/
				m = sous_menu(m, choix);
				choix = 1;
			}

			if(choix < 1)
				choix = m->nb_choix;
			else if(choix > m->nb_choix)
				choix = 1;
		}

		pause(1);
		SDL_GL_SwapBuffers();
	}

	//FSOUND_Stream_Stop(zik);
	//FSOUND_Stream_Close(zik);

	return m->valeur;
}
