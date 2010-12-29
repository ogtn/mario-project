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

/* ancienne version, va virer sous peu */
void draw_sprite(int x, int y, int largeur, int hauteur, GLuint texture,
				 float coordx1,	 float coordx2, float coordy1, float coordy2)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(coordx1, -coordy1);		glVertex2i(x, y);
		glTexCoord2f(coordx2, -coordy1);		glVertex2i(x + largeur, y);
		glTexCoord2f(coordx2, -coordy2);		glVertex2i(x + largeur, y + hauteur);
		glTexCoord2f(coordx1, -coordy2);		glVertex2i(x, y + hauteur);
	}
	glEnd();
}

void draw_sprite_90(int x, int y, int largeur, int hauteur, GLuint texture,
				 float coordx1,	 float coordx2, float coordy1, float coordy2)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(coordx1, -coordy1);		glVertex2i(x, y);
		glTexCoord2f(coordx1, -coordy2);		glVertex2i(x + largeur, y);
		glTexCoord2f(coordx2, -coordy2);		glVertex2i(x + largeur, y + hauteur);
		glTexCoord2f(coordx2, -coordy1);		glVertex2i(x, y + hauteur);

	}
	glEnd();
}


/* version optimisée */
void draw_sprite_(sprite *s, GLuint last)
{
	/* Si c'est une texture differente de l'actuelle on change */
	if(s->text_id != last)
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, s->text_id);
		glBegin(GL_QUADS);
	}

	glTexCoord2f(s->point_bg.x, -s->point_bg.y);
	glVertex2i(s->position.x, s->position.y);

	glTexCoord2f(s->point_hd.x, -s->point_bg.y);
	glVertex2i(s->position.x + s->taille.x, s->position.y);

	glTexCoord2f(s->point_hd.x, -s->point_hd.y);
	glVertex2i(s->position.x + s->taille.x, s->position.y + s->taille.y);

	glTexCoord2f(s->point_bg.x, -s->point_hd.y);
	glVertex2i(s->position.x, s->position.y + s->taille.y);
}

void draw_sprite_90_(sprite *s, GLuint last)
{
	/* Si c'est une texture differente de l'actuelle on change */
	if(s->text_id != last)
	{
		glEnd();
		glBindTexture(GL_TEXTURE_2D, s->text_id);
		glBegin(GL_QUADS);
	}

	glTexCoord2f(s->point_bg.x, -s->point_bg.y);
	glVertex2i(s->position.x, s->position.y);

	glTexCoord2f(s->point_bg.x, -s->point_hd.y);
	glVertex2i(s->position.x + s->taille.x, s->position.y);

	glTexCoord2f(s->point_hd.x, -s->point_hd.y);
	glVertex2i(s->position.x + s->taille.x, s->position.y + s->taille.y);

	glTexCoord2f(s->point_hd.x, -s->point_bg.y);
	glVertex2i(s->position.x, s->position.y + s->taille.y);
}


void draw_triangle_illum(int x1, int y1, int x2, int y2, int x3, int y3, GLuint texture,
						 float coordx1,	 float coordy1,
						 float coordx2, float coordy2,
						 float coordx3,	 float coordy3)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_TRIANGLES);
	{
		glTexCoord2f(coordx1, coordy1);		glVertex2i(x1, y1);
		glTexCoord2f(coordx2, coordy2);		glVertex2i(x2, y2);
		glTexCoord2f(coordx3, coordy3);		glVertex2i(x3, y3);
	}
	glEnd();
}


void draw_text(char *msg, coordi pos, float r, float g, float b)
{
	unsigned int i, px, py;
	float x1, x2, y1, y2;
	size_t length =  strlen(msg);
	int largeur = 16, hauteur = 16, x, y;

	glColor3f(r, g, b);
	glBindTexture(GL_TEXTURE_2D, 2);
	glBegin(GL_QUADS);

	for(i = 0; i < length; i++)
	{
		px = 15 - msg[i]/16;
		py = msg[i]%16;
		y1 = px/16.0f;
		y2 = (px+1)/16.0f;
		x1 = py/16.0f;
		x2 = (py+1)/16.0f;
		x = i * 8 + pos.x;
		y = pos.y;

		glTexCoord2f(x1, -y1);	glVertex2i(x, y);
		glTexCoord2f(x2, -y1);	glVertex2i(x + largeur, y);
		glTexCoord2f(x2, -y2);	glVertex2i(x + largeur, y + hauteur);
		glTexCoord2f(x1, -y2);	glVertex2i(x, y + hauteur);
	}

	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}


void draw_position_souris(void)
{
	int mx = 0, my = 0;
	char msg[TAILLE_NOM_TEXTURE];
	coordi pos;

	pos.x = 0;
	pos.y = HAUTEUR_FENETRE - 32;

	SDL_GetMouseState(&mx, &my);
	my = HAUTEUR_FENETRE - my;

	sprintf(msg,"Souris_x: %3d   Souris_y: %3d", mx, my);
	draw_text(msg, pos, 1, 1, 1);
}


void draw_perso(perso *perso, Uint32 duree)
{
	data_texture *data;

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
		haut = (float) (nb_etats - 4) / nb_etats;
		bas = (float) (nb_etats - 5) / nb_etats;
		break;

	case MARCHE_CARAPACE :
		v_anim = V_ANIM_MARCHE;
		haut = (float) (nb_etats - 4) / nb_etats;
		bas = (float) (nb_etats - 5) / nb_etats;
		break;

	case SAUT_CARAPACE :
		gauche = (float) (data->nb_sprites[MARCHE_CARAPACE] - 1) / data->nb_sprites_max;
		droite = (float) (data->nb_sprites[MARCHE_CARAPACE]) / data->nb_sprites_max;
		haut = (float) (nb_etats - 4) / nb_etats;
		bas = (float) (nb_etats - 5) / nb_etats;
		break;

	case POUSSE_CARAPACE : 
		gauche = 0;
		droite = (float) 1 / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case BAISSE_CARAPACE : case SAUT_BAISSE_CARAPACE:
		gauche = (float) 1 / data->nb_sprites_max;
		droite = (float) 2 / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
		break;

	case REGARDE_HAUT_CARAPACE :
		gauche = (float) 2 / data->nb_sprites_max;
		droite = (float) 3 / data->nb_sprites_max;
		haut = (float) (nb_etats - 5) / nb_etats;
		bas = (float) (nb_etats - 6) / nb_etats;
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
		haut = (float) (nb_etats - 6) / nb_etats;
		bas = (float) (nb_etats - 7) / nb_etats;
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
		if(!perso->tps_transformation){
			phase = (duree % v_anim) / (v_anim / (data->nb_sprites[MARCHE] - 1));
		
			gauche += phase * (float)1 / data->nb_sprites_max;
			droite += phase * (float)1 / data->nb_sprites_max;

		}
	}

	/* Animation de l'attaque spéciale si le niveau de transformation de Mario est assez haut */
	if(perso->etat == ATTAQUE_SPECIALE && (perso->transformation == FIRE_MARIO || perso->transformation == SUPER_FIRE_MARIO)){

		if(!perso->tps_transformation){
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
	if(perso->est_invincible && perso->est_invincible % 2 != 0)
		glColor4f(1, 1, 1, 0.5);

	/* Si mario a pris l'étoile, il clignote */
	if(perso->est_invincible_etoile) {
		if(perso->est_invincible_etoile % 3 == 0)
			glColor4f(1, 0, 0, 1);
		else if(perso->est_invincible_etoile % 3 == 1)
			glColor4f(0, 1, 0, 1);
		else
			glColor4f(0, 0, 1, 1);
	}

	draw_sprite((int)perso->position.x, (int)perso->position.y, perso->taille.x, perso->taille.y, data->texture, gauche, droite, bas, haut);

	glColor4f(1, 1, 1, 1);

}

void draw_monstre(occ_monstre *monstre, Uint32 duree, Uint32 tps_transformation){

	float gauche = 0, droite = 0, haut = 0, bas = 0, temp;
	int v_anim = monstre->type_monstre->v_anim, phase, nb_etats_presents = M_NB_ETATS - m_nb_etats_absents(monstre->type_monstre);
	int nb_sprites_max = (monstre->type_monstre->nb_sprites_marche > monstre->type_monstre->nb_sprites_carapace)?monstre->type_monstre->nb_sprites_marche:monstre->type_monstre->nb_sprites_carapace;

	switch(monstre->etat) {
		case M_MARCHE : case M_SORT_DU_TUYAU:
			gauche = 0;
			droite = (float)1 / nb_sprites_max;
			haut = 1;
			bas = 1 - (float)1 / nb_etats_presents;
			v_anim = monstre->type_monstre->v_anim;
			break;
		case M_MORT_PAR_PROJ : 
			gauche = (float)1 / nb_sprites_max;
			droite = (float)2 / nb_sprites_max;
			haut = 0;
			bas = (float)1 / nb_etats_presents; // inversement du haut et bas pour renverser le sprite
			break;
		case M_MORT_PAR_SAUT: case M_RETRACTED: case M_RETRACTED_PORTED:
			gauche = 0;
			droite = (float)1 / nb_sprites_max;
			haut = 1 - (float) 1 / nb_etats_presents;
			bas = haut - (float) 1 / nb_etats_presents;
			break;
	}

	if(monstre->cote == COTE_GAUCHE && monstre->etat != M_RETRACTED)
	{
		temp = gauche;
		gauche = droite;
		droite = temp;
	}

	if(monstre->etat == M_MARCHE || monstre->etat == M_SORT_DU_TUYAU || (monstre->etat == M_RETRACTED && monstre->vitesse.x != 0))
	{

		if(!tps_transformation && monstre->etat == M_RETRACTED)
 			phase = (duree % v_anim) / (v_anim / (monstre->type_monstre->nb_sprites_carapace - 1));
		else
 			phase = (duree % v_anim) / (v_anim / (monstre->type_monstre->nb_sprites_marche));

		gauche += phase * (float)1 / nb_sprites_max;
		droite += phase * (float)1 / nb_sprites_max;

	}
	
	draw_sprite((int)monstre->position.x, (int)monstre->position.y, monstre->type_monstre->taille.x, monstre->type_monstre->taille.y, monstre->type_monstre->texture, gauche, droite, bas, haut);

}

void draw_projectile(occ_projectile* proj, Uint32 duree) {

	float gauche = 0, droite = (float) 1 / proj->type_projectile->nb_sprites_marche, haut = 0, bas = 0, temp;
	int phase = 0, i, v_anim;

	if(!proj->tps_apparition && proj->tps_vie){

		v_anim = proj->type_projectile->v_anim_marche;

		phase = (duree % v_anim) / (v_anim / proj->type_projectile->nb_sprites_marche);

		haut = 1;
		bas = (proj->type_projectile->nb_sprites_mort == 0)?0.0F:0.5F;

		gauche += phase * (float)1 / proj->type_projectile->nb_sprites_marche;
		droite += phase * (float)1 / proj->type_projectile->nb_sprites_marche;

		/* Pour le changement de côté */
		if(proj->cote == COTE_GAUCHE)
		{
			temp = gauche;
			gauche = droite;
			droite = temp;
		}

	}
	else if(proj->tps_disparition && !proj->tps_vie){

		phase = proj->tps_disparition / proj->type_projectile->nb_sprites_mort;

		haut = 0.5;
		bas = 0;

		for(i = 1; i <= proj->type_projectile->nb_sprites_mort; i++){
			if( i * proj->type_projectile->v_anim_mort / proj->type_projectile->nb_sprites_mort > phase){
				gauche = (float) (i - 1) / proj->type_projectile->nb_sprites_marche;
				droite = (float) i / proj->type_projectile->nb_sprites_marche;
				break;
			}
		}	
	}

	draw_sprite((int)proj->position.x, (int)proj->position.y, proj->type_projectile->taille.x, proj->type_projectile->taille.y, proj->type_projectile->texture, gauche, droite, bas, haut);
}

void draw_HUD(perso* p) {

	if(p->hud->personnage == MARIO)
		screen_printf_dbg("Mario x%d\n", p->hud->nb_vies);
	else
		screen_printf_dbg("Luigi x%d\n", p->hud->nb_vies);

	screen_printf_dbg("Nom du niveau : %s\n", p->hud->nom_niveau);
	screen_printf_dbg("Nombre de pieces : %d\n", p->hud->nb_pieces);
	screen_printf_dbg("Score : %d\n", p->hud->score);
	screen_printf_dbg("Temps restant : %d\n", p->hud->time);
}

void draw_item(occ_item* item, Uint32 duree) {

	int phase;
	float gauche = 0, droite = (float) 1 / item->type_item->nb_sprites;
	
	if(item->type_item->nb_sprites > 1) 
	{
		phase = (duree % 500) / (500 / (item->type_item->nb_sprites));

		gauche += phase * (float)1 / item->type_item->nb_sprites;
		droite += phase * (float)1 / item->type_item->nb_sprites;


		draw_sprite(
			(int)item->position.x, 
			(int)item->position.y,
			item->type_item->taille.x, 
			item->type_item->taille.y, 
			item->type_item->texture, 
			gauche,
			droite,
			0, 
			1);
	}
	else
	{
		draw_sprite(
			(int)item->position.x, 
			(int)item->position.y, 
			item->type_item->taille.x, 
			item->type_item->taille.y, 
			item->type_item->texture, 
			0, 
			1, 
			0, 
			1);
	}
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
		s.point_bg.x = 1;
		s.point_bg.y = 0;
		s.point_hd.x = 0;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(t->longueur * LARGEUR_BLOC);
		s.taille.y = (int)(2 * LARGEUR_BLOC);
		draw_sprite_90(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Reste du tuyau

		s.position.x = (int)((t->position.x + t->longueur) * LARGEUR_BLOC);
		s.point_bg.x = 1;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 0;
		s.point_hd.y = 1;
		s.taille.x = (int)(LARGEUR_BLOC);
		draw_sprite_90(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Haut du tuyau
		break;

	case VERS_LA_GAUCHE:
		s.point_bg.x = 0;
		s.point_bg.y = 1;
		s.point_hd.x = 1;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(LARGEUR_BLOC);
		s.taille.y = (int)(2 * LARGEUR_BLOC);
		draw_sprite_90(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Haut du tuyau

		s.position.x = (int)((t->position.x + 1) * LARGEUR_BLOC);
		s.point_bg.x = 0;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 1;
		s.point_hd.y = 0;
		s.taille.x = (int)(t->longueur * LARGEUR_BLOC);
		draw_sprite_90(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Haut du tuyau // reste du tuyau
		break;

	case VERS_LE_BAS:
		s.point_bg.x = 1;
		s.point_bg.y = 1;
		s.point_hd.x = 0;
		s.point_hd.y = 0.5F;
		s.taille.x = (int)(2 * LARGEUR_BLOC);
		s.taille.y = (int)(LARGEUR_BLOC);
		draw_sprite(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Haut du tuyau

		s.position.y = (int)((t->position.y + 1) * LARGEUR_BLOC);
		s.point_bg.x = 1;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 0;
		s.point_hd.y = 0;
		s.taille.y = (int)(t->longueur * LARGEUR_BLOC);
		draw_sprite(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Reste du tuyau
		break;

	default : // VERS_LE_HAUT
		s.point_bg.x = 0;
		s.point_bg.y = 0;
		s.point_hd.x = 1;
		s.point_hd.y = 0.5;
		s.taille.x = (int)(2 * LARGEUR_BLOC);
		s.taille.y = (int)(t->longueur * LARGEUR_BLOC);
		draw_sprite(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Reste du tuyau

		s.position.y = (int)((t->position.y + t->longueur) * LARGEUR_BLOC);
		s.point_bg.x = 0;
		s.point_bg.y = 0.5F;
		s.point_hd.x = 1;
		s.point_hd.y = 1;
		s.taille.y = (int)(LARGEUR_BLOC);
		draw_sprite(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y); // Haut du tuyau
		break;
	}
}

void draw_particules(particule_generator* generator)
{
	int i;
    sprite s = {0};
	int nb_particules_dessinees = 0;

    s.point_hd.x = s.point_hd.y = 1;
	s.taille = generator->taille_particule;
    s.text_id = generator->texture_id;
	screen_printf_dbg("nombre de particules : %d\n", generator->nb_particules);
	screen_printf_dbg("Indice de début : %d\n", generator->indice_debut);
	screen_printf_dbg("indice de fin : %d\n", generator->indice_fin);

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
			//draw_sprite_(&s, 1);
			nb_particules_dessinees++;
			draw_sprite(s.position.x, s.position.y, s.taille.x, s.taille.y, s.text_id, s.point_bg.x, s.point_hd.x, s.point_bg.y, s.point_hd.y);
        }
    }
	screen_printf_dbg("Nb particules dessinees : %d\n", nb_particules_dessinees);
	glColor4f(1, 1, 1, 1);
}

void draw_contours(ecran e)
{
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

	glBegin(GL_QUADS);
	{
		/* coin haut gauche */
		s.point_bg.y = 0.875;
		s.point_hd.y = 1;
		s.position.x = pos.x;
		s.position.y = pos.y + taille.y - taille_text;
		draw_sprite_(&s, 0);

		/* coin haut droite */
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x + taille.x - taille_text;
		draw_sprite_(&s, texture);

		/* coin bas gauche */
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x;
		s.position.y = pos.y;
		draw_sprite_(&s, texture);

		/* coin bas droite */
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x + taille.x - taille_text;
		draw_sprite_(&s, texture);

		/* haut */
		s.taille.x = taille.x - (2 * taille_text);
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x + taille_text;
		s.position.y = pos.y + taille.y - taille_text;
		draw_sprite_(&s, texture);

		/* droite */
		s.taille.x = taille_text;
		s.taille.y = taille.y - (2 * taille_text);
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x + taille.x - taille_text;
		s.position.y = pos.y + taille_text;
		draw_sprite_(&s, texture);

		/* bas */
		s.taille.x = taille.x - (2 * taille_text);
		s.taille.y = taille_text;
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x + taille_text;
		s.position.y = pos.y;
		draw_sprite_(&s, texture);

		/* gauche */
		s.taille.x = taille_text;
		s.taille.y = taille.y - (2 * taille_text);
		s.point_bg.y -= 0.125;
		s.point_hd.y -= 0.125;
		s.position.x = pos.x;
		s.position.y = pos.y + taille_text;
		draw_sprite_(&s, texture);
	}
	glEnd();
}
