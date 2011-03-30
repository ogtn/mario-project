/*============================================================================*/
/*== [texture.c] [Version: 0.028a]                                          ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/


#include "texture.h"


GLuint charger_texture_bis(char *nom, coordi *taille)
{
	GLuint texture_ogl;
	SDL_Surface * texture_sdl;
    glEnd();
    
	/* creation de la texture OpenGl */
	glGenTextures(1, &texture_ogl);
	glBindTexture(GL_TEXTURE_2D, texture_ogl);

	/* Parametrage */
	/*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texture_sdl = IMG_Load(nom);

	if(texture_sdl == NULL)
	{
        printf("Echec du chargement de texture: %s\n", SDL_GetError());
		return 1;
	}

	/* Passage de SDL à OpenGL */
	if(texture_sdl->format->BytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_sdl->w, texture_sdl->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_sdl->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_sdl->w, texture_sdl->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_sdl->pixels);

	/* Si besoin/possible on donne la taille */
	if(taille != NULL)
	{
		taille->x = texture_sdl->w;
		taille->y = texture_sdl->h;
	}

	SDL_FreeSurface(texture_sdl);
    glBegin(GL_QUADS);

    return texture_ogl;
}


void charger_infos_texture(texture *t)
{
	int nb_sprites;
	char nom[TAILLE_NOM_TEXTURE];
	FILE *flux = NULL;
	int i;

	strcpy(nom, t->nom);
	supprime_extension(nom);
	strcat(nom, "txtr2");

	flux = fopen(nom, "r");

    if(flux == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom);
        return;
    }

	fscanf(flux, "%d %d %d %d",&(t->taille.x), &(t->taille.y), &(t->taille_sprite.x), &(t->taille_sprite.y));
	nb_sprites = (t->taille.x / t->taille_sprite.x) * (t->taille.y / t->taille_sprite.y);
	t->phys = malloc(sizeof(int) * nb_sprites);

	if(t->phys != NULL)
	{
		for(i = 0; i < nb_sprites; i++)
		{
			fscanf(flux, "%x", &t->phys[i]);
		}
	}
	fclose(flux);
}



void charger_cfg_texture(char* nom_cfg, texture *t, coordi taille_blocs)
{
	int nb_sprites;
	FILE *flux = NULL;
	int i, nb_blocs_x, nb_blocs_y;

	flux = fopen(nom_cfg, "r");

    if(flux == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom_cfg);
		return;
    }
    
	fscanf(flux, "%d %d",&nb_blocs_y, &nb_blocs_x);
	nb_sprites = nb_blocs_x * nb_blocs_y;

	if(nb_sprites == 1 && t->taille.x > taille_blocs.x)
	{
		t->taille_sprite = taille_blocs;
	}
	else
	{
		t->taille_sprite.x = t->taille.x / nb_blocs_x;
		t->taille_sprite.y = t->taille.y / nb_blocs_y;
	}
    t->phys = malloc(sizeof(int) * nb_sprites);

	if(t->phys != NULL)
	{
		for(i = 0; i < nb_sprites; i++)
        {
			fscanf(flux, "%x", &t->phys[i]);
        }
	}
	fclose(flux);
}

void charger_texture_bloc(char* nom, texture* t, coordi taille_blocs)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	char nom_cfg[TAILLE_NOM_TEXTURE];
	char* cfg;

	/* Chargement de l'image */
	strcpy(nom_texture, "textures/blocs/");
	strcat(nom_texture, nom);
	strcpy(t->nom, nom);
	t->id_text = charger_texture_bis(nom_texture, &t->taille);

	/* Chargement de la config */
	strcpy(nom_cfg, "textures/blocs/");
	cfg = strstr(nom, "/") + 1;

	while(strstr(cfg, "/"))
	{
		cfg = strstr(cfg, "/") + 1;
	}

	strcpy(cfg - 1,"\0");
	strcat(nom_cfg, nom);
	strcat(nom_cfg, "/");
	cfg = strstr(nom, "/");

	if(cfg != NULL)
	{
		cfg += 1;
		while(strstr(cfg, "/"))
		{
			cfg = strstr(cfg, "/") + 1;
		}
		strcat(nom_cfg, cfg);
	}
	else
	{
		strcat(nom_cfg, nom);
	}

	strcat(nom_cfg, ".cfg");
	charger_cfg_texture(nom_cfg, t, taille_blocs);
}

char *supprime_extension(char *nom)
{
	int i, extension = 0;

	for(i = 0; i < TAILLE_NOM_TEXTURE; i++)
	{
		if(extension == 1)
		{
			nom[i] = '\0';
		}

		if(nom[i] == '.')
		{
			extension = 1;
		}
	}

	return nom;
}

coordi calcul_nouvelle_taille(SDL_Surface *t)
{
	coordi taille;
	int i = 0;

	/* calcul de la puissance de 2 strictement supérieure
	à la la largeur de la texture */
	while(pow(2, i) < t->w)
		i++;

	taille.x = (int)pow(2, i);

	/* On réinitialise le compteur */
	i = 0;

	/* Et on recommence avec la hauteur */
	while(pow(2, i) < t->h)
		i++;

	taille.y = (int)pow(2, i);
	
	return taille;
}


/******** Gestion des listes chainées ************/
liste_texture *new_liste_texture()
{
    liste_texture *liste = malloc(sizeof(liste_texture));
    liste->debut = NULL;
    liste->nb = 0;

    return liste;
}


void free_liste_texture(liste_texture **liste)
{
    if(liste != NULL && *liste != NULL)
    {
        elt_liste_texture *actuel = (*liste)->debut;
        elt_liste_texture *precedent = NULL;

        while(actuel != NULL)
        {
            precedent = actuel;
            actuel = actuel->suivant;
            free_elt_liste_texture(&actuel);
        }

        free(*liste);
        *liste = NULL;
    }
}


elt_liste_texture *new_elt_liste_texture()
{
    elt_liste_texture *elt = malloc(sizeof(elt_liste_texture));
    elt->suivant = NULL;
    
    return elt;
}


void free_elt_liste_texture(elt_liste_texture **elt)
{
    if(elt != NULL && *elt != NULL)
    {
        free(*elt);
        *elt = NULL;
    }
}


void ajouter_text_liste(liste_texture *liste, texture elt)
{
    if(liste != NULL)
    {
        elt_liste_texture *actuel = liste->debut;
        elt_liste_texture *precedent = NULL;

        while(actuel != NULL)
        {
            if(strcmp(actuel->elt.nom, elt.nom) == 0)
            {
                return;
            }

            precedent = actuel;
            actuel = actuel->suivant;
        }

        precedent->suivant = new_elt_liste_texture();
        precedent->elt = elt;
        liste->nb++;
    }
}


void supprimer_text_liste(liste_texture *liste, texture elt)
{
    if(liste != NULL)
    {
        elt_liste_texture *actuel = liste->debut;
        elt_liste_texture *precedent = NULL;

        while(actuel != NULL)
        {
            if(strcmp(actuel->elt.nom, elt.nom) == 0)
            {
                if(actuel == liste->debut)
                {
                    liste->debut = actuel->suivant;
                }
                else
                {
                    precedent->suivant = actuel->suivant;
                }

                free_elt_liste_texture(&actuel);
                liste->nb--;
            }

            precedent = actuel;
            actuel = actuel->suivant;
        }
    }
}


void text_liste_a_tableau(liste_texture *liste, int *taille, texture **tab)
{
    if(liste != NULL)
    {       
        int i;
        elt_liste_texture *actuel = liste->debut;

        if(tab != NULL)
        {
            free(tab);
        }

        *tab = malloc(sizeof(texture) * liste->nb);
        *taille = liste->nb;

        for(i = 0; i < liste->nb; i++)
        {
            *tab[i] = actuel->elt;
            actuel = actuel->suivant;
        }
    }
}
