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

	/* creation de la texture OpenGl */
	glGenTextures(1, &texture_ogl);
	glBindTexture(GL_TEXTURE_2D, texture_ogl);

	/* Parametrage */
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texture_sdl = IMG_Load(nom);

	if(texture_sdl == NULL)
	{
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

	return texture_ogl;
}


void charger_infos_texture(texture *t)
{
	NB nb_sprites;
	char nom[TAILLE_NOM_TEXTURE];
	FILE *flux = NULL;
	int i;

    perror("entree charger_infos_texture()");
    

	strcpy(nom, t->nom);
	supprime_extension(nom);
	strcat(nom, "txtr2");
    printf("nom = %s\n", nom);

    perror("fscanf()");
	flux = fopen(nom, "r");
    perror("fscanf()");
    
	if(flux == NULL || t ==  NULL)
	{
		/* gestion d'erreur? valeurs par defaut? à terminer... */
	}

    
	fscanf(flux, "%d %d %d %d",&(t->taille.x), &(t->taille.y), &(t->taille_sprite.x), &(t->taille_sprite.y));
	nb_sprites = (t->taille.x / t->taille_sprite.x) * (t->taille.y / t->taille_sprite.y);
    printf("(%d / %d) * (%d / %d) = %d\n", t->taille.x, t->taille_sprite.x, t->taille.y, t->taille_sprite.y, nb_sprites);
    t->phys = malloc(sizeof(id) * nb_sprites);
    printf("malloc = %p\n", t->phys);
    perror("apres malloc()");

	if(t->phys != NULL)
	{
		for(i = 0; i < nb_sprites; i++)
        {
            int dummy;
            printf("i = %d\n", i);
            fflush(stdout);
            //fscanf(flux, "%x", &dummy);
            fscanf(flux, "%x", &t->phys[i]);
        }
	}

    perror("apres boucle");
	fclose(flux);
    perror("sortie charger_infos_texture()");
}


void sauver_infos_texture(texture *t)
{
	NB nb_sprites, i;
	char nom[TAILLE_NOM_TEXTURE];
	FILE *flux = NULL;

	strcpy(nom, t->nom);
	supprime_extension(nom);
	strcat(nom, "txtr2");

	flux = fopen(nom, "w");

	if(flux == NULL)
	{
		/* gestion d'erreur? valeurs par defaut? à terminer... */
	}

	fprintf(flux, "%d ", t->taille.x);
	fprintf(flux, "%d ", t->taille.y);
	fprintf(flux, "%d ", t->taille_sprite.x);
	fprintf(flux, "%d ", t->taille_sprite.y);

	nb_sprites = (t->taille.x / t->taille_sprite.x) * (t->taille.y / t->taille_sprite.y);


	if(t->phys != NULL)
	{
		for(i = 0; i < nb_sprites; i++)
		fprintf(flux, "%d ",t->phys[i]);
	}

	fclose(flux);
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

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = ((Uint8 *)surface->pixels) + y * surface->pitch + x * bpp;
   
	switch (bpp) {
       case 1:
           return *p;
   
       case 2:
           return *(Uint16 *)p;
   
       case 3:
           if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
               return p[0] << 16 | p[1] << 8 | p[2];
           else
               return p[0] | p[1] << 8 | p[2] << 16;
   
       case 4:
           return *(Uint32 *)p;
   
       default:
           return 0;       /* shouldn't happen, but avoids warnings */
     } // switch
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
