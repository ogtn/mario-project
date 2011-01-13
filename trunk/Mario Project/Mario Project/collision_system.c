/*============================================================================*/
/*== [collision_system.c] [Version: 0.028a]                                 ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "collision_system.h"

void main_collisions(world *w)
{
	id i;
	int j, diff_x, diff_y, nb_images_inter;
	perso* p_bis = NULL;
	occ_monstre* m_bis = NULL;
	occ_projectile* proj_bis = NULL;
	occ_item *i_bis = NULL;

	/* PARTIE TUYAUX */
	for(i = 0; i < w->niveau->nb_tuyaux; i++)
		MAJ_tuyau(w->niveau, w->niveau->tuyaux[i], w->temps_ecoule);

	/* PARTIE PROJECTILES */
	for(i = 0; i < w->niveau->nb_projectiles; i++)
	{
		elem_projectile *proj_actuel = w->niveau->projectiles[i]->occ_projectiles->projectile, *proj_prec;

		while(proj_actuel != NULL){

			/* Si le projectile a fini son animation de mort, on le supprime */
			if(proj_actuel->occ_projectile->tps_disparition == 0) {
				proj_prec = proj_actuel;
				proj_actuel = proj_actuel->suivant;
				supprime_projectile(w->niveau->projectiles[i]->occ_projectiles, proj_prec->occ_projectile);
			}
			else
			{
				proj_bis = copy_projectile(proj_actuel->occ_projectile);

				MAJ_collision_projectile(proj_bis, w->ecran, w->temps_ecoule);

				solve_collisions_projectile(proj_bis, w->niveau);

				diff_x = abs((int)proj_bis->position_prec.x - (int)proj_bis->position.x);
				diff_y = abs((int)proj_bis->position_prec.y - (int)proj_bis->position.y);

				/* Teste si il y a plus de pixels d'écart entre les 2 positions */
				if(diff_x < w->niveau->taille_blocs.x && diff_y < w->niveau->taille_blocs.y)
				{
					/* Si non, on laisse comme c'était */
					*proj_actuel->occ_projectile = *proj_bis;

				}
				else
				{

					/* Si oui, on stocke la différence */
					nb_images_inter = max(diff_x / w->niveau->taille_blocs.x, diff_y / w->niveau->taille_blocs.y);

					/* Pour chaque image intermédiaire de chaque perso, on teste si il n'y a pas eu de collision entre temps */
					for(j = 0; j <= nb_images_inter; j++)
					{
						MAJ_collision_projectile(proj_actuel->occ_projectile, w->ecran, w->temps_ecoule / (nb_images_inter + 1));

						/* Fonction résolvant les collisions */
						solve_collisions_projectile(proj_actuel->occ_projectile, w->niveau);
					}
				}

				free(proj_bis);
				proj_actuel = proj_actuel->suivant;
			}
		}
	}

	/* PARTIE PERSONNAGE */
	for(i = 0; i < w->nb_persos; i++) {

		MAJ_HUD(w->persos[i], w->temps_ecoule, w->ecran);

		if(w->persos[i]->etat == MORT && w->persos[i]->position.y < 0)
		{
			if(w->persos[i]->passe_checkpoint)
			{
				w->persos[i]->position.x = (float)(w->niveau->checkpoint.x);
				w->persos[i]->position.y = (float)(w->niveau->checkpoint.y);
			}
			else
			{
				w->persos[i]->position.x = (float)(w->niveau->spawn.x);
				w->persos[i]->position.y = (float)(w->niveau->spawn.y);
			}
			w->persos[i]->etat = DEBOUT;
		}
		else if(w->persos[i]->etat == MORT) 
		{
			MAJ_collision_perso(w->persos[i], w->niveau, w->keystate, w->temps_ecoule);
		}
		else
		{
			p_bis = copy_perso(w->persos[i]);

			MAJ_collision_perso(p_bis, w->niveau, w->keystate, w->temps_ecoule);

			/* Fonction résolvant les collisions sur la copie de Mario */
			solve_collisions_perso(p_bis, w->niveau, w->keystate);

			diff_x = abs((int)p_bis->position_prec.x - (int)p_bis->position.x);
			diff_y = abs((int)p_bis->position_prec.y - (int)p_bis->position.y);

			/* Teste si il y a plus de pixels d'écart entre les 2 positions */
			if(diff_x < w->niveau->taille_blocs.x && diff_y < w->niveau->taille_blocs.y)
			{
				/* Si non, on laisse comme c'était */
				*w->persos[i] = *p_bis;
			}
			else
			{
				/* Si oui, on stocke la différence */
				nb_images_inter = max(diff_x / w->niveau->taille_blocs.x, diff_y / w->niveau->taille_blocs.y);

				/* Pour chaque image intermédiaire de chaque perso, on teste si il n'y a pas eu de collision entre temps */
				for(j = 0; j <= nb_images_inter; j++)
				{
					MAJ_collision_perso(w->persos[i], w->niveau, w->keystate, w->temps_ecoule / (nb_images_inter + 1));

					/* Fonction résolvant les collisions */
					solve_collisions_perso(w->persos[i], w->niveau, w->keystate);
				}
			}
			free_copy_perso(p_bis);
		}
	}

	/* PARTIE MONSTRE */
	for(i = 0; i < w->niveau->nb_monstres; i++)
	{
		elem_monstre *mstr_actuel = w->niveau->monstres[i]->occ_monstres->monstre, *mstr_prec;

		while(mstr_actuel != NULL)
		{

			if(mstr_actuel->occ_monstre->position.y + mstr_actuel->occ_monstre->type_monstre->taille.y < 0
				|| (mstr_actuel->occ_monstre->etat == M_MORT_PAR_SAUT && mstr_actuel->occ_monstre->tps_disparition == 0)) {
					mstr_prec = mstr_actuel;
					mstr_actuel = mstr_actuel->suivant;
					supprime_monstre(w->niveau->monstres[i]->occ_monstres, mstr_prec->occ_monstre);
			}
			else
			{

				m_bis = copy_monstre(mstr_actuel->occ_monstre);

				if(m_bis->etat != M_RETRACTED_PORTED)
					MAJ_collision_monstre(m_bis, w->ecran, w->temps_ecoule);

				/* Fonction résolvant les collisions sur la copie du monstre */
				if(m_bis->etat != M_MORT_PAR_PROJ)
					solve_collisions_monstre(m_bis, mstr_actuel->occ_monstre, w->persos[0], w->niveau, w->temps_ecoule);

				diff_x = abs((int)m_bis->position_prec.x - (int)m_bis->position.x);
				diff_y = abs((int)m_bis->position_prec.y - (int)m_bis->position.y);

				/* Teste si il y a plus de pixels d'écart entre les 2 positions */
				if(diff_x < w->niveau->taille_blocs.x && diff_y < w->niveau->taille_blocs.y)
				{
					/* Si non, on laisse comme c'était */
					*mstr_actuel->occ_monstre = *m_bis;
				}
				else
				{
					/* Si oui, on stocke la différence */
					nb_images_inter = max(diff_x / w->niveau->taille_blocs.x, diff_y / w->niveau->taille_blocs.y);

					/* Pour chaque image intermédiaire de chaque perso, on teste si il n'y a pas eu de collision entre temps */
					for(j = 0; j <= nb_images_inter; j++)
					{
						MAJ_collision_monstre(mstr_actuel->occ_monstre, w->ecran, w->temps_ecoule / (nb_images_inter + 1));

						/* Fonction résolvant les collisions */
						if(mstr_actuel->occ_monstre->etat != M_MORT_PAR_PROJ)
							solve_collisions_monstre(mstr_actuel->occ_monstre, NULL, w->persos[0], w->niveau, w->temps_ecoule);
					}
				}
				mstr_actuel = mstr_actuel->suivant;
				free_copy_monstre(m_bis);
			}
		}
	}

	/* PARTIE ITEMS */
	for(i = 0; i < w->niveau->nb_items; i++)
	{
		elem_item *item_actuel = w->niveau->items[i]->occ_items->item, *precedent = NULL;
		while(item_actuel != NULL)
		{
			int item_suppr;
			i_bis = item_actuel->occ_item;

			item_suppr = MAJ_collision_item(i_bis, w->ecran, w->temps_ecoule);

			/* Si l'item a été supprimé, on revient au précédent dans la liste */
			if(item_suppr)
			{
				item_actuel = precedent;
			}
			else
			{
				solve_collisions_item(i_bis, w->persos, w->niveau, w->temps_ecoule);

				diff_x = abs((int)i_bis->position_prec.x - (int)i_bis->position.x);
				diff_y = abs((int)i_bis->position_prec.y - (int)i_bis->position.y);

				/* Teste si il y a beaucoup de pixels d'écart entre les 2 positions */
				if(diff_x < w->niveau->taille_blocs.x && diff_y < w->niveau->taille_blocs.y)
				{
					/* Si non, on laisse comme c'était */
					*item_actuel->occ_item = *i_bis;

				}
				else
				{

					/* Si oui, on stocke la différence */
					nb_images_inter = max(diff_x / w->niveau->taille_blocs.x, diff_y / w->niveau->taille_blocs.y);

					/* Pour chaque image intermédiaire de chaque perso, on teste si il n'y a pas eu de collision entre temps */
					for(j = 0; j <= nb_images_inter; j++)
					{
						MAJ_collision_item(item_actuel->occ_item, w->ecran, w->temps_ecoule / (nb_images_inter + 1));

						/* Fonction résolvant les collisions */
						solve_collisions_item(item_actuel->occ_item, w->persos, w->niveau, w->temps_ecoule);
					}
				}
			}
			precedent = item_actuel;
			if(precedent != NULL)
				item_actuel = item_actuel->suivant;
		}
	}
}

void MAJ_tuyau(niveau* n, tuyau *t, Uint32 duree)
{
	if(t->index_monstre > 0)
	{
		if(t->tps_sortie_monstre <= 0) {
			occ_monstre* occ_m = NULL;

			switch(t->sens_sortie)
			{
			case VERS_LE_HAUT :
				occ_m = new_occ_monstre((float)t->position.x * LARGEUR_BLOC + LARGEUR_BLOC / 2, (float)t->position.y * LARGEUR_BLOC + t->longueur * LARGEUR_BLOC, n->monstres[t->index_monstre]);
				occ_m->vitesse.y = V_SORTIE_MONSTRE;
				occ_m->vitesse.x = 0;
				break;
			case VERS_LE_BAS :
				occ_m = new_occ_monstre((float)t->position.x * LARGEUR_BLOC + LARGEUR_BLOC / 2, (float)t->position.y * LARGEUR_BLOC, n->monstres[t->index_monstre]);
				occ_m->vitesse.y = -V_SORTIE_MONSTRE;
				occ_m->vitesse.x = 0;
				break;
			case VERS_LA_DROITE :
				occ_m = new_occ_monstre((float)(t->position.x + t->longueur) * LARGEUR_BLOC, (float)t->position.y * LARGEUR_BLOC, n->monstres[t->index_monstre]);
				occ_m->vitesse.x = V_SORTIE_MONSTRE;
				occ_m->vitesse.y = 0;
				occ_m->cote = COTE_DROIT;
				break;
			case VERS_LA_GAUCHE :
				occ_m = new_occ_monstre((float)t->position.x * LARGEUR_BLOC, (float)t->position.y * LARGEUR_BLOC, n->monstres[t->index_monstre]);
				occ_m->vitesse.x = -V_SORTIE_MONSTRE;
				occ_m->vitesse.y = 0;
				occ_m->cote = COTE_GAUCHE;
				break;
			default :
				break;
			}

			occ_m->etat = M_SORT_DU_TUYAU;
			occ_m->tps_sortie_tuyau = occ_m->type_monstre->tps_sortie_tuyau;

			occ_m->type_monstre->occ_monstres = ajout_monstre(occ_m->type_monstre->occ_monstres, occ_m);

			t->tps_sortie_monstre = TPS_SORTIE_MONSTRE;
		}
		else
		{
			t->tps_sortie_monstre -= duree;
		}
	}
}

void MAJ_HUD(perso *p, Uint32 duree, ecran e) {

	file_pts* file;
	coordi position;

	/* MAJ des temps d'affichage, s'il y a lieu */
	file = p->hud->file_points;
	while(file != NULL) {
		position.x = file->element->pos_points.x - e.scroll.x;
		position.y = file->element->pos_points.y - e.scroll.y;
		if(file->element->points == 10000)
			screen_printf(position, p->hud->font, COLOR_WHITE, "1UP");
		else
			screen_printf(position, p->hud->font, COLOR_WHITE, "%d", file->element->points);

		file->element->tps_affichage -= duree;
		file = file->suivant;
	}

	/* Suppression de certains scores si besoin */
	p->hud->file_points = free_file_pts(p->hud->file_points);

	if(p->etat == MORT && p->position.y < 0)
		p->hud->nb_vies--;
}

void MAJ_collision_projectile(occ_projectile* projectile, ecran e, Uint32 duree) {

	pause_projectile(projectile, e);

	if(projectile->actif)
	{
		/* Voir pour la gravité en fonction du type de projectile */
		if(projectile->type_projectile->soumission & SOUMIS_GRAVITE)
			gravity(&projectile->vitesse, duree);


		/* MAJ des temps d'apparition, de disparition, de vie */
		if(projectile->tps_apparition){
			projectile->tps_apparition -= duree;
			if(projectile->tps_apparition > pow(2, 31))
				projectile->tps_apparition = 0;
		}
		else if(projectile->tps_vie)
		{

			/* Sauvegarde de la position precedente */
			projectile->position_prec.x = projectile->position.x;
			projectile->position_prec.y = projectile->position.y;

			/* Mise à jour des positions à partir de la vitesse */
			projectile->position.x += projectile->vitesse.x * duree;
			projectile->position.y += projectile->vitesse.y * duree;

			/* ça veut dire que le projectile est apparut */
			projectile->tps_vie -= duree;
			if(projectile->tps_vie > pow(2, 31))
				projectile->tps_vie = 0;
		}
		else 
		{
			projectile->tps_disparition -= duree;
			if(projectile->tps_disparition > pow(2, 31))
				projectile->tps_disparition = 0;
		}
	}
}

void MAJ_collision_monstre(occ_monstre* monstre, ecran e, Uint32 duree) {

	pause_monstre(monstre, e);

	if(monstre->actif)
	{
		if(monstre->etat == M_SORT_DU_TUYAU)
		{
			/* Sauvegarde de la position precedente */
			monstre->position_prec.x = monstre->position.x;
			monstre->position_prec.y = monstre->position.y;

			/* Mise à jour des positions à partir de la vitesse */
			monstre->position.x += monstre->vitesse.x * duree;
			monstre->position.y += monstre->vitesse.y * duree;

			monstre->tps_sortie_tuyau -= duree;
			if(monstre->tps_sortie_tuyau < 0) {
				monstre->tps_sortie_tuyau = 0;
				monstre->etat = M_MARCHE;
				monstre->vitesse.x = (monstre->vitesse.x < 0)?-M_V_MARCHE:M_V_MARCHE;
			}
		}
		else if(monstre->etat != M_MORT_PAR_SAUT) {

			/* Gravité */
			gravity(&monstre->vitesse, duree);

			if(monstre->etat != M_RETRACTED)
			{
				if(monstre->vitesse.x > M_V_MARCHE)
					monstre->vitesse.x = M_V_MARCHE;
				else if(monstre->vitesse.x < -M_V_MARCHE)
					monstre->vitesse.x = -M_V_MARCHE;
			}

			/* Sauvegarde de la position precedente */
			monstre->position_prec.x = monstre->position.x;
			monstre->position_prec.y = monstre->position.y;

			/* Mise à jour des positions à partir de la vitesse */
			monstre->position.x += monstre->vitesse.x * duree;
			monstre->position.y += monstre->vitesse.y * duree;
		} 
		else
		{
			monstre->tps_disparition -= duree;
			if(monstre->tps_disparition < 0)
				monstre->tps_disparition = 0;
		}
	}
}

int MAJ_collision_item(occ_item* item, ecran e, Uint32 duree) {

	pause_item(item, e);

	if(item->actif)
	{
		/* Test si l'item est sorti du bloc */
		if(item->etat == NORMAL)
		{
			/* Gravité */
			if(item->type_item->soumission & SOUMIS_GRAVITE)
				gravity(&item->vitesse, duree);
		}
		else
		{
			if(item->tps_sortie_bloc <= 0)
			{
				if(item->type_item->nom == PIECE)
				{
					supprime_item(item->type_item->occ_items, item);
					return 1;
				}
				else
				{
					item->etat = NORMAL;
					item->vitesse.x = item->type_item->vitesse.x;
					item->vitesse.y = item->type_item->vitesse.y;
				}
			}
			else
			{
				item->tps_sortie_bloc -= duree;
			}
		}

		/* Sauvegarde de la position precedente */
		item->position_prec.x = item->position.x;
		item->position_prec.y = item->position.y;

		/* Mise à jour des positions à partir de la vitesse */
		item->position.x += item->vitesse.x * duree;
		item->position.y += item->vitesse.y * duree;
	}
	return 0;
}

void MAJ_collision_perso(perso *perso, niveau* lvl, keystate* keystate, Uint32 duree){

	occ_projectile *p = NULL;

	if(perso->etat != MORT && perso->etat != RENTRE_TUYAU_HORIZONTAL && perso->etat != SORT_TUYAU_HORIZONTAL
		&& perso->etat != RENTRE_TUYAU_VERTICAL && perso->etat != SORT_TUYAU_VERTICAL)
	{
		/* Determination de l'acceleration en fonction de l'environement */
		solve_acc(perso, keystate);

		/* Deplacements lateraux, determination de la vitesse */
		if(perso->etat != POUSSE_CARAPACE)
			lateral_move(perso, keystate, duree);

		/* Limitation de la vitesse */
		limit_speed(perso, keystate);

		/* Attaquer => ajout du nouveau projectile */
		if(keystate->actuel[RUN] && !keystate->actuel[BAS] && !keystate->precedent[RUN]
		&& perso->transformation >= FIRE_MARIO && perso->etat != DERAPE 
			&& perso->etat != REGARDE_HAUT && !perso->tps_attaque)
			throw_projectile_perso(perso, lvl, p);

		/* Sauter */
		if(keystate->actuel[SAUTER] && !keystate->precedent[SAUTER] && !keystate->actuel[HAUT]
		&& perso->environnement == SOL_DUR && perso->etat != FINISH && perso->etat != FINISH_CHATEAU)
			jump_perso(perso);

		if(perso->vitesse.y != 0)
			perso->environnement = AIR;

		if(perso->vitesse.y < 0 && (perso->etat == SAUT || perso->etat == ATTAQUE))
			perso->etat = SAUT_DESCENDANT;

		/* S'asseoir -> vérifier si la touche BAS est pressée, si l'environnement est SOL_DUR et si le bloc sur lequel se trouve
		Mario est une pente ensuite on le dirigera selon le coté de la pente */

		/* Se baisser */
		if(perso->monstre_porte == NULL)
		{
			if(keystate->actuel[SAUTER] 
			&& keystate->precedent[BAS] 
			&& perso->environnement == SOL_DUR)
				perso->etat = SAUT_BAISSE;
			else if(keystate->actuel[BAS] 
			&&  perso->environnement == SOL_DUR && 
				(perso->etat == DEBOUT || perso->etat == MARCHE
				|| perso->etat == COURSE_1 || perso->etat == COURSE_2
				|| perso->etat == DERAPE))
				perso->etat = BAISSE;
		}
		else
		{
			if(keystate->actuel[SAUTER] && keystate->actuel[BAS] 
			&& perso->environnement == SOL_DUR)
				perso->etat = SAUT_BAISSE_CARAPACE;
			else if(keystate->actuel[BAS] && perso->environnement == SOL_DUR)
				perso->etat = BAISSE_CARAPACE;
		}

		/* Regarder en haut */
		if(perso->monstre_porte == NULL)
		{
			if(keystate->actuel[HAUT]
			&& perso->environnement == SOL_DUR
				&& perso->etat == DEBOUT)
				perso->etat = REGARDE_HAUT;
			else if(keystate->actuel[HAUT] && keystate->actuel[BAS])
				perso->etat = DEBOUT;
		}
		else
		{
			if(keystate->actuel[HAUT] && perso->environnement == SOL_DUR
				&& perso->etat == DEBOUT_CARAPACE)
				perso->etat = REGARDE_HAUT_CARAPACE;
			else if(keystate->actuel[HAUT] && keystate->actuel[BAS])
				perso->etat = DEBOUT_CARAPACE;
		}

		/* Mise à jour de la position du monstre si le personnage en porte un */
		if(perso->monstre_porte != NULL)
		{
			perso->monstre_porte->position_prec.x = perso->monstre_porte->position.x;
			perso->monstre_porte->position_prec.y = perso->monstre_porte->position.y;

			if(perso->cote == COTE_DROIT)
				perso->monstre_porte->position.x = perso->position.x + perso->taille.x - perso->monstre_porte->type_monstre->taille.x / 2.5F;
			else
				perso->monstre_porte->position.x = perso->position.x - perso->monstre_porte->type_monstre->taille.x + perso->monstre_porte->type_monstre->taille.x / 1.25F;

			perso->monstre_porte->position.y = perso->position.y + perso->texture_act->ordonnee_haut / 2;

			if(perso->etat == BAISSE_CARAPACE || perso->etat == SAUT_BAISSE_CARAPACE)
				perso->monstre_porte->position.y = perso->position.y + perso->taille.x / 4;
		}

		/* Si Mario est invincible, on décrémente son temps d'invincibilité */
		if(perso->est_invincible > 0){
			perso->est_invincible -= duree;
			if(perso->est_invincible > pow(2, 31))
				perso->est_invincible = 0;
		}

		/* Si Mario est invincible, on décrémente son temps d'invincibilité */
		if(perso->est_invincible_etoile > 0){
			perso->est_invincible_etoile -= duree;
			if(perso->est_invincible_etoile > pow(2, 31)) {
				perso->est_invincible_etoile = 0;
				FSOUND_StopSound(SND_INVINCIBLE);
			}
		}

		/* Si Mario effectue son attaque spéciale, on décrémente son temps */
		if(perso->tps_attaque_speciale > 0){
			perso->tps_attaque_speciale -= duree;
			if(perso->tps_attaque_speciale > pow(2, 31)
				|| perso->tps_attaque_speciale == 0)
				perso->tps_attaque_speciale = 0;
		}

		/* Idem, pour son attaque basique, s'il en a une */
		if(perso->tps_attaque > 0){
			perso->tps_attaque -= duree;
			if(perso->tps_attaque > pow(2, 31) || perso->tps_attaque == 0)
				perso->tps_attaque = 0;
		}

		/* Idem, lorsqu'il pousse un objet */
		if(perso->tps_pousse_carapace > 0){
			perso->tps_pousse_carapace -= duree;

			if(perso->tps_pousse_carapace > pow(2,31) || perso->tps_pousse_carapace == 0)
			{
				perso->etat = DEBOUT;
				perso->tps_pousse_carapace = 0;
			}
		}

		/* Lorsque Mario termine el niveau */
		if(perso->tps_finish > 0){
			perso->tps_finish -= duree;
			if(perso->tps_finish > pow(2, 31) || perso->tps_finish == 0)
				perso->tps_finish = 0;
		}
	}


	/* Gravité */
	if(perso->etat != RENTRE_TUYAU_HORIZONTAL && perso->etat != SORT_TUYAU_HORIZONTAL
		&& perso->etat != RENTRE_TUYAU_VERTICAL && perso->etat != SORT_TUYAU_VERTICAL)
		gravity(&perso->vitesse, duree);
	else
	{
		/* Si Mario rentre dans un tuyau vertical */
		if(perso->etat == RENTRE_TUYAU_VERTICAL) {
			switch(perso->tuyau->sens_sortie)
			{
			case VERS_LE_HAUT:
				if(perso->position.y + perso->taille.y <= perso->tuyau->position.y * LARGEUR_BLOC + (perso->tuyau->longueur + 1) * LARGEUR_BLOC)
					MAJ_perso_tuyau(perso, lvl);
				break;

			case VERS_LE_BAS:
				if(perso->position.y >= perso->tuyau->position.y * LARGEUR_BLOC)
					MAJ_perso_tuyau(perso, lvl);
				break;
			default:
				break;
			}
		}

		if(perso->etat == RENTRE_TUYAU_HORIZONTAL)
		{
			switch(perso->tuyau->sens_sortie)
			{
			case VERS_LA_GAUCHE:
				if(perso->position.x >= perso->tuyau->position.x * LARGEUR_BLOC)
					MAJ_perso_tuyau(perso, lvl);
				break;

			case VERS_LA_DROITE:
				if(perso->position.x + perso->texture_act->abscisse_bas <= (perso->tuyau->position.x + perso->tuyau->longueur) * LARGEUR_BLOC)
					MAJ_perso_tuyau(perso, lvl);
				break;
			default:
				break;
			}
		}

		/* Si Mario est en train de sortir d'un tuyau vertical */
		if(perso->etat == SORT_TUYAU_VERTICAL) {
			switch(perso->tuyau->sens_sortie)
			{
			case VERS_LE_HAUT:
				perso->vitesse.y = VIT_SORTIE_BLOC;
				perso->vitesse.x = 0;
				if(perso->position.y > perso->tuyau->position.y * LARGEUR_BLOC + (perso->tuyau->longueur + 1) * LARGEUR_BLOC)
					perso->etat = DEBOUT;
				break;

			case VERS_LE_BAS:
				perso->vitesse.y = -VIT_SORTIE_BLOC;
				perso->vitesse.x = 0;
				if(perso->position.y + perso->texture_act->ordonnee_haut < perso->tuyau->position.y * LARGEUR_BLOC)
					perso->etat = DEBOUT;
				break;
			default:
				break;
			}			
		}

		/* Si Mario est en train de sortir d'un tuyau horizontal */
		if(perso->etat == SORT_TUYAU_HORIZONTAL) {
			switch(perso->tuyau->sens_sortie)
			{
			case VERS_LA_GAUCHE:
				perso->vitesse.x = -VIT_SORTIE_BLOC;
				perso->vitesse.y = 0;
				if(perso->position.x + perso->taille.x - perso->texture_act->abscisse_bas < perso->tuyau->position.x * LARGEUR_BLOC)
					perso->etat = DEBOUT;
				break;

			case VERS_LA_DROITE:
				perso->vitesse.x = VIT_SORTIE_BLOC;
				perso->vitesse.y = 0;
				if(perso->position.x + perso->texture_act->abscisse_bas > (perso->tuyau->position.x + perso->tuyau->longueur + 1) * LARGEUR_BLOC)
					perso->etat = DEBOUT;
				break;
			default:
				break;
			}			
		}
	}

	/* Sauvegarde de la position precedente */
	perso->position_prec.x = perso->position.x;
	perso->position_prec.y = perso->position.y;

	/* Mise à jour des positions à partir de la vitesse */
	perso->position.x += perso->vitesse.x * duree;
	perso->position.y += perso->vitesse.y * duree;

	if(perso->position.x > lvl->checkpoint.x)
		perso->passe_checkpoint = VRAI;
}

void MAJ_perso_tuyau(perso* perso, niveau* lvl)
{
	perso->tuyau = lvl->tuyaux[perso->tuyau->pipe_dest];

	/* Calcul de la position d'arrivée en fonction du sens de sortie du tuyau d'arrivée */
	switch(perso->tuyau->sens_sortie)
	{
	case VERS_LE_HAUT:
		perso->etat = SORT_TUYAU_VERTICAL;
		perso->position.x = perso->tuyau->position.x * LARGEUR_BLOC + LARGEUR_BLOC - perso->taille.x / 2;
		perso->position.y = perso->tuyau->position.y * LARGEUR_BLOC + (perso->tuyau->longueur + 1) * LARGEUR_BLOC - perso->taille.y;
		break;
	case VERS_LE_BAS:
		perso->etat = SORT_TUYAU_VERTICAL;
		perso->position.x = perso->tuyau->position.x * LARGEUR_BLOC + LARGEUR_BLOC - perso->taille.x / 2;
		perso->position.y = perso->tuyau->position.y * LARGEUR_BLOC;
		break;
	case VERS_LA_DROITE:
		perso->cote = COTE_DROIT;
		perso->etat = SORT_TUYAU_HORIZONTAL;
		perso->position.x = (perso->tuyau->position.x + perso->tuyau->longueur) * LARGEUR_BLOC;
		perso->position.y = perso->tuyau->position.y * LARGEUR_BLOC;
		break;
	case VERS_LA_GAUCHE:
		perso->cote = COTE_GAUCHE;
		perso->etat = SORT_TUYAU_HORIZONTAL;
		perso->position.x = perso->tuyau->position.x * LARGEUR_BLOC;
		perso->position.y = perso->tuyau->position.y * LARGEUR_BLOC;
		break;

	default:
		break;
	}
	FSOUND_PlaySound(FSOUND_FREE, perso->sons[SND_PIPE]);
}

void throw_projectile_perso(perso *perso, niveau* lvl, occ_projectile *p)
{
	
	perso->etat = ATTAQUE;
	
	// Le tps d'attaque doit être proportionnel aux nombre de sprites de l'attaque
	perso->tps_attaque = perso->texture_act->v_anim[ATTAQUE] * perso->texture_act->nb_sprites[ATTAQUE]; 

	/* Condition à remplir pour une attaque spéciale ?? */
	//perso->etat = ATTAQUE_SPECIALE;
	//
	//// Le tps d'attaque doit être proportionnel aux nombre de sprites de l'attaque
	//perso->tps_attaque_speciale = perso->texture_act->v_anim[ATTAQUE_SPECIALE] * perso->texture_act->nb_sprites[ATTAQUE_SPECIALE]; 

	FSOUND_PlaySound(FSOUND_FREE, perso->sons[SND_FIREBALL]);


	p = new_occ_projectile(lvl->projectiles[FIREBALL]->tps_vie, lvl->projectiles[FIREBALL]->tps_apparition, lvl->projectiles[FIREBALL]->tps_disparition);

	p->type_projectile = lvl->projectiles[FIREBALL];

	p->envoyeur = GENTIL;

	p->cote = perso->cote;
	p->position.y = perso->position.y + perso->texture_act->ordonnee_haut / 4;

	// Calcul de la position et de la vitesse initiales du projectile
	// Le projectile se situera derrière la main du perso
	if(p->cote == COTE_GAUCHE)
	{
		p->vitesse.x = -p->type_projectile->vitesse.x;
		p->position.x = perso->position.x;
	}
	else
	{
		p->vitesse.x = p->type_projectile->vitesse.x;
		p->position.x = perso->position.x + perso->taille.x - p->type_projectile->taille.x + p->type_projectile->abscisse_bas;
	}

	p->position_prec.x = p->position.x;
	p->position_prec.y = p->position.y;


	// Ajout du projectile dans l'environnement du niveau
	p->type_projectile->occ_projectiles = ajout_projectile(p->type_projectile->occ_projectiles, p);
}

void solve_collisions_perso(perso* p, niveau *n, keystate* keystate)
{
	/* Variables pour l'initialisation */
	coordi bloc_bg, bloc_hd;
	int i, j;

	/* Variables pour la detection et la résolution de collisions */
	int phys_bloc_actuel, ordonnee_haut;
	float hauteur;
	carre perso = {0}, block = {0}, monstre = {0}, projectile = {0}, item = {0}, tuyau = {0}, finish = {0};
	collision collision;

	/* Variable pour l'affichage des points */
	//coordi text_points;

	/************************************************* DETECTIONS ET RESOLUTIONS DE COLLISIONS *******************************************/

	/***** Collision de base avec les bords du niveau ******/
	if(p->position.x + p->texture_act->abscisse_bas < 0)
		p->position.x = (float)-p->texture_act->abscisse_bas;

	if(p->position.x + p->taille.x > n->taille.x * n->taille_blocs.x)
		p->position.x = (float)(n->taille.x * n->taille_blocs.x) - p->taille.x;

	if(p->position.y + p->texture_act->ordonnee_haut < 0)
		p->etat = MORT;

	if(p->etat == BAISSE || p->etat == SAUT_BAISSE || p->etat == SAUT_BAISSE_CARAPACE)
		ordonnee_haut = (int)(p->texture_act->ordonnee_haut / 2.0);
	else
		ordonnee_haut = p->texture_act->ordonnee_haut;

	/* Initialisation des blocs sur lequels se trouve le personnage */
	bloc_bg.x = (int) (p->position.x + p->texture_act->abscisse_bas) / n->taille_blocs.x;
	bloc_bg.y = (int) p->position.y / n->taille_blocs.y;

	bloc_hd.x = (int) (p->position.x + p->taille.x - p->texture_act->abscisse_bas) / n->taille_blocs.x;
	bloc_hd.y = (int) (p->position.y + ordonnee_haut) / n->taille_blocs.y;

	/* Initialisation du carre perso */
	perso.position.x = p->position.x + p->texture_act->abscisse_bas;
	perso.position.y = p->position.y;

	perso.position_prec.x = p->position_prec.x + p->texture_act->abscisse_bas;
	perso.position_prec.y = p->position_prec.y;

	perso.taille.x = p->taille.x - 2 * p->texture_act->abscisse_bas;
	perso.taille.y = ordonnee_haut;

	perso.est_bloc_pente = FAUX;

	if(p->etat != MORT && p->etat != RENTRE_TUYAU_HORIZONTAL && p->etat != SORT_TUYAU_HORIZONTAL
		&& p->etat != RENTRE_TUYAU_VERTICAL && p->etat != SORT_TUYAU_VERTICAL) 
	{
			/************ COLLISIONS PERSO <=> NIVEAU ***************/
			for(i = bloc_bg.x; i <= bloc_hd.x; i++)
			{
				for(j = bloc_bg.y; j <= bloc_hd.y; j++)
				{
					if(i >= 0 && j >= 0)
					{
						if(n->occ_blocs[i][j]->bloc_actuel >= 0)
						{
							bloc bloc_actuel = n->blocs[n->occ_blocs[i][j]->bloc_actuel];

							/* On récupère la physique du bloc */
							phys_bloc_actuel = bloc_actuel.phys;

							block.position.x = (float)i * n->taille_blocs.x;
							block.position.y = (float)j * n->taille_blocs.y;

							block.position_prec.x = block.position.x;
							block.position_prec.y = block.position.y;

							block.taille.x = n->taille_blocs.x;
							block.taille.y = n->taille_blocs.y;

							find_angle_height_with_phys(phys_bloc_actuel, &block);

							block.est_bloc_pente = (block.angle_pente != 0);

							determinate_collision(perso, block, &collision);

							/* Collision avec le sol */
							if((phys_bloc_actuel == SOL 
								|| phys_bloc_actuel == BORD_A_GAUCHE
								|| phys_bloc_actuel == BORD_A_DROITE
								|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
								|| phys_bloc_actuel == COIN_HAUT_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC)
								&& collision.type_collision == PAR_LE_BAS)
							{

								// Si le perso est en train d'attaquer, on ne change pas son etat 
								if(!p->tps_attaque_speciale && !p->tps_attaque)
								{
									if(p->monstre_porte == NULL)
									{
										if(p->etat == SAUT_BAISSE)
											p->etat = BAISSE;
										else if(p->etat == SAUT || p->etat == SAUT_DESCENDANT) {
											p->etat = DEBOUT;
											p->hud->nb_monstres_tues = 0;
										}
									}
									else
									{
										if(p->etat == SAUT_BAISSE_CARAPACE)
											p->etat = BAISSE_CARAPACE;
										else if(p->etat == SAUT_CARAPACE) {
											p->etat = DEBOUT_CARAPACE;
											p->hud->nb_monstres_tues = 0;
										}
									}
								}


								p->position.y = (float)block.position.y + block.taille.y;

								p->environnement = SOL_DUR;
								p->vitesse.y = 0;

								// MAJ du carré perso
								perso.position.y = p->position.y;
								determinate_collision(perso, block, &collision);
							}

							/* Collision avec les pentes */
							if((phys_bloc_actuel == PENTE_15_DROITE_16
								|| phys_bloc_actuel == PENTE_15_GAUCHE_16
								|| phys_bloc_actuel == PENTE_15_DROITE_12
								|| phys_bloc_actuel == PENTE_15_GAUCHE_12
								|| phys_bloc_actuel == PENTE_15_DROITE_8
								|| phys_bloc_actuel == PENTE_15_GAUCHE_8
								|| phys_bloc_actuel == PENTE_15_DROITE_4
								|| phys_bloc_actuel == PENTE_15_GAUCHE_4

								|| phys_bloc_actuel == PENTE_30_DROITE_0
								|| phys_bloc_actuel == PENTE_30_DROITE_8
								|| phys_bloc_actuel == PENTE_30_GAUCHE_8
								|| phys_bloc_actuel == PENTE_30_GAUCHE_16
								|| phys_bloc_actuel == PENTE_45_DROITE
								|| phys_bloc_actuel == PENTE_45_GAUCHE)
								&& 
								(collision.type_collision == PENTE_PAR_LE_BAS
								|| collision.type_collision == PENTE_PAR_LA_GAUCHE
								|| collision.type_collision == PENTE_PAR_LA_DROITE))
							{

								/* Test si le perso est en train d'attaquer */
								if(!p->tps_attaque_speciale && !p->tps_attaque)
								{
									if(p->monstre_porte == NULL)
									{
										if(p->etat == SAUT_BAISSE)
											p->etat = BAISSE;
										else if(p->etat == SAUT || p->etat == SAUT_DESCENDANT) {
											p->hud->nb_monstres_tues = 0;
											p->etat = DEBOUT;
										}
									}
									else
									{
										if(p->etat == SAUT_BAISSE_CARAPACE)
											p->etat = BAISSE_CARAPACE;
										else if(p->etat == SAUT_CARAPACE) {
											p->etat = DEBOUT_CARAPACE;
											p->hud->nb_monstres_tues = 0;
										}
									}
								}

								/* Calcul de la nouvelle ordonnée du personnage 
								avec l'équation de la pente et en fonction du bloc */
								if (phys_bloc_actuel == PENTE_45_GAUCHE)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(135.0F)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(135.0F)));
								}
								else if(phys_bloc_actuel == PENTE_45_DROITE)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(45.0F)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(45.0F)));
								}
								else if(phys_bloc_actuel == PENTE_30_GAUCHE_8)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
								}
								else if(phys_bloc_actuel == PENTE_30_GAUCHE_16)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
								}
								else if(phys_bloc_actuel == PENTE_30_DROITE_0)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(26.5650512F)));
								}
								else// if(phys_bloc_actuel == PENTE_30_DROITE_8)
								{
									hauteur = (float) (-(perso.position.x + perso.taille.x / 2.0F) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(26.5650512F)));
								}

								p->position.y = hauteur;

								p->environnement = SOL_DUR;
								p->vitesse.y = 0;

								// MAJ du carré perso
								perso.position.y = p->position.y;
								determinate_collision(perso, block, &collision);
							}

							/* Collision avec le plafond */
							if((phys_bloc_actuel == PLAFOND
								|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
								|| phys_bloc_actuel == COIN_BAS_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC)
								&& collision.type_collision == PAR_LE_HAUT)
							{

								p->vitesse.y = 0;
								p->position.y = (float)block.position.y - p->taille.y + (p->taille.y - ordonnee_haut);

								if(!(bloc_actuel.type_bloc & EST_VIDE))
								{
									/* Ajout de l'occurence d'item */
									coordf vitesse;
									occ_item *item;

									vitesse.x = 0;

									n->occ_blocs[i][j]->etat = POUSSE_PAR_LE_HAUT;
									/* Si le bloc ne contient pas un pointeur sur un item,
									l'item sera en fonction de la transformation actuelle du personnage */
									if(n->occ_blocs[i][j]->item < 0)
									{
										int index = (p->transformation == FIRE_MARIO)? p->transformation - 1: p->transformation;
										vitesse.y = VIT_SORTIE_BLOC;
										item = new_occ_item(n->occ_blocs[i][j]->position.x, n->occ_blocs[i][j]->position.y, n->items[index], vitesse, SORT_DU_BLOC);
										FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_ITEM_BLOCK]);
									}
									else
									{
										vitesse.y =  VIT_SORTIE_BLOC * 4;
										item = new_occ_item(n->occ_blocs[i][j]->position.x, n->occ_blocs[i][j]->position.y, n->items[n->occ_blocs[i][j]->item], vitesse, SORT_DU_BLOC);
										prend_item(p, item->type_item->nom);

										if((bloc_actuel.type_bloc & DISTRIBUTEUR_PIECE)	&& bloc_actuel.tps_piece == 0)
										{
											bloc_actuel.tps_piece = 50000;
										}
									}

									item->tps_sortie_bloc = TPS_ITEM_SORT_BLOC;
									item->type_item->occ_items = ajout_item(item->type_item->occ_items, item);

									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_UNBREAKABLE_BLOCK]);

									/* Devient incassable si ce n'est pas un distributeur de pièce ou que son temps de distibution de pièces est écoulé */
									if(!(bloc_actuel.type_bloc & DISTRIBUTEUR_PIECE) || bloc_actuel.tps_piece < 0)
									{
										bloc_actuel.tps_piece = 0;
										n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif;
										n->occ_blocs[i][j]->bloc_alternatif = -1;
									}

								}
								else if((bloc_actuel.type_bloc & CASSABLE) && p->transformation >= SUPER_MARIO)
								{
									/* Creation et ajout des 4 débris de blocs */
									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], n->occ_blocs[i][j]->position.x, n->occ_blocs[i][j]->position.y, -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], n->occ_blocs[i][j]->position.x, (int)(n->occ_blocs[i][j]->position.y + n->taille_blocs.y / 2), -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], n->occ_blocs[i][j]->position.x, n->occ_blocs[i][j]->position.y, n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], n->occ_blocs[i][j]->position.x, (int)(n->occ_blocs[i][j]->position.y + n->taille_blocs.y / 2), n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));

									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_BREAK_BLOCK]);
									n->occ_blocs[i][j]->id_perso = p->personnage;
									n->occ_blocs[i][j]->etat = POUSSE_PAR_LE_HAUT;

									n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif;
									n->occ_blocs[i][j]->bloc_alternatif = -1;	// Suppression du bloc
								}
								else 
								{

									if((bloc_actuel.type_bloc & CASSABLE) && p->transformation < SUPER_MARIO)
									{
										n->occ_blocs[i][j]->etat = POUSSE_PAR_LE_HAUT;
										n->occ_blocs[i][j]->id_perso = p->personnage;
									}
									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_UNBREAKABLE_BLOCK]);
								}

								// MAJ du carré perso
								perso.position.y = p->position.y;
								determinate_collision(perso, block, &collision);
							}

							/* Collision avec le mur à gauche du personnage */
							if((phys_bloc_actuel == MUR_A_GAUCHE 
								|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
								|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
								|| phys_bloc_actuel == BLOC_SPEC) 
								&& collision.type_collision == PAR_LA_GAUCHE)
							{
								p->vitesse.x = 0;
								p->position.x = (float) block.position.x + block.taille.x - p->texture_act->abscisse_bas;

								// MAJ du carré perso
								perso.position.x = p->position.x + p->texture_act->abscisse_bas;
								determinate_collision(perso, block, &collision);
							}

							/* Collision avec le mur à droite du personnage */
							if((phys_bloc_actuel == MUR_A_DROITE
								|| phys_bloc_actuel == COIN_HAUT_A_DROITE
								|| phys_bloc_actuel == COIN_BAS_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC) 
								&& collision.type_collision == PAR_LA_DROITE)
							{
								p->vitesse.x = 0;
								p->position.x = (float) block.position.x - p->taille.x + p->texture_act->abscisse_bas;

								// MAJ du carré perso
								perso.position.x = p->position.x + p->texture_act->abscisse_bas;
								determinate_collision(perso, block, &collision);
							}
						}
					}
				}
			}


			/************* Collisions PERSO <=> TUYAUX *************/
			for(i = 0; i < n->nb_tuyaux; i++)
			{
				tuyau.angle_pente = 0;
				tuyau.est_bloc_pente = 0;
				tuyau.hauteur_a_retirer = 0;

				tuyau.position.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				tuyau.position_prec.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position_prec.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				if(n->tuyaux[i]->sens_sortie == VERS_LE_HAUT ||
					n->tuyaux[i]->sens_sortie == VERS_LE_BAS) {
						tuyau.taille.x = (int)(2 * LARGEUR_BLOC);
						tuyau.taille.y = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
				}
				else 
				{
					tuyau.taille.x = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
					tuyau.taille.y = (int)(2 * LARGEUR_BLOC);
				}

				determinate_collision(perso, tuyau, &collision);

				if(collision.type_collision == PAR_LE_BAS) {
					if(keystate->actuel[BAS] 
						&& p->position.x + p->texture_act->abscisse_bas >= tuyau.position.x + LARGEUR_BLOC / 2 
						&& p->position.x + p->taille.x - p->texture_act->abscisse_bas <= tuyau.position.x + 3 * LARGEUR_BLOC / 2 
						&& p->position.y <= tuyau.position.y + tuyau.taille.y
						&& n->tuyaux[i]->etat == OUVERT && n->tuyaux[i]->sens_sortie == VERS_LE_HAUT)
					{
						p->etat = RENTRE_TUYAU_VERTICAL;
						p->vitesse.x = 0;
						p->vitesse.y = -VIT_SORTIE_BLOC;
						p->tuyau = n->tuyaux[i];
						FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_PIPE]);
						break;
					}
					// Si le perso est en train d'attaquer, on ne change pas son etat 
					else //if(!p->tps_attaque_speciale && !p->tps_attaque)
					{
						if(p->monstre_porte == NULL)
						{
							if(p->etat == SAUT_BAISSE)
								p->etat = BAISSE;
							else if(p->etat == SAUT || p->etat == SAUT_DESCENDANT) {
								p->etat = DEBOUT;
								p->hud->nb_monstres_tues = 0;
							}
						}
						else
						{
							if(p->etat == SAUT_BAISSE_CARAPACE)
								p->etat = BAISSE_CARAPACE;
							else if(p->etat == SAUT_CARAPACE) {
								p->etat = DEBOUT_CARAPACE;
								p->hud->nb_monstres_tues = 0;
							}
						}

						p->vitesse.y = 0;
						p->environnement = SOL_DUR;
						p->position.y = tuyau.position.y + tuyau.taille.y;

						// MAJ du carré perso
						perso.position.y = p->position.y;
					}
				}

				if(collision.type_collision == PAR_LE_HAUT) {
					if(keystate->actuel[HAUT] 
						&& p->position.x + p->texture_act->abscisse_bas >= tuyau.position.x + LARGEUR_BLOC / 2 
						&& p->position.x + p->texture_act->abscisse_bas <= tuyau.position.x * LARGEUR_BLOC + 3 * LARGEUR_BLOC / 2 
						&& p->position.y + p->texture_act->ordonnee_haut >= tuyau.position.y
						&& n->tuyaux[i]->etat == OUVERT && n->tuyaux[i]->sens_sortie == VERS_LE_BAS)
					{
						p->etat = RENTRE_TUYAU_VERTICAL;
						p->vitesse.x = 0;
						p->vitesse.y = VIT_SORTIE_BLOC;
						p->tuyau = n->tuyaux[i];
						FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_PIPE]);
						break;
					}
					else 
					{
						p->vitesse.y = 0;
						p->position.y = (float)tuyau.position.y - p->taille.y + (p->taille.y - ordonnee_haut);

						// MAJ du carré perso
						perso.position.y = p->position.y;
					}
				}

				if(collision.type_collision == PAR_LA_GAUCHE) {
					if(keystate->actuel[GAUCHE] 
						&& p->position.x + p->taille.x - p->texture_act->abscisse_bas >= tuyau.position.x
						&& p->position.x + p->taille.x - p->texture_act->abscisse_bas >= tuyau.position.x + tuyau.taille.x
						&& p->position.y >= tuyau.position.y
						&& p->position.y + p->texture_act->ordonnee_haut <= tuyau.position.y + tuyau.taille.y
						&& n->tuyaux[i]->etat == OUVERT && n->tuyaux[i]->sens_sortie == VERS_LA_DROITE)
					{
						p->etat = RENTRE_TUYAU_HORIZONTAL;
						p->vitesse.y = 0;
						p->vitesse.x = -VIT_SORTIE_BLOC;
						p->tuyau = n->tuyaux[i];
						FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_PIPE]);
						break;
					}
					else
					{
						p->vitesse.x = 0;
						p->position.x = (float) tuyau.position.x + tuyau.taille.x - p->texture_act->abscisse_bas;

						// MAJ du carré perso
						perso.position.x = p->position.x + p->texture_act->abscisse_bas;
					}
				}

				if(collision.type_collision == PAR_LA_DROITE) 
				{
					if(keystate->actuel[DROITE] 
						&& p->position.x + p->texture_act->abscisse_bas <= tuyau.position.x + tuyau.taille.x
						&& p->position.x + p->texture_act->abscisse_bas <= tuyau.position.x
						&& p->position.y >= tuyau.position.y
						&& p->position.y + p->taille.y <= tuyau.position.y + tuyau.taille.y
						&& n->tuyaux[i]->etat == OUVERT && n->tuyaux[i]->sens_sortie == VERS_LA_GAUCHE)
					{
						p->etat = RENTRE_TUYAU_HORIZONTAL;
						p->vitesse.y = 0;
						p->vitesse.x = VIT_SORTIE_BLOC;
						p->tuyau = n->tuyaux[i];
						FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_PIPE]);
						break;
					}
					else 
					{
						p->vitesse.x = 0;
						p->position.x = (float) tuyau.position.x - p->taille.x + p->texture_act->abscisse_bas;

						// MAJ du carré perso
						perso.position.x = p->position.x + p->texture_act->abscisse_bas;
					}
				}

			}
	}

	/************* Collisions PERSO <=> MONSTRES *************/
	for(i = 0; i < n->nb_monstres; i++)
	{
		elem_monstre *mstr_actuel = n->monstres[i]->occ_monstres->monstre;

		while(mstr_actuel != NULL)
		{
			if(mstr_actuel->occ_monstre->etat != M_MORT_PAR_SAUT
				&& (mstr_actuel->occ_monstre->etat != M_MORT_PAR_PROJ || mstr_actuel->occ_monstre == p->monstre_porte)
				&& mstr_actuel->occ_monstre->etat != M_SORT_DU_TUYAU
				&& p->etat != MORT && p->etat != RENTRE_TUYAU_HORIZONTAL
				&& p->etat != RENTRE_TUYAU_VERTICAL && p->etat != SORT_TUYAU_HORIZONTAL
				&& p->etat != SORT_TUYAU_VERTICAL
				&& p->etat != FINISH && p->etat != FINISH_CHATEAU) {

					monstre.position.x = mstr_actuel->occ_monstre->position.x + mstr_actuel->occ_monstre->type_monstre->abscisse_bas;
					monstre.position.y = mstr_actuel->occ_monstre->position.y;

					monstre.position_prec.x = mstr_actuel->occ_monstre->position_prec.x + mstr_actuel->occ_monstre->type_monstre->abscisse_bas;
					monstre.position_prec.y = mstr_actuel->occ_monstre->position_prec.y;

					monstre.taille.x = mstr_actuel->occ_monstre->type_monstre->taille.x - 2 *  mstr_actuel->occ_monstre->type_monstre->abscisse_bas;

					monstre.est_bloc_pente = 0;

					/* Si le monstre est retracté, il est plus petit */
					if(mstr_actuel->occ_monstre->etat == M_RETRACTED)
						monstre.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y / 2;
					else
						monstre.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y;


					determinate_collision(perso, monstre, &collision);

					if(p->est_invincible_etoile == 0)
					{
						/* Cas spécial où le personnage lache le monstre qu'il porte */
						if(!keystate->actuel[RUN] && keystate->precedent[RUN] && p->monstre_porte == mstr_actuel->occ_monstre)
						{

							if(p->cote == COTE_DROIT)
								mstr_actuel->occ_monstre->position.x = (float) p->position.x + p->taille.x - p->monstre_porte->type_monstre->taille.x / 2;
							else
								mstr_actuel->occ_monstre->position.x = (float) p->position.x - p->monstre_porte->type_monstre->taille.x + p->monstre_porte->type_monstre->taille.x / 2;

							mstr_actuel->occ_monstre->position.y = p->position.y;

							p->monstre_porte->etat = M_RETRACTED;
							p->etat = POUSSE_CARAPACE;
							mstr_actuel->occ_monstre->vitesse.x = (p->cote == COTE_DROIT)? V_CARAPACE : -V_CARAPACE;

							p->monstre_porte = NULL;
							p->tps_pousse_carapace = TPS_POUSSE_CARAPACE;

							FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_PROJ_ON]);
						}

						/* Cas spécial où le monstre porté est mort dans les bras du personnage */
						if(p->monstre_porte == mstr_actuel->occ_monstre && p->monstre_porte->etat == M_MORT_PAR_PROJ)
							p->monstre_porte = NULL;

						/* Si le deuxième carré est touché c'est-à-dire le monstre */
						if(collision.carre1_est_touche || collision.carre2_est_touche) {

							if(!mstr_actuel->occ_monstre->type_monstre->peut_sauter_dessus)
							{
								touche_perso(p);
							}
							else if(!mstr_actuel->occ_monstre->type_monstre->est_tuable_par_saut) {
								// Si le monstre est retracté
								if(mstr_actuel->occ_monstre->etat == M_RETRACTED)
								{
									if(keystate->actuel[RUN] && p->environnement == SOL_DUR && mstr_actuel->occ_monstre->vitesse.x == 0)
									{
										p->etat = DEBOUT_CARAPACE;
										mstr_actuel->occ_monstre->position.x = p->position.x + p->taille.x / 2;
										mstr_actuel->occ_monstre->position.y = p->position.y + p->taille.x / 2;
										p->monstre_porte = mstr_actuel->occ_monstre;
										p->monstre_porte->etat = M_RETRACTED_PORTED;
										p->hud->nb_monstres_tues_carapace = 0;

									}
									// Cas où mario arrive sur la carapace
									else if((p->etat == MARCHE || p->etat == DEBOUT || p->etat == DERAPE || p->etat == COURSE_1 || p->etat == COURSE_2) && mstr_actuel->occ_monstre->vitesse.x == 0)
									{
										mstr_actuel->occ_monstre->vitesse.x = (p->cote == COTE_DROIT)? V_CARAPACE : -V_CARAPACE;

										if(p->etat == DERAPE)
											mstr_actuel->occ_monstre->vitesse.x *= -1;
										p->etat = POUSSE_CARAPACE;

										FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_PROJ_ON]);
										p->tps_pousse_carapace = TPS_POUSSE_CARAPACE;

										// Comptage et affichage des points
										compte_points(p, mstr_actuel->occ_monstre);
									}
									// Cas où Mario saute sur le monstre retracté et mobile
									else if((p->etat == SAUT_DESCENDANT || p->etat == SAUT_BAISSE || p->etat == SAUT_CARAPACE)
										&& mstr_actuel->occ_monstre->vitesse.x != 0)
									{
										/* Si la touche de saut est active, le personnage saute plus haut */
										if(keystate->actuel[SAUTER])
											p->vitesse.y = V_REBOND_SAUT;
										else
											p->vitesse.y = V_REBOND;

										// Immobilisation du monstre
										mstr_actuel->occ_monstre->vitesse.x = 0;
										FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_JUMP_ON]);

										// Comptage et affichage des points
										compte_points(p, mstr_actuel->occ_monstre);
									}
									// Cas où Mario saute sur le monstre retracté et immobile
									else if((p->etat == SAUT_DESCENDANT || p->etat == SAUT_BAISSE || p->etat == SAUT_CARAPACE)
										&& mstr_actuel->occ_monstre->vitesse.x == 0)
									{
										// Test pour savoir de quel coté ira la carapace
										if(p->position.x + p->taille.x / 2 > 
											mstr_actuel->occ_monstre->position.x + mstr_actuel->occ_monstre->type_monstre->taille.x / 2)

											mstr_actuel->occ_monstre->vitesse.x = -V_CARAPACE;
										else
											mstr_actuel->occ_monstre->vitesse.x = V_CARAPACE;


										FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_PROJ_ON]);

										// Comptage et affichage des points
										compte_points(p, mstr_actuel->occ_monstre);
									}
								}
								// Cas où le monstre n'était pas retracté mais touché quand même
								else if(collision.carre2_est_touche)
								{
									// Il devient retracté
									mstr_actuel->occ_monstre->etat = M_RETRACTED;
									mstr_actuel->occ_monstre->vitesse.x = 0;

									/* Si la touche de saut est active, le personnage saute plus haut */
									if(keystate->actuel[SAUTER])
										p->vitesse.y = V_REBOND_SAUT;
									else
										p->vitesse.y = V_REBOND;

									FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_JUMP_ON]);

									// Comptage et affichage des points
									compte_points(p, mstr_actuel->occ_monstre);
								}
							}
							// Cas où le monstre meurt à cause d'un saut
							else 
							{
								if(collision.carre2_est_touche) {
									/* Si la touche de saut est active, le personnage saute plus haut */
									if(keystate->actuel[SAUTER])
										p->vitesse.y = V_REBOND_SAUT;
									else
										p->vitesse.y = V_REBOND;

									mstr_actuel->occ_monstre->etat = M_MORT_PAR_SAUT;
									mstr_actuel->occ_monstre->tps_disparition = 500;
									mstr_actuel->occ_monstre->vitesse.x = 0;

									FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_JUMP_ON]);

									// Comptage et affichage des points
									compte_points(p, mstr_actuel->occ_monstre);
								}
							}
						}

						/* Si le premier carré est touché c'est-à-dire le personnage */
						if(collision.carre1_est_touche && !p->est_invincible && p->monstre_porte != mstr_actuel->occ_monstre 
							&& p->etat != POUSSE_CARAPACE && mstr_actuel->occ_monstre->etat != M_MORT_PAR_PROJ 
							&& mstr_actuel->occ_monstre->etat != M_MORT_PAR_SAUT)
							touche_perso(p);
					}
					else 
					{
						/* Cas où Mario a pris l'étoile */
						if(collision.carre1_est_touche || collision.carre2_est_touche) {
							coordi text_points;

							mstr_actuel->occ_monstre->etat = M_MORT_PAR_PROJ;
							mstr_actuel->occ_monstre->vitesse.x = (p->vitesse.x < 0)? -mstr_actuel->occ_monstre->vitesse.x : mstr_actuel->occ_monstre->vitesse.x;
							mstr_actuel->occ_monstre->vitesse.y = VITESSE_Y_EJECTION;

							/* Comptage des points */
							text_points.x = (int) (mstr_actuel->occ_monstre->position.x);
							text_points.y = (int) (mstr_actuel->occ_monstre->position.y + mstr_actuel->occ_monstre->type_monstre->taille.y);
							p->hud->file_points = add_file_pts(p->hud->file_points, mstr_actuel->occ_monstre->type_monstre->points, text_points);

							p->hud->score += mstr_actuel->occ_monstre->type_monstre->points;

							FSOUND_PlaySound(FSOUND_FREE, mstr_actuel->occ_monstre->type_monstre->sons[SND_PROJ_ON]);
						}
					}
			}

			mstr_actuel =  mstr_actuel->suivant;
		}
	}


	/************* Collisions PERSO <=> PROJECTILES *************/
	for(i = 0; i < n->nb_projectiles; i ++)
	{

		elem_projectile *proj_actuel = n->projectiles[i]->occ_projectiles->projectile;

		while(proj_actuel != NULL)
		{

			if(proj_actuel->occ_projectile->tps_vie > 0 
				&& proj_actuel->occ_projectile->envoyeur == MECHANT)
			{

				// Initialisation du bloc projectile
				projectile.position.x = proj_actuel->occ_projectile->position.x + proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.position.y = proj_actuel->occ_projectile->position.y - proj_actuel->occ_projectile->type_projectile->taille.y + proj_actuel->occ_projectile->type_projectile->ordonnee_haut;

				projectile.position_prec.x = proj_actuel->occ_projectile->position_prec.x + proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.position_prec.y = proj_actuel->occ_projectile->position_prec.y - proj_actuel->occ_projectile->type_projectile->taille.y + proj_actuel->occ_projectile->type_projectile->ordonnee_haut;

				projectile.taille.x = proj_actuel->occ_projectile->type_projectile->taille.x - 2 *  proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.taille.y = 2 * proj_actuel->occ_projectile->type_projectile->ordonnee_haut - proj_actuel->occ_projectile->type_projectile->taille.y;

				projectile.est_bloc_pente = 0;

				determinate_collision(perso, projectile, &collision);

				/* Si le premier carré est touché c'est-à-dire le perso */
				if(collision.carre1_est_touche && !p->est_invincible)
					touche_perso(p);
			}

			proj_actuel = proj_actuel->suivant;
		}
	}

	/************ Collisions PERSO <=> ITEMS *******************/
	for(i = 0; i < n->nb_items; i ++)
	{
		elem_item* item_actuel = n->items[i]->occ_items->item, *precedent = NULL;

		while(item_actuel != NULL)
		{

			if(item_actuel->occ_item->etat == NORMAL)
			{

				// Initialisation du bloc item
				item.position.x = item_actuel->occ_item->position.x;
				item.position.y = item_actuel->occ_item->position.y;

				item.position_prec.x = item_actuel->occ_item->position_prec.x;
				item.position_prec.y = item_actuel->occ_item->position_prec.y;

				item.taille.x = item_actuel->occ_item->type_item->taille.x;
				item.taille.y = item_actuel->occ_item->type_item->taille.y;

				item.est_bloc_pente = 0;

				determinate_collision(perso, item, &collision);

				/* Si un des deux carrés est touché, le personnage se transforme */
				if(collision.carre1_est_touche || collision.carre2_est_touche)
				{
					/* Il prend l'item et on le supprime de la liste */
					prend_item(p, item_actuel->occ_item->type_item->nom);
					supprime_item(n->items[i]->occ_items, item_actuel->occ_item);
					item_actuel = precedent;
				}

				/* au cas où, ce soit le premier de la liste */
				if(item_actuel != NULL)
				{
					precedent = item_actuel;
					item_actuel = item_actuel->suivant;
				}
			}
			else
				item_actuel = item_actuel->suivant;
		}
	}

	/******* COLLISIONS PERSOS <=> FINISH ********/
	for(i = 0; i < n->nb_finish; i++)
	{
		finish.angle_pente = 0;
		finish.est_bloc_pente = 0;
		finish.hauteur_a_retirer = 0;

		finish.taille = n->finishes[i].taille;
		finish.position = n->finishes[i].position;
		finish.position_prec = finish.position;

		determinate_collision(perso, finish, &collision);

		if(collision.carre1_est_touche || collision.carre2_est_touche)
		{
			if(perso.position.x + perso.taille.x / 2 >= finish.position.x + finish.taille.x / 2)
			{
				p->position.x = finish.position.x + finish.taille.x / 2 - perso.taille.x / 2 - p->texture_act->abscisse_bas;
				p->etat = FINISH_CHATEAU;
			}

			if(p->etat != FINISH && p->etat != FINISH_CHATEAU)
			{
				FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_CLEAR]);
				p->tps_finish = TPS_FINISH;
				p->etat = FINISH;
			}
		}
	}
}
void solve_collisions_monstre(occ_monstre* m, occ_monstre* mstr_copie, perso* p, niveau* n, Uint32 duree)
{

	/* Variables pour l'initialisation */
	coordi bloc_bg, bloc_hd;
	int i, j;

	/* Variables pour la detection et la résolution de collisions */
	elem_monstre* mstr_actuel = m->type_monstre->occ_monstres->monstre;
	int phys_bloc_actuel;
	static int prec_collision_sol;
	float hauteur;
	carre block = {0}, monstre = {0}, monstre_2 = {0}, projectile = {0}, tuyau = {0};
	collision collision;

	coordi text_points;

	/************************************************* DETECTIONS ET RESOLUTIONS DE COLLISIONS *******************************************/

	/* Collision de base avec les bords du niveau */
	if(m->position.x + m->type_monstre->taille.x < 0
		|| m->position.y + m->type_monstre->taille.y < 0
		|| (m->position.x  > n->taille.x * n->taille_blocs.x && (m->etat != M_RETRACTED_PORTED || m->etat != M_MARCHE))) {
			m->etat = M_MORT;
			p->hud->nb_monstres_tues_carapace = 0;
	}

	/* Initialisation des blocs sur lequels se trouve le personnage */
	bloc_bg.x = (int) m->position.x / n->taille_blocs.x;
	bloc_bg.y = (int) m->position.y / n->taille_blocs.y;

	bloc_hd.x = (int) (m->position.x + m->type_monstre->taille.x - m->type_monstre->abscisse_bas) / n->taille_blocs.x;
	bloc_hd.y = (int) (m->position.y + m->type_monstre->taille.y) / n->taille_blocs.y;

	/* Initialisation du carre monstre */
	monstre.position.x = m->position.x + m->type_monstre->abscisse_bas;
	monstre.position.y = m->position.y;

	monstre.position_prec.x = m->position_prec.x + m->type_monstre->abscisse_bas;
	monstre.position_prec.y = m->position_prec.y;

	monstre.taille.x = m->type_monstre->taille.x - 2 *  m->type_monstre->abscisse_bas;

	/* Si le monstre est retracté, il est plus petit */
	if(m->etat == M_RETRACTED || m->etat == M_RETRACTED_PORTED)
		monstre.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y / 2;
	else
		monstre.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y;

	monstre.est_bloc_pente = 0;

	if( m->etat != M_RETRACTED_PORTED && m->etat != M_MORT)
	{

		/************* Collisions MONSTRES <=> NIVEAU *************/
		for(i = bloc_bg.x; i <= bloc_hd.x; i++)
		{
			for(j = bloc_bg.y; j <= bloc_hd.y; j++)
			{
				if (i < n->taille.x && i >= 0)
				{
					if(n->occ_blocs[i][j]->bloc_actuel < -1 && n->occ_blocs[i][j]->etat == POUSSE_PAR_LE_HAUT)
					{
						/* Le monstre touché meurt */
						m->etat = M_MORT_PAR_PROJ;
						m->vitesse.y = VITESSE_Y_EJECTION * 2;

						FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_PROJ_ON]);

						compte_points(p, mstr_actuel->occ_monstre);
					}
					if(n->occ_blocs[i][j]->bloc_actuel >= 0)
					{

						bloc bloc_actuel = n->blocs[n->occ_blocs[i][j]->bloc_actuel];

						/* On récupère la physique du bloc */
						phys_bloc_actuel = bloc_actuel.phys;

						/* Initialisation du carré bloc */
						block.position.x = (float)i * n->taille_blocs.x;
						block.position.y = (float)j * n->taille_blocs.y;

						block.position_prec.x = block.position.x;
						block.position_prec.y = block.position.y;

						block.taille.x = n->taille_blocs.x;
						block.taille.y = n->taille_blocs.y;

						find_angle_height_with_phys(phys_bloc_actuel, &block);

						block.est_bloc_pente = (block.angle_pente != 0);

						determinate_collision(monstre, block, &collision);

						/* Collision avec le sol */
						if((phys_bloc_actuel == SOL 
							|| phys_bloc_actuel == BORD_A_GAUCHE
							|| phys_bloc_actuel == BORD_A_DROITE
							|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
							|| phys_bloc_actuel == COIN_HAUT_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LE_BAS)
						{
							
							if(n->occ_blocs[i][j]->etat == POUSSE_PAR_LE_HAUT)
							{
								/* Le monstre touché meurt */
								m->etat = M_MORT_PAR_PROJ;
								m->vitesse.y = VITESSE_Y_EJECTION * 2;

								FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_PROJ_ON]);

								compte_points(p, mstr_actuel->occ_monstre);
							}
							else
							{
								m->vitesse.y = 0;
								m->position.y = (float)block.position.y + block.taille.y;
							}

							// MAJ du carré monstre
							monstre.position.y = m->position.y;
							determinate_collision(monstre, block, &collision);
						}

						/* Collision avec les pentes */
						if((phys_bloc_actuel == PENTE_15_DROITE_16
							|| phys_bloc_actuel == PENTE_15_GAUCHE_16
							|| phys_bloc_actuel == PENTE_15_DROITE_12
							|| phys_bloc_actuel == PENTE_15_GAUCHE_12
							|| phys_bloc_actuel == PENTE_15_DROITE_8
							|| phys_bloc_actuel == PENTE_15_GAUCHE_8
							|| phys_bloc_actuel == PENTE_15_DROITE_4
							|| phys_bloc_actuel == PENTE_15_GAUCHE_4

							|| phys_bloc_actuel == PENTE_30_DROITE_0
							|| phys_bloc_actuel == PENTE_30_DROITE_8
							|| phys_bloc_actuel == PENTE_30_GAUCHE_8
							|| phys_bloc_actuel == PENTE_30_GAUCHE_16
							|| phys_bloc_actuel == PENTE_45_DROITE
							|| phys_bloc_actuel == PENTE_45_GAUCHE)
							&& 
							(collision.type_collision == PENTE_PAR_LE_BAS
							|| collision.type_collision == PENTE_PAR_LA_GAUCHE
							|| collision.type_collision == PENTE_PAR_LA_DROITE))
						{


							/* Calcul de la nouvelle ordonnée du personnage 
							avec l'équation de la pente et en fonction du bloc */
							if(phys_bloc_actuel == PENTE_45_GAUCHE)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(135.0)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(135.0)));
							}
							else if(phys_bloc_actuel == PENTE_45_DROITE)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(45.0)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(45.0)));
							}
							else if(phys_bloc_actuel == PENTE_30_GAUCHE_8)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(180.0f - 26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
							}
							else if(phys_bloc_actuel == PENTE_30_GAUCHE_16)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(180.0f - 26.5650512F)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
							}
							else if(phys_bloc_actuel == PENTE_30_DROITE_0)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(26.5650512F)));
							}
							else// if(phys_bloc_actuel == PENTE_30_DROITE_8)
							{
								hauteur = (float) (-(monstre.position.x + monstre.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(26.5650512F)));
							}

							m->position.y = hauteur;

							m->vitesse.y = 0;

							// MAJ du carré monstre
							monstre.position.y = m->position.y;
							determinate_collision(monstre, block, &collision);
						}

						/* Collision avec le plafond */
						if((phys_bloc_actuel == PLAFOND
							|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
							|| phys_bloc_actuel == COIN_BAS_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LE_HAUT)
						{
							m->vitesse.y = 0;
							m->position.y = (float)block.position.y - block.taille.y - m->type_monstre->taille.y;

							// MAJ du carré monstre
							monstre.position.y = m->position.y;
							determinate_collision(monstre, block, &collision);
						}

						/* Collision avec le mur à gauche du monstre */
						if((phys_bloc_actuel == MUR_A_GAUCHE
							|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
							|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LA_GAUCHE)
						{
							m->vitesse.x = -m->vitesse.x;
							m->cote = COTE_DROIT;
							m->position.x += m->vitesse.x * duree + 2 * m->type_monstre->abscisse_bas;

							if(m->etat == M_RETRACTED)
							{
								if(!(bloc_actuel.type_bloc & EST_VIDE))
								{
									/* Ajout de l'occurence d'item */
									coordf vitesse;
									occ_item *item;

									vitesse.x = 0;
									n->occ_blocs[i][j]->etat = POUSSE_PAR_LA_DROITE;

									/* Si le bloc ne contient pas un pointeur sur un item,
									l'item sera en fonction de la transforùation actuelle 
									du personnage */
									if(n->occ_blocs[i][j]->item < 0) 
									{
										int index = (p->transformation == FIRE_MARIO)? p->transformation - 1: p->transformation;
										vitesse.y = VIT_SORTIE_BLOC;
										item = new_occ_item(i * LARGEUR_BLOC, j * LARGEUR_BLOC, n->items[index], vitesse, SORT_DU_BLOC);
									}
									else
									{
										vitesse.y =  VIT_SORTIE_BLOC * 5;
										item = new_occ_item(i * LARGEUR_BLOC, j * LARGEUR_BLOC, n->items[n->occ_blocs[i][j]->item], vitesse, SORT_DU_BLOC);
									}

									item->tps_sortie_bloc = TPS_ITEM_SORT_BLOC;

									item->type_item->occ_items = ajout_item(item->type_item->occ_items, item);
									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_UNBREAKABLE_BLOCK]);
									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_ITEM_BLOCK]);

									/* Devient incassable */
									n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif; 
									n->occ_blocs[i][j]->bloc_alternatif = -1;
								}
								else if(bloc_actuel.type_bloc & CASSABLE)
								{
									n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif;//  A rendre plus propre
									n->occ_blocs[i][j]->bloc_alternatif = -1;	// Suppression du bloc, rajouter les débris

									/* Creation et ajout des 4 débris de blocs */
									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC), -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC + LARGEUR_BLOC / 2), -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC), n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC + LARGEUR_BLOC / 2), n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));


									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_BREAK_BLOCK]);
								}
								else
									FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_LEVEL_ON]);
							}


							// MAJ du carré monstre
							monstre.position.x = m->position.x;

							determinate_collision(monstre, block, &collision);
						}

						/* Collision avec le mur à droite du monstre */
						if((phys_bloc_actuel == MUR_A_DROITE
							|| phys_bloc_actuel == COIN_HAUT_A_DROITE
							|| phys_bloc_actuel == COIN_BAS_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LA_DROITE)
						{

							m->vitesse.x = -m->vitesse.x;
							m->cote = COTE_GAUCHE;
							m->position.x += m->vitesse.x * duree;

							if(m->etat == M_RETRACTED)
							{
								if(!(bloc_actuel.type_bloc & EST_VIDE))
								{
									/* Ajout de l'occurence d'item */
									coordf vitesse;
									occ_item *item;

									vitesse.x = 0;

									n->occ_blocs[i][j]->etat = POUSSE_PAR_LA_GAUCHE;

									/* Si le bloc ne contient pas un pointeur sur un item,
									l'item sera en fonction de la transforùation actuelle 
									du personnage */
									if(n->occ_blocs[i][j]->item < 0) {
										int index = (p->transformation == FIRE_MARIO)? p->transformation - 1: p->transformation;
										vitesse.y = VIT_SORTIE_BLOC;
										item = new_occ_item(i * LARGEUR_BLOC, j * LARGEUR_BLOC, n->items[index], vitesse, SORT_DU_BLOC);
									}
									else
									{
										vitesse.y =  (bloc_actuel.type_bloc & EST_VIDE)? VIT_SORTIE_BLOC * 5 : VIT_SORTIE_BLOC;
										item = new_occ_item(i * LARGEUR_BLOC, j * LARGEUR_BLOC, n->items[n->occ_blocs[i][j]->item], vitesse, SORT_DU_BLOC);
									}

									item->tps_sortie_bloc = TPS_ITEM_SORT_BLOC;

									item->type_item->occ_items = ajout_item(item->type_item->occ_items, item);
									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_UNBREAKABLE_BLOCK]);
									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_ITEM_BLOCK]);

									/* Devient incassable */
									n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif; 
									n->occ_blocs[i][j]->bloc_alternatif = -1;
								}
								else if(bloc_actuel.type_bloc & CASSABLE) {
									n->occ_blocs[i][j]->bloc_actuel = n->occ_blocs[i][j]->bloc_alternatif;
									n->occ_blocs[i][j]->bloc_alternatif = -1;

									/* Creation et ajout des 4 débris de blocs */
									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC), -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC + LARGEUR_BLOC / 2), -n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC), n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION));

									n->projectiles[DEBRIS]->occ_projectiles = ajout_projectile(n->projectiles[DEBRIS]->occ_projectiles, 
										create_debris(n->projectiles[DEBRIS], (int)(i * LARGEUR_BLOC), (int)(j * LARGEUR_BLOC + LARGEUR_BLOC / 2), n->projectiles[DEBRIS]->vitesse.x, VITESSE_Y_EJECTION * 2));


									FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_BREAK_BLOCK]);
								}
								else
								{
									FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_LEVEL_ON]);
								}
							}

							// MAJ du carré monstre
							monstre.position.x = m->position.x;

							determinate_collision(monstre, block, &collision);
						}
					}
				}
			}
		}
	}

	/************ COLLISIONS MONSTRES <=> TUYAUX **************/
	for(i = 0; i < n->nb_tuyaux; i++)
	{
		tuyau.angle_pente = 0;
		tuyau.est_bloc_pente = 0;
		tuyau.hauteur_a_retirer = 0;

		tuyau.position.x = n->tuyaux[i]->position.x * n->taille_blocs.x;
		tuyau.position.y = n->tuyaux[i]->position.y * n->taille_blocs.y;

		tuyau.position_prec.x = n->tuyaux[i]->position.x * n->taille_blocs.x;
		tuyau.position_prec.y = n->tuyaux[i]->position.y * n->taille_blocs.y;

		if(n->tuyaux[i]->sens_sortie == VERS_LE_HAUT ||
			n->tuyaux[i]->sens_sortie == VERS_LE_BAS) {
				tuyau.taille.x = (int)(2 * n->taille_blocs.x);
				tuyau.taille.y = (int)((n->tuyaux[i]->longueur + 1) * n->taille_blocs.y);
		}
		else 
		{
			tuyau.taille.x = (int)((n->tuyaux[i]->longueur + 1) * n->taille_blocs.x);
			tuyau.taille.y = (int)(2 * n->taille_blocs.y);
		}

		determinate_collision(monstre, tuyau, &collision);

		switch(collision.type_collision)
		{
		case PAR_LE_BAS :
			m->vitesse.y = 0;
			m->position.y = tuyau.position.y + tuyau.taille.y;

			// MAJ du carré monstre
			monstre.position.y = m->position.y;
			break;
		case PAR_LE_HAUT:
			m->vitesse.y = 0;
			m->position.y = (float)tuyau.position.y - m->type_monstre->taille.y;

			// MAJ du carré monstre
			monstre.position.y = m->position.y;
			break;
		case PAR_LA_GAUCHE :
			m->vitesse.x = -m->vitesse.x;
			m->cote = COTE_DROIT;
			m->position.x = (float) tuyau.position.x + tuyau.taille.x;

			if(m->etat == M_RETRACTED)
				FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_LEVEL_ON]);

			// MAJ du carré monstre
			monstre.position.x = m->position.x + m->type_monstre->abscisse_bas;
			break;
		case PAR_LA_DROITE:
			m->vitesse.x = -m->vitesse.x;
			m->cote = COTE_GAUCHE;
			m->position.x = (float) tuyau.position.x - LARGEUR_BLOC;

			if(m->etat == M_RETRACTED)
				FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_LEVEL_ON]);

			// MAJ du carré monstre
			monstre.position.x = m->position.x + m->type_monstre->abscisse_bas;
			break;
		default:
			break;
		}
	}

	/************* Collisions MONSTRES <=> MONSTRES *************/
	while(mstr_actuel != NULL && ((m->vitesse.x != 0) || m->etat == M_RETRACTED_PORTED))
	{

		if(mstr_actuel->occ_monstre->etat != M_MORT_PAR_PROJ && mstr_actuel->occ_monstre->etat != M_MORT_PAR_SAUT 
			&& mstr_actuel->occ_monstre->etat != M_RETRACTED_PORTED
			&& m != mstr_copie)
		{
			/* Initialisation du carre monstre_2 */
			monstre_2.position.x = mstr_actuel->occ_monstre->position.x + mstr_actuel->occ_monstre->type_monstre->abscisse_bas;
			monstre_2.position.y = mstr_actuel->occ_monstre->position.y;

			monstre_2.position_prec.x = mstr_actuel->occ_monstre->position_prec.x + mstr_actuel->occ_monstre->type_monstre->abscisse_bas;
			monstre_2.position_prec.y = mstr_actuel->occ_monstre->position_prec.y;

			monstre_2.taille.x = mstr_actuel->occ_monstre->type_monstre->taille.x - 2 *  mstr_actuel->occ_monstre->type_monstre->abscisse_bas;

			/* Si le monstre est retracté, il est plus petit */
			if(mstr_actuel->occ_monstre->etat == M_RETRACTED)
				monstre_2.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y / 2;
			else
				monstre_2.taille.y = mstr_actuel->occ_monstre->type_monstre->taille.y;

			monstre_2.est_bloc_pente = 0;

			determinate_collision(monstre, monstre_2, &collision);

			if((collision.carre1_est_touche || collision.carre2_est_touche) && collision.type_collision != SUPERPOSITION)
			{
				/* Cas où l'autre monstre est retracté mais qu'il ne bouge pas ou qu'il marche */
				if(mstr_actuel->occ_monstre->etat == M_RETRACTED 
					|| mstr_actuel->occ_monstre->etat == M_MARCHE)
				{
					if(m->etat == M_RETRACTED && m->vitesse.x != 0)
					{

						if(mstr_actuel->occ_monstre->etat == M_RETRACTED && mstr_actuel->occ_monstre->vitesse.x != 0)
						{							
							/* Le monstre touché meurt */
							m->etat = M_MORT_PAR_PROJ;
							m->vitesse.y = VITESSE_Y_EJECTION;

							// Comptage des points
							text_points.x = (int)(mstr_actuel->occ_monstre->position.x);
							text_points.y = (int)(mstr_actuel->occ_monstre->position.y + mstr_actuel->occ_monstre->type_monstre->taille.y);

							p->hud->file_points = add_file_pts(p->hud->file_points, m->type_monstre->points, text_points);

							p->hud->score += m->type_monstre->points;
							p->hud->nb_monstres_tues_carapace = 0;
						}

						/* Le monstre touché meurt */
						mstr_actuel->occ_monstre->etat = M_MORT_PAR_PROJ;
						mstr_actuel->occ_monstre->vitesse.y = VITESSE_Y_EJECTION;
						FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_PROJ_ON]);

						// Comptage des points
						text_points.x = (int)(m->position.x);
						text_points.y = (int)(m->position.y + m->type_monstre->taille.y);						

						if(p->hud->nb_monstres_tues_carapace >= TAB_POINTS_LENGTH) {
							p->hud->file_points = add_file_pts(p->hud->file_points, 10000, text_points);
							prend_item(p, CHAMPI_VIE); // pour éviter la duplication de code
						}
						else 
						{
							p->hud->file_points = add_file_pts(p->hud->file_points, p->hud->tab_points[p->hud->nb_monstres_tues_carapace], text_points);
							p->hud->score += p->hud->tab_points[p->hud->nb_monstres_tues_carapace];
							p->hud->nb_monstres_tues_carapace++;
						}
					}


					/* Si l'un deux est porté, les deux meurent */
					if(m->etat == M_RETRACTED_PORTED)
					{
						mstr_actuel->occ_monstre->etat = M_MORT_PAR_PROJ;
						mstr_actuel->occ_monstre->vitesse.y = VITESSE_Y_EJECTION;

						m->etat = M_MORT_PAR_PROJ;
						m->vitesse.y = VITESSE_Y_EJECTION;
						FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_PROJ_ON]);

						// Comptage des points
						p->hud->nb_monstres_tues_carapace = 0;

						text_points.x = (int)(mstr_actuel->occ_monstre->position.x);
						text_points.y = (int)(mstr_actuel->occ_monstre->position.y + mstr_actuel->occ_monstre->type_monstre->taille.y);

						p->hud->file_points = add_file_pts(p->hud->file_points, p->hud->tab_points[p->hud->nb_monstres_tues_carapace], text_points);

						text_points.x = (int)(m->position.x);
						text_points.y = (int)(m->position.y + m->type_monstre->taille.y);
						p->hud->file_points = add_file_pts(p->hud->file_points, p->hud->tab_points[p->hud->nb_monstres_tues_carapace], text_points);

						p->hud->score += m->type_monstre->points;
					}

				}

				/* Cas où deux monstres se touchent */
				if(m->etat != M_RETRACTED)
				{ 
					if(m->cote == COTE_DROIT)
						m->cote = COTE_GAUCHE;
					else
						m->cote = COTE_DROIT;

					m->vitesse.x *= -1;
				}
			}
		}

		mstr_actuel = mstr_actuel->suivant;
	}	

	/************* Collisions MONSTRES <=> PROJECTILES *************/
	for(i = 0; i < n->nb_projectiles; i ++)
	{

		elem_projectile* proj_actuel = n->projectiles[i]->occ_projectiles->projectile;
		while(proj_actuel != NULL && m->etat != M_MORT_PAR_PROJ)
		{

			if(proj_actuel->occ_projectile->tps_vie > 0 && proj_actuel->occ_projectile->envoyeur == GENTIL && m->etat != M_MORT_PAR_SAUT)
			{

				// Initialisation du bloc monstre
				projectile.position.x = proj_actuel->occ_projectile->position.x + proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.position.y = proj_actuel->occ_projectile->position.y - proj_actuel->occ_projectile->type_projectile->taille.y + proj_actuel->occ_projectile->type_projectile->ordonnee_haut;

				projectile.position_prec.x = proj_actuel->occ_projectile->position_prec.x + proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.position_prec.y = proj_actuel->occ_projectile->position_prec.y - proj_actuel->occ_projectile->type_projectile->taille.y + proj_actuel->occ_projectile->type_projectile->ordonnee_haut;

				projectile.taille.x = proj_actuel->occ_projectile->type_projectile->taille.x - 2 *  proj_actuel->occ_projectile->type_projectile->abscisse_bas;
				projectile.taille.y = 2 * proj_actuel->occ_projectile->type_projectile->ordonnee_haut - proj_actuel->occ_projectile->type_projectile->taille.y;

				determinate_collision(monstre, projectile, &collision);

				/* Si le premier carré est touché c'est-à-dire le monstre */
				if(collision.carre1_est_touche)
				{
					/* Si le monstre peut être tué par une fireball, il meurt */
					if(m->type_monstre->est_tuable_par_boule_feu)
					{
						m->etat = M_MORT_PAR_PROJ;
						m->vitesse.x = (proj_actuel->occ_projectile->vitesse.x < 0)? -m->vitesse.x : m->vitesse.x;
						m->vitesse.y = VITESSE_Y_EJECTION;

						/* Comptage des points */
						text_points.x = (int)(m->position.x);
						text_points.y = (int)(m->position.y + m->type_monstre->taille.y);
						p->hud->file_points = add_file_pts(p->hud->file_points, m->type_monstre->points, text_points);

						p->hud->score += m->type_monstre->points;
					}

					if(m->type_monstre->est_tuable_par_boule_feu)
						FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_PROJ_ON]);
					else
						FSOUND_PlaySound(FSOUND_FREE, m->type_monstre->sons[SND_LEVEL_ON]);

					proj_actuel->occ_projectile->tps_vie = 0;
				}
			}

			proj_actuel = proj_actuel->suivant;
		}
	}
}
void solve_collisions_projectile(occ_projectile* p, niveau *n)
{

	/* Variables pour l'initialisation */
	coordi bloc_bg, bloc_hd;
	int i, j;

	/* Variables pour la detection et la résolution de collisions */
	int phys_bloc_actuel;
	float hauteur;
	carre projectile = {0}, block = {0}, tuyau = {0};
	collision collision;

	/* Initialisation des blocs sur lequels se trouve le personnage */
	bloc_bg.x = (int) (p->position.x) / n->taille_blocs.x;
	bloc_bg.y = (int) (p->position.y) / n->taille_blocs.y;

	bloc_hd.x = (int) (p->position.x + p->type_projectile->taille.x - p->type_projectile->abscisse_bas) / n->taille_blocs.x;
	bloc_hd.y = (int) (p->position.y + p->type_projectile->ordonnee_haut) / n->taille_blocs.y;

	/************************************************ DETECTIONS ET RESOLUTIONS DE COLLISIONS *******************************************/

	/* Collision de base avec les bords du niveau */
	if(p->position.x + p->type_projectile->abscisse_bas < 0){
		p->tps_vie = 0;
		p->tps_disparition = 0; // pas d'animation de mort
	}

	if(p->position.x + p->type_projectile->taille.x - p->type_projectile->abscisse_bas > n->taille.x * n->taille_blocs.x){		
		p->tps_vie = 0;
		p->tps_disparition = 0; // idem
	}

	if(p->position.y + p->type_projectile->taille.y < 0){
		p->tps_vie = 0;
		p->tps_disparition = 0; // idem
	}


	/* Si le projectile est soumis aux collisions avec le niveau => détection et résolution */
	if(p->type_projectile->soumission & SOUMIS_COLLISIONS_SOL
		&& p->tps_vie > 0){

			/* Initialisation du carre projectile */
			projectile.position.x = p->position.x + p->type_projectile->abscisse_bas;
			projectile.position.y = p->position.y;

			projectile.position_prec.x = p->position_prec.x + p->type_projectile->abscisse_bas;
			projectile.position_prec.y = p->position_prec.y;

			projectile.taille.x = p->type_projectile->taille.x - 2 *  p->type_projectile->abscisse_bas;
			projectile.taille.y = p->type_projectile->taille.y - p->type_projectile->ordonnee_haut;

			/************ COLLISIONS PROJECTILES <=> NIVEAU ***************/
			for(i = bloc_bg.x; i <= bloc_hd.x; i++){
				for(j = bloc_bg.y; j <= bloc_hd.y; j++){

					if(n->occ_blocs[i][j]->bloc_actuel >= 0)
					{

						/* On récupère la physique du bloc */
						phys_bloc_actuel = n->blocs[n->occ_blocs[i][j]->bloc_actuel].phys;

						/* Initialisation du carré bloc */
						block.position.x = (float)i * n->taille_blocs.x;
						block.position.y = (float)j * n->taille_blocs.y;

						block.position_prec.x = block.position.x;
						block.position_prec.y = block.position.y;

						block.taille.x = n->taille_blocs.x;
						block.taille.y = n->taille_blocs.y;

						find_angle_height_with_phys(phys_bloc_actuel, &block);

						block.est_bloc_pente = (block.angle_pente != 0);

						determinate_collision(projectile, block, &collision);

						/* Collision avec le sol */
						if((phys_bloc_actuel == SOL 
							|| phys_bloc_actuel == BORD_A_GAUCHE
							|| phys_bloc_actuel == BORD_A_DROITE
							|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
							|| phys_bloc_actuel == COIN_HAUT_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LE_BAS)
						{
							p->vitesse.y = V_REBOND_PROJECTILE;
							p->position.y = (float)block.position.y + block.taille.y - p->type_projectile->taille.y + p->type_projectile->ordonnee_haut;

							// MAJ du carré projectile
							projectile.position.y = p->position.y;
							determinate_collision(projectile, block, &collision);
						}

						/* Collision avec les pentes */
						if((phys_bloc_actuel == PENTE_15_DROITE_16
							|| phys_bloc_actuel == PENTE_15_GAUCHE_16
							|| phys_bloc_actuel == PENTE_15_DROITE_12
							|| phys_bloc_actuel == PENTE_15_GAUCHE_12
							|| phys_bloc_actuel == PENTE_15_DROITE_8
							|| phys_bloc_actuel == PENTE_15_GAUCHE_8
							|| phys_bloc_actuel == PENTE_15_DROITE_4
							|| phys_bloc_actuel == PENTE_15_GAUCHE_4

							|| phys_bloc_actuel == PENTE_30_DROITE_0
							|| phys_bloc_actuel == PENTE_30_DROITE_8
							|| phys_bloc_actuel == PENTE_30_GAUCHE_8
							|| phys_bloc_actuel == PENTE_30_GAUCHE_16
							|| phys_bloc_actuel == PENTE_45_DROITE
							|| phys_bloc_actuel == PENTE_45_GAUCHE)
							&& 
							(collision.type_collision == PENTE_PAR_LE_BAS
							|| collision.type_collision == PENTE_PAR_LA_GAUCHE
							|| collision.type_collision == PENTE_PAR_LA_DROITE))
						{

							/* Calcul de la nouvelle ordonnée du personnage 
							avec l'équation de la pente et en fonction du bloc */
							if(phys_bloc_actuel == PENTE_45_GAUCHE)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(135.0)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(135.0)));
							}
							else if(phys_bloc_actuel == PENTE_45_DROITE)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(45.0)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(45.0)));
							}
							else if(phys_bloc_actuel == PENTE_30_GAUCHE_8)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
							}
							else if(phys_bloc_actuel == PENTE_30_GAUCHE_16)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
							}
							else if(phys_bloc_actuel == PENTE_30_DROITE_0)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(26.5650512F)));
							}
							else// if(phys_bloc_actuel == PENTE_30_DROITE_8)
							{
								hauteur = (float) (-(projectile.position.x + projectile.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(26.5650512F)));
							}

							p->position.y = hauteur;
							p->vitesse.y = V_REBOND_PROJECTILE;

							// MAJ du carré projectile
							projectile.position.y = p->position.y;
							determinate_collision(projectile, block, &collision);
						}

						/* Collision avec le plafond */
						if((phys_bloc_actuel == PLAFOND
							|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
							|| phys_bloc_actuel == COIN_BAS_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC )
							&& collision.type_collision == PAR_LE_HAUT)
						{
							p->vitesse.y = 0;
							p->position.y = (float)block.position.y - block.taille.y - p->type_projectile->taille.y;

							// MAJ du carré projectile
							projectile.position.y = p->position.y;
							determinate_collision(projectile, block, &collision);
						}

						/* Collision avec le mur à gauche du monstre */
						if((phys_bloc_actuel == MUR_A_GAUCHE
							|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
							|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
							|| phys_bloc_actuel == BLOC_SPEC)
							&& collision.type_collision == PAR_LA_GAUCHE)
						{
							p->tps_vie = 0;
						}

						/* Collision avec le mur à droite du monstre */
						if((phys_bloc_actuel == MUR_A_DROITE
							|| phys_bloc_actuel == COIN_HAUT_A_DROITE
							|| phys_bloc_actuel == COIN_BAS_A_DROITE
							|| phys_bloc_actuel == BLOC_SPEC) 
							&& collision.type_collision == PAR_LA_DROITE)
						{
							p->tps_vie = 0;
						}
					}
				}
			}

			/************ COLLISIONS PROJECTILES <=> TUYAUX */
			for(i = 0; i < n->nb_tuyaux; i++)
			{
				tuyau.angle_pente = 0;
				tuyau.est_bloc_pente = 0;
				tuyau.hauteur_a_retirer = 0;

				tuyau.position.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				tuyau.position_prec.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position_prec.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				if(n->tuyaux[i]->sens_sortie == VERS_LE_HAUT ||
					n->tuyaux[i]->sens_sortie == VERS_LE_BAS) {
						tuyau.taille.x = (int)(2 * LARGEUR_BLOC);
						tuyau.taille.y = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
				}
				else 
				{
					tuyau.taille.x = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
					tuyau.taille.y = (int)(2 * LARGEUR_BLOC);
				}

				determinate_collision(projectile, tuyau, &collision);

				switch(collision.type_collision)
				{
				case PAR_LE_BAS :
					p->vitesse.y = V_REBOND_PROJECTILE;
					p->position.y = (float)tuyau.position.y + tuyau.taille.y - p->type_projectile->taille.y + p->type_projectile->ordonnee_haut;

					// MAJ du carré projectile
					projectile.position.y = p->position.y;
					break;
				case PAR_LE_HAUT:
					p->vitesse.y = 0;
					p->position.y = (float)tuyau.position.y - tuyau.taille.y - p->type_projectile->taille.y;

					// MAJ du carré projectile
					projectile.position.y = p->position.y;
					break;
				case PAR_LA_GAUCHE :
					p->tps_vie = 0;
					break;
				case PAR_LA_DROITE:
					p->tps_vie = 0;
					break;
				default:
					break;
				}
			}
	}
}

void solve_collisions_item(occ_item* it, perso** persos, niveau* n, Uint32 duree)
{
	/* Variables pour l'initialisation */
	coordi bloc_bg, bloc_hd;
	int i, j, collision_bloc_releve = 0;

	/* Variables pour la detection et la résolution de collisions */
	int phys_bloc_actuel;
	carre item = {0}, block = {0}, tuyau = {0};
	collision collision;
	float hauteur;

	/* Initialisation des blocs sur lequels se trouve le personnage */
	bloc_bg.x = (int) (it->position.x) / n->taille_blocs.x;
	bloc_bg.y = (int) (it->position.y) / n->taille_blocs.y;

	/* SI c'est une pièce on étend le champ des blocs testables d'un cran vers le bas */
	if(it->type_item->nom == PIECE)
		bloc_bg.y -= 1;

	bloc_hd.x = (int) (it->position.x + it->type_item->taille.x) / n->taille_blocs.x;
	bloc_hd.y = (int) (it->position.y + it->type_item->taille.y) / n->taille_blocs.y;

	/************************************************ DETECTIONS ET RESOLUTIONS DE COLLISIONS *******************************************/

	/* Si l'item est soumis aux collisions avec le niveau => détection et résolution */
	if(it->etat == NORMAL) {

			/* Initialisation du carre projectile */
			item.position.x = it->position.x;
			item.position.y = it->position.y;

			item.position_prec.x = it->position_prec.x;
			item.position_prec.y = it->position_prec.y;

			item.taille.x = it->type_item->taille.x;
			item.taille.y = it->type_item->taille.y;

			/************ COLLISIONS ITEM <=> NIVEAU ***************/
			for(i = bloc_bg.x; i <= bloc_hd.x; i++)
			{
				for(j = bloc_bg.y; j <= bloc_hd.y; j++)
				{

					if(i >= 0 && i < n->taille.x && j >= 0)
					{
						if(n->occ_blocs[i][j]->bloc_actuel < 0 && n->occ_blocs[i][j]->etat == POUSSE_PAR_LE_HAUT && it->type_item->nom == PIECE)
						{
							n->occ_blocs[i][j]->etat = IMMOBILE;
							it->etat = SORT_DU_BLOC;
							it->vitesse.y = VIT_SORTIE_BLOC * 3;
							it->tps_sortie_bloc = TPS_ITEM_SORT_BLOC;
							prend_item(persos[n->occ_blocs[i][j]->id_perso], it->type_item->nom);
						}
						else if(n->occ_blocs[i][j]->bloc_actuel >= 0)
						{
							/* On récupère la physique du bloc */
							phys_bloc_actuel = n->blocs[n->occ_blocs[i][j]->bloc_actuel].phys;

							/* Initialisation du carré bloc */
							block.position.x = (float)n->occ_blocs[i][j]->position.x;
							block.position.y = (float)n->occ_blocs[i][j]->position.y;

							block.position_prec.x = (float)n->occ_blocs[i][j]->position_prec.x;
							block.position_prec.y = (float)n->occ_blocs[i][j]->position_prec.y;

							block.taille.x = n->taille_blocs.x;
							block.taille.y = n->taille_blocs.y;

							find_angle_height_with_phys(phys_bloc_actuel, &block);

							block.est_bloc_pente = (block.angle_pente != 0);

							determinate_collision(item, block, &collision);

							/* Collision avec le sol */
							if((phys_bloc_actuel == SOL 
								|| phys_bloc_actuel == BORD_A_GAUCHE
								|| phys_bloc_actuel == BORD_A_DROITE
								|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
								|| phys_bloc_actuel == COIN_HAUT_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC)
								&& collision.type_collision == PAR_LE_BAS)
							{

								it->position.y = (float)block.position.y + block.taille.y;
								
								/* Si une collision a déjà eu lieu avec un bloc relevé, on ne remodifie pas la vitesse verticale */
								if(!collision_bloc_releve)
									it->vitesse.y = it->type_item->vitesse.y;

								/* Cas où l'item entre en collision avec un bloc tappé par Mario */
 								if(n->occ_blocs[i][j]->etat == POUSSE_PAR_LE_HAUT)
								{
									/* Si jamais la collision a lieu un peu avant que l'item arrive au milieu du bloc */
									if(it->vitesse.x > 0)
									{
										if(n->occ_blocs[i][j]->position.x > item.position.x + item.taille.x / 2)
										{
											it->vitesse.x = -it->vitesse.x;
											it->vitesse.y = VIT_SORTIE_BLOC * 8;
											it->position.x = (float)it->position.x + it->vitesse.x * duree;
										}
									}
									else if (it->vitesse.x < 0)
									{
										if(n->occ_blocs[i][j]->position.x + n->taille_blocs.x / 2 < item.position.x)
										{
											it->vitesse.x = -it->vitesse.x;
											it->vitesse.y = VIT_SORTIE_BLOC * 8;
											it->position.x = (float)it->position.x + it->vitesse.x * duree;
										}
									}
									else // vitesse = 0 -> PIECE
									{
										it->etat = SORT_DU_BLOC;
										it->tps_sortie_bloc = TPS_ITEM_SORT_BLOC;
										it->vitesse.y = VIT_SORTIE_BLOC * 3;
										prend_item(persos[n->occ_blocs[i][j]->id_perso], it->type_item->nom);
									}

									collision_bloc_releve = 1;
								}
								

								// MAJ du carré item
								item.position.y = it->position.y;
								item.position.x = it->position.x;
								determinate_collision(item, block, &collision);
							}

							/* Collision avec les pentes */
							if((phys_bloc_actuel == PENTE_15_DROITE_16
								|| phys_bloc_actuel == PENTE_15_GAUCHE_16
								|| phys_bloc_actuel == PENTE_15_DROITE_12
								|| phys_bloc_actuel == PENTE_15_GAUCHE_12
								|| phys_bloc_actuel == PENTE_15_DROITE_8
								|| phys_bloc_actuel == PENTE_15_GAUCHE_8
								|| phys_bloc_actuel == PENTE_15_DROITE_4
								|| phys_bloc_actuel == PENTE_15_GAUCHE_4

								|| phys_bloc_actuel == PENTE_30_DROITE_0
								|| phys_bloc_actuel == PENTE_30_DROITE_8
								|| phys_bloc_actuel == PENTE_30_GAUCHE_8
								|| phys_bloc_actuel == PENTE_30_GAUCHE_16
								|| phys_bloc_actuel == PENTE_45_DROITE
								|| phys_bloc_actuel == PENTE_45_GAUCHE)
								&& 
								(collision.type_collision == PENTE_PAR_LE_BAS
								|| collision.type_collision == PENTE_PAR_LA_GAUCHE
								|| collision.type_collision == PENTE_PAR_LA_DROITE))
							{

								/* Calcul de la nouvelle ordonnée de l'item 
								avec l'équation de la pente et en fonction du bloc */
								if(phys_bloc_actuel == PENTE_45_GAUCHE)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(135.0)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(135.0)));
								}
								else if(phys_bloc_actuel == PENTE_45_DROITE)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(45.0)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(45.0)));
								}
								else if(phys_bloc_actuel == PENTE_30_GAUCHE_8)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
								}
								else if(phys_bloc_actuel == PENTE_30_GAUCHE_16)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(180.0F - 26.5650512F)) + block.position.y + block.taille.y - LARGEUR_BLOC + block.position.x * tan(deg_to_rad(180.0F - 26.5650512F)));
								}
								else if(phys_bloc_actuel == PENTE_30_DROITE_0)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y + block.position.x * tan(deg_to_rad(26.5650512F)));
								}
								else //if(phys_bloc_actuel == PENTE_30_DROITE_8)
								{
									hauteur = (float) (-(item.position.x + item.taille.x / 2) * tan(deg_to_rad(26.5650512F)) + block.position.y + block.taille.y - 8 + block.position.x * tan(deg_to_rad(26.5650512F)));
								}

								it->position.y = hauteur;
								it->vitesse.y = it->vitesse.y;

								// MAJ du carré item
								item.position.y = it->position.y;
								determinate_collision(item, block, &collision);
							}

							/* Collision avec le plafond */
							if((phys_bloc_actuel == PLAFOND
								|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
								|| phys_bloc_actuel == COIN_BAS_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC )
								&& collision.type_collision == PAR_LE_HAUT)
							{
								it->vitesse.y = 0;
								it->position.y = (float)block.position.y - block.taille.y - it->type_item->taille.y;

								// MAJ du carré item
								item.position.y = it->position.y;
								determinate_collision(item, block, &collision);
							}

							/* Collision avec le mur à gauche du monstre */
							if((phys_bloc_actuel == MUR_A_GAUCHE
								|| phys_bloc_actuel == COIN_HAUT_A_GAUCHE
								|| phys_bloc_actuel == COIN_BAS_A_GAUCHE
								|| phys_bloc_actuel == BLOC_SPEC)
								&& collision.type_collision == PAR_LA_GAUCHE)
							{
								it->vitesse.x = -it->vitesse.x;
								it->position.x = (float)it->position.x + it->vitesse.x * duree;

								// MAJ du carré item
								item.position.x = it->position.x;
								determinate_collision(item, block, &collision);
							}

							/* Collision avec le mur à droite du monstre */
							if((phys_bloc_actuel == MUR_A_DROITE
								|| phys_bloc_actuel == COIN_HAUT_A_DROITE
								|| phys_bloc_actuel == COIN_BAS_A_DROITE
								|| phys_bloc_actuel == BLOC_SPEC) 
								&& collision.type_collision == PAR_LA_DROITE)
							{
								it->vitesse.x = -it->vitesse.x;
								it->position.x = (float)it->position.x + it->vitesse.x * duree;								

								// MAJ du carré item
								item.position.x = it->position.x;
								determinate_collision(item, block, &collision);
							}
						}
					}
				}
			}
			/************ COLLISIONS ITEMS <=> TUYAUX */
 			for(i = 0; i < n->nb_tuyaux; i++)
			{
				tuyau.angle_pente = 0;
				tuyau.est_bloc_pente = 0;
				tuyau.hauteur_a_retirer = 0;

				tuyau.position.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				tuyau.position_prec.x = n->tuyaux[i]->position.x * LARGEUR_BLOC;
				tuyau.position_prec.y = n->tuyaux[i]->position.y * LARGEUR_BLOC;

				if(n->tuyaux[i]->sens_sortie == VERS_LE_HAUT ||
					n->tuyaux[i]->sens_sortie == VERS_LE_BAS) {
						tuyau.taille.x = (int)(2 * LARGEUR_BLOC);
						tuyau.taille.y = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
				}
				else 
				{
					tuyau.taille.x = (int)((n->tuyaux[i]->longueur + 1) * LARGEUR_BLOC);
					tuyau.taille.y = (int)(2 * LARGEUR_BLOC);
				}

				determinate_collision(item, tuyau, &collision);

				switch(collision.type_collision)
				{
				case PAR_LE_BAS :
					it->vitesse.y = it->type_item->vitesse.y;
					it->position.y = (float)tuyau.position.y + tuyau.taille.y;

					// MAJ du carré item
					item.position.y = it->position.y;
					break;
				case PAR_LE_HAUT:
					it->vitesse.y = 0;
					it->position.y = (float)tuyau.position.y - tuyau.taille.y - it->type_item->taille.y;

					// MAJ du carré item
					item.position.y = it->position.y;
					break;
				case PAR_LA_GAUCHE :
					it->vitesse.x = -it->vitesse.x;
					it->position.x = (float)it->position.x + it->vitesse.x * duree;

					// MAJ du carré item
					item.position.x = it->position.x;
					break;
				case PAR_LA_DROITE:
					it->vitesse.x = -it->vitesse.x;
					it->position.x = (float)it->position.x + it->vitesse.x * duree;

					// MAJ du carré item
					item.position.x = it->position.x;
					break;
				default:
					break;
				}
			}
	}
}

void determinate_collision(carre c1, carre c2, collision* collision)
{

	float y_pos, y_pos_prec;

	// Calcul des côtés du carré C1
	float leftC1 = c1.position.x;
	float rightC1 = (float) c1.position.x + c1.taille.x;
	float topC1 = (float) c1.position.y + c1.taille.y;
	float bottomC1 = c1.position.y;

	float leftC1_prec = c1.position_prec.x;
	float rightC1_prec = (float) c1.position_prec.x + c1.taille.x;
	float topC1_prec = (float) c1.position_prec.y + c1.taille.y;
	float bottomC1_prec = c1.position_prec.y;


	// Calcul des côtés de C2
	float leftC2 = c2.position.x;
	float rightC2 = (float) c2.position.x + c2.taille.x;
	float topC2 = (float) c2.position.y + c2.taille.y;
	float bottomC2 = c2.position.y;

	float leftC2_prec = c2.position_prec.x;
	float rightC2_prec = (float) c2.position_prec.x + c2.taille.x;
	float topC2_prec = (float) c2.position_prec.y + c2.taille.y;
	float bottomC2_prec = c2.position_prec.y;

	if(!c2.est_bloc_pente)
	{

		/**** Detection d'un collision en bas du carré 1 *****/
		if(bottomC1 <= topC2
			&& bottomC1_prec >= topC2_prec
			&& 
			((leftC2 <= leftC1 && leftC1 <= rightC2)
			|| (leftC2 <= rightC1 && rightC1 <= rightC2)
			|| (leftC1 <= leftC2 && leftC2 <= rightC1) 
			|| (leftC1 <= rightC2 && rightC2 <= rightC1)))
		{
			collision->type_collision = PAR_LE_BAS;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 1;
		}
		/**** Detection d'une collision en haut du carré 1 *****/
		else if(topC1 >= bottomC2
			&& topC1_prec < bottomC2_prec
			&& 
			((leftC2 <= leftC1 && leftC1 <= rightC2) 
			|| (leftC2 <= rightC1 && rightC1 <= rightC2)
			|| (leftC1 <= leftC2 && leftC2 <= rightC1) 
			|| (leftC1 <= rightC2 && rightC2 <= rightC1)))
		{
			collision->type_collision = PAR_LE_HAUT;
			collision->carre1_est_touche = 1;
			collision->carre2_est_touche = 0;
		}
		/**** Detection d'un collision à droite du carré 1 *****/
		else if(rightC1 >= leftC2
			&& rightC1_prec <= leftC2_prec
			&&
			((bottomC2 <= bottomC1 && bottomC1 <= topC2) 
			|| (bottomC2 <= topC1 && topC1 <= topC2)
			|| (bottomC1 <= bottomC2 && bottomC2 <= topC1) 
			|| (bottomC1 <= topC2 && topC2 <= topC1)))
		{
			collision->type_collision = PAR_LA_DROITE;
			collision->carre1_est_touche = 1;
			collision->carre2_est_touche = 0;
		}
		/**** Detection d'un collision à gauche du carré 1 ******/
		else if(leftC1 <= rightC2
			&& leftC1_prec >= rightC2_prec
			&&
			((bottomC2 <= bottomC1 && bottomC1 <= topC2)
			|| (bottomC2 <= topC1 && topC1 <= topC2)
			|| (bottomC1 <= bottomC2 && bottomC2 <= topC1) 
			|| (bottomC1 <= topC2 && topC2 <= topC1)))
		{
			collision->type_collision = PAR_LA_GAUCHE;
			collision->carre1_est_touche = 1;
			collision->carre2_est_touche = 0;
		}
		/** CAS PARTICULIER : Si les deux carrés se superposent **/
		/* Superposition sur le centre du carré 1 */
		else if(leftC1 + c1.taille.x / 2 > leftC2
			&& leftC1 + c1.taille.x / 2 < rightC2
			&& leftC1_prec + c1.taille.x / 2 > leftC2_prec
			&& leftC1_prec + c1.taille.x / 2 < rightC2_prec
			&& 
			bottomC1 + c1.taille.y / 2 > bottomC2 
			&& bottomC1 + c1.taille.y / 2 < topC2
			&& bottomC1_prec + c1.taille.y / 2 > bottomC2_prec
			&& bottomC1_prec + c1.taille.y / 2 < topC2_prec)
		{
			collision->type_collision = SUPERPOSITION;
			collision->carre1_est_touche = 1;
			collision->carre2_est_touche = 0;
		}
		else
		{
			collision->type_collision = PAS_DE_COLLISION;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 0;
		}
	}
	else
	{

		/**************  CAS OU CE SONT DES PENTES *****************/

		/* Calculs de l'ordonnée du perso avec l'équation de la pente pour savoir s'il est au dessus ou en dessous.

		Equation : y = -x * tan(angle) + c2.position.y + c2.taille.y - c2.hauteur_a_retirer + c2.position.x * tan(angle);

		où angle est en radians !
		*/

		y_pos = (float)( -(c1.position.x + c1.taille.x / 2) * tan(deg_to_rad((float) c2.angle_pente)));
		y_pos += c2.position.y + c2.taille.y - c2.hauteur_a_retirer;
		y_pos += (float)(c2.position.x * tan(deg_to_rad((float) c2.angle_pente)));

		y_pos_prec = (float) (-(c1.position_prec.x + c1.taille.x / 2) * tan(deg_to_rad((float) c2.angle_pente)) + c2.position.y + c2.taille.y - c2.hauteur_a_retirer + (c2.position.x * tan(deg_to_rad((float) c2.angle_pente))));

		// Collision par le bas 
		if(c1.position.y <= y_pos
			&& c1.position_prec.y >= y_pos_prec)
		{
			collision->type_collision = PENTE_PAR_LE_BAS;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 0;
		}
		// Collision sur le côté gauche
		else if(c1.position.y <= y_pos
			&& c1.position_prec.y >= y_pos_prec
			&& c1.position.x < c1.position_prec.x)
		{
			collision->type_collision = PENTE_PAR_LA_GAUCHE;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 0;
		}
		// Collision sur le côté droit
		else if((c1.position.y <= y_pos
			&& c1.position_prec.y >= y_pos_prec)
			&& c1.position.x > c1.position_prec.x)
		{
			collision->type_collision = PENTE_PAR_LA_DROITE;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 0;
		}
		else
		{
			collision->type_collision = PAS_DE_COLLISION;
			collision->carre1_est_touche = 0;
			collision->carre2_est_touche = 0;
		}
	}
}

void touche_perso(perso* p){

	if(p->transformation >= FIRE_MARIO) {
		transforme_perso(SUPER_MARIO, p);
		p->est_invincible = 2000;
		p->tps_transformation = 1000;
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_TOUCHE]);
	}
	else if(p->transformation == SMALL_MARIO) {
		p->etat = MORT;
		p->vitesse.y = VITESSE_Y_EJECTION * 5;
		p->vitesse.x = 0;
		p->tps_mort = TPS_MORT;
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_DIE]);
	}
	else {
		transforme_perso(SMALL_MARIO, p);
		p->est_invincible = 2000;
		p->tps_transformation = 1000;
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_TOUCHE]);
	}


}

void prend_item(perso* p, int item) {

	switch(item)
	{
	case FLEUR :
		/* on teste le niveau de transformation par rapport à
		celui de l'item */
		if(p->transformation <= item)
		{ 
			transforme_perso(FIRE_MARIO, p);
			p->tps_transformation = 1000;
		}
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_TRANSFORME_FEU]);
		p->hud->score += 1000;
		break;
	case CHAMPIGNON :
		if(p->transformation <= item)
		{
			transforme_perso(SUPER_MARIO, p);
			p->tps_transformation = 1000;
		}
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_TRANSFORME_FEU]);
		p->hud->score += 1000;
		break;
	case PIECE :
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_PIECE]);
		p->hud->nb_pieces++;
		/* si le perso n'a pas 100 pièces on s'arrête */
		if(p->hud->nb_pieces < 100)
			break;
		else 
			/* sinon, on remet le nombre de pièces à 0 
			et on continue sur le case suivant qui ajoute une vie */
			p->hud->nb_pieces = 0;
	case CHAMPI_VIE : 
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_VIE]);
		p->hud->nb_vies++;
		break;
	case CHAMPI_POISON :
		if(!p->est_invincible_etoile)
			touche_perso(p);
		break;
	case ETOILE:
		p->est_invincible_etoile = 10000;
		FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_INVINCIBLE]);
		break;
	default: break;
	}
}

vectf *gravity(vectf *v, Uint32 duree)
{
	v->y -= GRAVITY * duree;

	//if(v->y > V_MAX)
	//{
	//	v->y = V_MAX;
	//}

	if(v->y < -V_MAX)
	{
		v->y = -V_MAX;
	}

	return v;
}


vectf *jump(vectf *v)
{
	v->y = VITESSE_SAUT;

	return v;
}


void jump_perso(perso* p)
{
	jump(&p->vitesse);
	p->vitesse.y += p->accel;
	FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_SAUT]);


	p->environnement = AIR;

	/* S'il porte un monstre, ses états différents */
	if(p->monstre_porte == NULL)
	{
		if(p->etat == BAISSE)
			p->etat = SAUT_BAISSE;
		else
			p->etat = SAUT;
	}
	else
	{
		if(p->etat == BAISSE_CARAPACE)
			p->etat = SAUT_BAISSE_CARAPACE;
		else
			p->etat = SAUT_CARAPACE;
	}
}


void frottements(perso *p, Uint32 duree)
{
	float coeff;

	/* Frottements sont différents selon l'environnement 
	dans le lequel se trouve le personnage */
	if(p->environnement == SOL_DUR)
		coeff = FROTT_CLASSIQUE;
	else
		coeff = FROTT_AIR;

	if(duree != 0)
	{
		/* application des frottements */
		p->vitesse.x /= 1 + 5 * coeff * duree * COEFF_GLISSE;
	}

	/* pour eviter que le perso ne se deplace brusquement d'un
	pixel après un moment d'immobilité */
	if(p->vitesse.x < V_MIN && p->vitesse.x > -V_MIN)
		p->vitesse.x = 0;
}


vectf *lim_vitesse(vectf *v)
{
	if(v->x > V_MARCHE)
		v->x = V_MARCHE;

	else if(v->x < -V_MARCHE)
		v->x = -V_MARCHE;

	return v;
}

void solve_acc(perso *p, keystate *k)
{
	float coeff;

	/* Frottements sont différents selon l'environnement 
	dans le lequel se trouve le personnage */
	if(p->environnement == SOL_DUR)
		coeff = FROTT_CLASSIQUE;
	else
		coeff = FROTT_AIR;

	/* Modification de l'accélèration en fonction de l'appui
	ou non sur la touche d'accélèration */
	if(p->etat != FINISH)
	{
		if(k->actuel[RUN])
			p->accel = ACC_COURSE * coeff;
		else
			p->accel = ACC_MARCHE * coeff;
	}
	else
	{
		p->accel = 10 * ACC_COURSE * coeff;
	}
}

/* En principe lorsque le perso est en train de courir et que la touche RUN est lachée, la vitesse
doit diminuer progressivement, avec cette methode, c'est brutal, à ameliorer donc */
void limit_speed(perso *p, keystate *k)
{
	float v = 0;

	if(k->actuel[RUN])
		v = V_COURSE;
	else
		v = V_MARCHE;

	if(p->vitesse.x > v)
		p->vitesse.x = v;
	else if(p->vitesse.x < -v)
		p->vitesse.x = -v;
}

void lateral_move(perso *p, keystate *k, Uint32 t)
{
	if(p->etat != FINISH_CHATEAU)
	{
		if(p->etat != FINISH)
		{
			/* Determination du coté vers lequel regarde le perso */
			if(k->actuel[DROITE])
				p->cote = COTE_DROIT;
			else if(k->actuel[GAUCHE])
				p->cote = COTE_GAUCHE;

			if(k->actuel[GAUCHE] && k->actuel[DROITE])
			{
				p->vitesse.x = 0;
				p->etat = DEBOUT;
			}
			else if(!k->actuel[BAS])
			{
				if(k->actuel[DROITE])
				{
					/* Si il avance à l'opposé de la touche enfoncée, il derape */
					if(p->vitesse.x < 0)
					{
						if(p->environnement != AIR && p->monstre_porte == NULL
							&& p->etat != DERAPE)
						{
							p->etat = DERAPE;
							FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_DERAPE]);
						}
						frottements(p, t);
					}
					else
					{
						if(p->monstre_porte == NULL)
						{
							if(p->environnement == SOL_DUR)
							{
								if(k->actuel[RUN])
									p->etat = COURSE_1;
								else
									p->etat = MARCHE;
							}
						}
						else
						{
							if(p->environnement == SOL_DUR)
								p->etat = MARCHE_CARAPACE;
						}
					}

					p->vitesse.x += p->accel * t;
				}
				else if(k->actuel[GAUCHE])
				{
					/* S'il avance à l'opposé de la touche enfoncée, il derape */
					if(p->vitesse.x > 0)
					{
						if(p->environnement != AIR && p->monstre_porte == NULL
							&& p->etat != DERAPE)
						{
							p->etat = DERAPE;
							FSOUND_PlaySound(FSOUND_FREE, p->sons[SND_DERAPE]);
						}
						frottements(p, t);
					}
					else
					{
						if(p->monstre_porte == NULL)
						{
							if(p->environnement == SOL_DUR)
							{
								if(k->actuel[RUN])
									p->etat = COURSE_1;
								else
									p->etat = MARCHE;
							}
						}
						else
						{
							if(p->environnement == SOL_DUR)
								p->etat = MARCHE_CARAPACE;
						}
					}

					p->vitesse.x -= p->accel * t;
				}
				else if(!p->tps_attaque_speciale && !p->tps_attaque){

					if(p->monstre_porte == NULL) {
						if(p->environnement == AIR)
						{
							if(k->precedent[BAS] && k->actuel[BAS])
								p->etat = SAUT_BAISSE;
						}
						else {
							p->etat = DEBOUT;
							p->hud->nb_monstres_tues = 0;
						}
					}
					else
					{
						if(p->environnement == AIR)
						{
							if(k->precedent[BAS] && k->actuel[BAS])
								p->etat = SAUT_BAISSE_CARAPACE;
						}
						else {
							p->etat = DEBOUT_CARAPACE;
							p->hud->nb_monstres_tues = 0;
						}
					}
				}
			}

			if((!k->actuel[DROITE] && !k->actuel[GAUCHE])
				|| k->actuel[BAS])
				frottements(p, t);
		}
		else
		{
			if(p->vitesse.x > 0)
				p->vitesse.x = p->accel * t;
			else
				p->vitesse.x = p->accel * t;

		}
	}
}

void find_angle_height_with_phys(int phys_bloc, carre* bloc)
{
	switch(phys_bloc)
	{
	case PENTE_45_DROITE :
		bloc->angle_pente = 45;
		bloc->hauteur_a_retirer = 0;
		break;
	case PENTE_45_GAUCHE :
		bloc->angle_pente = 135;
		bloc->hauteur_a_retirer = (int)LARGEUR_BLOC;
		break;
	case PENTE_30_DROITE_0 : 
		bloc->angle_pente = 26.5650512F;
		bloc->hauteur_a_retirer = 0;
		break;
	case PENTE_30_DROITE_8 : 
		bloc->angle_pente = 26.5650512F;
		bloc->hauteur_a_retirer = 8;
		break;
	case PENTE_30_GAUCHE_8 :
		bloc->angle_pente = 153.4349488F;
		bloc->hauteur_a_retirer = 8;
		break;
	case PENTE_30_GAUCHE_16 :
		bloc->angle_pente = 153.4349488F;
		bloc->hauteur_a_retirer = (int)LARGEUR_BLOC;
		break;
	default :
		bloc->angle_pente = 0;
		bloc->hauteur_a_retirer = 0;
		break;
	}
}

void pause_monstre(occ_monstre* monstre, ecran e)
{
	/* Teste si le personnage est hors écran */
	if(monstre->position.x > e.origine.x + e.scroll.x + e.taille.x
		|| monstre->position.x + monstre->type_monstre->taille.x < e.origine.x + e.scroll.x
		|| monstre->position.y > e.origine.y + e.scroll.y + e.taille.y
		|| monstre->position.y + monstre->type_monstre->taille.y < e.origine.y + e.scroll.y)
	{
		/* Si oui, teste si sa position initiale l'est aussi */
		if(monstre->position_ini.x > e.origine.x + e.scroll.x + e.taille.x
		|| monstre->position_ini.x + monstre->type_monstre->taille.x < e.origine.x + e.scroll.x
		|| monstre->position_ini.y > e.origine.y + e.scroll.y + e.taille.y
		|| monstre->position_ini.y + monstre->type_monstre->taille.y < e.origine.y + e.scroll.y)
		{
			monstre->position = monstre->position_ini;
			monstre->position_prec = monstre->position_ini;
			monstre->cote = COTE_DROIT;
		}
		monstre->actif = 0;
	}
	else
	{
		monstre->actif = 1;
	}
}

void pause_projectile(occ_projectile* projectile, ecran e)
{
	/* Teste si le projectile est hors écran */
	if(projectile->position.x > e.origine.x + e.scroll.x + e.taille.x
		|| projectile->position.x + projectile->type_projectile->taille.x < e.origine.x + e.scroll.x
		|| projectile->position.y > e.origine.y + e.scroll.y + e.taille.y
		|| projectile->position.y + projectile->type_projectile->taille.y < e.origine.y + e.scroll.y)
		projectile->actif = 0;
	else
		projectile->actif = 1;
}

void pause_item(occ_item* item, ecran e)
{
	/* Teste si l'item est hors écran */
	if(item->position.x > e.origine.x + e.scroll.x + e.taille.x
		|| item->position.x + item->type_item->taille.x < e.origine.x + e.scroll.x
		|| item->position.y > e.origine.y + e.scroll.y + e.taille.y
		|| item->position.y + item->type_item->taille.y < e.origine.y + e.scroll.y)
		item->actif = 0;
	else
		item->actif = 1;
}

void compte_points(perso* p, occ_monstre* monstre)
{
	coordi text_points = {0};

	text_points.x = (int)(monstre->position.x);
	text_points.y = (int)(monstre->position.y + monstre->type_monstre->taille.y);

	if(p->hud->nb_monstres_tues > TAB_POINTS_LENGTH - 1) {
		p->hud->file_points = add_file_pts(p->hud->file_points, 10000, text_points);
		prend_item(p, CHAMPI_VIE); // pour éviter la dupilcation de code
	}
	else 
	{
		p->hud->file_points = add_file_pts(p->hud->file_points, p->hud->tab_points[p->hud->nb_monstres_tues], text_points);
		p->hud->score += p->hud->tab_points[p->hud->nb_monstres_tues];
		p->hud->nb_monstres_tues++;
	}
}