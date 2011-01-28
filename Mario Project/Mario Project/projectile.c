/*============================================================================*/
/*== [projectile.c] [Version: 0.028a]                                       ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/


#include "projectile.h"

projectile* new_projectile(char* nom)
{

	projectile* proc = malloc(sizeof(projectile));

	return init_projectile(proc, nom);
}

projectile *init_projectile(projectile* p, char* nom)
{
	if(p != NULL)
	{
		p->taille.x = 0;
		p->taille.y = 0;

		p->texture = 0;

		p->vitesse.x = 0;
		p->vitesse.y = 0;
		p->occ_projectiles = new_liste_projectile();

		p->v_anim_marche = 0;
		p->v_anim_mort = 0;
		strcpy(p->nom_text, nom);
	}

	return p;
}



occ_projectile* new_occ_projectile(int tps_vie, int tps_apparition, int tps_disparition){

	occ_projectile* occ_proc = malloc(sizeof(occ_projectile));

	return init_occ_projectile(occ_proc, tps_vie, tps_apparition, tps_disparition);
}

occ_projectile* init_occ_projectile(occ_projectile *p, int tps_vie, int tps_apparition, int tps_disparition){

	if(p != NULL){
		p->position.x = 0;
		p->position.y = 0;

		p->position_prec.x = 0;
		p->position_prec.y = 0;

		p->vitesse.x = 0;
		p->vitesse.y = 0;

		p->cote = COTE_DROIT;

		p->type_projectile = NULL;

		p->tps_apparition = tps_apparition;
		p->tps_vie = tps_vie;
		p->tps_disparition = tps_disparition;		
	}
	return p;
}

occ_projectile* copy_projectile(occ_projectile* projectile){

	occ_projectile* p_bis = NULL;
	
	p_bis = malloc(sizeof(occ_projectile));
	*p_bis = *projectile;

    return p_bis;
}

elem_projectile* creer_element_projectile(occ_projectile* proj){

	elem_projectile* elt = malloc(sizeof(elem_projectile));

	if(elt != NULL){
		elt->occ_projectile = proj;
		elt->suivant = NULL;
	}

	return elt;
}

liste_projectile* new_liste_projectile(){

	liste_projectile* lp = malloc(sizeof(liste_projectile));
	
	if(lp != NULL){
		lp->projectile= NULL;
		lp->nb_elements = 0;
	}
	return lp;
}

liste_projectile* ajout_projectile(liste_projectile* liste, occ_projectile* proj){

	elem_projectile* nouveau = creer_element_projectile(proj);

	/* Ajoute un projectile en début de liste si celui-ci n'est pas nul */
	if(liste->projectile != NULL)
		nouveau->suivant = liste->projectile;

	liste->projectile = nouveau;
	liste->nb_elements++;

	return liste;
}

liste_projectile* supprime_projectile(liste_projectile* liste, occ_projectile *p){

	elem_projectile *actuel = liste->projectile;
	elem_projectile *precedent = NULL;

	/* Si la liste est vide, on renvoie NULL */
	if(liste->projectile == NULL)
		return NULL;


	/* Parcours de la liste des items */
	while(actuel->occ_projectile != p
		&& actuel->suivant != NULL)
	{
		precedent = actuel;
		actuel = actuel->suivant;
	}

	/* Si l'item a été trouvé */
	if(actuel->occ_projectile == p)
	{
		/* Si c'était le premier élément de la liste */
		if(precedent == NULL)
			liste->projectile = actuel->suivant;
		else
			precedent->suivant = actuel->suivant;

		free(actuel->occ_projectile);
		free(actuel);
	}
	
	liste->nb_elements--;

	return liste;
}

occ_projectile* create_debris(projectile* proj, int x, int y, float vitesse_x, float vitesse_y)
{
	occ_projectile* occ_p = new_occ_projectile(proj->tps_vie, proj->tps_apparition, proj->tps_disparition);
	
	occ_p->position.x = (float)x;
	occ_p->position.y = (float)y;

	occ_p->vitesse.x = vitesse_x;
	occ_p->vitesse.y = vitesse_y;
	occ_p->type_projectile = proj;

	occ_p->envoyeur = NIVEAU;

	return occ_p;
}

projectile* charger_projectile(char* nom){

	char nom_texture[TAILLE_NOM_TEXTURE];
	FILE *proj_file;
	float tmp;
	int tmp_2;
	projectile* proj = new_projectile(nom);

	// Chargement de la texture et initialisation de la taille
	strcpy(nom_texture, "textures/projectiles/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	proj->texture = charger_texture_bis(nom_texture, &proj->taille);

	// Chargement des infos concernant le projectile
	strcpy(nom_texture, "textures/projectiles/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".proj");

	proj_file = fopen(nom_texture, "r");

    if(proj_file == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom_texture);
        return NULL;
    }

	fscanf(proj_file, "vitesse_x : %f\n", &tmp);
	proj->vitesse.x = tmp;

	/* Chargement des constantes pour le calcul des collisions */
	fscanf(proj_file, "abscisse_bas : %d\n", &tmp_2);
	proj->abscisse_bas = tmp_2;

	fscanf(proj_file, "ordonnee_haut : %d\n", &tmp_2);
	proj->ordonnee_haut = tmp_2;

	/* Chargement du nombre de sprites de chaque état du projectile */
	fscanf(proj_file, "nb_sprites_marche : %d\n", &tmp_2);
	proj->nb_sprites_marche = tmp_2;

	fscanf(proj_file, "nb_sprites_mort : %d\n", &tmp_2);
	proj->nb_sprites_mort = tmp_2;

	/* Chargement de la vitesse d'animation de chaque état du projectile */
	fscanf(proj_file, "v_anim_marche : %d\n", &tmp_2);
	proj->v_anim_marche = tmp_2;
	
	fscanf(proj_file, "v_anim_mort : %d\n", &tmp_2);
	proj->v_anim_mort = tmp_2;

	/* Chargement des temps d'apparition, de vie et de disparition du projectile */
	fscanf(proj_file, "tps_apparition : %d\n", &tmp_2);
	proj->tps_apparition = tmp_2;

	fscanf(proj_file, "tps_vie : %d\n", &tmp_2);
	proj->tps_vie = tmp_2;

	fscanf(proj_file, "tps_disparition : %d\n", &tmp_2);
	proj->tps_disparition = tmp_2;

	/* Chargement du type de soumission du projectile */
	fscanf(proj_file, "soumission : %x\n", &tmp_2);
	proj->soumission = tmp_2;

	fclose(proj_file);

	/* Calcul de la taille d'un sprite à partir des données chargées */
	proj->taille.x = proj->taille.x / proj->nb_sprites_marche;
	proj->taille.y = (proj->nb_sprites_mort == 0)? proj->taille.y : proj->taille.y / 2;

	return proj;
}
