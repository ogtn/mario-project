/*============================================================================*/
/*== [particules.c] [Version: 0.028a]                                       ==*/
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

particule_generator* new_particule_generator(coordf position, coordi taille, Uint32 vie_particules, Uint32 debit, char* nom_texture, int gravity, int couleur_debut, int couleur_fin)
{
	particule_generator* generator = malloc(sizeof(particule_generator));

	generator->taille = taille;

	generator->indice_debut = 0;
	generator->indice_fin = 0;	

	generator->position = position;
	generator->vie_particules = vie_particules;

	generator->debit = debit;
	generator->max_particules = (vie_particules / 1000) * debit + 1;
	generator->particules = malloc(sizeof(particule) * generator->max_particules);
	memset(generator->particules, 0, sizeof(particule) * generator->max_particules);

	strcpy(generator->nom_text, nom_texture);
	generator->texture_id = charger_texture_bis(nom_texture, &generator->taille_particule);
	generator->gravity = gravity; // Particules soumises à la gravité
	
	generator->couleur_debut = couleur_debut;
	generator->couleur_fin = couleur_fin;

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
}
