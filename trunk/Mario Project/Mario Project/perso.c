/*============================================================================*/
/*== [personnage.c] [Version: 0.028a]                                       ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "perso.h"

perso *new_perso(void)
{
	return init_perso(malloc(sizeof(perso)));
}

perso *init_perso(perso *p)
{
	int i;

	if(p != NULL)
	{
		p->etat = 0;
		p->transformation = SMALL_MARIO;

		p->personnage = 0;
		p->environnement = AIR;

		p->position.x = 32;
		p->position.y = 500;
		p->position_prec.x = p->position.x;
		p->position_prec.y = p->position.y;

		p->vitesse.x = 0;
		p->vitesse.y = 0;

		p->est_invincible = 0;
		p->checkpoint = -1;
		p->est_invincible_etoile = 0;
		p->monstre_porte = NULL;

		p->tps_transformation = 0;
		p->tps_attaque_speciale = 0;
		p->tps_attaque = 0;
		p->tps_pousse_carapace = 0;
		p->tps_mort = 0;
		p->tuyau = NULL;

		p->cote = COTE_DROIT;

		p->taille.x = 1;
		p->taille.y = 1;

		p->texture_act = malloc(sizeof(data_texture));
		p->texture_prec = malloc(sizeof(data_texture));

		/* Initialisation du tableau de vitesse d'animation */
		for (i = 0; i < NB_ETATS_TEXTURE; i++){
			p->texture_act->nb_sprites[i] = 0;
			p->texture_prec->nb_sprites[i] = 0;
			p->texture_act->v_anim[i] = 0;
			p->texture_prec->v_anim[i] = 0;
		}

		/* Initialisation du tableau des sons */
		p->sons[SND_SAUT] = FSOUND_Sample_Load(FSOUND_FREE, "musics/Mario_Jump.wav", 0, 0, 0);
		p->sons[SND_TOUCHE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/Mario_Retrecit.wav", 0, 0, 0);
		p->sons[SND_TRANSFORME_FEU] = FSOUND_Sample_Load(FSOUND_FREE, "musics/Transformation_fire.wav", 0, 0, 0);
		p->sons[SND_VIE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/1-up.wav", 0, 0, 0);
		p->sons[SND_FIREBALL] = FSOUND_Sample_Load(FSOUND_FREE, "musics/fireball.wav", 0, 0, 0);
		p->sons[SND_THUNDER_BALL] = FSOUND_Sample_Load(FSOUND_FREE, "musics/thunder_ball.wav", 0, 0, 0);
		p->sons[SND_DERAPE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/Derape.wav", 0, 0, 0);
		p->sons[SND_PIECE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/coin.wav", 0, 0, 0);
		p->sons[SND_BREAK_BLOCK] = FSOUND_Sample_Load(FSOUND_FREE, "musics/break_block.wav", 0, 0, 0);
		p->sons[SND_ITEM_BLOCK] = FSOUND_Sample_Load(FSOUND_FREE, "musics/item_block.wav", 0, 0, 0);
		p->sons[SND_UNBREAKABLE_BLOCK] = FSOUND_Sample_Load(FSOUND_FREE, "musics/unbreakable_block.wav", 0, 0, 0);
		p->sons[SND_INVINCIBLE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/invincible.mp3", 0, 0, 0);
		p->sons[SND_DIE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/die.ogg", 0, 0, 0);
		p->sons[SND_PIPE] = FSOUND_Sample_Load(FSOUND_FREE, "musics/pipe.wav", 0, 0, 0);
		p->sons[SND_CLEAR] = FSOUND_Sample_Load(FSOUND_FREE, "musics/smw-clear.wav", 0, 0, 0);
		p->sons[SND_CHECKPOINT] = FSOUND_Sample_Load(FSOUND_FREE, "musics/checkpoint.wav", 0, 0, 0);


		/* initialisation du HUD */
		init_HUD(p);
	}

	return p;
}

perso* free_perso(perso *p)
{
	int i;

	if(p != NULL)
	{
		for(i = 0; i < NB_SONS; i++)
			FSOUND_Sample_Free(p->sons[i]);
		free(p->texture_act);
		free(p->texture_prec);
		free_file_pts(p->hud->file_points);
		free(p->hud);
		free(p);
	}
	return NULL;
}

perso* copy_perso(perso *p)
{
	perso *p_bis = NULL;
	int i;

	p_bis = malloc(sizeof(perso));
	*p_bis = *p;

	// Copie des infos sur la texture 
	p_bis->texture_act->abscisse_bas = p->texture_act->abscisse_bas;
	p_bis->texture_act->ordonnee_haut = p->texture_act->ordonnee_haut;
	p_bis->texture_act->nb_sprites_max = p->texture_act->nb_sprites_max;
	p_bis->texture_act->texture = p->texture_act->texture;

	for(i = 0; i < NB_ETATS_TEXTURE; i++)
	{
		p_bis->texture_prec->v_anim[i] = p->texture_prec->v_anim[i];
		p_bis->texture_prec->nb_sprites[i] = p->texture_prec->nb_sprites[i];		

		p_bis->texture_act->v_anim[i] = p->texture_act->v_anim[i];
		p_bis->texture_act->nb_sprites[i] = p->texture_act->nb_sprites[i];

	}

	return p_bis;
}

perso* free_copy_perso(perso *p)
{
	if(p != NULL)
		free(p);

	return NULL;
}

int nb_etats_absents(data_texture *d){

	int i, cpt = 0;

	for(i = 0; i < NB_ETATS_TEXTURE; i++){
		if(!d->nb_sprites[i])
			cpt++;
	}

	return cpt;
}

void charger_perso(char *nom, perso* p){

	char nom_texture[TAILLE_NOM_TEXTURE];
	FILE *perso_file;
	int nb_1 = 0, nb_2 = 0, nb_3 = 0, nb_4 = 0, nb_5 = 0, nb_6 = 0, nb_7 = 0, nb_8 = 0, nb_lignes = 0;

	/* Si le nom du fichier contient la chaine 'mario', alors c'est Mario */
	if(strstr(nom, "mario"))
		p->personnage = MARIO;
	else
		p->personnage = LUIGI;

	/* Chargement de la texture */
	strcpy(nom_texture, "textures/persos/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	p->texture_act->texture = charger_texture_bis(nom_texture, &p->taille);

	/* Chargement des données */
	strcpy(nom_texture, "textures/persos/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".perso");

	perso_file = fopen(nom_texture, "r");

    if(perso_file == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom_texture);
        return;
    }

	/* Chargement des constantes pour le calcul des collisions */
	fscanf(perso_file, "abscisse_bas : %d\n", &nb_1);
	p->texture_act->abscisse_bas = nb_1;

	fscanf(perso_file, "ordonnee_haut : %d\n", &nb_1);
	p->texture_act->ordonnee_haut = nb_1;

	/* Chargement du nombre de sprites de chaque état du personnage */
	fscanf(perso_file, "nb_sprites_marche : %d\n", &nb_1);
	p->texture_act->nb_sprites[MARCHE] = nb_1;

	if(nb_1){
		nb_lignes++;
		nb_1 = p->taille.x / nb_1; // stockage de la taille du sprite
	}

	fscanf(perso_file, "nb_sprites_course : %d\n", &nb_2);
	p->texture_act->nb_sprites[COURSE_2] = nb_2;

	if(nb_2){
		nb_lignes++;
		nb_2 = p->taille.x / nb_2;
	}

	fscanf(perso_file, "nb_sprites_saut : %d\n", &nb_3);
	p->texture_act->nb_sprites[SAUT] = nb_3;

	if(nb_3){
		nb_lignes++;
		nb_3 = p->taille.x / nb_3;
	}

	fscanf(perso_file, "nb_sprites_verticaux : %d\n", &nb_4);
	p->texture_act->nb_sprites[DEBOUT] = nb_4;

	if(nb_4){
		nb_lignes++;
		nb_4 = p->taille.x / nb_4;
	}

	fscanf(perso_file, "nb_sprites_marche_carapace : %d\n", &nb_5);
	p->texture_act->nb_sprites[MARCHE_CARAPACE] = nb_5;

	if(nb_5){
		nb_lignes++;
		nb_5 = p->taille.x / nb_5;
	}

	fscanf(perso_file, "nb_sprites_carapace : %d\n", &nb_6);
	p->texture_act->nb_sprites[POUSSE_CARAPACE] = nb_6;

	if(nb_6){
		nb_lignes++;
		nb_6 = p->taille.x / nb_6;
	}

	fscanf(perso_file, "nb_sprites_attaque : %d\n", &nb_7);
	p->texture_act->nb_sprites[ATTAQUE] = nb_7;

	if(nb_7){
		nb_lignes++;
	}
	else nb_7 = 1000; // au cas où il n'y a pas de sprites pour cet état

	fscanf(perso_file, "nb_sprites_attaque_speciale : %d\n", &nb_8);
	p->texture_act->nb_sprites[ATTAQUE_SPECIALE] = nb_8;

	if(nb_8){
		nb_lignes++;
		nb_8 = p->taille.x / nb_8;
	}
	else nb_8 = 1000; // au cas où il n'y a pas de sprites pour cet état

	/* Donne le nombre de sprites maximal sur une ligne */
	p->texture_act->nb_sprites_max = max(p->texture_act->nb_sprites[ATTAQUE], max(p->texture_act->nb_sprites[DEBOUT], max(p->texture_act->nb_sprites[ATTAQUE_SPECIALE], p->texture_act->nb_sprites[POUSSE_CARAPACE])));

	// Cas particulier où il n'y a qu'un seul sprite pour un état
	if(nb_7 == 1) // -> ATTAQUE
		nb_7 = p->taille.x / p->texture_act->nb_sprites_max;

	/* Détermine la taille du personnage en abscisse ... */
	p->taille.x = min(nb_7, min(nb_6, min(nb_5, min(nb_4, min(nb_3, min(nb_1, nb_2))))));

	/* ... et en ordonnee */
	p->taille.y = p->taille.y / nb_lignes;

	/* Chargement des vitesses d'animation de chaque état du personnage */
	fscanf(perso_file, "v_anim_marche : %d\n", &nb_7);
	p->texture_act->v_anim[MARCHE] = nb_7;

	fscanf(perso_file, "v_anim_course : %d\n", &nb_7);
	p->texture_act->v_anim[COURSE_2] = nb_7;

	fscanf(perso_file, "v_anim_saut : %d\n", &nb_7);
	p->texture_act->v_anim[SAUT] = nb_7;

	fscanf(perso_file, "v_anim_verticaux : %d\n", &nb_7);
	p->texture_act->v_anim[DEBOUT] = nb_7;

	fscanf(perso_file, "v_anim_marche_carapace : %d\n", &nb_7);
	p->texture_act->v_anim[MARCHE_CARAPACE] = nb_7;

	fscanf(perso_file, "v_anim_carapace : %d\n", &nb_7);
	p->texture_act->v_anim[POUSSE_CARAPACE] = nb_7;

	fscanf(perso_file, "v_anim_attaque : %d\n", &nb_7);
	p->texture_act->v_anim[ATTAQUE] = nb_7;

	fscanf(perso_file, "v_anim_attaque_speciale : %d\n", &nb_7);
	p->texture_act->v_anim[ATTAQUE_SPECIALE] = nb_7;

	/* fermeture du fichier */
	fclose(perso_file);
}

void copy_data_texture(data_texture* data, data_texture* copy)
{
	int i;

	copy->abscisse_bas = data->abscisse_bas;
	copy->ordonnee_haut = data->ordonnee_haut;
	copy->nb_sprites_max = data->nb_sprites_max;
	copy->texture = data->texture;

	for(i = 0; i < NB_ETATS_TEXTURE; i++)
	{
		copy->nb_sprites[i] = data->nb_sprites[i];
		copy->v_anim[i] = data->v_anim[i];
	}
}

void transforme_perso(int transformation_future, perso* p){

	data_texture copy, *text_act = p->texture_act;
	int i;

	copy_data_texture(text_act, &copy);

	/* Selon le futur état du personnage,
	on charge la texture appropriée */
	switch(transformation_future) {
		case SMALL_MARIO :
			charger_perso("small_mario", p);
			p->transformation = SMALL_MARIO;
			break;
		case SUPER_MARIO :
			charger_perso("super_mario", p);
			p->transformation = SUPER_MARIO;
			break;
		case FIRE_MARIO :
			charger_perso("fire_mario", p);
			p->transformation = FIRE_MARIO;
			break;
		default : break;
	}

	/* Copie des champs en rapport avec la texture précédente */
	p->texture_prec->abscisse_bas = copy.abscisse_bas;
	p->texture_prec->ordonnee_haut = copy.ordonnee_haut;
	p->texture_prec->nb_sprites_max = copy.nb_sprites_max;
	p->texture_prec->texture = copy.texture;

	for(i = 0; i < NB_ETATS_TEXTURE; i++)
	{
		p->texture_prec->v_anim[i] = copy.v_anim[i];
		p->texture_prec->nb_sprites[i] = copy.nb_sprites[i];
	}
}

void init_HUD(perso* p)
{
	p->hud = malloc(sizeof(HUD));

	p->hud->nb_pieces = 0;
	p->hud->nb_vies = 3;
	p->hud->nom_niveau = NULL;
	//p->hud->reserve_item = NULL;
	p->hud->score = 0;
	p->hud->nb_monstres_tues = 0;
	p->hud->nb_monstres_tues_carapace = 0;
	p->hud->personnage = p->personnage;
	p->hud->file_points = NULL;
    p->hud->font = load_font_2d("fonts/smb.fnt");

	/* Définition du tableau des scores */
	p->hud->tab_points[0] = 100;
	p->hud->tab_points[1] = 200;
	p->hud->tab_points[2] = 400;
	p->hud->tab_points[3] = 800;
	p->hud->tab_points[4] = 1000;
	p->hud->tab_points[5] = 2000;
	p->hud->tab_points[6] = 4000;
	p->hud->tab_points[7] = 8000;
}

/* Fonctions se rapportant à la gestion de la file dans le HUD */
file_pts* new_file_pts(int points, coordi position)
{
	file_pts* file = malloc(sizeof(file_pts));
	file->element = malloc(sizeof(elem_file_pts));

	file->element->points = points;
	file->element->pos_points = position;
	file->element->tps_affichage = 1000;
	file->suivant = NULL;
	file->precedent = NULL;

	return file;
}

file_pts* add_file_pts(file_pts* file, int points, coordi position) {

	file_pts* debut_file = new_file_pts(points, position);

	debut_file->precedent = NULL;
	debut_file->suivant = file;

	if(file != NULL)
		file->precedent = debut_file;
	else
		file = debut_file;

	return debut_file;
}

file_pts* free_file_pts(file_pts* file)
{
	file_pts* file_prec = NULL;

	/* On se déplace en fin de file */
	if(file != NULL) {
		while(file->suivant != NULL)
			file = file->suivant;

		/* Ensuite, tant que le temps d'affichage est <= 0, on supprime les éléments de la file */
		while(file != NULL && file->element->tps_affichage <= 0) {
			file_prec = file->precedent;

			free(file->element);
			free(file);

			file = file_prec;
		}

		if(file != NULL) {
			/* On replace le pointeur en début de file */
			while(file->precedent != NULL)
				file = file->precedent;

			if(file != NULL)
				file->suivant = NULL;
		}
	}

	return file;
}
