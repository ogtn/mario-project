/*============================================================================*/
/*== [main.c] [Version: 0.028a]                                             ==*/
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
/*=================================[INCLUDES]=================================*/
/******************************************************************************/

#include "constantes.h"
#include "world.h"
#include "collision_system.h"
#include "menus.h"
#include "editeur.h"
#include "texte_interface.h"

/******************************************************************************/
/*================================[PROTOTYPES]================================*/
/******************************************************************************/

/* Initialisation de la SDL en mode OpenGL, on fournit la resolution desirée */
void init_SDL(int x, int y);

/* Initialisation de OpenGL, on fournit la resolution desirée également */
void init_OpenGL(int x, int y);

/* Lance une partie */
void jouer(world *w);

/* Affiche les informations du mode debug */
void affichage_debug(world *w);

/******************************************************************************/
/*============================[DEBUT DU PROGRAMME]============================*/
/******************************************************************************/


/* on garde de coté au cas ou...
void print_time(void)
{
    static LARGE_INTEGER freq = {0, 0};
    static LARGE_INTEGER time, time_prec;

    if(freq.QuadPart == 0)
        QueryPerformanceFrequency(&freq);

    QueryPerformanceCounter(&time);

    printf("%5f ms\n", (double)(time.QuadPart - time_prec.QuadPart) / freq.QuadPart * 1000);

    time_prec = time;
}


int main_bak(int argc, char *argv[])
{
	init_SDL(LARGEUR_FENETRE, HAUTEUR_FENETRE);
	init_OpenGL(LARGEUR_FENETRE, HAUTEUR_FENETRE);
	init_text("fonts/debug.fnt", 50, COLOR_RED);

	screen_printf_dbg("Hello, World!");
	screen_flush();
	SDL_GL_SwapBuffers();
	pause(1000);

	SDL_Quit();
	quit_text();
	return 0;
}
*/


int main(int argc, char *argv[])
{
    //int choix = 1;
    keystate *k = NULL;
    perror("main()");
    //FSOUND_STREAM *musique = NULL;

    //Pour le module de surveillance des allocations
#ifdef _DEBUG
    fic = fopen("allocations.txt", "w");
#endif

    // Chargement de la SDL, de OpenGL et de FMOD 
    init_SDL(LARGEUR_FENETRE, HAUTEUR_FENETRE);
    init_OpenGL(LARGEUR_FENETRE, HAUTEUR_FENETRE);
    FSOUND_Init(44100, 32, 0);

	init_text("fonts/consolas.fnt", 100, COLOR_RED);

    // chargement de textures de base à faire plus proprement plus tard 
    charger_texture_bis("textures/rien.png", NULL);
    charger_texture_bis("textures/ascii_2.png", NULL);
    charger_texture_bis("textures/title_screen.jpg", NULL);
    charger_texture_bis("textures/button_blu.png", NULL);
    charger_texture_bis("textures/button_red.png", NULL);
    charger_texture_bis("textures/button_blk.png", NULL);


    //musique = charger_musique("musics/Ice Mario.mp3", 255, 1);

    // Menu Principal 
    k = new_keystate();
    //FSOUND_Stream_Play(FSOUND_FREE, musique);
    //choix = main_menu(k);

    //while(choix != QUITTER)
    //{
    //jouer();
    //choix = main_menu(k);
    //}
    jouer(NULL);
    //main_editeur();
 
    // Astuce pour que le compilo ne dise pas que ces deux parametres ne sont pas
    //utilisés. On peut eventuellement les utiliser pour un mode debug,
    //ou fenetre/plein ecran par la suite 
    argc;
    argv;

    // liberation de la memoire (à completer)
//    FSOUND_Stream_Close(musique);
    FSOUND_Close();
    SDL_Quit();
    quit_text();

#ifdef _DEBUG
    affich_list();
    fclose(fic);
#endif

    return 0;
}


void init_SDL(int x, int y)
{
	char str[100];

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        exit(0);
    }

	SDL_SetVideoMode(x, y, 32, SDL_OPENGL);

    if(SDL_GetVideoSurface() == NULL){
        exit(0);
    }
    SDL_EnableKeyRepeat(10, 10);

	SDL_ShowCursor(0);


    sprintf(str, "=Mario Project= V0.028a [%d * %d]", x, y);	
	SDL_WM_SetCaption(str, NULL);
}


void init_OpenGL(int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, x, 0, y);


    glEnable(GL_BGR_EXT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glMatrixMode(GL_MODELVIEW);
}


void jouer(world *w_)
{
    int continuer = VRAI;
    world *w;
	Uint32 temps_rendu = 0;

    /* On cache le curseur, ni vu ni connu je t'embrouille */
    SDL_ShowCursor(0);

    /* Si pas de monde, on en fait un */
    if(w_ == NULL)
    {
        w = new_world();
        load_world(w);
    }
    else
    {
        w = w_;
    }

    /* initialisations (temporaires?) de l'ecran */
    w->ecran.taille.x = LARGEUR_FENETRE;
    w->ecran.taille.y = HAUTEUR_FENETRE;
    w->ecran.origine.x = 0;
    w->ecran.origine.y = 0;

	
	/* Boucle principale du programme à modifier pour qu'elle soit plus "intelligente"
    il faut que lorsqu' on presse echap le menu pause s'ouvre, et qu'on retourne dans
    la boucle en quittant le menu (sauf si on decide d'arreter de jouer) */
    while(continuer)
    {
		temps_rendu = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        /* Mise à jour de l'etat des touches du clavier */
        maj_keystate(w->keystate, &continuer);

        /* Test de collisions */
        if(perso_mort_ou_transforme(w))
		{
			/* Mise à jour de toutes les positions des objets/enemis */
			main_collisions(w);

			/* Mise à jour des positions des particules */
			MAJ_particules(w->niveau, w->temps_ecoule);
		}
        else
        {
            w->persos[0]->tps_transformation -= w->temps_ecoule;
            if(w->persos[0]->tps_transformation > pow(2, 31))
                w->persos[0]->tps_transformation = 0;

			w->persos[0]->tps_mort -= w->temps_ecoule;
            if(w->persos[0]->tps_mort > pow(2, 31))
                w->persos[0]->tps_mort = 0;
        }

		/* Vérification du nombre vies restantes, fin de niveau, etc... */
		check_lives_finish(w, &continuer);

        /* Mise à jour de la position de l'ecran (en fonction de celle du perso) */
        update_screen(w);

		draw_main(w->niveau, w->persos, w->ecran, w->temps_actuel);

        /* Infos debug */
#ifdef _DEBUG
        //affichage_debug(w);
#endif

        /* audio? */

        /* Gestion du temps */
        if(perso_mort_ou_transforme(w))
		{
			update_time(w);
		}

		temps_rendu = SDL_GetTicks() - temps_rendu;
		//if(temps_rendu > 17)
		//{
		//	 temps_rendu = SDL_GetTicks();

		//	/* Dessin de la scene */
		//	draw_main(w->niveau, w->persos, w->ecran, w->temps_actuel);

		//	screen_flush();
		//	SDL_GL_SwapBuffers();
		//}
		//else
		//{
		//	pause(1);
		//}

		if(temps_rendu < 17)
			pause(17 - temps_rendu);

       screen_flush();
	   SDL_GL_SwapBuffers();
    }

    if(w_ == NULL)
    {
        liberer_textures_niveau(w->niveau);
        free_world(w);
    }
}


void affichage_debug(world *w)
{
    /* Titre */
    screen_printf_dbg(" ===MODE DEBUG===\n");

    /* FPS */
    screen_printf_dbg("FPS: %4d\n", w->fps);

    /* Environement du perso principal */
    screen_printf_dbg("environnement: %s\n", chaines_environement[w->persos[0]->environnement]);

    /* Etat du personnage */
    screen_printf_dbg("etat: %s\n", chaines_etats_perso[w->persos[0]->etat]);

    /* Position du personnage principal */
    screen_printf_dbg("pos_x: %f, pos_y: %f\n", w->persos[0]->position.x, w->persos[0]->position.y);

    /* Position précédente du personnage */
    screen_printf_dbg("pos_prec_x: %f, pos_prec_y: %f\n", w->persos[0]->position_prec.x, w->persos[0]->position_prec.y);

    /* Vitesse du personnage principal */
    screen_printf_dbg("speed_x: %f, speed_y: %f\n", w->persos[0]->vitesse.x, w->persos[0]->vitesse.y);

    /* Position de l'ecran dans le niveau */
    screen_printf_dbg("ecran_x: %d, ecran_y: %d\n", w->ecran.scroll.x, w->ecran.scroll.y);

    /* Acceleration du personnage principal */
    screen_printf_dbg("accel_x: %f, accel_y: %f\n", w->persos[0]->accel, w->persos[0]->accel);

    /* Transformation actuelle du personnage */
    screen_printf_dbg("transformation: %s\n", chaines_transformations[w->persos[0]->transformation]);

    /* Coordonnées du milieu du bas de mario */
    screen_printf_dbg("pos_milieu_x : %f, pos_milieu_y : %f\n", w->persos[0]->position.x + w->persos[0]->taille.x / 2, w->persos[0]->position.y);

    /* Bloc où il ya collision en dessous du bas du perso */
    {
        coordi bloc;
        bloc.x = (int) (w->persos[0]->position.x + w->persos[0]->texture_act->abscisse_bas + (w->persos[0]->taille.x - 2 * w->persos[0]->texture_act->abscisse_bas) / 2) / w->niveau->taille_blocs.x;
        bloc.y = (int) w->persos[0]->position.y / w->niveau->taille_blocs.y;

		switch(w->niveau->occ_blocs[bloc.x][bloc.y]->bloc_actuel->phys)
        {
        case SOL :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : SOL\n");
            break;
        case PENTE_30_GAUCHE_8 :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : PENTE_30_GAUCHE_8\n");
            break;
        case PENTE_30_GAUCHE_16 :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : PENTE_30_GAUCHE_16\n");
            break;
        case PENTE_45_DROITE :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : PENTE_45_DROITE\n");
            break;
        case PENTE_45_GAUCHE :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : PENTE_45_GAUCHE\n");
            break;
        default :
            screen_printf_dbg("Type de bloc en dessous du milieu de Mario : BLOC_VIDE\n");
        }
    }

    /* Hitbox */
    screen_printf_dbg("Hitbox : bg.x : %f bg.y : %f\n", w->persos[0]->position.x + w->persos[0]->texture_act->abscisse_bas, w->persos[0]->position.y);
}