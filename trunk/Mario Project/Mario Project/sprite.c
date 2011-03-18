/*============================================================================*/
/*== [sprite.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "sprite.h"

void draw_sprite_dr(sprite *s, layer depth, angle a)
{
    static GLuint last;

	/* Si c'est une texture differente de l'actuelle on change */
	if(s->text_id != last)
	{
        last = s->text_id;
		glEnd();
		glBindTexture(GL_TEXTURE_2D, s->text_id);
		glBegin(GL_QUADS);
	}

    switch(a)
    {
        case ANGLE_0:
        glTexCoord2f(s->point_bg.x, -s->point_bg.y);
        glVertex3i(s->position.x, s->position.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_bg.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_hd.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y + s->taille.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_hd.y);
        glVertex3i(s->position.x, s->position.y + s->taille.y, depth);
        break;

        case ANGLE_90:
        glTexCoord2f(s->point_bg.x, -s->point_bg.y);
        glVertex3i(s->position.x, s->position.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_hd.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_hd.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y + s->taille.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_bg.y);
        glVertex3i(s->position.x, s->position.y + s->taille.y, depth);
        break;

        case ANGLE_180:
        glTexCoord2f(s->point_hd.x, -s->point_hd.y);
        glVertex3i(s->position.x, s->position.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_hd.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_bg.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y + s->taille.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_bg.y);
        glVertex3i(s->position.x, s->position.y + s->taille.y, depth);
        break;

        case ANGLE_270:
        glTexCoord2f(s->point_hd.x, -s->point_hd.y);
        glVertex3i(s->position.x, s->position.y, depth);
        glTexCoord2f(s->point_hd.x, -s->point_bg.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_bg.y);
        glVertex3i(s->position.x + s->taille.x, s->position.y + s->taille.y, depth);
        glTexCoord2f(s->point_bg.x, -s->point_hd.y);
        glVertex3i(s->position.x, s->position.y + s->taille.y, depth);
        break;
    }
}


void draw_sprite_d(sprite *s, layer depth)
{
    draw_sprite_dr(s, depth, ANGLE_0);
}


void draw_sprite_r(sprite *s, angle a)
{
    draw_sprite_dr(s, 0, a);
}


void draw_sprite(sprite *s)
{
    draw_sprite_dr(s, 0, ANGLE_0);
}


void draw_position_souris(void)
{
	int mx, my;

	SDL_GetMouseState(&mx, &my);
	my = HAUTEUR_FENETRE - my;

	screen_printf_dbg("Souris_x: %3d   Souris_y: %3d", mx, my);
}


void draw_perso(perso *perso, Uint32 duree)
{
	data_texture *data;
    sprite s;

	/* Coordonnées de texture pour la texture actuelle */
	float gauche = 0, droite;
	float haut = 0, bas = 0, temp;
	int v_anim = 0, phase, nb_etats, i;

	if(perso->tps_transformation && perso->tps_transformation % 2 == 0)
		data = perso->texture_prec;
	else
		data = perso->texture_act;

	droite = (float) 1 / data->nb_sprites_max;
	nb_etats = NB_ETATS_TEXTURE - nb_etats_absents(data);

	switch(perso->etat)
	{

	case DEBOUT:
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = 1;
		bas = (float) (nb_etats - 1) / nb_etats;
		break;

	case MARCHE: case RENTRE_TUYAU_HORIZONTAL : case SORT_TUYAU_HORIZONTAL: case FINISH:
		v_anim = V_ANIM_MARCHE;
		haut = 1;
		bas = (float) (nb_etats - 1) / nb_etats;
		break;

	case COURSE_1:
		v_anim = V_ANIM_COURSE;
		haut = 1;
		bas = (float) (nb_etats - 1) / nb_etats;
		break;

	case COURSE_2:
		v_anim = V_ANIM_COURSE;
		haut = (float) (nb_etats - 1) / nb_etats;
		bas = (float) (nb_etats - 2) / nb_etats;
		break;

	case FINISH_CHATEAU:
		gauche = (float) (data->nb_sprites[MARCHE] - 1) / data->nb_sprites_max;
		droite = (float) data->nb_sprites[MARCHE] / data->nb_sprites_max;
		haut = (float) (nb_etats - 1) / nb_etats;
		bas = (float) (nb_etats - 2) / nb_etats;
		break;

	case DERAPE:
		gauche = (float) (data->nb_sprites[MARCHE] - 1) / data->nb_sprites_max;
		droite = (float) data->nb_sprites[MARCHE] / data->nb_sprites_max;
		haut = 1;
		bas = (float) (nb_etats - 1) / nb_etats;
		break;

	case SAUT:
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = (float) (nb_etats - 2) / nb_etats;
		bas = (float) (nb_etats - 3) / nb_etats;
		break;

	case SAUT_DESCENDANT:
		gauche = (float) 1 / data->nb_sprites_max;
		droite = (float) 2 / data->nb_sprites_max;
		haut = (float) (nb_etats - 2) / nb_etats;
		bas = (float) (nb_etats - 3) / nb_etats;
		break;
	
	case MONTE_ECHELLE :
		haut = (float) (nb_etats - 4) / nb_etats;
		bas = (float) (nb_etats - 5) / nb_etats;
		break;

	case REGARDE_HAUT:
		gauche = (float) 2 / data->nb_sprites_max;
		droite = (float) 3 / data->nb_sprites_max;
		haut = (float) (nb_etats - 3) / nb_etats;
		bas = (float) (nb_etats - 4) / nb_etats;
		break;

	case RENTRE_TUYAU_VERTICAL: case SORT_TUYAU_VERTICAL:
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = (float) (nb_etats - 3) / nb_etats;
		bas = (float) (nb_etats - 4) / nb_etats;
		break;

	case BAISSE: case SAUT_BAISSE :
		gauche = (float) 3 / data->nb_sprites_max;
		droite = (float) 4 / data->nb_sprites_max;
		haut = (float) (nb_etats - 3) / nb_etats;
		bas = (float) (nb_etats - 4) / nb_etats;
		break;

	case DEBOUT_CARAPACE :
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case MARCHE_CARAPACE :
		v_anim = V_ANIM_MARCHE;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case SAUT_CARAPACE :
		gauche = (float) (data->nb_sprites[MARCHE_CARAPACE] - 1) / data->nb_sprites_max;
		droite = (float) (data->nb_sprites[MARCHE_CARAPACE]) / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case POUSSE_CARAPACE : 
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = (float) (nb_etats - 6) / nb_etats;
		bas = (float) (nb_etats - 7) / nb_etats;
		break;

	case BAISSE_CARAPACE : case SAUT_BAISSE_CARAPACE:
		gauche = (float) 1 / data->nb_sprites_max;
		droite = (float) 2 / data->nb_sprites_max;
		haut = (float) (nb_etats - 6) / nb_etats;
		bas = (float) (nb_etats - 7) / nb_etats;
		break;

	case REGARDE_HAUT_CARAPACE :
		gauche = (float) 2 / data->nb_sprites_max;
		droite = (float) 3 / data->nb_sprites_max;
		haut = (float) (nb_etats - 6) / nb_etats;
		bas = (float) (nb_etats - 7) / nb_etats;
		break;

	case ASSIS:
		gauche = (float) 3 / data->nb_sprites_max;
		droite = (float) 4 / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case MORT:
		gauche = (float) data->nb_sprites[MARCHE] / data->nb_sprites_max;
		droite = (float) (data->nb_sprites[MARCHE] + 1) / data->nb_sprites_max;
		haut = 1;
		bas = (float) (nb_etats - 1) / nb_etats;
		break;

	case ATTAQUE:
		haut = (float) (nb_etats - 7) / nb_etats;
		bas = (float) (nb_etats - 8) / nb_etats;
		break;

	case ATTAQUE_SPECIALE:
		haut = (float) 1 / nb_etats;
		bas = 0;
		break;

	default: 
		break;
	}

	/* variation de la vitesse d'animation en fonction de la vitesse du perso
	fonctionne pas terrible, apparement ça saute des images :( */
	//if(perso->vitesse.x > 0)
	//	v_anim = (int)(perso->vitesse.x * -7000) + 1500;
	//else
	//	v_anim = (int)(perso->vitesse.x * 7000) + 1500;

	if(perso->etat == MARCHE || perso->etat == COURSE_1 || perso->etat == COURSE_2 || perso->etat == FINISH
		|| perso->etat == MARCHE_CARAPACE || perso->etat == SORT_TUYAU_HORIZONTAL
		|| perso->etat == RENTRE_TUYAU_HORIZONTAL)
	{
		/* Si le perso n'est pas en train de se transformer, on l'anime */
		if(!perso->tps_transformation)
		{
			phase = (duree % v_anim) / (v_anim / (data->nb_sprites[MARCHE] - 1));
		
			gauche += phase * (float)1 / data->nb_sprites_max;
			droite += phase * (float)1 / data->nb_sprites_max;

		}
	}

	/* Animation de l'attaque de base si le niveau de transformation de Mario est assez haut */
	if(perso->etat == MONTE_ECHELLE)
	{
		if(!perso->tps_transformation)
		{
			if(perso->vitesse.y == 0 && perso->vitesse.x == 0)
				phase = data->phase_prec;
			else
			{
				phase = (duree % data->v_anim[DEBOUT]) / (data->v_anim[DEBOUT] / (data->nb_sprites[MONTE_ECHELLE]));
				data->phase_prec = phase;
			}
		
			gauche += phase * (float)1 / data->nb_sprites_max;
			droite += phase * (float)1 / data->nb_sprites_max;
		}
	}

	/* Animation de l'attaque spéciale si le niveau de transformation de Mario est assez haut */
	if(perso->etat == ATTAQUE_SPECIALE && (perso->transformation == FIRE_MARIO || perso->transformation == SUPER_FIRE_MARIO)){

		if(!perso->tps_transformation)
		{
			phase = data->v_anim[ATTAQUE_SPECIALE] - perso->tps_attaque_speciale / data->nb_sprites[ATTAQUE_SPECIALE];

			for(i = 0; i <= data->nb_sprites[ATTAQUE_SPECIALE]; i++){
				if( i * data->v_anim[ATTAQUE_SPECIALE] / data->nb_sprites[ATTAQUE_SPECIALE] >= phase){
					gauche = (float) (i - 1) / data->nb_sprites_max;
					droite = (float) i / data->nb_sprites_max;
					break;
				}
			}
		}
	}

	/* Animation de l'attaque de base si le niveau de transformation de Mario est assez haut */
	if(perso->etat == ATTAQUE && perso->transformation >= FIRE_MARIO){

		if(!perso->tps_transformation){
			phase = data->v_anim[ATTAQUE] - perso->tps_attaque / data->nb_sprites[ATTAQUE];
		
			for(i = 0; i <= data->nb_sprites[ATTAQUE]; i++){
				if( i * data->v_anim[ATTAQUE] / data->nb_sprites[ATTAQUE] >= phase){
					gauche = (float) (i - 1) / data->nb_sprites_max;
					droite = (float) i / data->nb_sprites_max;
					break;
				}
			}	
		}
	}

	


	if(perso->cote == COTE_GAUCHE)
	{
		temp = gauche;
		gauche = droite;
		droite = temp;
	}
	
	/* Si mario est invincible, il est transparent une fois sur deux */
	if(perso->tps_invincible && perso->tps_invincible % 2 != 0)
		glColor4f(1, 1, 1, 0.5);

	/* Si mario a pris l'étoile, il clignote */
	if(perso->tps_invincible_etoile) {
		if(perso->tps_invincible_etoile % 3 == 0)
			glColor4f(1, 0, 0, 1);
		else if(perso->tps_invincible_etoile % 3 == 1)
			glColor4f(0, 1, 0, 1);
		else
			glColor4f(0, 0, 1, 1);
	}

    s.position.x = (int)perso->position.x;
    s.position.y = (int)perso->position.y;
    s.taille = perso->taille;
    s.text_id = data->texture;
    s.point_bg.x = gauche;
    s.point_bg.y = bas;
    s.point_hd.x = droite;
    s.point_hd.y = haut;
	draw_sprite(&s);

	glColor4f(1, 1, 1, 1);

}


void draw_monstre(occ_monstre *monstre, Uint32 duree)
{
	int i;
	float gauche = 0, droite = 0, haut = 0, bas = 0, temp;
	int phase, nb_etats_presents = M_NB_ETATS - m_nb_etats_absents(monstre->type_monstre);
    sprite s;

	switch(monstre->etat) {
		case M_MARCHE : case M_SORT_DU_TUYAU:
			gauche = 0;
			droite = (float)1 / monstre->type_monstre->nb_sprites_max;
			haut = 1;
			bas = 1 - (float)1 / nb_etats_presents;
			break;
		case M_MORT_PAR_PROJ :
			gauche = (float)1 / monstre->type_monstre->nb_sprites_max;
			droite = (float)2 / monstre->type_monstre->nb_sprites_max;
			bas = 1 - (float) 1 / nb_etats_presents;
			haut = bas - (float) 1 / nb_etats_presents;
			break;
		case M_MORT_PAR_SAUT: case M_RETRACTE: case M_RETRACTE_PORTED:
			gauche = 0;
			droite = (float)1 / monstre->type_monstre->nb_sprites_max;
			haut = 1 - (float) 1 / nb_etats_presents;
			bas = haut - (float) 1 / nb_etats_presents;
			break;
		case M_RETRACTE_RETOURNE:
			gauche = 0;
			droite = (float)1 / monstre->type_monstre->nb_sprites_max;
			haut = 1 - (float) 1 / nb_etats_presents;
			bas = haut - (float) 1 / nb_etats_presents;
			// Réduction de moitié de la taille du sprite et inversement du sprite
			temp = (float)(haut + bas) / 2;
			haut = bas;
			bas = temp;
			break;
		case M_SORT_CARAPACE:
			haut = (float) 1 / nb_etats_presents;
			bas = 0;
			break;
	}

	/* Animation de la sortie de carapace du monstre */
	if(monstre->etat == M_SORT_CARAPACE && monstre->tps_retracte > 0)
	{
		phase = monstre->type_monstre->v_anim[M_SORT_CARAPACE] - monstre->tps_retracte / monstre->type_monstre->nb_sprites[M_SORT_CARAPACE];

		for(i = 0; i <= monstre->type_monstre->nb_sprites[M_SORT_CARAPACE]; i++){
			if( i * monstre->type_monstre->v_anim[M_SORT_CARAPACE] / monstre->type_monstre->nb_sprites[M_SORT_CARAPACE] >= phase)
			{
				gauche = (float) (i - 1) / monstre->type_monstre->nb_sprites_max;
				droite = (float) i / monstre->type_monstre->nb_sprites_max;
				break;
			}
		}
	}

	if(monstre->etat == M_MARCHE 
		|| monstre->etat == M_SORT_DU_TUYAU 
		|| ((monstre->etat == M_RETRACTE || monstre->etat == M_RETRACTE_RETOURNE) && monstre->vitesse.x != 0))
	{
		if(monstre->est_gele)
		{
			phase = 0;
		}
		else
		{
			if(monstre->etat == M_RETRACTE || monstre->etat == M_RETRACTE_RETOURNE)
				phase = (duree % monstre->type_monstre->v_anim[M_RETRACTE]) / (monstre->type_monstre->v_anim[M_RETRACTE] / (monstre->type_monstre->nb_sprites[M_RETRACTE] - 1));
			else
				phase = (duree % monstre->type_monstre->v_anim[M_MARCHE]) / (monstre->type_monstre->v_anim[M_MARCHE] / (monstre->type_monstre->nb_sprites[M_MARCHE]));
		}

		gauche += phase * (float)1 / monstre->type_monstre->nb_sprites_max;
		droite += phase * (float)1 / monstre->type_monstre->nb_sprites_max;
	}

	/* Changement de côté si besoin */
	if(monstre->cote == COTE_GAUCHE && monstre->etat != M_RETRACTE && monstre->etat != M_RETRACTE_RETOURNE)
	{
		temp = gauche;
		gauche = droite;
		droite = temp;
	}

    s.position.x = (int)monstre->position.x;
    s.position.y = (int)monstre->position.y;
    s.taille = monstre->type_monstre->taille;
    s.text_id = monstre->type_monstre->texture;
    s.point_bg.x = gauche;
    s.point_bg.y = bas;
    s.point_hd.x = droite;
    s.point_hd.y = haut;
    
	if(monstre->etat == M_RETRACTE_RETOURNE)
        s.taille.y = monstre->type_monstre->taille.y / 2;
		
    /* Dessin */
    draw_sprite(&s);

	//if(monstre->est_gele)
	//{
	//	s.position.x = (int)monstre->position.x;
	//	s.position.y = (int)monstre->position.y;
	//	s.text_id = charger_texture_bis("textures/blocs/speciaux/incassables/icecube.png", NULL);
	//	s.taille = monstre->type_monstre->taille;
	//	s.point_bg.x = 0;
	//	s.point_bg.y = 0;
	//	s.point_hd.x = 1;
	//	s.point_hd.y = 1;

	//	if(monstre->etat == M_RETRACTE_RETOURNE)
 //       s.taille.y = monstre->type_monstre->taille.y / 2;
	//	
	//	/* Dessin */
	//	draw_sprite(&s);
	//}
}


void draw_projectile(occ_projectile* proj, Uint32 duree)
{
	float gauche = 0, droite = (float) 1 / proj->type_projectile->nb_sprites_marche, haut = 0, bas = 0, temp;
	int phase = 0, i, v_anim;
    sprite s;

	if(!proj->tps_apparition && proj->tps_vie)
	{
		v_anim = proj->type_projectile->v_anim_marche;
		phase = (duree % v_anim) / (v_anim / proj->type_projectile->nb_sprites_marche);

		haut = 1;
		bas = (proj->type_projectile->nb_sprites_mort == 0)? 0.0F : 0.5F;

		gauche += phase * (float)1 / proj->type_projectile->nb_sprites_marche;
		droite += phase * (float)1 / proj->type_projectile->nb_sprites_marche;

		/* Pour le changement de côté */
		if(proj->cote == COTE_GAUCHE)
		{
			temp = gauche;
			gauche = droite;
			droite = temp;
		}

		s.position.x = (int)proj->position.x;
		s.position.y = (int)proj->position.y;
		s.taille = proj->type_projectile->taille;
		s.text_id = proj->type_projectile->texture;
		s.point_bg.x = gauche;
		s.point_bg.y = bas;
		s.point_hd.x = droite;
		s.point_hd.y = haut;
		draw_sprite(&s);
	}
	else if(proj->tps_disparition && !proj->tps_vie)
	{
		phase = proj->tps_disparition / proj->type_projectile->nb_sprites_mort;

		haut = 0.5;
		bas = 0;

		for(i = 1; i <= proj->type_projectile->nb_sprites_mort; i++)
		{
			if( i * proj->type_projectile->v_anim_mort / proj->type_projectile->nb_sprites_mort > phase)
			{
				gauche = (float) (i - 1) / proj->type_projectile->nb_sprites_marche;
				droite = (float) i / proj->type_projectile->nb_sprites_marche;
				break;
			}
		}

		s.position.x = (int)proj->position.x;
		s.position.y = (int)proj->position.y;
		s.taille = proj->type_projectile->taille;
		s.text_id = proj->type_projectile->texture;
		s.point_bg.x = gauche;
		s.point_bg.y = bas;
		s.point_hd.x = droite;
		s.point_hd.y = haut;
		draw_sprite(&s);
	}
}


void draw_HUD(perso* p)
{

	if(p->hud->personnage == MARIO)
		screen_printf_dbg("Mario x%d\n", p->hud->nb_vies);
	else
		screen_printf_dbg("Luigi x%d\n", p->hud->nb_vies);

	screen_printf_dbg("Nom du niveau : %s\n", p->hud->nom_niveau);
	screen_printf_dbg("Nombre de pieces : %d\n", p->hud->nb_pieces);
	screen_printf_dbg("Score : %d\n", p->hud->score);
	screen_printf_dbg("Temps restant : %d\n", p->hud->time);
	screen_printf_dbg("Environnement : %s\n", chaines_environement[p->environnement]);
	screen_printf_dbg("Etat : %s\n", chaines_etats_perso[p->etat]);
}


void draw_item(item* model, occ_item* item, Uint32 duree)
{
	int phase;
    sprite s;
	float gauche = 0, droite = 1.F / model->nb_sprites;

    s.position.x = (int)item->position.x;
    s.position.y = (int)item->position.y;
    s.taille = model->taille;
    s.text_id = model->texture;
    s.point_bg.x = s.point_bg.y = 0;
    s.point_hd.x = s.point_hd.y = 1;
	
	if(model->nb_sprites > 1)
	{
		phase = (duree % 500) / (500 / (model->nb_sprites));

		gauche += phase * 1.F / model->nb_sprites;
		droite += phase * 1.F / model->nb_sprites;

        s.point_bg.x = gauche;
        s.point_hd.x = droite;
	}

    draw_sprite(&s);
}

void draw_pipe(tuyau* t)
{
	sprite s;

	s.position.x = (int)(t->position.x * LARGEUR_BLOC);
	s.position.y = (int)(t->position.y * LARGEUR_BLOC);
	s.text_id = t->texture;

	/* On dessine selon le sens de sortie du tuyau */
	switch(t->sens_sortie)
	{
	case VERS_LA_DROITE:
		// Reste du tuyau (Plus pratique de commencer par le reste)
		s.point_bg.x = 1;
		s.point_bg.y = 0;
		s.point_hd.x = 0;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(t->longueur * LARGEUR_BLOC);
		s.taille.y = (int)(2 * LARGEUR_BLOC);
        draw_sprite_r(&s, ANGLE_90);

		// Haut du tuyau
		s.position.x = (int)((t->position.x + t->longueur) * LARGEUR_BLOC);
		s.point_bg.x = 1;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 0;
		s.point_hd.y = 1;
		s.taille.x = (int)(LARGEUR_BLOC);
		draw_sprite_r(&s, ANGLE_90);
		break;

	case VERS_LA_GAUCHE:
		// Haut du tuyau
		s.point_bg.x = 1;
		s.point_bg.y = 1;
		s.point_hd.x = 0;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(LARGEUR_BLOC);
		s.taille.y = (int)(2 * LARGEUR_BLOC);
		draw_sprite_r(&s, ANGLE_90);

		 // Reste du tuyau
		s.position.x = (int)((t->position.x + 1) * LARGEUR_BLOC);
		s.point_bg.x = 1;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 0;
		s.point_hd.y = 0;
		s.taille.x = (int)(t->longueur * LARGEUR_BLOC);
		draw_sprite_r(&s, ANGLE_90);
		break;

	case VERS_LE_BAS:
		// Haut du tuyau
		s.point_bg.x = 0;
		s.point_bg.y = 1;
		s.point_hd.x = 1;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(2 * LARGEUR_BLOC);
		s.taille.y = (int)(LARGEUR_BLOC);
        draw_sprite(&s);

		// Reste du tuyau
		s.position.y = (int)((t->position.y + 1) * LARGEUR_BLOC);
		s.point_bg.x = 0;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 1;
		s.point_hd.y = 0;
		s.taille.y = (int)(t->longueur * LARGEUR_BLOC);
        draw_sprite(&s);
		break;

	default : // VERS_LE_HAUT
		 // Reste du tuyau (Plus pratique de commencer par le reste)
		s.point_bg.x = 0;
		s.point_bg.y = 0;
		s.point_hd.x = 1;
		s.point_hd.y = 0.5;
		s.taille.x = (int)(2 * LARGEUR_BLOC);
		s.taille.y = (int)(t->longueur * LARGEUR_BLOC);
        draw_sprite(&s);

		// Haut du tuyau
		s.position.y = (int)((t->position.y + t->longueur) * LARGEUR_BLOC);
		s.point_bg.x = 0;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 1;
		s.point_hd.y = 1;
		s.taille.y = (int)(LARGEUR_BLOC);
		draw_sprite(&s);
		break;
	}
}

void draw_particules(particule_generator* generator)
{
	int i;
    sprite s = {{0, 0}, {0, 0}, 0, {0, 0}, {0, 0}};

    s.point_hd.x = s.point_hd.y = 1;
	s.taille = generator->taille_particule;
    s.text_id = generator->texture_id;

	for(i = 0; i < generator->max_particules; i ++)
    {
        particule p = generator->particules[i];
        float alpha, red, green, blue, fade;

        fade = p.vie / (float) generator->vie_particules;

		/* récupération des composantes de la couleur et calcul de l'interpolation selon la vie restantes */
		alpha = (fade * ((generator->couleur_debut >> 24) & 0xFF)) + ((1 - fade) * ((generator->couleur_fin >> 24) & 0xFF));
        red = (fade * ((generator->couleur_debut >> 16) & 0xFF)) + ((1 - fade) * ((generator->couleur_fin >> 16) & 0xFF));
        green = (fade * ((generator->couleur_debut >> 8) & 0xFF)) + ((1 - fade) * ((generator->couleur_fin >> 8) & 0xFF));
        blue = (fade * (generator->couleur_debut & 0xFF)) + ((1 - fade) * (generator->couleur_fin & 0xFF));
        
		glColor4f(red/256, green/256, blue/256, alpha/256);

        if(p.vie)
        {
            s.position.x = (int)p.position.x;
            s.position.y = (int)p.position.y;
			draw_sprite(&s);
        }
    }
    
	glColor4f(1, 1, 1, 1);
}

void draw_checkpoint(checkpoint *check, Uint32 duree)
{
	int phase = 0;
	float gauche = 0, droite = (float) 1 / max(check->nb_sprites_actif, check->nb_sprites_inactif), haut, bas;
	sprite s;
	
	if(check->etat == NOT_PASSED)
	{
		haut = 1;
		bas = 0.5;

		if(check->nb_sprites_inactif > 1)
		{
			phase = (duree % check->v_anim) / (check->v_anim / (check->nb_sprites_inactif - 1));
			gauche += phase * (float) 1 / check->nb_sprites_inactif;
			droite += phase * (float) 1 / check->nb_sprites_inactif;
		}
	}
	else
	{
		haut = 0.5;
		bas = 0;

		if(check->nb_sprites_actif  > 1)
		{
			phase = (duree % check->v_anim) / (check->v_anim / (check->nb_sprites_actif - 1));
			gauche += phase * (float) 1 / check->nb_sprites_actif;
			droite += phase * (float) 1 / check->nb_sprites_actif;
		}
	}

	s.position = check->position;
	s.taille = check->taille;
	s.text_id = check->id_text;
	s.point_bg.x = gauche;
	s.point_bg.y = bas;
	s.point_hd.x = droite;
	s.point_hd.y = haut;
	draw_sprite(&s);
}

void draw_contours(ecran e)
{
    glEnd();
    glColor3ub(0x31, 0x49, 0x6A);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	{
		glVertex2i(0, 0);
		glVertex2i(0, e.origine.y);
		glVertex2i(LARGEUR_FENETRE, e.origine.y);
		glVertex2i(LARGEUR_FENETRE, 0);

		glVertex2i(0, e.origine.y + e.taille.y);
		glVertex2i(0, HAUTEUR_FENETRE);
		glVertex2i(LARGEUR_FENETRE, HAUTEUR_FENETRE);
		glVertex2i(LARGEUR_FENETRE, e.origine.y + e.taille.y);

		glVertex2i(0, e.origine.y);
		glVertex2i(0, e.origine.y + e.taille.y);
		glVertex2i(e.origine.x, e.origine.y + e.taille.y);
		glVertex2i(e.origine.x, e.origine.y);

		glVertex2i(e.origine.x + e.taille.x, e.origine.y);
		glVertex2i(e.origine.x + e.taille.x, e.origine.y + e.taille.y);
		glVertex2i(LARGEUR_FENETRE, e.origine.y + e.taille.y);
		glVertex2i(LARGEUR_FENETRE, e.origine.y);
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
}


void draw_cadre(coordi pos, coordi taille, GLuint texture, int taille_text)
{
	sprite s;
	glColor3f(1, 1, 1);

	/* parametres fixes */
	s.text_id = texture;
	s.taille.x = taille_text;
	s.taille.y = taille_text;
	s.point_bg.x = 0;
	s.point_hd.x = 1;

    /* coin haut gauche */
    s.point_bg.y = 0.875;
    s.point_hd.y = 1;
    s.position.x = pos.x;
    s.position.y = pos.y + taille.y - taille_text;
    draw_sprite(&s);

    /* coin haut droite */
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x + taille.x - taille_text;
    draw_sprite(&s);

    /* coin bas gauche */
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x;
    s.position.y = pos.y;
    draw_sprite(&s);

    /* coin bas droite */
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x + taille.x - taille_text;
    draw_sprite(&s);

    /* haut */
    s.taille.x = taille.x - (2 * taille_text);
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x + taille_text;
    s.position.y = pos.y + taille.y - taille_text;
    draw_sprite(&s);

    /* droite */
    s.taille.x = taille_text;
    s.taille.y = taille.y - (2 * taille_text);
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x + taille.x - taille_text;
    s.position.y = pos.y + taille_text;
    draw_sprite(&s);

    /* bas */
    s.taille.x = taille.x - (2 * taille_text);
    s.taille.y = taille_text;
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x + taille_text;
    s.position.y = pos.y;
    draw_sprite(&s);

    /* gauche */
    s.taille.x = taille_text;
    s.taille.y = taille.y - (2 * taille_text);
    s.point_bg.y -= 0.125;
    s.point_hd.y -= 0.125;
    s.position.x = pos.x;
    s.position.y = pos.y + taille_text;
    draw_sprite(&s);
}
