/******************************************************************************/
/*	texte.c		v0.8		05/02/2010			Guittonneau '[ZBM]' Olivier   */
/******************************************************************************/

#include "texte.h"

static g_vars G;

/* note: Lionel je t'emmerde, j'ai pas envie de commenter ce fichier. De toute 
façon mon TGV arrive, faut que je range le PC :p */
font_2d *new_font_2d(void)
{
    font_2d *f =  malloc(sizeof(font_2d));
    return init_font_2d(f);
}


font_2d *init_font_2d(font_2d *f)
{
    int i;
    caractere car = {0};

    for(i = 0; i < NB_CARACTERES; i++)
        f->carac[i] = car;

    f->id_texture = 0;
    f->nom_texture[0] = '\0';
    f->resolution.x = 0;
    f->resolution.y = 0;

    return f;
}


void free_font_2d(font_2d **f)
{
    if(f != NULL && *f != NULL)
	{
		free(*f);
		*f = NULL;
	}
}


font_2d *load_font_2d(char *nom)
{
    FILE *flux = NULL;
    font_2d *f;
    int i;
    char nom_texture[TAILLE_NOM_TEXTURE];

    flux = fopen(nom, "r");
    if(flux == NULL)
        return NULL;

    f = new_font_2d();
    if(f == NULL)
        return f;

    lire_font_2d(flux, f);

    for(i = 0; i < NB_CARACTERES; i++)
    {
        /* Calcul des coordonées relatives à partir des coordonées absolues */
        /* ATTENTION! Bitmap Font Generator utilise un axe y inversé par rapport
        à celui d'OpenGL, il faut donc inverser les coordonées de texture du 
        haut et du bas sur l'axe y pour chaque caractere! */
        f->carac[i].coords_bg.x = f->carac[i].position.x / (float)f->resolution.x;
        f->carac[i].coords_hd.y = -f->carac[i].position.y / (float)f->resolution.y;
        f->carac[i].coords_hd.x = (f->carac[i].position.x + f->carac[i].taille.x) / (float)f->resolution.x;
        f->carac[i].coords_bg.y = -(f->carac[i].position.y + f->carac[i].taille.y) / (float)f->resolution.y;
    }

    strcpy(nom_texture, f->nom_texture);
    strcpy(f->nom_texture, "fonts/");
    strcpy(f->nom_texture + 6, nom_texture);

    f->id_texture = charger_texture_bis(f->nom_texture, NULL);
    fclose(flux);

    return f;
}


font_2d *lire_font_2d(FILE *flux, font_2d *f)
{
    size_t ascii_actuel;
    char buff[300];
    char *debut;

    while(fgets(buff, 299 - 1, flux) != NULL)
    {
        debut = strstr(buff, "char ");
        if(debut == buff)
        {
            debut = chaine_apres_char(buff, '=');
            sscanf(debut, "%d", &ascii_actuel);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].position.x);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].position.y);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].taille.x);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].taille.y);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].offset.x);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].offset.y);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->carac[ascii_actuel].avance_x);
        }

        debut = strstr(buff, "page");
        if(debut == buff)
        {
            debut = chaine_apres_char(buff, '"');
            strcpy(f->nom_texture, debut);
            f->nom_texture[strlen(f->nom_texture) - 2] = '\0';
        }

        debut = strstr(buff, "common");
        if(debut == buff)
        {
            debut = strstr(buff, "lineHeight");
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->hauteur_ligne);
            debut = strstr(buff, "scaleW");
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->resolution.x);
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->resolution.y);
        }

        debut = strstr(buff, "info");
        if(debut == buff)
        {
            debut = strstr(buff, "outline");
            debut = chaine_apres_char(debut, '=');
            sscanf(debut, "%d", &f->outline);
        }
    }

    return f;
}


/* Oui je suis con, c'est strstr() + 1 au final */
char *chaine_apres_char(const char *chaine, char delim)
{
    while(*chaine != '\0' && *chaine != delim)
        chaine++;

    if(*chaine == '\0')
        return (char*)chaine;
    else
        return (char*)++chaine;
}


void draw_str_2d(string_2d str_2d)
{
    size_t i;
    vecti t;
    vectf c_bg, c_hd;
    vecti pos = str_2d.pos, pos_ini, offset;
    sprite s;
    float alpha, red, green, blue;
    couleur c = str_2d.c;
    char *msg = str_2d.str;
    font_2d *font = str_2d.font;

    pos_ini = pos;

    glBindTexture(GL_TEXTURE_2D, font->id_texture);

    s.position = pos;
    s.text_id = font->id_texture;

    /* On recupere les 4 composantes de la couleur */
    alpha = ((c >> 24) & 0xFF) / 255.0f;
    red = ((c >> 16) & 0xFF) / 255.0f;
    green = ((c >> 8) & 0xFF) / 255.0f;
    blue =  (c & 0xFF) / 255.0f;
    glColor4f(red, green, blue, alpha);

    glBegin(GL_QUADS);

    /* Dessin des sprites pour chaque caractere */
    for(i = 0; msg[i] != '\0' && i < STR_MAX_LEN; i++)
    {
        if(msg[i] != '\n')
        {
            if(msg[i] < 0)
            {
                t = font->carac[msg[i] + 256].taille;
                offset = font->carac[msg[i] + 256].offset;
                c_bg = font->carac[msg[i] + 256].coords_bg;
                c_hd = font->carac[msg[i] + 256].coords_hd;
            }
            else
            {
                t = font->carac[msg[i]].taille;
                offset = font->carac[msg[i]].offset;
                c_bg = font->carac[msg[i]].coords_bg;
                c_hd = font->carac[msg[i]].coords_hd;
            }

            /* L'offset permet de decaler le polygone qui contient le caractere,
            par exemple pour faire monter l'appostrophe et descendre le p
            On doit le recalculer sur l'axe y car les coordonées de Bitmap Font Generator
            utilisent un axe y inversé par rapport à OpenGL */
            offset.y = font->hauteur_ligne - (offset.y + t.y);
            pos.x  += offset.x;
            pos.y  += offset.y;

            s.position = pos;
            s.taille = t;
            s.point_bg = c_bg;
            s.point_hd = c_hd;
            draw_sprite_(&s, s.text_id);

            if(msg[i] < 0)
                pos.x += font->carac[msg[i] + 256].avance_x + font->outline;
            else
                pos.x += font->carac[msg[i]].avance_x + font->outline;

            /* On annule l'effet de l'offset pour ne pas affecter le prochain caractere */
            pos.x  -= offset.x;
            pos.y  -= offset.y;
        }
        else
        {
            pos.x = pos_ini.x;
            pos.y -= font->hauteur_ligne + font->outline;
        }
    }

    glEnd();
}


int determine_colonne(string_2d str_2d)
{
    size_t i;
    vecti t;
    vectf c_bg, c_hd;
    vecti pos = str_2d.pos, pos_ini, offset;
    char *msg = str_2d.str;
    font_2d *font = str_2d.font;
    pos_ini = pos;

    /* Dessin des sprites pour chaque caractere */
    for(i = 0; msg[i] != '\0' && i < STR_MAX_LEN; i++)
    {
        if(msg[i] != '\n')
        {
            if(msg[i] < 0)
            {
                t = font->carac[msg[i] + 256].taille;
                offset = font->carac[msg[i] + 256].offset;
                c_bg = font->carac[msg[i] + 256].coords_bg;
                c_hd = font->carac[msg[i] + 256].coords_hd;
            }
            else
            {
                t = font->carac[msg[i]].taille;
                offset = font->carac[msg[i]].offset;
                c_bg = font->carac[msg[i]].coords_bg;
                c_hd = font->carac[msg[i]].coords_hd;
            }

            /* L'offset permet de decaler le polygone qui contient le caractere,
            par exemple pour faire monter l'appostrophe et descendre le p
            On doit le recalculer sur l'axe y car les coordonées de Bitmap Font Generator
            utilisent un axe y inversé par rapport à OpenGL */
            offset.y = font->hauteur_ligne - (offset.y + t.y);
            pos.x  += offset.x;
            pos.y  += offset.y;

            if(msg[i] < 0)
                pos.x += font->carac[msg[i] + 256].avance_x + font->outline;
            else
                pos.x += font->carac[msg[i]].avance_x + font->outline;

            /* On annule l'effet de l'offset pour ne pas affecter le prochain caractere */
            pos.x  -= offset.x;
            pos.y  -= offset.y;
        }
        else
        {
            pos.x = pos_ini.x;
            pos.y -= font->hauteur_ligne + font->outline;
        }
    }

    return pos.x;
}


void screen_vprintf(vecti pos, font_2d *font, couleur c, char *format, va_list ap)
{
    char msg[STR_MAX_LEN + 1];
    string_2d *s;

    vsnprintf(msg, STR_MAX_LEN, format, ap);
    s = &G.buffer_2d[G.nb_msg_2d];
    s->c = c;
    s->font = font;
    s->pos = pos;
    s->str = malloc(strlen(msg) + 1);
    strcpy(s->str, msg);
}


void screen_printf(vecti pos, font_2d *font, couleur c, char *format, ...)
{
    if(G.nb_msg_2d < G.nb_msg_max)
    {
        va_list ap;

        va_start(ap, format);

        if(font == NULL)
            font = G.debug_font;

        screen_vprintf(pos, font, c, format, ap);
        va_end(ap);

        G.nb_msg_2d++;
    }
    else if(G.nb_msg_2d == G.nb_msg_max)
    {
        vecti pos = {50, 50};

        screen_vprintf(pos, G.debug_font, G.couleur, "!!!ERREUR: BUFFER 2D TROP PETIT!!!", NULL);
        G.nb_msg_2d++;
    }
}


void screen_printf_dbg(char *format, ...)
{
    if(G.nb_msg_2d < G.nb_msg_max)
    {
        va_list ap;
        vecti pos;
        size_t i;

        pos.x = G.colonne_actuelle;
        pos.y = HAUTEUR_FENETRE - (G.ligne_actuelle + 1) * G.debug_font->hauteur_ligne - DEBUT_LIGNE_Y;

        va_start(ap, format);
        screen_vprintf(pos, G.debug_font, G.couleur, format, ap);
        va_end(ap);

        for(i = 0; format[i] && i < STR_MAX_LEN; i++)
        {
            if(format[i] == '\n')
            {
                G.ligne_actuelle++;
            }
        }

        if(format[i - 1] == '\n')
        {
            G.colonne_actuelle = DEBUT_LIGNE_X;
        }
        else
        {
            G.colonne_actuelle = determine_colonne(G.buffer_2d[G.nb_msg_2d]);
        }

        G.nb_msg_2d++;
    }
    else if(G.nb_msg_2d == G.nb_msg_max)
    {
        vecti pos = {50, 50};

        screen_vprintf(pos, G.debug_font, G.couleur, "!!!ERREUR: BUFFER 2D TROP PETIT!!!", NULL);
        G.nb_msg_2d++;
    }
}


void screen_flush(void)
{
    size_t i;

    for(i = 0; i < G.nb_msg_2d; i++)
    {
        draw_str_2d(G.buffer_2d[i]);
        free(G.buffer_2d[i].str);
    }

    G.ligne_actuelle = 0;
	G.colonne_actuelle = DEBUT_LIGNE_X;
    G.nb_msg_2d = 0;
}


void init_text(char *font_name, size_t buff_size, couleur c)
{
    G.buffer_2d = malloc(sizeof(string_2d) * (buff_size + 1));
    G.nb_msg_max = buff_size;
    G.debug_font = load_font_2d(font_name);
    G.couleur = c;

    G.colonne_actuelle = DEBUT_LIGNE_X;
    G.ligne_actuelle = 0;
    G.nb_msg_2d = 0;
}


void quit_text(void)
{
    free_font_2d(&G.debug_font);
    free(G.buffer_2d);
}


void add_str_2d_to_list(str_2d_list *list, vecti pos, font_2d *font, couleur c, Uint32 time_left, char *format, ...)
{
    char msg[STR_MAX_LEN + 1];
    va_list ap;
    str_2d_elt *elt;

    va_start(ap, format);
    vsnprintf(msg, STR_MAX_LEN, format, ap);
    va_end(ap);

    /* Initialisation nouvel element */
    elt = malloc(sizeof(str_2d_elt));
    elt->str.c = c;
    elt->str.font = font;
    elt->str.pos = pos;
    elt->time_left = time_left;
    elt->str.str = malloc(strlen(msg) + 1);
    strcpy(elt->str.str, msg);

    if(list->nb_elts)
    {
        elt->next = list->first;
    }
    else
    {
        elt->next = NULL;
    }

    /* Passage en tete de liste */
    list->first = elt;
    list->nb_elts++;
}


void rem_str_2d_from_list(str_2d_list *list, str_2d_elt *elt)
{
    str_2d_elt *current = list->first, *prev = NULL;

    while(current != elt && current != NULL)
    {
        prev = current;
        current = current->next;
    }

    if(current == elt)
    {
        if(prev == NULL)
        {
            list->first = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        free(current->str.str);
        free(current);
    }
}
