/*============================================================================*/
/*== [monstre.c] [Version: 0.028a]                                          ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "monstre.h"


monstre* new_monstre(void)
{
	monstre *m = malloc(sizeof(monstre));

	return init_monstre(m);
}

monstre* init_monstre(monstre *m)
{

	if(m != NULL)
	{
		int i;

		m->taille.x = 0;
		m->taille.y = 0;
		m->abscisse_bas = 0;
		m->est_tuable_par_boule_feu = 0;
		m->texture = 0;
		m->tps_disparition = 0;
		m->tps_sortie_tuyau = 650;
		m->occ_monstres = new_liste_monstre();

		/* Initialisation du tableau des sons */
		m->sons[SND_JUMP_ON] = FSOUND_Sample_Load(FSOUND_FREE, "musics/JumpOnMonster.wav", 0, 0, 0);
		m->sons[SND_PROJ_ON] = FSOUND_Sample_Load(FSOUND_FREE, "musics/ProjOnMonster.wav", 0, 0, 0);
		m->sons[SND_LEVEL_ON] = FSOUND_Sample_Load(FSOUND_FREE, "musics/CollisionsMonsterLevel.wav", 0, 0, 0);

		for(i = 0; i < M_NB_ETATS; i++)
		{
			m->nb_sprites[i] = 0;
			m->v_anim[i] = 0;
		}
	}

	return m;
}

monstre* free_monstre(monstre *m)
{
	
	int i;

	if(m != NULL)
	{
		/* A compléter avec les nouveaux champs qui viendront s'y ajouter */
		for(i = 0; i < NB_M_SONS; i++)
            FSOUND_Sample_Free(m->sons[i]);
		free(m);

	}

	return NULL;
}

occ_monstre* new_occ_monstre(float position_x, float position_y, monstre* type_monstre)
{
	occ_monstre *m = malloc(sizeof(occ_monstre));

	return init_occ_monstre(m, position_x, position_y, type_monstre);
}

occ_monstre*  init_occ_monstre(occ_monstre *m, float position_x, float position_y, monstre* type_monstre)
{

	if(m != NULL)
	{
		m->position.x = position_x;
		m->position.y = position_y;

		m->position_ini = m->position;
		m->position_prec = m->position;

		m->vitesse.x = -(float)M_V_MARCHE;
		m->vitesse.y = 0;
	
		m->etat = M_MARCHE;
		m->cote = COTE_GAUCHE;
		m->actif = 1;

		m->type_monstre = type_monstre;
		m->tps_retracte = 0;
	}
	return m;
}

occ_monstre* copy_monstre(occ_monstre *m){
	
	occ_monstre *m_bis = NULL;

    m_bis = malloc(sizeof(occ_monstre));
    *m_bis = *m;

    return m_bis;
}

occ_monstre* free_copy_monstre(occ_monstre *m){

	if(m != NULL)
		free(m);

	return NULL;
}

elem_monstre* creer_element_monstre(occ_monstre* m){

	elem_monstre* elt = malloc(sizeof(elem_monstre));

	if(elt != NULL)
	{
		elt->occ_monstre = m;
		elt->suivant = NULL;
	}
	return elt;
}

liste_monstre* new_liste_monstre(){

	liste_monstre* liste = malloc(sizeof(liste_monstre));

	if(liste != NULL){
		liste->monstre = NULL;
		liste->nb_elements = 0;
	}

	return liste;
}

liste_monstre* ajout_monstre(liste_monstre* liste, occ_monstre *m){

	elem_monstre* nouveau = creer_element_monstre(m);

	/* Ajoute un monstre en début de liste */
	if(liste != NULL)
		nouveau->suivant = liste->monstre;

	liste->monstre = nouveau;
	liste->nb_elements++;

	return liste;
}

liste_monstre *supprime_monstre(liste_monstre* liste, occ_monstre *m)
{
	elem_monstre* actuel = liste->monstre;
	elem_monstre* precedent = NULL;

	if(liste == NULL)
		return NULL;

	/* Parcours de la liste de monstres */
	while(actuel->occ_monstre != m
		&& actuel->suivant != NULL)
	{
		precedent = actuel;
		actuel = actuel->suivant;
	}

	/* Si le monstre a été trouvée */
	if(actuel->occ_monstre == m)
	{
		/* Si c'était le premier élément de la liste */
		if(precedent == NULL)
		{
			liste->monstre = actuel->suivant;
			free(actuel->occ_monstre);
			free(actuel);
		}
		else
		{
			precedent->suivant= actuel->suivant;
			free(actuel->occ_monstre);
			free(actuel);
		}
	}

	liste->nb_elements--;

	return liste;
}

monstre *charger_monstre(char* nom){
	
	char nom_texture[TAILLE_NOM_TEXTURE];
	FILE *mstr_file;
	monstre* m = new_monstre();
	int nb = 0, nb_1 = INT_MAX, nb_2 = INT_MAX, nb_3 = INT_MAX, nb_lignes = 0;

	// Chargement de la texture 
	strcpy(nom_texture, "textures/monstres/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".png");

	m->texture = charger_texture_bis(nom_texture, &m->taille);
	
	/* Chargement des données */
	strcpy(nom_texture, "textures/monstres/");
	strcat(nom_texture, nom);
	strcat(nom_texture, ".mstr");

	mstr_file = fopen(nom_texture, "r");

    if(mstr_file == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom_texture);
        return NULL;
    }

	/* Chargement des constantes pour les collisions */
	fscanf(mstr_file, "abscisse_bas : %d\n", &nb);
	m->abscisse_bas = nb;

	/* Chargement du nombre de sprites de chaque état du monstre */
	fscanf(mstr_file, "nb_sprites_marche : %d\n", &nb);
	m->nb_sprites[M_MARCHE] = nb;
	
	if(nb){
		nb_lignes++;
		nb_1 = m->taille.x / nb;
	}

	fscanf(mstr_file, "nb_sprites_carapace : %d\n", &nb);
	m->nb_sprites[M_RETRACTE] = nb;
	
	if(nb)
	{
		nb_lignes++;
		nb_2 = m->taille.x / nb;
	}

	fscanf(mstr_file, "nb_sprites_sort_carapace : %d\n", &nb);
	m->nb_sprites[M_SORT_CARAPACE] = nb;
	
	if(nb)
	{
		nb_lignes++;
		nb_3 = m->taille.x / nb;
	}

	fscanf(mstr_file, "est_tuable_par_saut : %d\n", &nb);
	m->est_tuable_par_saut = nb;

	if(nb)
	{
		nb_lignes++;
		m->tps_disparition = M_TPS_DISPARITION;
	}

	fscanf(mstr_file, "est_tuable_par_boule_feu : %d\n", &nb);
	m->est_tuable_par_boule_feu = nb;

	if(!m->est_tuable_par_saut && m->est_tuable_par_boule_feu
		&& m->nb_sprites[M_RETRACTE] == 0) {
		nb_lignes++;
	}	

	fscanf(mstr_file, "peut_sauter_dessus : %d\n", &nb);
	m->peut_sauter_dessus = nb;

	fscanf(mstr_file, "reste_sur_plateforme : %d\n", &nb);
	m->reste_sur_plateforme = nb;

	fscanf(mstr_file, "v_anim_marche : %d\n", &nb);
	m->v_anim[M_MARCHE] = nb;

	fscanf(mstr_file, "v_anim_carapace : %d\n", &nb);
	m->v_anim[M_RETRACTE] = nb;

	fscanf(mstr_file, "v_anim_sort_carapace : %d\n", &nb);
	m->v_anim[M_SORT_CARAPACE] = nb;

	/* Calcul la taille d'un sprite de la texture */
	m->taille.x = (m->nb_sprites[M_RETRACTE] == 0)?nb_1:min(nb_3, min(nb_1, nb_2));
	m->taille.y = m->taille.y / nb_lignes;

	m->nb_sprites_max = max(m->nb_sprites[M_RETRACTE], max(m->nb_sprites[M_MARCHE], m->nb_sprites[M_SORT_CARAPACE]));
    strcpy(m->nom, nom);

	fclose(mstr_file);

	return m;
}

int m_nb_etats_absents(monstre* m)
{
	int cpt = 0;

	if(m->nb_sprites[M_MARCHE] == 0)
		cpt++;

	if(m->est_tuable_par_saut || m->est_tuable_par_boule_feu)
		cpt--;

	if(m->nb_sprites[M_RETRACTE] == 0)
		cpt++;

	if(m->nb_sprites[M_SORT_CARAPACE] == 0)
		cpt++;


	return (cpt < 0)?0:cpt;
}
