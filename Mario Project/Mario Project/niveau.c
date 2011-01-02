/*============================================================================*/
/*== [niveau.c] [Version: 0.028a]                                           ==*/
/*============================================================================*/
/*===         _____         _        _____           _         _           ===*/
/*===        |     |___ ___|_|___   |  _  |___ ___  |_|___ ___| |_         ===*/
/*===        | | | | .'|  _| | . |  |   __|  _| . | | | -_|  _|  _|        ===*/
/*===        |_|_|_|__,|_| |_|___|  |__|  |_| |___|_| |___|___|_|          ===*/
/*===                                             |___|                    ===*/
/*============================================================================*/
/*===      =LagMan=                =McCaml=                 =MouleMan=     ===*/
/*============================================================================*/

#include "niveau.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

niveau* new_niveau(void)
{ 
	niveau *n = NULL;

	n = malloc(sizeof(niveau));

	return init_niveau(n);
}


niveau* init_niveau(niveau *n)
{
	int i;

	if(n == NULL) return n;

	for(i = 0; i < TAILLE_NOM_NIVEAU; i++)
	{
		n->nom[i] = '\0';
	}

	for(i = 0; i < TAILLE_TITRE_MUSIQUE; i++)
	{
		n->titre_zik[i] = '\0';
	}

	/* Points clés */
	n->taille.x = 0;
	n->taille.y = 0;
	n->spawn.x = 0;
	n->spawn.y = 0;
	n->checkpoint.x = 0;
	n->checkpoint.y = 0;
	n->last_texture = 0;

	/* Finish */
	n->nb_finish = 0;
	n->finishes = NULL;

	/* Arriere plans */
	n->nb_backgrounds = 0;
	n->backgrounds = NULL;

	/* Premiers plans */
	n->nb_foregrounds = 0;
	n->foregrounds = NULL;

	/* Monstres */
	n->nb_monstres = 0;
	n->monstres = NULL;

	/* Items */
	n->nb_items = 0;
	n->items = NULL;

	/* Projectiles */
	n->nb_projectiles = 0;
	n->projectiles = NULL;

	/* Particules */
	n->nb_background_generators = 0;
	n->nb_foreground_generators = 0;

	n->background_generators = NULL;
	n->foreground_generators = NULL;

	/* Tuyaux */
	n->nb_tuyaux = 0;
	n->tuyaux = NULL;

	/* Objets */
	n->nb_objets = 0;
	n->objets = NULL;
	n->id_objets = NULL;

	/* Blocs */
	n->nb_blocs = 0;
	n->blocs = NULL;
	n->occ_blocs = NULL;
	n->taille_blocs.x = 0;
	n->taille_blocs.y = 0;

	/* Textures des blocs */
	n->nb_textures = 0;
	n->textures = NULL;

	return n;
}


/************************** à mettre à jour ***********************/
/* ajouter un appel à void liberer_textures_niveau(niveau*n) dedans */
niveau *free_niveau(niveau *n)
{
	int i;

	if(n == NULL) 
		return NULL;

	/* Libération items */
	if(n->items != NULL)
	{
		for(i = 0; i < n->nb_items; i++)
		{
			elem_item* item_suiv = NULL;
			while(n->items[i]->occ_items->item != NULL) {
				item_suiv = n->items[i]->occ_items->item->suivant;
				supprime_item(n->items[i]->occ_items, n->items[i]->occ_items->item->occ_item);
				n->items[i]->occ_items->item = item_suiv;
			}
		}
		free(n->items);
	}

	/* Libération monstres */
	if(n->monstres != NULL)
	{
		for(i = 0; i < n->nb_monstres; i++)
		{
			elem_monstre* mstr_suiv = NULL;
			while(n->monstres[i]->occ_monstres->monstre != NULL) {
				mstr_suiv = n->monstres[i]->occ_monstres->monstre->suivant;
				supprime_monstre(n->monstres[i]->occ_monstres, n->monstres[i]->occ_monstres->monstre->occ_monstre);
				n->monstres[i]->occ_monstres->monstre = mstr_suiv;
			}
			free(n->monstres[i]->occ_monstres);
			free(n->monstres[i]);
		}
		free(n->monstres);
	}

	/* Libération projectiles */
	if(n->projectiles != NULL)
	{
		for(i = 0; i < n->nb_projectiles; i++)
		{
			free(n->projectiles[i]->occ_projectiles);
			free(n->projectiles[i]);
		}
		free(n->projectiles);
	}

	if(n->finishes != NULL)
	{
		free(n->finishes);
	}

	/* Libération blocs */
	if(n->blocs != NULL)
	{
		for(i = 0; i < n->taille.x; i++)
		{
			if(n->occ_blocs[i] != NULL) 
				free(n->occ_blocs[i]);
		}

		free(n->occ_blocs);
	}

	if(n->blocs != NULL)
		free(n->blocs);

	/* Libération des objets */
	if(n->id_objets != NULL)
	{
		for(i = 0; i < n->taille.x; i++)
		{
			if(n->id_objets[i] != NULL) 
				free(n->id_objets[i]);
		}

		free(n->id_objets);
	}

	if(n->objets != NULL)
		free(n->objets);

	/* Libérations tuyaux */
	if(n->tuyaux != NULL)
	{
		for(i = 0; i < n->nb_tuyaux; i++)
		{
			free_tuyau(n->tuyaux[i]);
		}
		free(n->tuyaux);
	}

	/* Libérations des générateurs */
	if(n->background_generators != NULL)
	{
		for(i = 0; i < n->nb_background_generators; i++)
		{
			free_particule_generator(&n->background_generators[i]);
		}
		free(n->background_generators);
	}

	if(n->foreground_generators != NULL)
	{
		for(i = 0; i < n->nb_foreground_generators; i++)
		{
			free_particule_generator(&n->foreground_generators[i]);
		}
		free(n->foreground_generators);
	}
	

	/* Libération arrières-plans */
	if(n->backgrounds != NULL)
		free(n->backgrounds);

	/* Libération avants-plans */
	if(n->foregrounds != NULL)
		free(n->foregrounds);

	/* Libération textures */
	if(n->textures != NULL)
		free(n->textures);

	/* Libération du reste du niveau */
	free(n);

	return NULL;
}


void balise_level(niveau *n, const char **attrs)
{
    strcpy(n->nom, attrs[1]);
    n->taille.x = atoi(attrs[3]);
    n->taille.y = atoi(strchr(attrs[3], ':') + 1);

    /* Projectiles
    n->occ_projectiles = new_liste_projectile();
    */

    /* Items
    n->occ_items = new_liste_item();
    */
}


void balise_spawn(niveau *n, const char **attrs)
{
    n->spawn.x = atoi(attrs[1]);
    n->spawn.y = atoi(strchr(attrs[1], ':') + 1);
}


void balise_check(niveau *n, const char **attrs)
{
    n->checkpoint.x = atoi(attrs[1]);
    n->checkpoint.y = atoi(strchr(attrs[1], ':') + 1);
}


void balise_backgrounds(niveau *n, const char **attrs)
{
    n->nb_backgrounds = atoi(attrs[1]);
    n->backgrounds = malloc(sizeof(background) * n->nb_backgrounds);
}


void balise_background(niveau *n, const char **attrs)
{
    int i = atoi(attrs[1]);                             // remplaçable par un int static, incrémenté à chaque passage
    strcpy(n->backgrounds[i].nom_text, attrs[3]);
}


void balise_monsters(niveau *n, const char **attrs)
{
    /*
    n->nb_monstres = atoi(attrs[1]);
	n->monstres = malloc(sizeof(monstre*) * n->nb_monstres);
    n->occ_monstres = new_liste_monstre();
    */
}


void balise_monster(niveau *n, const char **attrs)
{
    /*
    n->monstres[atoi(attrs[3])] = charger_monstre(attrs[1]);
    */
}


void balise_occ(niveau *n, const char **attrs)
{
    /*
    n->occ_monstres = ajout_monstre(n->occ_monstres, new_occ_monstre());
    n->occ_monstres->monstre->occ_monstre->type_monstre = n->monstres[i];          // il faut un compteur pour i
	n->occ_monstres->monstre->occ_monstre->position.x = atoi(attrs[1]);
	n->occ_monstres->monstre->occ_monstre->position.y = atoi(strchr(attrs[1], ':') + 1);
    n->occ_monstres->monstre->occ_monstre->tps_disparition = n->occ_monstres->monstre->occ_monstre->type_monstre->tps_disparition;  // WTF????
    */
}


void balise_blocs(niveau *n, const char **attrs)
{
    int i;

    n->nb_blocs = atoi(attrs[1]);
    n->blocs = malloc(n->nb_blocs * sizeof(bloc));
    n->taille_blocs.x = LARGEUR_BLOC;
	n->taille_blocs.y = LARGEUR_BLOC;
    n->occ_blocs = malloc(n->taille.x * sizeof(id*));

	for(i = 0; i < n->taille.x; i++)
	{
		n->occ_blocs[i] = malloc(n->taille.y * sizeof(id));
	}
}


void balise_bloc(niveau *n, const char **attrs)
{
    attrs[1];   // img
}


void balise_layers(niveau *n, const char **attrs)
{
    attrs[1];   // nombre
}


void balise_layer(niveau *n, const char **attrs)
{
    attrs[1];   // index
}


void debut_element(void *user_data, const xmlChar *name, const xmlChar **attrs) 
{
    static const xmlChar *elements[] = {
        BAD_CAST"level", 
        BAD_CAST"spawn", 
        BAD_CAST"check", 
        BAD_CAST"backgrounds", 
        BAD_CAST"background", 
        BAD_CAST"monsters", 
        BAD_CAST"monster", 
        BAD_CAST"occ", 
        BAD_CAST"blocs", 
        BAD_CAST"bloc", 
        BAD_CAST"layers", 
        BAD_CAST"layer"
    };
    static const balise_func functions[] = {
        balise_level,
        balise_spawn,
        balise_check,
        balise_backgrounds,
        balise_background,
        balise_monsters,
        balise_monster,
        balise_occ,
        balise_blocs,
        balise_blocs,
        balise_layers,
        balise_layer
    };
    int i;

    for(i = 0; i < 12; i++)
    {
        if(!xmlStrcmp(name, elements[i]))
        {
            functions[i](user_data, (const char **)attrs);
            break;
        }
    }
}


void caractere(void *ctx, const xmlChar *ch, int len)
{
}


void charger_niveau(char *nom, niveau *n)
{
    xmlSAXHandler sh = {NULL};
    sh.startElement = debut_element;
    sh.characters = caractere;

    if(xmlSAXUserParseFile(&sh, n, nom))
        puts("Il y a eu une couille");
}


static int tabs = 0;


void open_element(FILE *f, char *name)
{
    int i;

    for(i = 0; i < tabs; i++)
        fprintf(f, "\t");

    tabs++;

    fprintf(f, "<%s", name);
}


void end_element(FILE *f)
{
    fprintf(f, ">\n");
}


void close_element(FILE *f, char *name)
{
    int i;
    tabs--;

    for(i = 0; i < tabs; i++)
        fprintf(f, "\t");

    fprintf(f, "</%s>\n", name);
}


void close_element_short(FILE *f)
{
    tabs--;

    fprintf(f, "/>\n");
}


void add_attrib(FILE *f, char *name, char *format, ...)
{
    char value[256];
    va_list va;

    va_start(va, format);
    vsnprintf(value, 256, format, va);
    va_end(va);

    fprintf(f, " %s=\"%s\"", name, value);
}


void sauver_niveau(char *nom, niveau *n)
{
    int i, j, k;
    FILE *fic = fopen(nom, "wb");

    /* Balise niveau */
    open_element(fic, "level");
    add_attrib(fic, "name", "%s", n->nom);
    add_attrib(fic, "taille", "%d:%d", n->taille.x, n->taille.y);
    end_element(fic);

    /* Spawn */
    open_element(fic, "spawn");
    add_attrib(fic, "pos", "%d:%d", n->spawn.x, n->spawn.y);
    close_element_short(fic);

    /* Checkpoint */
    open_element(fic, "check");
    add_attrib(fic, "pos", "%d:%d", n->checkpoint.x, n->checkpoint.y);
    close_element_short(fic);

    /* Backgrounds */
    open_element(fic, "backgrounds");
    add_attrib(fic, "nb", "%d", n->nb_backgrounds);
    end_element(fic);
    for(i = 0; i < n->nb_backgrounds; i++)
    {
        open_element(fic, "background");
        add_attrib(fic, "index", "%d", i);
        add_attrib(fic, "img", "%s", n->backgrounds[i].nom_text);
        close_element_short(fic);
    }
    close_element(fic, "backgrounds");

    /* Monstres */
    open_element(fic, "monsters");
    add_attrib(fic, "nb", "%d", n->nb_monstres);
    end_element(fic);
    for(i = 0; i < n->nb_monstres; i++)
    {
        monstre *m = n->monstres[i];
        elem_monstre *occ_m = m->occ_monstres->monstre;

        open_element(fic, "monster");
        add_attrib(fic, "name", "%s", m->nom);
		add_attrib(fic, "nb", "%d", m->occ_monstres->nb_elements);
        end_element(fic);

        while(occ_m != NULL)
        {
            open_element(fic, "occ");
            add_attrib(fic, "pos", "%d:%d",
                (int)occ_m->occ_monstre->position.x,
                (int)occ_m->occ_monstre->position.y);
            close_element_short(fic);
            occ_m = occ_m->suivant;
        }
        close_element(fic, "monster");
    }
    close_element(fic, "monsters");

    /* Blocs */
    open_element(fic, "blocs");
    add_attrib(fic, "nb", "%d", n->nb_textures);
    end_element(fic);
    for(i = 0; i < n->nb_textures; i++)
    {
        open_element(fic, "bloc");
        add_attrib(fic, "img", "%s", n->textures[i].nom);
        close_element_short(fic);
    }
    open_element(fic, "layers");
    add_attrib(fic, "nb", "%d", 1);
    end_element(fic);
    for(i = 0; i < 1; i++)
    {
        open_element(fic, "layer");
        add_attrib(fic, "index", "%d", i);
        end_element(fic);
        fseek(fic, -1, SEEK_CUR);
        for(j = 0; j < n->taille.x; j++)
            for(k = 0; k < n->taille.y; k++)
                fprintf(fic, "%d:", n->occ_blocs[j][k]);
        close_element(fic, "layer");
    }
    close_element(fic, "layers");
    close_element(fic, "blocs");

    close_element(fic, "level");

    fclose(fic);
}


void charger_textures_niveau(niveau *n)
{
	NB i;

	/* Chargement des textures de fond */
	for(i = 0; i < n->nb_backgrounds; i++)
	{
		charger_objet_background(&n->backgrounds[i], FAUX);
	}

	/* Chargement des textures de premier plan */
	for(i = 0; i < n->nb_foregrounds; i++)
	{
		charger_objet_background(&n->foregrounds[i], FAUX);
	}

	/* Chargement des textures d'arrivées */
	for(i = 0; i < n->nb_finish; i++)
	{
		charger_finish(&n->finishes[i]);
	}

	/* Chargement des textures des objets */
	for(i = 0; i < n->nb_objets ; i++)
	{
		charger_objet_background((background*) &n->objets[i], VRAI);
	}

	/* Chargement des infos de texture des blocs */
	for(i = 0; i < n->nb_textures; i++)
	{
		charger_infos_texture(&n->textures[i]);
	}
    perror("charger_textures_niveau()");

	/* Chargement des textures des blocs */
	for(i = 0; i < n->nb_textures; i++)
	{
		n->textures[i].id_text = 
			charger_texture_bis(n->textures[i].nom, &n->textures[i].taille);
	}
}


void liberer_textures_niveau(niveau* n)
{
	NB i;
	int j;

	/* Liberation des textures de fond */
	for(i = 0; i < n->nb_backgrounds; i++)
	{
		for(j = 0; j < n->backgrounds[i].nb_sprites; j++)
			glDeleteTextures(1, &n->backgrounds[i].id_text[j]);
	
		free(n->backgrounds[i].id_text);
	}

	/* Libération des textures d'arrivée */
	for(i = 0; i < n->nb_finish; i++)
		glDeleteTextures(1, &n->finishes[i].id_text);

	/* Libération des textures des items */
	for(i = 0; i < n->nb_items; i++)
		glDeleteTextures(1, &n->items[i]->texture);

	/* Libération des textures des monstres */
	for(i = 0; i < n->nb_monstres; i++)
		glDeleteTextures(1, &n->monstres[i]->texture);

	/* Libération des textures des projectiles */
	for(i = 0; i < n->nb_projectiles; i++)
		glDeleteTextures(1, &n->projectiles[i]->texture);

	/* Liberation des textures des objets */
	for(i = 0; i < n->nb_objets; i++) {
		for(j = 0; j < n->objets[i].nb_sprites; j++)
			glDeleteTextures(1, &n->objets[i].id_text[j]);

			free(n->objets[i].id_text);
	}

	/* Liberation des textures des blocs */
	for(i = 0; i < n->nb_textures; i++)
		glDeleteTextures(1, &n->textures[i].id_text);
}

void charger_finish(finish* f)
{
	char nom_texture[TAILLE_NOM_TEXTURE];
	int i;

	strcpy(nom_texture, "textures/Objects/");
	strcat(nom_texture, f->nom_text);
	strcat(nom_texture, ".png");

	f->id_text = charger_texture_bis(nom_texture, &f->taille);
}

void charger_objet_background(background* b, int is_object)
{
	FILE* back_file;
	char nom_texture[TAILLE_NOM_TEXTURE], test[TAILLE_NOM_TEXTURE];
	int nb_1, i;

	/* Chargement de la texture */
	if(is_object)
		strcpy(nom_texture, "textures/Objects/");
	else
		strcpy(nom_texture, "textures/Backgrounds/");

	strcat(nom_texture, b->nom_text);
	strcat(nom_texture, ".obj");

	back_file = fopen(nom_texture, "r");

	fscanf(back_file, "nb_sprites : %d\n", &nb_1);
	b->nb_sprites = nb_1;

	if(b->nb_sprites > 1)
	{
		fscanf(back_file, "v_anim : %d\n", &nb_1);
		b->v_anim = nb_1;
	}
	else
		b->v_anim = 0;

	fclose(back_file);

	/* Chargement de la texture */
	b->id_text = malloc(sizeof(GLuint) * b->nb_sprites);

	if(b->nb_sprites > 1)
	{
		for(i = 0; i < b->nb_sprites; i ++)
		{
			if(is_object)
				strcpy(nom_texture, "textures/Objects/");
			else
				strcpy(nom_texture, "textures/Backgrounds/");

			strcat(nom_texture, b->nom_text);
			strcat(nom_texture, itoa(i, test, 10));
			strcat(nom_texture, ".png");

			b->id_text[i] = charger_texture_bis(nom_texture, &b->taille);
		}
	}
	else
	{
		if(is_object)
			strcpy(nom_texture, "textures/Objects/");
		else
			strcpy(nom_texture, "textures/Backgrounds/");

		strcat(nom_texture, b->nom_text);
		strcat(nom_texture, ".png");

		b->id_text[0] = charger_texture_bis(nom_texture, &b->taille);
	}
}


void charger_niveau_test_vide(niveau *n)
{
	int i, j;

	strcpy(n->nom, "==Ceci est un niveau vide==");

	/* Points clés */
	n->taille.x = 100;
	n->taille.y = 50;
	n->spawn.x = 64;
	n->spawn.y = 320;
	n->checkpoint.x = 600;
	n->checkpoint.y = 160;

	/* Layer monstre */
	n->nb_monstres = 0;
	
	/* Layer projectile */
	n->nb_projectiles = 0;
	n->projectiles = NULL;

	/* Layer items */
	n->nb_items = 0;
	n->items = NULL;
	
	/* Arriere plans */
	n->nb_backgrounds = 1;
	n->backgrounds = malloc(sizeof(background) * n->nb_backgrounds);
	strcpy(n->backgrounds[0].nom_text, "SnowHills");
	
	/* Layer tuyaux */
	n->nb_tuyaux = 0;
	n->tuyaux = NULL;

	/* Init Blocs et objets */
	n->nb_blocs = 0;
	n->nb_objets = 0;
	n->blocs = NULL;
	n->objets = NULL;
	n->taille_blocs.x = (int)LARGEUR_BLOC;
	n->taille_blocs.y = (int)LARGEUR_BLOC;
	n->id_objets = malloc(n->taille.x * sizeof(id*));

	for(i = 0; i < n->taille.x; i++)
	{
		n->id_objets[i] = malloc(n->taille.y * sizeof(id));
	}

	// Remplissage des ID des objets 
	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			n->id_objets[i][j] = OBJET_VIDE;
		}
	}

	/************ layer blocs ***********/

	n->occ_blocs = malloc(n->taille.x * sizeof(id*));
	for(i = 0; i < n->taille.x; i++)
	{
		n->occ_blocs[i] = malloc(n->taille.y * sizeof(id));
	}

	/* Remplissage des ID des blocs */
	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
				n->occ_blocs[i][j] = NULL;
		}
	}
	
	n->nb_textures = 0;
	n->textures = NULL;
}


void charger_niveau_test(niveau *n)
{
	int i, j;

	strcpy(n->nom, "==Ceci est un niveau test==");

	/* Points clés */
	n->taille.x = 100;
	n->taille.y = 50;
	n->spawn.x = 64;
	n->spawn.y = 320;
	n->checkpoint.x = 600;
	n->checkpoint.y = 160;

	/* Layer monstre */
	n->nb_monstres = 1;
	n->monstres = malloc(sizeof(monstre*) * n->nb_monstres);
	n->monstres[0] = charger_monstre("koopa");

	n->monstres[0]->occ_monstres = ajout_monstre(n->monstres[0]->occ_monstres, new_occ_monstre(400, 400, n->monstres[0]));

	/* Layer projectile */
	n->nb_projectiles = 2;
	n->projectiles = malloc(sizeof(projectile*) * n->nb_projectiles);
	n->projectiles[FIREBALL] = charger_projectile("fireball_dead", FIREBALL);
	n->projectiles[DEBRIS] = charger_projectile("debris", DEBRIS);	

	/* Layer items */
	n->nb_items = 3;
	n->items = malloc(sizeof(item*) * n->nb_items);
	n->items[0] = charger_piece();
	n->items[1] = charger_champignon();
	n->items[2] = charger_fleur();
	
	// Pièces
	n->items[0]->occ_items = ajout_item(n->items[0]->occ_items, new_occ_item(128, 172, n->items[0], n->items[0]->vitesse, NORMAL));
	n->items[0]->occ_items = ajout_item(n->items[0]->occ_items, new_occ_item(160, 172, n->items[0], n->items[0]->vitesse, NORMAL));
	n->items[0]->occ_items = ajout_item(n->items[0]->occ_items, new_occ_item(192, 172, n->items[0], n->items[0]->vitesse, NORMAL));

	/* Finish */
	/*n->nb_finish = 1;
	n->finishes = malloc(sizeof(finish) * n->nb_finish);
	strcpy(n->finishes[0].nom_text, "big_castle");
	n->finishes[0].position.x = 20 * LARGEUR_BLOC;
	n->finishes[0].position.y = 2 * LARGEUR_BLOC;*/
	
	/* Premiers plans */
	/*n->nb_foregrounds = 1;
	n->foregrounds = malloc(sizeof(background) * n->nb_foregrounds);

	strcpy(n->foregrounds[0].nom_text, "fire");*/

	/* Arriere plans */
	n->nb_backgrounds = 1;
	n->backgrounds = malloc(sizeof(background) * n->nb_backgrounds);

	strcpy(n->backgrounds[0].nom_text, "SnowHills");
	
	/* Layer tuyaux */
	n->nb_tuyaux = 2;
	n->tuyaux = malloc(n->nb_tuyaux * sizeof(tuyau*));

	n->tuyaux[0] = charger_tuyau("green_pipe", VERS_LA_DROITE);
	n->tuyaux[0]->longueur = 2;
	n->tuyaux[0]->position.x = 0;
	n->tuyaux[0]->position.y = 20;
	n->tuyaux[0]->etat = FERME;
	n->tuyaux[0]->pipe_dest = 0;
	n->tuyaux[0]->level_dest = NULL;

	n->tuyaux[0]->monstre = NULL;

	n->tuyaux[1] = charger_tuyau("green_pipe", VERS_LE_HAUT);
	n->tuyaux[1]->longueur = 3;
	n->tuyaux[1]->position.x = 10;
	n->tuyaux[1]->position.y = 0;
	n->tuyaux[1]->etat = OUVERT;
	n->tuyaux[1]->pipe_dest = 0;
	n->tuyaux[1]->level_dest = NULL;

	n->tuyaux[1]->monstre = NULL;

	/* Layer Particules */
	/*n->nb_foreground_generators = 1;
	n->foreground_generators = malloc(sizeof(particule_generator*) * n->nb_foreground_generators);
	n->foreground_generators[0] = new_particule_generator_snow(pos_gen, 4000, 200);*/

	/* Init Blocs et objets */
	n->nb_blocs = 24;

	n->nb_objets = 0;
	n->blocs = malloc(n->nb_blocs * sizeof(bloc));
	n->objets = malloc(n->nb_objets * sizeof(objet));
	n->taille_blocs.x = (int)LARGEUR_BLOC;
	n->taille_blocs.y = (int)LARGEUR_BLOC;

	/*********** layer objets ************/
	n->id_objets = malloc(n->taille.x * sizeof(id*));

	for(i = 0; i < n->taille.x; i++)
	{
		n->id_objets[i] = malloc(n->taille.y * sizeof(id));
	}

	// Remplissage des ID des objets 
	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			n->id_objets[i][j] = OBJET_VIDE;
		}
	}

	/* On place quelques objets pour tester... */
	//n->id_objets[10][2] = 0;
	//n->id_objets[10][20] = 1;

	// remplissage des objets 
	//strcpy(n->objets[0].nom_text, "cristal");

	//strcpy(n->objets[1].nom_text, "cloud_2_test");

	/************ layer blocs ***********/

	n->occ_blocs = malloc(n->taille.x * sizeof(occ_bloc***));

	for(i = 0; i < n->taille.x; i++)
	{
		n->occ_blocs[i] = malloc(n->taille.y * sizeof(occ_bloc**));
	}

	/* Remplissage des ID des blocs */
	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			if(j == 1 && i < 75)
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, &n->blocs[0], NULL);
			else if (i == 0)
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, &n->blocs[2], NULL);
			else if (i > 10 && i < 16 && (j == 6 || j == 7))
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, &n->blocs[18], NULL);
			else if (i == 1 && j == 6)
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, &n->blocs[17], &n->blocs[19]);
			/*else if (i == 17 && j == 6)
				n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, &n->blocs[18], NULL);
			else if (i == 14 && j == 6)
				n->occ_blocs[i][j] = 18;
			else if (i == 14 && j == 10)
				n->occ_blocs[i][j] = 17;
			else if (i == 13 && j == 14)
				n->occ_blocs[i][j] = 18;
			else if (i == 15 && j == 6)
				n->occ_blocs[i][j] = 17;
			else if(j == 1 && i >= 75)
				n->occ_blocs[i][j] = 23;
			//// pente 30° en bas à gauche
			//else if(j == 1 && i <= 1)
			//	n->occ_blocs[i][j] = 0;
			//else if(j == 1 && i == 2)
			//	n->occ_blocs[i][j] = 14;
			//else if(j == 1 && i == 3)
			//	n->occ_blocs[i][j] = 15;
			//// pente 45° à gauche
			//else if(j == 9 && i <= 1)
			//	n->occ_blocs[i][j] = 0;
			//else if(j == 8 && i == 3)
			//	n->occ_blocs[i][j ] = 11;
			//else if(j == 9 && i == 2)
			//	n->occ_blocs[i][j] = 11;
			//else if(j == 8 && i == 2)
			//	n->occ_blocs[i][j] = 8;
			//else if(j == 8 && i == 1)
			//	n->occ_blocs[i][j] = 9;
			//// pente 30° en haut à droite
			//else if(j == 15 && i == 41)
			//	n->occ_blocs[i][j] = 13;
			//else if(j == 15 && i == 42)
			//	n->occ_blocs[i][j] = 12;
			//else if(j == 15 && i == 43)
			//	n->occ_blocs[i][j] = 0;
			//// pente 45° à droite
			//else if (j == 8 && i == 9)
			//	n->occ_blocs[i][j]= 16;
			//else if (j == 8 && i == 10)
			//	n->occ_blocs[i][j]= 11;
			//// Autres
			//else if(j == 7 && 0 <= i && i < 25)
			//	n->occ_blocs[i][j] = 0;
			//else if(j == 14 && 25 < i && i < 45)
			//	n->occ_blocs[i][j] = 0;
			//else if(j == 6 && 0 <= i && i < 25)
			//	n->occ_blocs[i][j] = 1;
			//else if(j == 13 && 25 < i && i < 45)
			//	n->occ_blocs[i][j] = 1;
			//else if (i == 44 && j > 14)
			//	n->occ_blocs[i][j] = 3;
			//else if(j == 7 && i == 25)
			//	n->occ_blocs[i][j] = 4;
			//else if(j == 14 && i == 25)
			//	n->occ_blocs[i][j] = 5;
			//else if(j == 6 && i == 25)
			//	n->occ_blocs[i][j] = 6;
			//else if(j == 13 && i == 25)
			//	n->occ_blocs[i][j] = 7;*/
			else
				n->occ_blocs[i][j] = new_occ_bloc(i * LARGEUR_BLOC, j * LARGEUR_BLOC, NULL, NULL);
		}
	}

	/* remplissage des blocs du niveau */
	n->blocs[0].texture = 0;
	n->blocs[0].coord_sprite.x = 2;
	n->blocs[0].coord_sprite.y = 0;
	n->blocs[0].phys = SOL;
	n->blocs[0].est_vide = VRAI;
	n->blocs[0].est_cassable = FAUX;
	n->blocs[0].item = NULL;

	n->blocs[1].texture = 0;
	n->blocs[1].coord_sprite.x = 8;
	n->blocs[1].coord_sprite.y = 0;
	n->blocs[1].phys = PLAFOND;
	n->blocs[1].est_vide = VRAI;
	n->blocs[1].est_cassable = FAUX;
	n->blocs[1].item = NULL;

	n->blocs[2].texture = 0;
	n->blocs[2].coord_sprite.x = 6;
	n->blocs[2].coord_sprite.y = 0;
	n->blocs[2].phys = MUR_A_GAUCHE;
	n->blocs[2].est_vide = VRAI;
	n->blocs[2].est_cassable = FAUX;
	n->blocs[2].item = NULL;

	n->blocs[3].texture = 0;
	n->blocs[3].coord_sprite.x = 4;
	n->blocs[3].coord_sprite.y = 0;
	n->blocs[3].phys = MUR_A_DROITE;
	n->blocs[3].est_vide = VRAI;
	n->blocs[3].est_cassable = FAUX;
	n->blocs[3].item = NULL;

	n->blocs[4].texture = 0;
	n->blocs[4].coord_sprite.x = 3;
	n->blocs[4].coord_sprite.y = 0;
	n->blocs[4].phys = COIN_HAUT_A_GAUCHE;
	n->blocs[4].est_vide = VRAI;
	n->blocs[4].est_cassable = FAUX;
	n->blocs[4].item = NULL;

	n->blocs[5].texture = 0;
	n->blocs[5].coord_sprite.x = 1;
	n->blocs[5].coord_sprite.y = 0;
	n->blocs[5].phys = COIN_HAUT_A_DROITE;
	n->blocs[5].est_vide = VRAI;
	n->blocs[5].est_cassable = FAUX;
	n->blocs[5].item = NULL;

	n->blocs[6].texture = 0;
	n->blocs[6].coord_sprite.x = 9;
	n->blocs[6].coord_sprite.y = 0;
	n->blocs[6].phys = COIN_BAS_A_GAUCHE;
	n->blocs[6].est_vide = VRAI;
	n->blocs[6].est_cassable = FAUX;
	n->blocs[6].item = NULL;

	n->blocs[7].texture = 0;
	n->blocs[7].coord_sprite.x = 7;
	n->blocs[7].coord_sprite.y = 0;
	n->blocs[7].phys = COIN_BAS_A_DROITE;
	n->blocs[7].est_vide = VRAI;
	n->blocs[7].est_cassable = FAUX;
	n->blocs[7].item = NULL;

	n->blocs[8].texture = 0;
	n->blocs[8].coord_sprite.x = 4;
	n->blocs[8].coord_sprite.y = 1;
	n->blocs[8].phys = PLEIN;
	n->blocs[8].est_vide = VRAI;
	n->blocs[8].est_cassable = FAUX;
	n->blocs[8].item = NULL;

	n->blocs[9].texture = 0;
	n->blocs[9].coord_sprite.x = 5;
	n->blocs[9].coord_sprite.y = 0;
	n->blocs[9].phys = PLEIN;
	n->blocs[9].est_vide = VRAI;
	n->blocs[9].est_cassable = FAUX;
	n->blocs[9].item = NULL;
	
	n->blocs[10].texture = 0;
	n->blocs[10].coord_sprite.x = 11;
	n->blocs[10].coord_sprite.y = 0;
	n->blocs[10].phys = BORD_A_DROITE;
	n->blocs[10].est_vide = VRAI;
	n->blocs[10].est_cassable = FAUX;
	n->blocs[10].item = NULL;

	n->blocs[11].texture = 0;
	n->blocs[11].coord_sprite.x = 0;
	n->blocs[11].coord_sprite.y = 1;
	n->blocs[11].phys = PENTE_45_DROITE;
	n->blocs[11].est_vide = VRAI;
	n->blocs[11].est_cassable = FAUX;
	n->blocs[11].item = NULL;

	n->blocs[12].texture = 0;
	n->blocs[12].coord_sprite.x = 3;
	n->blocs[12].coord_sprite.y = 2;
	n->blocs[12].phys = PENTE_30_GAUCHE_8;
	n->blocs[12].est_vide = VRAI;
	n->blocs[12].est_cassable = FAUX;
	n->blocs[12].item = NULL;

	n->blocs[13].texture = 0;
	n->blocs[13].coord_sprite.x = 2;
	n->blocs[13].coord_sprite.y = 2;
	n->blocs[13].phys = PENTE_30_GAUCHE_16;
	n->blocs[13].est_vide = VRAI;
	n->blocs[13].est_cassable = FAUX;
	n->blocs[13].item = NULL;

	n->blocs[14].texture = 0;
	n->blocs[14].coord_sprite.x = 0;
	n->blocs[14].coord_sprite.y = 2;
	n->blocs[14].phys = PENTE_30_DROITE_0;
	n->blocs[14].est_vide = VRAI;
	n->blocs[14].est_cassable = FAUX;
	n->blocs[14].item = NULL;

	n->blocs[15].texture = 0;
	n->blocs[15].coord_sprite.x = 1;
	n->blocs[15].coord_sprite.y = 2;
	n->blocs[15].phys = PENTE_30_DROITE_8;
	n->blocs[15].est_vide = VRAI;
	n->blocs[15].est_cassable = FAUX;
	n->blocs[15].item = NULL;

	n->blocs[16].texture = 0;
	n->blocs[16].coord_sprite.x = 1;
	n->blocs[16].coord_sprite.y = 1;
	n->blocs[16].phys = PENTE_45_GAUCHE;
	n->blocs[16].est_vide = VRAI;
	n->blocs[16].est_cassable = FAUX;
	n->blocs[16].item = NULL;

	/* BLOC '?' */
	n->blocs[17].texture = 1;
	n->blocs[17].coord_sprite.x = 0;
	n->blocs[17].coord_sprite.y = 3;
	n->blocs[17].phys = BLOC_SPEC;
	n->blocs[17].est_vide = FAUX;
	n->blocs[17].est_cassable = FAUX;
	n->blocs[17].item = NULL;

	/* Bloc cassable */
	n->blocs[18].texture = 1;
	n->blocs[18].coord_sprite.x = 0;
	n->blocs[18].coord_sprite.y = 4;
	n->blocs[18].phys = BLOC_SPEC;
	n->blocs[18].est_vide = VRAI;
	n->blocs[18].est_cassable = VRAI;
	n->blocs[18].item = NULL;

	/* Boc incassable */
	n->blocs[19].texture = 1;
	n->blocs[19].coord_sprite.x = 0;
	n->blocs[19].coord_sprite.y = 2;
	n->blocs[19].phys = BLOC_SPEC;
	n->blocs[19].est_vide = VRAI;
	n->blocs[19].est_cassable = FAUX;
	n->blocs[19].item = NULL;

	n->blocs[20].texture = 1;
	n->blocs[20].coord_sprite.x = 0;
	n->blocs[20].coord_sprite.y = 4;
	n->blocs[20].phys = BLOC_SPEC;
	n->blocs[20].est_vide = VRAI;
	n->blocs[20].est_cassable = FAUX;
	n->blocs[20].item = n->items[1];

	n->blocs[21].texture = 1;
	n->blocs[21].coord_sprite.x = 0;
	n->blocs[21].coord_sprite.y = 4;
	n->blocs[21].phys = BLOC_SPEC;
	n->blocs[21].est_vide = VRAI;
	n->blocs[21].est_cassable = FAUX;
	n->blocs[21].item = n->items[1];

	n->blocs[22].texture = 1;
	n->blocs[22].coord_sprite.x = 0;
	n->blocs[22].coord_sprite.y = 4;
	n->blocs[22].phys = BLOC_SPEC;
	n->blocs[22].est_vide = VRAI;
	n->blocs[22].est_cassable = FAUX;
	n->blocs[22].item = n->items[1];

	n->blocs[23].texture = 1;
	n->blocs[23].coord_sprite.x = 0;
	n->blocs[23].coord_sprite.y = 1;
	n->blocs[23].phys = EAU;
	n->blocs[23].est_vide = VRAI;
	n->blocs[23].est_cassable = FAUX;
	n->blocs[23].item = NULL;

	// Nouvelle ecriture pour la texture : n->[n->occ_blocs[i][j]].texture
	// Flag correspondant à la physique du bloc : n->blocs[n->occ_blocs[i][j]].phys
	// Ajout d'informations supplémentaires : n->blocs[n->occ_blocs[i][j]].est_vide & n->blocs[n->occ_blocs[i][j]].est_cassable
	
	n->nb_textures = 2;
	n->textures = malloc(sizeof(texture) * n->nb_textures);

	strcpy(n->textures[0].nom, "floor_rc4.png");
	strcpy(n->textures[1].nom, "blocs_spec.png");
}


void affiche_occ_blocs(niveau* n)
{
	int i, j;
	FILE* flux = fopen("test.txt","w");

	for(i = n->taille.x - 1; i >= 0 ; i--){ 
		for(j = 0; j < n->taille.y; j++){

			if(n->occ_blocs[i][j]->bloc_actuel != NULL)
			{
				fprintf(flux, "%d ", n->occ_blocs[i][j]);
			}
			else
			{
				fprintf(flux, "0 ", n->occ_blocs[i][j]);
			}
		}
		fprintf(flux,"\n");
	}
	fclose(flux);
}


void draw_main(niveau *lvl, perso **persos, ecran e, Uint32 duree)
{
    id i;

    /* decallage de la "camera" pour commencer au point d'origine choisit */
    glPushMatrix();
	glTranslated(e.origine.x, e.origine.y, 0);

    /* Dessin du ou des arrieres-plans */
    draw_background(lvl, e, duree);

	for(i = 0; i < lvl->nb_background_generators; i++)
	{
		draw_particules(lvl->background_generators[i]);
	}

    /* decallage de la "camera" pour gerer le scrolling */
    glTranslated(-e.scroll.x, -e.scroll.y, 0);

    /* Dessin des objets en arriere plan */
    draw_objects(lvl, duree);

	/* Dessin des arrivées */
	for(i = 0; i < lvl->nb_finish; i++)
	{
		finish f = lvl->finishes[i];
		draw_sprite(f.position.x, f.position.y, f.taille.x, f.taille.y, f.id_text, 0, 1, 0, 1);
	}

	/* Dessin des items */
	for(i = 0; i < lvl->nb_items; i++)
	{
		elem_item *tmp_item = lvl->items[i]->occ_items->item;
		while(tmp_item != NULL)
		{
			if(tmp_item->occ_item->actif)
				draw_item(tmp_item->occ_item, duree);

			tmp_item = tmp_item->suivant;
		}
	}

	/* Dessin du plan du personnage */
    draw_blocs(lvl, e, duree);
	
	/* Dessin des monstres */
	for(i = 0; i < lvl->nb_monstres; i++)
	{
		elem_monstre *tmp_monstre = lvl->monstres[i]->occ_monstres->monstre;
		while(tmp_monstre != NULL)
		{
			if(tmp_monstre->occ_monstre->actif)
				draw_monstre(tmp_monstre->occ_monstre, duree, persos[0]->tps_transformation);
			tmp_monstre = tmp_monstre->suivant;
		}
	}

	/* Dessin des projectiles */
	for(i = 0; i < lvl->nb_projectiles; i++)
	{
		elem_projectile *tmp_proj = lvl->projectiles[i]->occ_projectiles->projectile;
		while(tmp_proj != NULL)
		{
			draw_projectile(tmp_proj->occ_projectile, duree);
			tmp_proj = tmp_proj->suivant;
		}
	}

	/* Dessin du/des personnage/s */
    for(i = 0; i < 1; i++) /* i < nb_persos */
    {
        draw_perso(persos[i], duree);
		draw_HUD(persos[i]);
	}
	
	for(i = 0; i < lvl->nb_tuyaux; i++)
	{
		draw_pipe(lvl->tuyaux[i]);
	}

#ifdef _DEBUG

	// Mise en couleur de la hitbox du personnage
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4f(0, 1, 0, 0.5);

	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y + persos[0]->texture_act->ordonnee_haut);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->taille.x - persos[0]->texture_act->abscisse_bas,(GLfloat) persos[0]->position.y + persos[0]->texture_act->ordonnee_haut);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->taille.x - persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y);

	glColor4f(1, 1, 1, 1);
	glEnd();
	glEnable(GL_TEXTURE_2D);

#endif

	/* Dessin des particules en avant-plan s'il y en a */
	for(i = 0; i < lvl->nb_foreground_generators; i++)
	{
		draw_particules(lvl->foreground_generators[i]);
	}

	/* Dessin du ou des avants-plans */
    draw_foreground(lvl, e, duree);

    glPopMatrix();
}


void draw_main_options(niveau *lvl, ecran e, Uint32 duree, int bck, int blocs, int objets)
{
    int i;

    glScissor(e.origine.x, e.origine.y, e.taille.x, e.taille.y);
    glEnable(GL_SCISSOR_TEST);

    /* decallage de la "camera" pour commencer au point d'origine choisit */
    glPushMatrix();
    glTranslated(e.origine.x, e.origine.y, 0);

    if(bck)
        draw_background(lvl, e, duree);

    glTranslated(-e.scroll.x, -e.scroll.y, 0);

    if(objets)
        draw_objects(lvl, duree);

    /* Dessin des items */
	for(i = 0; i < lvl->nb_items; i++)
	{
		elem_item *tmp_item = lvl->items[i]->occ_items->item;
		while(tmp_item != NULL)
		{
			draw_item(tmp_item->occ_item, duree);
			tmp_item = tmp_item->suivant;
		}
	}

    if(blocs)
        draw_blocs(lvl, e, duree);

    /* Dessin des monstres */
	for(i = 0; i < lvl->nb_monstres; i++)
	{
		elem_monstre *tmp = lvl->monstres[i]->occ_monstres->monstre;
		while(tmp != NULL)
		{
			draw_monstre(tmp->occ_monstre, duree, 0);
			tmp = tmp->suivant;
		}
	}

    /* Dessin des tuyaux */
    for(i = 0; i < lvl->nb_tuyaux; i++)
	{
		draw_pipe(lvl->tuyaux[i]);
	}

    /* Dessin du ou des avants-plans */
    draw_foreground(lvl, e, duree);

    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}

// A METTRE DANS SPRITE.C
void draw_foreground(niveau *n, ecran e, Uint32 duree)
{
    int i;
    coordf decallage;
    coordf text_coord;

	int phase, v_anim;

	decallage.x = (float)e.scroll.x / 5000 * n->nb_backgrounds * 2;
    decallage.y = (float)e.scroll.y / 20000;

    glColor4f(1, 1, 1, 1);

    for(i = 0; i < n->nb_foregrounds; i++)
    {
        text_coord.x  = (float)e.taille.x / n->foregrounds[i].taille.x;
        text_coord.y  = (float)e.taille.y / n->foregrounds[i].taille.y;

		if(n->foregrounds[i].nb_sprites > 1)
		{

			// stockage de la vitesse d'animation du foreground actuel
			v_anim = n->foregrounds[i].v_anim;

			// Détermination de quelle image doit être affichée
			phase = (duree % v_anim) / (v_anim / n->foregrounds[i].nb_sprites);

			draw_sprite(
			e.origine.x + e.scroll.x,
            0,
			e.taille.x,
			n->foregrounds[i].taille.y,
            n->foregrounds[i].id_text[phase],
			decallage.x,
			decallage.x + text_coord.x,
			decallage.y,
			1);
		}
		else 
		{
			// Affichage de l'image
			draw_sprite(
            e.origine.x + e.scroll.x,
            0,
			e.taille.x,
			n->foregrounds[i].taille.y,
            n->foregrounds[i].id_text[0],
			decallage.x,
			decallage.x + text_coord.x,
			decallage.y,
			1);
		}
    }
}

// A METTRE DANS SPRITE.C
void draw_background(niveau *n, ecran e, Uint32 duree)
{
    int i;
    coordf decallage;
    coordf text_coord;

	int phase, v_anim;

    decallage.x = (float)e.scroll.x / 5000;
    decallage.y = (float)e.scroll.y / 20000;

    glColor4f(1, 1, 1, 1);

    for(i = 0; i < n->nb_backgrounds; i++)
    {
        text_coord.x  = (float)e.taille.x / n->backgrounds[i].taille.x;
        text_coord.y  = (float)e.taille.y / n->backgrounds[i].taille.y;

		if(n->backgrounds[i].nb_sprites > 1)
		{

			// stockage de la vitesse d'animation du background actuel
			v_anim = n->backgrounds[i].v_anim;

			// Détermination de quelle image doit être affichée
			phase = (duree % v_anim) / (v_anim / n->backgrounds[i].nb_sprites);

			draw_sprite(
            0,
            0,
            e.taille.x,
            e.taille.y,
            n->backgrounds[i].id_text[phase],
			decallage.x,
			decallage.x + text_coord.x,
			decallage.y,
			decallage.y + text_coord.y);
		}

		else 
		{
			// Affichage de l'image
			draw_sprite(
            0,
            0,
            e.taille.x,
            e.taille.y,
            n->backgrounds[i].id_text[0],
            decallage.x,
            decallage.x + text_coord.x,
            decallage.y,
            decallage.y + text_coord.y);
		}

        decallage.x *= 2;
        decallage.y *= 2;
    }
}

// A METTRE DANS SPRITE.C
void draw_blocs(niveau *n, ecran e, Uint32 duree)
{
    int i, j;
    sprite sprite;
    id text_id;
	occ_bloc* occ;
    coordi nb_sprites;
    int debut_x, fin_x, debut_y, fin_y;
	int phase;

    /* Calcul des coordonées des blocs à dessiner (dans l'ecran)
    et traitement des cas ou le niveau est plus petit que l'ecran */
    debut_x = e.scroll.x / n->taille_blocs.x;
    if(e.taille.x / n->taille_blocs.x <= n->taille.x)
        fin_x = debut_x + e.taille.x / n->taille_blocs.x;
    else
        fin_x = n->taille.x;

    debut_y = e.scroll.y / n->taille_blocs.y;
    if(e.taille.y / n->taille_blocs.y <= n->taille.y)
        fin_y = debut_y + e.taille.y / n->taille_blocs.y;
    else
        fin_y = n->taille.y;

    /* Sauf si on est au bout du niveau, afficher un bloc de plus que ce que l'écran permet */
    if(fin_x != n->taille.x)
        fin_x++;
    if(fin_y != n->taille.y)
        fin_y++;

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    for(i = debut_x; i < fin_x; i++)
    {	
        for(j = fin_y - 1; j >= debut_y; j--)
        {
            occ = n->occ_blocs[i][j];
			
			if(occ->bloc_actuel != NULL)
            {
				text_id = occ->bloc_actuel->texture;
                sprite.taille.x = n->textures[text_id].taille_sprite.x;
                sprite.taille.y = n->textures[text_id].taille_sprite.y;

                if(sprite.taille.x != 0 && sprite.taille.y != 0)
                {
					switch(occ->etat)
					{
					case POUSSE_PAR_LA_DROITE:
						sprite.position.x = occ->position.x + occ->compteur_etat * 2;
						sprite.position.y = occ->position.y;
						occ->compteur_etat = (occ->compteur_etat + 1) % 6;
						break;
					case POUSSE_PAR_LA_GAUCHE:
						sprite.position.x = occ->position.x - occ->compteur_etat * 2;
						sprite.position.y = occ->position.y;
						occ->compteur_etat = (occ->compteur_etat + 1) % 6;
						break;
					case POUSSE_PAR_LE_HAUT:
						sprite.position.y = occ->position.y + occ->compteur_etat * 2;
						sprite.position.x = occ->position.x;
						occ->compteur_etat = (occ->compteur_etat + 1) % 6;
						break;
					default:
						sprite.position.x = i * n->taille_blocs.x;
						sprite.position.y = j * n->taille_blocs.y;
						break;
					}

					if(occ->compteur_etat == 0)
						occ->etat = IMMOBILE;
                    
                    sprite.text_id = n->textures[text_id].id_text;

                    nb_sprites.x = n->textures[text_id].taille.x / sprite.taille.x;
                    nb_sprites.y = n->textures[text_id].taille.y / sprite.taille.y;

					if(text_id == 1)
					{
						phase = (duree % V_ANIM_BLOC_SPEC) / (V_ANIM_BLOC_SPEC / (nb_sprites.x));

						sprite.point_bg.x = (float)1 / nb_sprites.x * (occ->bloc_actuel->coord_sprite.x + phase);
						sprite.point_bg.y = (float)1 / nb_sprites.y * occ->bloc_actuel->coord_sprite.y;
						sprite.point_hd.x = (float)1 / nb_sprites.x * (occ->bloc_actuel->coord_sprite.x + phase + 1);
						sprite.point_hd.y = (float)1 / nb_sprites.y * (occ->bloc_actuel->coord_sprite.y + 1);
					}
					else 
					{
						sprite.point_bg.x = (float)1 / nb_sprites.x * occ->bloc_actuel->coord_sprite.x;
						sprite.point_bg.y = (float)1 / nb_sprites.y * occ->bloc_actuel->coord_sprite.y;
						sprite.point_hd.x = (float)1 / nb_sprites.x * (occ->bloc_actuel->coord_sprite.x + 1);
						sprite.point_hd.y = (float)1 / nb_sprites.y * (occ->bloc_actuel->coord_sprite.y + 1);
					}

                    draw_sprite_(&sprite, n->last_texture);
                    n->last_texture = text_id;
                }
            }

			if(occ->etat != IMMOBILE && occ->compteur_etat == 0)
			{
				occ->etat = IMMOBILE;
			}
        }
    }

    glEnd();
}

// A METTRE DANS SPRITE.C
void draw_objects(niveau *n, Uint32 duree)
{
    int i, j, v_anim, phase;
    id object_id;
    sprite sprite;

    /* comme un seul sprite est placé dans l'image dans le cas des objets, les coordonnées de texture sont toujours les memes (on affiche toute l'image) */
    sprite.point_bg.x = 0;
    sprite.point_bg.y = 0;
    sprite.point_hd.x = 1;
    sprite.point_hd.y = 1;

    sprite.position.x = 0;

    glBegin(GL_QUADS);

    for(i = 0; i < n->taille.x; i++)
    {
        sprite.position.y = 0;

        for(j = 0; j < n->taille.y; j++)
        {
            object_id = n->id_objets[i][j];

            if(object_id != OBJET_VIDE)
            {
				/* Si l'objet comporte plusieurs images, 
				c'est qu'il est animé */
				if(n->objets[object_id].nb_sprites > 1)
				{

					// Récupération de la vitesse d'animation
					v_anim = n->objets[object_id].v_anim;

					// Détermination de quelle image doit être affichée
					phase = (duree % v_anim) / (v_anim / n->objets[object_id].nb_sprites);

					sprite.taille.x = n->objets[object_id].taille.x;
					sprite.taille.y = n->objets[object_id].taille.y;
					sprite.text_id = n->objets[object_id].id_text[phase];

					// Affichage de l'image
					draw_sprite_(&sprite, n->last_texture);
					n->last_texture = sprite.text_id;
				}
				else
				{
					// Sinon on affiche qu'une seule image 
					sprite.taille.x = n->objets[object_id].taille.x;
					sprite.taille.y = n->objets[object_id].taille.y;
					sprite.text_id = n->objets[object_id].id_text[0];

					draw_sprite_(&sprite, n->last_texture);
					n->last_texture = sprite.text_id;
				}
			}

            sprite.position.y += n->taille_blocs.y;
        }

        sprite.position.x += n->taille_blocs.x;
    }

    glEnd();
}

void MAJ_particules(niveau* n, Uint32 duree)
{
	int i;
	for(i = 0; i < n->nb_foreground_generators; i ++)
	{
		update_particule_genenerator(n->foreground_generators[i], duree);
	}

	for(i = 0; i < n->nb_background_generators; i ++)
	{
		update_particule_genenerator(n->background_generators[i], duree);
	}

}

/***************************** ******************************/
// Je savais pas où le mettre :/

float deg_to_rad(float degres)
{
	float convert = (float) (degres / 180 * M_PI);
	return convert;
}

