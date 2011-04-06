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

	if(n == NULL)
		return n;

	for(i = 0; i < TAILLE_NOM_NIVEAU; i++)
	{
		n->nom[i] = '\0';
	}

	for(i = 0; i < TAILLE_TITRE_MUSIQUE; i++)
	{
		n->titre_musique[i] = '\0';
	}

	n->musique = NULL;

	/* Points clés */
	n->taille.x = 0;
	n->taille.y = 0;
	n->spawn.x = 0;
	n->spawn.y = 0;
	n->last_texture = 0;
	
	/* Checkpoints */
	n->nb_checkpoints = 0;
	n->checkpoints = NULL;

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
	int i, j;

	if(n == NULL) 
		return NULL;

	/* Fermeture de la musique du jeu */
	FSOUND_Stream_Close(n->musique);

	/* Libération items */
	if(n->items != NULL)
	{
		for(i = 0; i < n->nb_items; i++)
		{
			elem_item* item_suiv = NULL;
			while(n->items[i]->occ_items->item != NULL)
			{
				item_suiv = n->items[i]->occ_items->item->suivant;
				supprime_item(n->items[i]->occ_items, n->items[i]->occ_items->item->occ_item);
				n->items[i]->occ_items->item = item_suiv;
			}
			free(n->items[i]->occ_items);
			free(n->items[i]);
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

	/* Libération finishes */
	if(n->finishes != NULL)
	{
		free(n->finishes);
	}

	/* Libération checkpoints */
	if(n->checkpoints != NULL)
	{
		for(i = 0; i < n->nb_checkpoints; i++)
		{
			free(n->checkpoints[i]);
		}
		free(n->checkpoints);
	}

	/* Libération blocs */
	if(n->blocs != NULL)
	{
		for(i = 0; i < n->taille.x; i++)
		{
			for(j = 0; j < n->taille.y; j++)
			{
				if(n->occ_blocs[i][j] != NULL)
				{
					free(n->occ_blocs[i][j]);
				}
			}

			free(n->occ_blocs[i]);
		}
		free(n->occ_blocs);
		free(n->blocs);
	}

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
	{
		for(i = 0; i < n->nb_backgrounds;i++)
			free(n->backgrounds[i].id_text);

		free(n->backgrounds);
	}

	/* Libération avants-plans */
	if(n->foregrounds != NULL)
	{
		for(i = 0; i < n->nb_foregrounds;i++)
			free(n->foregrounds[i].id_text);

		free(n->foregrounds);
	}
		

	/* Libération textures */
	if(n->textures != NULL)
	{
		for(i = 0; i < n->nb_textures; i++)
			free(n->textures[i].phys);

		free(n->textures);
	}

	/* Libération du reste du niveau */
	free(n);

	return NULL;
}


void balise_level(niveau *n, const char **attrs)
{
	int i, j;

	/* Nom et taille du niveau */
    strcpy(n->nom, attrs[1]);
    n->taille.x = atoi(attrs[3]);
    n->taille.y = atoi(strchr(attrs[3], ':') + 1);

	/* Musique */
	strcpy(n->titre_musique, attrs[5]);
	n->musique = charger_musique(attrs[5], 255, 1);

	/* Allocation des blocs du niveau */
	n->occ_blocs = malloc(n->taille.x * sizeof(occ_bloc***));

	for(i = 0; i < n->taille.x; i++)
	{
		n->occ_blocs[i] = malloc(n->taille.y * sizeof(occ_bloc**));
	}

	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			n->occ_blocs[i][j] = NULL;
		}
	}
}


void balise_spawn(niveau *n, const char **attrs)
{
    n->spawn.x = atoi(attrs[1]);
    n->spawn.y = atoi(strchr(attrs[1], ':') + 1);
}

void balise_checkpoints(niveau *n, const char **attrs)
{
	n->nb_checkpoints = atoi(attrs[1]);
    n->checkpoints = malloc(sizeof(checkpoint*) * n->nb_checkpoints);
}

void balise_checkpoint(niveau *n, const char **attrs)
{
    static int i = 0;
	n->checkpoints[i] = charger_checkpoint(attrs[1]);
	n->checkpoints[i]->position.x = atoi(attrs[3]);
    n->checkpoints[i]->position.y = atoi(strchr(attrs[3], ':') + 1);
	i++;
	i %= n->nb_checkpoints;
}


void balise_backgrounds(niveau *n, const char **attrs)
{
    n->nb_backgrounds = atoi(attrs[1]);
    n->backgrounds = malloc(sizeof(background) * n->nb_backgrounds);
}


void balise_background(niveau *n, const char **attrs)
{
    static int i = 0;
	strcpy(n->backgrounds[i].nom_text, attrs[1]);
	charger_objet_background(&n->backgrounds[i], FAUX);
	i++;
	i %= n->nb_backgrounds;
}

void balise_background_generators(niveau *n, const char **attrs)
{
	n->nb_background_generators = atoi(attrs[1]);
	n->background_generators = malloc(sizeof(particule_generator) * n->nb_background_generators);
}

void balise_background_generator(niveau *n, const char **attrs)
{
	static int i = 0;
	coordf position = { (float)atoi(attrs[2]), (float) atoi(strchr(attrs[2], ':') + 1) };
	coordi taille = { atoi(attrs[3]),   atoi(strchr(attrs[3], ':') + 1) };
	n->background_generators[i] = new_particule_generator(position, taille, atoi(attrs[4]), atoi(attrs[5]), attrs[6], atoi(attrs[7]), atoi(attrs[8]), atoi(attrs[9]));
	i++;
	i %= n->nb_background_generators;
}

void balise_foregrounds(niveau *n, const char **attrs)
{
	n->nb_foregrounds = atoi(attrs[1]);
	n->foregrounds = malloc(sizeof(foreground) * n->nb_foregrounds);
}

void balise_foreground(niveau *n, const char **attrs)
{
	static int i = 0;
	strcpy(n->foregrounds[i].nom_text, attrs[1]);
	charger_objet_background(&n->foregrounds[i], FAUX);
	i++;
	i %= n->nb_foregrounds;
}

void balise_foreground_generators(niveau *n, const char **attrs)
{
	n->nb_foreground_generators = atoi(attrs[1]);
	n->foreground_generators = malloc(sizeof(particule_generator) * n->nb_foreground_generators);
}

void balise_foreground_generator(niveau *n, const char **attrs)
{
	static int i = 0;
	coordf position = { (float)atoi(attrs[3]),  (float) atoi(strchr(attrs[3], ':') + 1) };
	coordi taille = { atoi(attrs[5]),   atoi(strchr(attrs[5], ':') + 1) };
	n->foreground_generators[i] = new_particule_generator(position, taille, atoi(attrs[7]), atoi(attrs[9]), attrs[1], atoi(attrs[11]), atoi(attrs[13]), atoi(attrs[15]));
	i++;
	i %= n->nb_foreground_generators;
}

void balise_objects(niveau *n, const char **attrs)
{
	int i, j;
	n->nb_objets = atoi(attrs[1]);
	if(n->nb_objets > 0)
	{
		n->objets = malloc(sizeof(objet) * n->nb_objets);
	}

	n->id_objets = malloc(sizeof(id*) * n->taille.x);
	for(i = 0; i < n->taille.x; i++)
	{
		n->id_objets[i] = malloc(sizeof(id) * n->taille.y);
	}

	for(i = 0; i < n->taille.x; i++)
	{
		for(j = 0; j < n->taille.y; j++)
		{
			n->id_objets[i][j] = OBJET_VIDE;
		}
	}
}

void balise_object(niveau *n, const char **attrs)
{
	static int i = 0;
	strcpy(n->objets[i].nom_text, attrs[1]);
	charger_objet_background(&n->objets[i], VRAI);
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_objets;
}

void balise_finishes(niveau *n, const char **attrs)
{
	n->nb_finish = atoi(attrs[1]);
	n->finishes = malloc(sizeof(finish) * n->nb_finish);
}

void balise_finish(niveau *n, const char **attrs)
{
	static int i = 0;
	strcpy(n->finishes[i].nom_text, attrs[1]);
	n->finishes[i].position.x = (float)atoi(attrs[3]);
	n->finishes[i].position.y = (float)atoi(strchr(attrs[3], ':') + 1);
	charger_finish(&n->finishes[i]);
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_finish;
}

void balise_projectiles(niveau *n, const char **attrs)
{
	n->nb_projectiles = atoi(attrs[1]);
	n->projectiles = malloc(sizeof(projectile) * n->nb_projectiles);
}

void balise_projectile(niveau *n, const char **attrs)
{
	static int i = 0;
	n->projectiles[i] = charger_projectile(attrs[1]);
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_projectiles;
}

void balise_items(niveau *n, const char **attrs)
{
	n->nb_items = atoi(attrs[1]);
	n->items = malloc(sizeof(item) * n->nb_items);
}

int id_item_actuel;

void balise_item(niveau *n, const char **attrs)
{
	static int i = 0;
	n->items[i] = charger_item(attrs[1], atoi(attrs[3]));
	id_item_actuel = i;
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_items;
}

void balise_occ_item(niveau *n, const char **attrs)
{
	n->items[id_item_actuel]->occ_items = ajout_item(n->items[id_item_actuel]->occ_items, new_occ_item((float)atoi(attrs[1]), (float)atoi(strchr(attrs[1], ':') + 1), id_item_actuel, n->items[id_item_actuel]->vitesse, NORMAL));
}


void balise_monsters(niveau *n, const char **attrs)
{
    n->nb_monstres = atoi(attrs[1]);
	n->monstres = malloc(sizeof(monstre*) * n->nb_monstres);
}

int id_mstr_actuel;

void balise_monster(niveau *n, const char **attrs)
{
	static int i = 0;
    n->monstres[i] = charger_monstre(attrs[1]);
	id_mstr_actuel = i;
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_monstres;
}

void balise_occ(niveau *n, const char **attrs)
{
	n->monstres[id_mstr_actuel]->occ_monstres = ajout_monstre(n->monstres[id_mstr_actuel]->occ_monstres, new_occ_monstre((float)atoi(attrs[1]), (float)atoi(strchr(attrs[1], ':') + 1), n->monstres[id_mstr_actuel]));
}

void balise_pipes(niveau *n, const char **attrs)
{
	n->nb_tuyaux = atoi(attrs[1]);
	n->tuyaux = malloc(sizeof(tuyau) * n->nb_tuyaux);
}

void balise_pipe(niveau *n, const char **attrs)
{
	static int i = 0;
	n->tuyaux[i] = charger_tuyau(attrs[1], atoi(attrs[3]), atoi(attrs[5]), atoi(attrs[7]), atoi(strchr(attrs[7], ':') + 1), atoi(attrs[9]), atoi(attrs[11]), attrs[13], atoi(attrs[15]));
	i++;

	/* Remise a zéro pour le prochain niveau */
	i %= n->nb_tuyaux;
}

void balise_blocs(niveau *n, const char **attrs)
{
    n->nb_textures = atoi(attrs[1]);
	n->textures = malloc(n->nb_textures * sizeof(texture));
	n->nb_blocs = atoi(attrs[3]);
	n->blocs = malloc(n->nb_blocs * sizeof(bloc));
    n->taille_blocs.x = atoi(attrs[5]);
	n->taille_blocs.y = atoi(strchr(attrs[5], ':') + 1);
}


void balise_textures(niveau *n, const char **attrs)
{
    static int i = 0;
	charger_texture_bloc(attrs[1], &n->textures[i], n->taille_blocs);
	i++;
	i %= n->nb_textures;
}

void balise_bloc(niveau *n, const char **attrs)
{
    static int i = 0;
	texture texture_bloc;
	n->blocs[i].texture = atoi(attrs[1]);
	
	texture_bloc = n->textures[n->blocs[i].texture];
	n->blocs[i].coord_sprite.x = atoi(attrs[3]);
	n->blocs[i].coord_sprite.y = atoi(strchr(attrs[3], ':') + 1);
	n->blocs[i].type_bloc = atoi(attrs[5]);
	n->blocs[i].phys = texture_bloc.phys[n->blocs[i].coord_sprite.x + n->blocs[i].coord_sprite.y * (texture_bloc.taille.x / texture_bloc.taille_sprite.x)];
	n->blocs[i].tps_piece = 0;
	i++;
	i %= n->nb_blocs;
}


void balise_layers(niveau *n, const char **attrs)
{
    attrs[1];   // nombre
}


void balise_layer(niveau *n, const char **attrs)
{
    attrs[1];   // index
}

void balise_occ_block(niveau *n, const char **attrs)
{
	//static int i = 0, j = 0;
	n->occ_blocs[atoi(attrs[1]) / n->taille_blocs.x][atoi(strchr(attrs[1], ':') + 1) / n->taille_blocs.y] = new_occ_bloc(atoi(attrs[1]), atoi(strchr(attrs[1], ':') + 1), atoi(attrs[3]), atoi(attrs[5]), atoi(attrs[7]));

	/*if(atoi(attrs[1]) == -1 && atoi(attrs[3]) == -1 && atoi(attrs[5]) == -1)
		n->occ_blocs[i][j] = NULL;
	else
		n->occ_blocs[i][j] = new_occ_bloc(i * n->taille_blocs.x, j * n->taille_blocs.y, atoi(attrs[1]), atoi(attrs[3]), atoi(attrs[5]));

	if(j == n->taille.y - 1)
	{
		j %= n->taille.y - 1;
		i++;
		i %= n->taille.x;
	}
	else
	{
		j++;
	}*/

}

void debut_element(void *user_data, const xmlChar *name, const xmlChar **attrs) 
{
    static const xmlChar *elements[] = {
        BAD_CAST"level", 
        BAD_CAST"spawn", 
        BAD_CAST"checkpoints", 
		BAD_CAST"checkpoint", 
        BAD_CAST"backgrounds", 
        BAD_CAST"background",
		BAD_CAST"background_generators",
		BAD_CAST"background_generator",
		BAD_CAST"foregrounds", 
        BAD_CAST"foreground",
		BAD_CAST"foreground_generators",
		BAD_CAST"foreground_generator",
		BAD_CAST"objects",
		BAD_CAST"object",
		BAD_CAST"finishes",
		BAD_CAST"finish",
		BAD_CAST"projectiles",
		BAD_CAST"projectile",
		BAD_CAST"items",
		BAD_CAST"item",
		BAD_CAST"occ_item",
        BAD_CAST"monsters", 
        BAD_CAST"monster", 
        BAD_CAST"occ",
		BAD_CAST"pipes",
		BAD_CAST"pipe",
        BAD_CAST"blocs", 
        BAD_CAST"textures", 
		BAD_CAST"bloc", 
        BAD_CAST"layers", 
        BAD_CAST"layer",
		BAD_CAST"occ_block"
    };
    static const balise_func functions[] = {
        balise_level,
        balise_spawn,
        balise_checkpoints,
		balise_checkpoint,
        balise_backgrounds,
        balise_background,
		balise_background_generators,
		balise_background_generator,
		balise_foregrounds,
		balise_foreground,
		balise_foreground_generators,
		balise_foreground_generator,
		balise_objects,
		balise_object,
		balise_finishes,
		balise_finish,
		balise_projectiles,
		balise_projectile,
		balise_items,
		balise_item,
		balise_occ_item,
        balise_monsters,
        balise_monster,
        balise_occ,
		balise_pipes,
		balise_pipe,
        balise_blocs,
		balise_textures,
		balise_bloc,
        balise_layers,
        balise_layer,
		balise_occ_block
    };
    int i;

    for(i = 0; i < 32; i++)
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

void erreur(void * ctx, const char * msg, ...)
{
	printf("error parsing XML : %s\n", msg);
}

void charger_niveau(char *nom, niveau *n)
{
    xmlSAXHandler sh = {NULL};
    sh.startElement = debut_element;
    sh.characters = caractere;
	sh.error = erreur;
	
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
    add_attrib(fic, "size", "%d:%d", n->taille.x, n->taille.y);
	add_attrib(fic, "music", "%s", n->titre_musique);
    end_element(fic);

    /* Spawn */
    open_element(fic, "spawn");
    add_attrib(fic, "pos", "%d:%d", n->spawn.x, n->spawn.y);
    close_element_short(fic);

    /* Checkpoint */
    open_element(fic, "checkpoints");
	add_attrib(fic, "nb", "%d", n->nb_checkpoints);
	if(n->nb_checkpoints > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_checkpoints; i++)
		{
			open_element(fic, "checkpoint");
			add_attrib(fic, "img", "%s", n->checkpoints[i]->nom_text);
			add_attrib(fic, "pos", "%d:%d", n->checkpoints[i]->position.x, n->checkpoints[i]->position.y);
			close_element_short(fic);
		}
		close_element(fic, "checkpoints");
	}
	else
	{
		close_element_short(fic);
	}

    /* Backgrounds */
    open_element(fic, "backgrounds");
    add_attrib(fic, "nb", "%d", n->nb_backgrounds);
	if(n->nb_backgrounds > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_backgrounds; i++)
		{
			open_element(fic, "background");
			add_attrib(fic, "img", "%s", n->backgrounds[i].nom_text);
			close_element_short(fic);
		}
		close_element(fic, "backgrounds");
	}
	else
	{
		close_element_short(fic);
	}

	/* Backgrounds Generators */
    open_element(fic, "background_generators");
	add_attrib(fic, "nb", "%d", n->nb_background_generators);
	
	if(n->nb_background_generators > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_background_generators; i++)
		{
			particule_generator* gen = n->background_generators[i];

			open_element(fic, "background_generator");
			add_attrib(fic, "img", "%s", gen->nom_text);
			add_attrib(fic, "position", "%d:%d", gen->position.x, gen->position.y);
			add_attrib(fic, "size", "%d:%d", gen->taille.x, gen->taille.y);
			add_attrib(fic, "particles_life", "%d", gen->vie_particules);
			add_attrib(fic, "flow", "%d", gen->debit);
			add_attrib(fic, "gravity", "%d", gen->gravity);
			add_attrib(fic, "beginning_color", "%d", gen->couleur_debut);
			add_attrib(fic, "end_color", "%d", gen->couleur_fin);
			close_element_short(fic);
		}
		close_element(fic, "background_generators");
	}
	else
	{
		close_element_short(fic);
	}

	/* Foregrounds */
    open_element(fic, "foregrounds");
	add_attrib(fic, "nb", "%d", n->nb_foregrounds);

	if(n->nb_foregrounds > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_foregrounds; i++)
		{
			open_element(fic, "foreground");
			add_attrib(fic, "img", "%s", n->foregrounds[i].nom_text);
			close_element_short(fic);
		}
		close_element(fic, "foregrounds");
	}
	else
	{
		close_element_short(fic);
	}

	/* Foreground Generators */
    open_element(fic, "foreground_generators");
	add_attrib(fic, "nb", "%d", n->nb_foreground_generators);
    
	if(n->foreground_generators > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_foreground_generators; i++)
		{
			particule_generator* gen = n->foreground_generators[i];

			open_element(fic, "foreground_generator");
			add_attrib(fic, "img", "%s", gen->nom_text);
			add_attrib(fic, "position", "%0.f:%0.f", gen->position.x, gen->position.y);
			add_attrib(fic, "size", "%d:%d", gen->taille.x, gen->taille.y);
			add_attrib(fic, "particles_life", "%d", gen->vie_particules);
			add_attrib(fic, "flow", "%d", gen->debit);
			add_attrib(fic, "gravity", "%d", gen->gravity);
			add_attrib(fic, "beginning_color", "%d", gen->couleur_debut);
			add_attrib(fic, "end_color", "%d", gen->couleur_fin);
			close_element_short(fic);
		}
		close_element(fic, "foreground_generators");
	}
	else
	{
		close_element_short(fic);
	}

	/* Objets */
    open_element(fic, "objects");
	add_attrib(fic, "nb", "%d", n->nb_foregrounds);
	if(n->nb_foregrounds > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_foregrounds; i++)
		{
			open_element(fic, "object");
			add_attrib(fic, "img", "%s", n->objets[i].nom_text);
			close_element_short(fic);
		}
		close_element(fic, "objects");
	}
	else
	{
		close_element_short(fic);
	}

	/* Finish */
    open_element(fic, "finishes");
    add_attrib(fic, "nb", "%d", n->nb_finish);
    
	if(n->nb_finish > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_finish; i++)
		{
			open_element(fic, "finish");
			add_attrib(fic, "img", "%s", n->finishes[i].nom_text);
			add_attrib(fic, "position", "%0.f:%0.f", n->finishes[i].position.x, n->finishes[i].position.y);
			close_element_short(fic);
		}
		close_element(fic, "finishes");
	}
	else
	{
		close_element_short(fic);
	}

	/* Projectiles */
    open_element(fic, "projectiles");
    add_attrib(fic, "nb", "%d", n->nb_projectiles);
    
	if(n->nb_projectiles > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_projectiles; i++)
		{
			open_element(fic, "projectile");
			add_attrib(fic, "img", "%s", n->projectiles[i]->nom_text);
			close_element_short(fic);
		}
		close_element(fic, "projectiles");
	}
	else
	{
		close_element_short(fic);
	}

	/* Items */
    open_element(fic, "items");
    add_attrib(fic, "nb", "%d", n->nb_items);

	if(n->nb_items > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_items; i++)
		{
			item *it = n->items[i];
			elem_item *occ_i = it->occ_items->item;

			open_element(fic, "item");
			add_attrib(fic, "img", "%s", it->nom_text);
			add_attrib(fic, "type", "%d", it->nom);

			if(it->occ_items->nb_elements > 0)
			{
				end_element(fic);
				while(occ_i != NULL)
				{
					open_element(fic, "occ_item");
					add_attrib(fic, "pos", "%d:%d", (int)occ_i->occ_item->position.x, (int)occ_i->occ_item->position.y);
					close_element_short(fic);
					occ_i = occ_i->suivant;
				}
				close_element(fic, "item");
			}
			else
			{
				close_element_short(fic);
			}
		}
		close_element(fic, "items");
	}
	else
	{
		close_element_short(fic);
	}

    /* Monstres */
    open_element(fic, "monsters");
    add_attrib(fic, "nb", "%d", n->nb_monstres);

	if(n->nb_monstres > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_monstres; i++)
		{
			monstre *m = n->monstres[i];
			elem_monstre *occ_m = m->occ_monstres->monstre;

			open_element(fic, "monster");
			add_attrib(fic, "name", "%s", m->nom);
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
	}
	else
	{
		close_element_short(fic);
	}

	/* Tuyaux */
    open_element(fic, "pipes");
    add_attrib(fic, "nb", "%d", n->nb_tuyaux);

	if(n->nb_tuyaux > 0)
	{
		end_element(fic);
		for(i = 0; i < n->nb_tuyaux; i++)
		{
			tuyau* t = n->tuyaux[i];
			open_element(fic, "pipe");
			add_attrib(fic, "img", "%s", t->nom_text);
			add_attrib(fic, "sens", "%d", t->sens_sortie);
			add_attrib(fic, "length", "%d", t->longueur);
			add_attrib(fic, "pos", "%d:%d", t->position.x, t->position.y);
			add_attrib(fic, "state", "%d", t->etat);
			add_attrib(fic, "destination_pipe", "%d", t->pipe_dest);
			
			if(t->level_dest != NULL)
				add_attrib(fic, "level_destination", "%s", t->level_dest);
			else
				add_attrib(fic, "level_destination", "");

			add_attrib(fic, "monster", "%d", t->index_monstre);

			close_element_short(fic);
		}
		close_element(fic, "pipes");
	}
	else
	{
		close_element_short(fic);
	}

    /* Blocs */
    open_element(fic, "blocs");
    add_attrib(fic, "nb_textures", "%d", n->nb_textures);
	add_attrib(fic, "nb_models", "%d", n->nb_blocs);
	add_attrib(fic, "bloc_size", "%d:%d", n->taille_blocs.x, n->taille_blocs.y);
    end_element(fic);

    /* texture utilisées pour les blocs */
    for(i = 0; i < n->nb_textures; i++)
    {
        open_element(fic, "textures");
        add_attrib(fic, "img", "%s", n->textures[i].nom);
        close_element_short(fic);
    }
    
	for(i = 0; i < n->nb_blocs; i++)
    {
		open_element(fic, "bloc");
		add_attrib(fic, "texture", "%d", n->blocs[i].texture);
		add_attrib(fic, "coord_sprite", "%d:%d", n->blocs[i].coord_sprite.x, n->blocs[i].coord_sprite.y);
		add_attrib(fic, "type_bloc", "%d", n->blocs[i].type_bloc);
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
        for(j = 0; j < n->taille.x; j++)
		{
            for(k = 0; k < n->taille.y; k++)
			{
				if(n->occ_blocs[j][k] != NULL)
				{
					open_element(fic, "occ_block");
					add_attrib(fic, "pos", "%d:%d", n->occ_blocs[j][k]->position.x, n->occ_blocs[j][k]->position.y);
					add_attrib(fic, "actual", "%d", n->occ_blocs[j][k]->bloc_actuel);
					add_attrib(fic, "alt", "%d", n->occ_blocs[j][k]->bloc_alternatif);
					add_attrib(fic, "item", "%d", n->occ_blocs[j][k]->item);
					close_element_short(fic);
				}
			}
		}
        close_element(fic, "layer");
    }
    close_element(fic, "layers");
    close_element(fic, "blocs");

    close_element(fic, "level");

    fclose(fic);

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
	}

	/* Liberation des textures de devant */
	for(i = 0; i < n->nb_foregrounds; i++)
	{
		for(j = 0; j < n->foregrounds[i].nb_sprites; j++)
			glDeleteTextures(1, &n->foregrounds[i].id_text[j]);
	}

	/* Libération des textures d'arrivée */
	for(i = 0; i < n->nb_finish; i++)
		glDeleteTextures(1, &n->finishes[i].id_text);

	/* Libération des checkpoints */
	for(i = 0; i < n->nb_checkpoints; i++)
		glDeleteTextures(1, &n->checkpoints[i]->id_text);

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
	for(i = 0; i < n->nb_objets; i++)
    {
		for(j = 0; j < n->objets[i].nb_sprites; j++)
			glDeleteTextures(1, &n->objets[i].id_text[j]);
	}

	/* Liberation des textures des blocs */
	for(i = 0; i < n->nb_textures; i++)
		glDeleteTextures(1, &n->textures[i].id_text);
}

void charger_finish(finish* f)
{
	char nom_texture[TAILLE_NOM_TEXTURE];

	strcpy(nom_texture, "textures/objects/");
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
		strcpy(nom_texture, "textures/objects/");
	else
		strcpy(nom_texture, "textures/backgrounds/");

	strcat(nom_texture, b->nom_text);
	strcat(nom_texture, ".obj");

	back_file = fopen(nom_texture, "r");

    if(back_file == NULL)
    {
        printf("Le fichier %s est introuvable.\n", nom_texture);
        return;
    }

	fscanf(back_file, "nb_sprites : %d\n", &nb_1);
	b->nb_sprites = nb_1;

	if(b->nb_sprites > 1)
	{
		fscanf(back_file, "v_anim : %d\n", &nb_1);
		b->v_anim = nb_1;
	}
	else
		b->v_anim = 0;

	fscanf(back_file, "repetition_verticale : %d\n", &nb_1);
	b->repetition_verticale = nb_1;

	fclose(back_file);

	/* Chargement de la texture */
	b->id_text = malloc(sizeof(GLuint) * b->nb_sprites);

	if(b->nb_sprites > 1)
	{
		for(i = 0; i < b->nb_sprites; i ++)
		{
			if(is_object)
				strcpy(nom_texture, "textures/objects/");
			else
				strcpy(nom_texture, "textures/backgrounds/");

			strcat(nom_texture, b->nom_text);
			strcat(nom_texture, itoa(i, test, 10));
			strcat(nom_texture, ".png");

			b->id_text[i] = charger_texture_bis(nom_texture, &b->taille);
		}
	}
	else
	{
		if(is_object)
			strcpy(nom_texture, "textures/objects/");
		else
			strcpy(nom_texture, "textures/backgrounds/");

		strcat(nom_texture, b->nom_text);
		strcat(nom_texture, ".png");

		b->id_text[0] = charger_texture_bis(nom_texture, &b->taille);
	}
}

void affiche_occ_blocs(niveau* n)
{
	int i, j;
	FILE* flux = fopen("test.txt","w");

	for(i = 0; i < n->taille.x ; i++){ 
		for(j = 0; j < n->taille.y; j++){

			if(n->occ_blocs[i][j]->bloc_actuel >= 0)
			{
				fprintf(flux, "i = %d, j = %d, bloc_actuel = %d\n", i, j, n->occ_blocs[i][j]->bloc_actuel);
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
    glEnd();
    glPushMatrix();
	glTranslated(e.origine.x, e.origine.y, 0);
    glBegin(GL_QUADS);

    /* Dessin du ou des arrieres-plans */
    draw_background(lvl, e, duree);

	for(i = 0; i < lvl->nb_background_generators; i++)
	{
		draw_particules(lvl->background_generators[i]);
	}

    /* decallage de la "camera" pour gerer le scrolling */
    glEnd();
    glTranslated(-e.scroll.x, -e.scroll.y, 0);
    glBegin(GL_QUADS);
    
    /* Dessin des objets en arriere plan */
	//draw_objects(lvl, duree);

	/* Dessin des arrivées */
	for(i = 0; i < lvl->nb_finish; i++)
	{
		finish f = lvl->finishes[i];
        sprite s;
        
        s.point_bg.x = s.point_bg.y = 0;
        s.point_hd.x = s.point_hd.y = 1;
        s.position.x = (int)f.position.x;
        s.position.y = (int)f.position.y;
        s.taille = f.taille;
        s.text_id = f.id_text;
		draw_sprite(&s);
	}

	/* Dessin des items */
	for(i = 0; i < lvl->nb_items; i++)
	{
		elem_item *tmp_item = lvl->items[i]->occ_items->item;
		while(tmp_item != NULL)
		{
			if(tmp_item->occ_item->actif)
				draw_item(lvl->items[i], tmp_item->occ_item, duree);

			tmp_item = tmp_item->suivant;
		}
	}

	/* Dessin du plan du personnage */
	draw_blocs(lvl, e, duree);

	/* Dessin des checkpoints */
	for(i = 0; i < lvl->nb_checkpoints; i++)
	{
		draw_checkpoint(lvl->checkpoints[i], duree);
	}
	
	/* Dessin des monstres */
	for(i = 0; i < lvl->nb_monstres; i++)
	{
		elem_monstre *tmp_monstre = lvl->monstres[i]->occ_monstres->monstre;
		while(tmp_monstre != NULL)
		{
			if(tmp_monstre->occ_monstre->actif)
				draw_monstre(tmp_monstre->occ_monstre, duree);
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
    glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glColor4f(0, 0, 1, 0.5);

	glVertex3f((GLfloat)persos[0]->position.x + persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y, -1);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y + persos[0]->texture_act->ordonnee_haut);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->taille.x - persos[0]->texture_act->abscisse_bas,(GLfloat) persos[0]->position.y + persos[0]->texture_act->ordonnee_haut);
	glVertex2f((GLfloat)persos[0]->position.x + persos[0]->taille.x - persos[0]->texture_act->abscisse_bas, (GLfloat)persos[0]->position.y);

	glColor4f(1, 1, 1, 1);
	glEnd();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

#endif

	/* Dessin des particules en avant-plan s'il y en a */
	for(i = 0; i < lvl->nb_foreground_generators; i++)
	{
		draw_particules(lvl->foreground_generators[i]);
	}

	/* Dessin du ou des avants-plans */
    draw_foreground(lvl, e, duree);

    glEnd();
    glPopMatrix();
    glBegin(GL_QUADS);
}


void draw_main_options(niveau *lvl, ecran e, Uint32 duree, int bck, int blocs, int objets)
{
    int i;

    /* decallage de la "camera" pour commencer au point d'origine choisit */
    glEnd();
    glScissor(e.origine.x, e.origine.y, e.taille.x, e.taille.y);
    glEnable(GL_SCISSOR_TEST);
    glPushMatrix();
    glTranslated(e.origine.x, e.origine.y, 0);
    glBegin(GL_QUADS);
    
    if(bck)
        draw_background(lvl, e, duree);

    glEnd();
    glTranslated(-e.scroll.x, -e.scroll.y, 0);
    glBegin(GL_QUADS);
    
    if(objets)
        draw_objects(lvl, duree);

	/* Dessin des arrivées */
	for(i = 0; i < lvl->nb_finish; i++)
	{
		finish f = lvl->finishes[i];
        sprite s;
        
        s.point_bg.x = s.point_bg.y = 0;
        s.point_hd.x = s.point_hd.y = 1;
        s.position.x = (int)f.position.x;
        s.position.y = (int)f.position.y;
        s.taille = f.taille;
        s.text_id = f.id_text;
		draw_sprite(&s);
	}

    /* Dessin des items */
	for(i = 0; i < lvl->nb_items; i++)
	{
		elem_item *tmp_item = lvl->items[i]->occ_items->item;
		while(tmp_item != NULL)
		{
			draw_item(lvl->items[i], tmp_item->occ_item, duree);
			tmp_item = tmp_item->suivant;
		}
	}

    if(blocs)
        draw_blocs(lvl, e, duree);

	/* Dessin des checkpoints */
	for(i = 0; i < lvl->nb_checkpoints; i++)
	{
		draw_checkpoint(lvl->checkpoints[i], duree);
	}

    /* Dessin des monstres */
	for(i = 0; i < lvl->nb_monstres; i++)
	{
		elem_monstre *tmp = lvl->monstres[i]->occ_monstres->monstre;
		while(tmp != NULL)
		{
			draw_monstre(tmp->occ_monstre, duree);
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

    glEnd();
    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
    glBegin(GL_QUADS);
}

// A METTRE DANS SPRITE.C
void draw_foreground(niveau *n, ecran e, Uint32 duree)
{
    int i;
    sprite s;
    coordf decallage;
    coordf text_coord;
	int phase, v_anim;

    decallage.x = (float)e.scroll.x / 5000 * n->nb_backgrounds * 2;
    decallage.y = (float)e.scroll.y / 20000;
    
    s.position.x = e.origine.x + e.scroll.x;
    s.position.y = 0;
    s.taille.x = e.taille.x;
    s.point_bg.x = decallage.x;
    s.point_bg.y = decallage.y;
    s.point_hd.y = 1;

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
        }
        else
            phase = 0;

        /* Dessin */
        s.taille.y = n->foregrounds[i].taille.y;
        s.text_id = n->foregrounds[i].id_text[phase];
        s.point_hd.x = decallage.x + text_coord.x;
        draw_sprite(&s);
    }
}

// A METTRE DANS SPRITE.C
void draw_background(niveau *n, ecran e, Uint32 duree)
{
    int i;
    sprite s;
    coordf decallage;
    coordf text_coord;
	int phase, v_anim;

    decallage.x = (float)e.scroll.x / 5000;
    decallage.y = (float)e.scroll.y / 20000;

	s.position.x = 0;
	s.position.y = 0;
    s.taille.x = e.taille.x;

    for(i = 0; i < n->nb_backgrounds; i++)
    {
        s.point_bg.x = decallage.x;
        s.point_bg.y = decallage.y;		

        text_coord.x  = (float)e.taille.x / n->backgrounds[i].taille.x;
        text_coord.y  = (float)e.taille.y / n->backgrounds[i].taille.y;

		if(n->backgrounds[i].nb_sprites > 1)
		{
			// stockage de la vitesse d'animation du background actuel
			v_anim = n->backgrounds[i].v_anim;

			// Détermination de quelle image doit être affichée
			phase = (duree % v_anim) / (v_anim / n->backgrounds[i].nb_sprites);
        }
        else
            phase = 0;

        /* Dessin */
        s.text_id = n->backgrounds[i].id_text[phase];
        s.point_hd.x = decallage.x + text_coord.x;

		if(n->backgrounds[i].repetition_verticale)
		{
			s.point_hd.y = text_coord.y + decallage.y;
			s.taille.y = e.taille.y;
		}
		else
		{
			s.point_hd.y = 1;
			s.taille.y = n->backgrounds[i].taille.y;
		}
        draw_sprite(&s);

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

    for(i = debut_x; i < fin_x; i++)
    {	
        for(j = debut_y; j < fin_y; j++)
        {
            occ = n->occ_blocs[i][j];
			
			if(occ == NULL)
				continue;

			if(occ->bloc_actuel >= 0)
			{
				text_id = n->blocs[occ->bloc_actuel].texture;
				sprite.taille.x = n->textures[text_id].taille_sprite.x;
				sprite.taille.y = n->textures[text_id].taille_sprite.y;

				if(sprite.taille.x != 0 && sprite.taille.y != 0)
				{
					sprite.text_id = n->textures[text_id].id_text;

					nb_sprites.x = n->textures[text_id].taille.x / sprite.taille.x;
					nb_sprites.y = n->textures[text_id].taille.y / sprite.taille.y;

					if(nb_sprites.x > 1 && nb_sprites.y == 1)
					{
						phase = (duree % V_ANIM_BLOC_SPEC) / (V_ANIM_BLOC_SPEC / nb_sprites.x);

						sprite.point_bg.x = (float)1 / nb_sprites.x * (n->blocs[occ->bloc_actuel].coord_sprite.x + phase);
						sprite.point_bg.y = (float)1 / nb_sprites.y * n->blocs[occ->bloc_actuel].coord_sprite.y;
						sprite.point_hd.x = (float)1 / nb_sprites.x * (n->blocs[occ->bloc_actuel].coord_sprite.x + phase + 1);
						sprite.point_hd.y = (float)1 / nb_sprites.y * (n->blocs[occ->bloc_actuel].coord_sprite.y + 1);
					}
					else 
					{
						sprite.point_bg.x = (float)1 / nb_sprites.x * n->blocs[occ->bloc_actuel].coord_sprite.x;
						sprite.point_bg.y = (float)1 / nb_sprites.y * n->blocs[occ->bloc_actuel].coord_sprite.y;
						sprite.point_hd.x = (float)1 / nb_sprites.x * (n->blocs[occ->bloc_actuel].coord_sprite.x + 1);
						sprite.point_hd.y = (float)1 / nb_sprites.y * (n->blocs[occ->bloc_actuel].coord_sprite.y + 1);
					}

					switch(occ->etat)
					{
					case POUSSE_PAR_LA_DROITE:
						sprite.position.x = occ->position.x + occ->compteur_etat * 2;
						sprite.position.y = occ->position.y;
						draw_sprite_d(&sprite, LAYER_BLOC_2);
						occ->compteur_etat = (occ->compteur_etat + 1) % 6;
						break;
					case POUSSE_PAR_LA_GAUCHE:
						sprite.position.x = occ->position.x - occ->compteur_etat * 2;
						sprite.position.y = occ->position.y;
						draw_sprite_d(&sprite, LAYER_BLOC_2);
						occ->compteur_etat = (occ->compteur_etat + 1) % 6;
						break;
					case POUSSE_PAR_LE_HAUT:
						sprite.position.y =  occ->position.y = occ->position.y + occ->compteur_etat;
						sprite.position.x = occ->position.x;
						draw_sprite_d(&sprite, LAYER_BLOC_2);
						occ->compteur_etat = (occ->compteur_etat + 1) % 5;
						break;
					default:
						sprite.position.x =  occ->position.x = i * n->taille_blocs.x;
						sprite.position.y =  occ->position.y = j * n->taille_blocs.y;
						draw_sprite_d(&sprite, LAYER_BLOC_1);
						break;
					}

					if(occ->compteur_etat == 0)
						occ->etat = IMMOBILE;

					n->last_texture = text_id;
				}

				/* Réduction du temps */
				if((n->blocs[occ->bloc_actuel].type_bloc & DISTRIBUTEUR_PIECE) && n->blocs[occ->bloc_actuel].tps_piece > 0)
				{
					n->blocs[occ->bloc_actuel].tps_piece -= duree / 150;
				}
			}

			if(occ->etat != IMMOBILE && occ->compteur_etat == 0)
			{
				occ->etat = IMMOBILE;
			}			
        }
    }
}

// A METTRE DANS SPRITE.C
void draw_objects(niveau *n, Uint32 duree)
{
    int i, j, v_anim, phase;
    id object_id;
    sprite s;

    /* comme un seul sprite est placé dans l'image dans le cas des objets, les coordonnées de texture sont toujours les memes (on affiche toute l'image) */
    s.point_bg.x = 0;
    s.point_bg.y = 0;
    s.point_hd.x = 1;
    s.point_hd.y = 1;
    s.position.x = 0;

    for(i = 0; i < n->taille.x; i++)
    {
        s.position.y = 0;

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

					s.taille = n->objets[object_id].taille;
					s.text_id = n->objets[object_id].id_text[phase];

					// Affichage de l'image
					draw_sprite(&s);
					n->last_texture = s.text_id;
				}
				else
				{
					// Sinon on affiche qu'une seule image 
					s.taille = n->objets[object_id].taille;
					s.text_id = n->objets[object_id].id_text[0];

					draw_sprite(&s);
					n->last_texture = s.text_id;
				}
			}

            s.position.y += n->taille_blocs.y;
        }

        s.position.x += n->taille_blocs.x;
    }
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

