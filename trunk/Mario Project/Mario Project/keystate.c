/*============================================================================*/
/*== [keystate.c] [Version: 0.028a]                                         ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/


#include "keystate.h"


keystate* new_keystate(void)
{
	return init_keystate(malloc(sizeof(keystate)));
}


keystate* init_keystate(keystate *k)
{
	int i;

	if(k == NULL) return k;

	for(i = 0; i < NB_TOUCHES; i++)
	{
		k->actuel[i] = 0;
		k->precedent[i] = 0;
	}

	return k;
}


keystate *free_keystate(keystate *k)
{
	if(k != NULL)
		free(k);

	return NULL;
}


void maj_keystate(keystate *keystate, int *continuer)
{
	SDL_Event event;
	Uint8 *SDL_keystate = NULL;
	int i;

	SDL_PollEvent(&event);
	SDL_keystate = SDL_GetKeyState(NULL);

	/* recuperation de l'etat precedent */
	for(i = 0; i < NB_TOUCHES; i++)
	{
		keystate->precedent[i] = keystate->actuel[i];
	}

	/* recuperation de l'etat actuel */
	keystate->actuel[HAUT] =    SDL_keystate[SDLK_UP];
	keystate->actuel[BAS] =     SDL_keystate[SDLK_DOWN];
	keystate->actuel[GAUCHE] =  SDL_keystate[SDLK_LEFT];
	keystate->actuel[DROITE] =  SDL_keystate[SDLK_RIGHT];
	keystate->actuel[SAUTER] =  SDL_keystate[SDLK_SPACE];
	keystate->actuel[RUN] =     SDL_keystate[SDLK_b];
	keystate->actuel[ENTRER] =  SDL_keystate[SDLK_RETURN];
	keystate->actuel[CLIC_G] =  SDL_GetMouseState(NULL, NULL) &SDL_BUTTON(SDL_BUTTON_LEFT);
	keystate->actuel[CLIC_D] =  SDL_GetMouseState(NULL, NULL) &SDL_BUTTON(SDL_BUTTON_RIGHT);
	keystate->actuel[ECHAP] =   SDL_keystate[SDLK_ESCAPE];
	keystate->actuel[CTRL_G] =  SDL_keystate[SDLK_LCTRL];
    keystate->actuel[SUPPR] =   SDL_keystate[SDLK_DELETE];
    keystate->actuel[CUT] =     SDL_keystate[SDLK_x];
    keystate->actuel[COPY] =    SDL_keystate[SDLK_c];
    keystate->actuel[PASTE] =   SDL_keystate[SDLK_v];

	if(event.type == SDL_QUIT 
		|| (keystate->actuel[ECHAP] && !keystate->precedent[ECHAP]))
		*continuer = 0;

	if(event.type == SDL_VIDEORESIZE)
	{
		SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_OPENGL | SDL_RESIZABLE);
	}
}
