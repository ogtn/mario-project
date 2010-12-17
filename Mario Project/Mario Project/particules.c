/*============================================================================*/
/*== [particules.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

/******************************************************************************/
/*==============================[DEFINES/ENUMS]===============================*/
/******************************************************************************/

#include "particules.h"
#include "texte.h"

particule_generator* new_particule_generator_smoke(coordf position, Uint32 vie_particules, Uint32 debit)
{
	particule_generator* generator = malloc(sizeof(particule_generator));

	generator->taille.x = 0;
	generator->taille.y = 0;

	generator->indice_debut = 0;
	generator->indice_fin = 0;	

	generator->position = position;
	generator->vie_particules = vie_particules;

	generator->debit = debit;
	generator->max_particules = (vie_particules / 1000) * debit + 1;
	generator->particules = malloc(sizeof(particule) * generator->max_particules);
	memset(generator->particules, 0, sizeof(particule) * generator->max_particules);

	generator->texture_id = charger_texture_bis("particle7.png", &generator->taille_particule);
	generator->gravity = 1; // Particules soumises à la gravité
	
	generator->couleur_debut = 0xFF808080;
	generator->couleur_fin = 0;

	generator->nb_particules = 0;
	generator->temps_ecoule = 0;

	srand(SDL_GetTicks());

	return generator;
}

particule_generator* new_particule_generator_fire(coordf position, Uint32 vie_particules, Uint32 debit)
{
	particule_generator* generator = malloc(sizeof(particule_generator));

	generator->taille.x = 0;
	generator->taille.y = 0;

	generator->indice_debut = 0;
	generator->indice_fin = 0;

	generator->position = position;
	generator->vie_particules = vie_particules;

	generator->debit = debit;
	generator->max_particules = (vie_particules / 1000) * debit + 1;
	generator->particules = malloc(sizeof(particule) * generator->max_particules);
	memset(generator->particules, 0, sizeof(particule) * generator->max_particules);

	generator->texture_id = charger_texture_bis("particle7.png", &generator->taille_particule);
	generator->gravity = 1; // Particules soumises à la gravité
	
	generator->couleur_debut = 0xFFFF0000;
	generator->couleur_fin = 0x00FFFF00;

	generator->nb_particules = 0;
	generator->temps_ecoule = 0;

	srand(SDL_GetTicks());

	return generator;
}

particule_generator* new_particule_generator_rain(coordf position, Uint32 vie_particules, Uint32 debit)
{
	particule_generator* generator = malloc(sizeof(particule_generator));

	generator->taille.x = LARGEUR_FENETRE;
	generator->taille.y = 0;

	generator->indice_debut = 0;
	generator->indice_fin = 0;

	generator->position = position;
	generator->vie_particules = vie_particules;

	generator->debit = debit;
	generator->max_particules = (vie_particules / 1000) * debit + 1;
	generator->particules = malloc(sizeof(particule) * generator->max_particules);
	memset(generator->particules, 0, sizeof(particule) * generator->max_particules);

	generator->texture_id = charger_texture_bis("particle6.png", &generator->taille_particule);
	generator->gravity = 1; // Particules non soumises à la gravité
	
	generator->couleur_debut = 0xFFFFFFFF;
	generator->couleur_fin = 0xFFFFFFFF;

	generator->nb_particules = 0;
	generator->temps_ecoule = 0;

	srand(SDL_GetTicks());

	return generator;
}

particule_generator* new_particule_generator_snow(coordf position, Uint32 vie_particules, Uint32 debit)
{
	particule_generator* generator = malloc(sizeof(particule_generator));

	generator->taille.x = LARGEUR_FENETRE;
	generator->taille.y = 0;

	generator->indice_debut = 0;
	generator->indice_fin = 0;

	generator->position = position;
	generator->vie_particules = vie_particules;

	generator->debit = debit;
	generator->max_particules = (vie_particules / 1000) * debit + 1;
	generator->particules = malloc(sizeof(particule) * generator->max_particules);
	memset(generator->particules, 0, sizeof(particule) * generator->max_particules);

	generator->texture_id = charger_texture_bis("Snow_back.png", &generator->taille_particule);
	generator->gravity = 1; // Particules soumises à la gravité
	
	generator->couleur_debut = 0xFFFFFFFF;
	generator->couleur_fin = 0xFFFFFFFF;

	generator->nb_particules = 0;
	generator->temps_ecoule = 0;

	srand(SDL_GetTicks());

	return generator;
}

void free_particule_generator(particule_generator **generator)
{
    if(generator && *generator)
    {
        if((*generator)->particules)
            free((*generator)->particules);

        free(*generator);
    }
}

void update_particule_genenerator(particule_generator *generator, Uint32 duration)
{
	int i = generator->indice_debut;
	static int cpt = 0;
	static int moyenne = 0;
	static Uint32 t = 0;
	Uint32 res;

	while(i != generator->indice_fin)
    {
		particule *p = &generator->particules[i];

		if(p->vie <= duration)
		{
			p->vie = 0;
			generator->nb_particules--;
			generator->indice_debut++;
			generator->indice_debut %= generator->max_particules;
		}
		else
		{
			p->vie -= duration;
			p->position.x += p->vitesse.x * duration;
			p->position.y += p->vitesse.y * duration;

			/* if(generator->gravity)
			p->vitesse.y -= 0.0006f * duration; */
		}

		i++;
		i %= generator->max_particules;
    }

	/* Calcul du nombre de particules à générer */
	res = generator->temps_ecoule + duration;

    while(res >= 1000 / generator->debit)
    {
		if(generator->nb_particules < generator->max_particules - 1)
		{
			float alea = 1.f * rand() / RAND_MAX;
			generator->particules[generator->indice_fin].position.x = generator->position.x + (generator->taille.x * alea);
			alea = 1.f * rand() / RAND_MAX;
			generator->particules[generator->indice_fin].position.y = generator->position.y + (generator->taille.y * alea);

			generator->particules[generator->indice_fin].vitesse.x = 0.5f * rand() / RAND_MAX -0.25f;
			generator->particules[generator->indice_fin].vitesse.y = -0.5f;//0.5f * rand() / RAND_MAX -0.25f;

			generator->particules[generator->indice_fin].vie = generator->vie_particules;

			generator->nb_particules++;
			cpt++;

			if(SDL_GetTicks() - t >= 500)
			{
				t = SDL_GetTicks();
				moyenne = cpt;
				cpt = 0;
			}

			generator->indice_fin++;
			generator->indice_fin %= generator->max_particules;
		}

		res -= (1000 / generator->debit);
		generator->temps_ecoule = res;
    }
	//screen_printf_dbg("%d part/s\n", moyenne * 2);
}
