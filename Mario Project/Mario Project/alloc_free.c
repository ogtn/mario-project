/******************************************************************************/
/*	alloc_free.c	v0.8	04/02/2010			Guittonneau '[ZBM]' Olivier   */
/******************************************************************************/

#include "alloc_free.h"

#ifdef _DEBUG

static allocations_list liste = {NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Pour que les appels à malloc() et free() realisés dans ce fichier soient les
vrais de <stdlib.h>, et non ceux de alloc_free.h */
#undef main
#undef malloc
#undef free


void add_list(size_t size, void *p, unsigned int line, char *file)
{
    ptr *elt = NULL;

    elt = malloc(sizeof(ptr));

    if(elt == NULL)
    {
        return;
    }

    /* Initialisation du nouvel element de la liste */
    elt->overflow = 0;
    elt->underflow = 0;
    elt->adress = p;
    elt->alloc_line = line;
    elt->size = size;
    elt->next = liste.first;
    strcpy(elt->file, find_filename(file));

    /* Maj de la liste */
    liste.first = elt;
    liste.allocated_blocs++;
    liste.alloc_size += size;
    liste.max_alloc_size = max(liste.max_alloc_size, liste.alloc_size);
}


void add_null_freed(unsigned int line, char *filename)
{
    null_ptr_freed *new_first = malloc(sizeof(null_ptr_freed));

    strcpy(new_first->filename, find_filename(filename));
    new_first->line = line;
    new_first->next = liste.first_null;

    liste.first_null = new_first;
    liste.null_freed++;
}


void add_unknown_freed(unsigned int line, char *filename)
{
    unknown_ptr_freed *new_first = malloc(sizeof(unknown_ptr_freed));

    strcpy(new_first->filename, find_filename(filename));
    new_first->line = line;
    new_first->next = liste.first_unknown;

    liste.first_unknown = new_first;
    liste.unknown_freed++;
}


int remove_from_list(void *adress, unsigned int line, char *filename)
{
    ptr *actual = liste.first, *prec = NULL;
    size_t i;

    while(actual != NULL && actual->adress != adress)
    {
        prec = actual;
        actual = actual->next;
    }

    /* Le pointeur n'apparait pas dans la liste */
    if(actual == NULL)
    {
        add_unknown_freed(line, filename);
        return -1;
    }

    /* Maj de la liste */
    liste.alloc_size -= actual->size;
    liste.allocated_blocs--;
    liste.free_size += actual->size;
    liste.freed_blocs++;

    /* Verification des debordements */
    test_overflow(actual);
    test_underflow(actual);

    /* Si debordement, on affiche ça */
    if(actual->overflow || actual->underflow)
    {
        if(liste.overflows + liste.underflows != 0)
        {
            fputs("-----------------------------------------------------------------------\n", fic);
        }

        fprintf(fic, "@%p:\t%d octets\n", actual->adress, actual->size);
        fprintf(fic, "Allocation:\t\t%s\tligne\t%d\n", actual->file, actual->alloc_line);
        fprintf(fic, "Desallocation:\t%s\tligne\t%d\n", find_filename(filename), line);

        if(actual->overflow == 1)
        {
            liste.overflows++;
            fprintf(fic, "Presence de buffer overflow!\n");
        }
        if(actual->underflow)
        {
            liste.underflows++;
            fprintf(fic, "Presence de buffer underflow\n");
        }
    }

    /* On supprime l'element de la liste */
    if(prec == NULL)
        liste.first = actual->next;
    else
        prec->next = actual->next;

    for(i = 0; i < actual->size; i++)
        ((char *)actual->adress)[i] = MAGIC_FREE_CST;

    free(actual);

    return 0;
}


void affich_list(void)
{
    ptr *actual_ptr = liste.first;
    ptr *prev_ptr = NULL;
    null_ptr_freed *actual_null_freed = liste.first_null;
    null_ptr_freed *prev_null_freed = NULL;
    unknown_ptr_freed *actual_unknown = liste.first_unknown;
    unknown_ptr_freed *prev_unknown = NULL;

    liste.first = NULL;
    liste.first_null = NULL;
    liste.first_unknown = NULL;

    if(liste.null_freed)
    {
        fputs("\n============[Tentatives de desallocation de pointeurs NULL]============\n", fic);
    }

    while(actual_null_freed != NULL)
    {
        fprintf(fic, "Tentative de desallocation:\t%s\tligne\t%d\n", actual_null_freed->filename, actual_null_freed->line);

        prev_null_freed = actual_null_freed;
        actual_null_freed = actual_null_freed->next;

        free(prev_null_freed);

        if(actual_null_freed != NULL)
        {
            fputs("-----------------------------------------------------------------------\n", fic);
        }
    }

    if(liste.unknown_freed)
    {
        fputs("\n========[Desallocations de pointeurs inconnus ou deja liberes]=========\n", fic);
    }

    while(actual_unknown != NULL)
    {
        fprintf(fic, "Tentative de desallocation:\t%s\tligne\t%d\n", actual_unknown->filename, actual_unknown->line);

        prev_unknown = actual_unknown;
        actual_unknown = actual_unknown->next;

        free(prev_unknown);

        if(actual_unknown != NULL)
        {
            fputs("-----------------------------------------------------------------------\n", fic);
        }
    }

    if(liste.allocated_blocs)
    {
        fputs("\n==========================[Fuites de memoire]==========================\n", fic);
    }

    while(actual_ptr != NULL)
    {
        char *ad;

        fprintf(fic, "@%p:\t%d octets\n", actual_ptr->adress, actual_ptr->size);
        fprintf(fic, "Allocation:\t%s\tligne\t%d\n", actual_ptr->file, actual_ptr->alloc_line);

        test_overflow(actual_ptr);
        test_underflow(actual_ptr);

        if(actual_ptr->overflow == 1)
        {
            liste.overflows++;
            fprintf(fic, "Presence de buffer overflow\n");
        }
        if(actual_ptr->underflow)
        {
            liste.underflows++;
            fprintf(fic, "Presence de buffer underflow\n");
        }

        prev_ptr = actual_ptr;
        actual_ptr = actual_ptr->next;
    
        /* Desallocation du bloc victime de fuite et de l'element de la liste */
        ad = prev_ptr->adress;
        ad -= UNDERFLOW_SIZE;
        //free(ad);
        free(prev_ptr);

        if(actual_ptr != NULL)
            fputs("-----------------------------------------------------------------------\n", fic);
    }

    fputs("\n========================[Resume de l'execution]========================\n", fic);

    if(liste.overflows == 1)
    {
        fputs("ATTENTION: un buffer overflow a ete detecte\n", fic);
    }
    else if(liste.overflows)
    {
        fprintf(fic, "ATTENTION: %d buffers overflows ont ete detectes\n", liste.overflows);
    }

    if(liste.underflows == 1)
    {
        fputs("ATTENTION: un buffer underflow a ete detecte\n", fic);
    }
    else if(liste.underflows)
    {
        fprintf(fic, "ATTENTION: %d buffers underflows ont ete detectes\n", liste.underflows);
    }

    if(!liste.overflows && !liste.underflows)
    {
        fputs("Aucun debordement de tampon detecte\n", fic);
    }

    if(liste.allocated_blocs == 1)
    {
        fprintf(fic, "ATTENTION: une fuite de memoire pour un total de %d octets (%dMo) a ete detectee\n", liste.alloc_size, liste.alloc_size / (1024 * 1024));
    }
    else if(liste.allocated_blocs)
    {
        fprintf(fic, "ATTENTION: %d fuites de memoire pour un total de %d octets (%dMo) ont ete detectees\n", liste.allocated_blocs, liste.alloc_size, liste.alloc_size / (1024 * 1024));
    }
    else
    {
        fputs("Aucune fuite de memoire detectee\n", fic);
    }

    if(liste.null_freed == 1)
    {
        fputs("ATTENTION: Une tentative de desallocation de pointeur NULL a ete detectee\n", fic);
    }
    else if(liste.null_freed)
    {
        fprintf(fic, "ATTENTION: %d tentatives de desallocations de pointeurs NULL ont ete detectees\n", liste.null_freed);
    }
    else
    {
        fputs("Aucune tentative de desallocation de pointeur NULL detectee\n", fic);
    }

    if(liste.unknown_freed == 1)
    {
        fputs("ATTENTION: Une tentative de desallocation de pointeur inconnu ou deja desalloue a ete detectee\n", fic);
    }
    else if(liste.null_freed)
    {
        fprintf(fic, "ATTENTION: %d tentatives de desallocations de pointeurs inconnus ou deja desalloues ont ete detectees\n", liste.null_freed);
    }
    else
    {
        fputs("Aucune tentative de desallocation de pointeur inconnu ou deja desalloue detectee\n", fic);
    }

    fprintf(fic, "Consomation maximale de ram: %d octets (%dMo)\n", liste.max_alloc_size, liste.max_alloc_size / (1024 * 1024));
}


void test_overflow(ptr *p)
{
    size_t i;
    char *testeur = NULL;

    if(p == NULL || p->adress == NULL)
        return;

    testeur = p->adress;

    for(i = p->size; i < p->size + OVERFLOW_SIZE; i++)
        if(testeur[i] != MAGIC_CST)
        {
            p->overflow = 1;
            return;
        }
}


void test_underflow(ptr *p)
{
    size_t i;
    char *testeur = NULL;

    if(p == NULL || p->adress == NULL)
        return;

    testeur = p->adress;
    testeur -= UNDERFLOW_SIZE;

    for(i = 0; i < UNDERFLOW_SIZE; i++)
        if(testeur[i] != MAGIC_CST)
        {
            p->underflow = 1;
            return;
        }
}


//int main(int argc, char *argv[])
//{
//    int res;
//
//    fic = fopen("allocations.txt", "w");
//
//    fputs("============[Debordements de tampon dans les blocs liberes]============\n", fic);
//
//    res = my_main(argc, argv);
//    affich_list();
//    fclose(fic);
//
//    return res;
//}


void *my_malloc(size_t size, unsigned int line, char *filename)
{
    char *p = NULL;
    size_t i;

    if(size == 0)
        return NULL;

    p = malloc(size + FLOWS_SIZE);

    if(p != NULL)
    {
        /* Remplissage du buffer d'underflow */
        for(i = 0; i < UNDERFLOW_SIZE; i++)
            p[i] = MAGIC_CST;

        /* Remplissage du buffer d'overflow */
        p += UNDERFLOW_SIZE;
        for(i = size; i < size + OVERFLOW_SIZE; i++)
            p[i] = MAGIC_CST;

        add_list(size, p, line, filename);
        test_overflow(liste.first);
        test_underflow(liste.first);
    }
    else
    {
        fprintf(fic, "Echec d'allocation:\t%s\tligne\t%d\n", find_filename(filename), line);
    }

    return (void*) p;
}


void my_free(void *adress, unsigned int line, char *filename)
{
    char *p;

    if(adress != NULL)
    {
        /* On ne libere la memoire que si on connait le pointeur */
        if(remove_from_list(adress, line, filename) == 0)
        {
            p = adress;
            p -= UNDERFLOW_SIZE;
            free(p);
        }
    }
    else
    {
        add_null_freed(line, filename);                                           
    }
}


void *my_realloc(void *adress, size_t size, unsigned int line, char *file)
{
    void *res;

    if(adress == NULL)
    {
        res = my_malloc(size, line, file);
    }
    else if(size == 0)
    {
        my_free(adress, line, file);
        res = NULL;
    }
    else
    {        
        ptr *actual = liste.first, *prec = NULL;

        while(actual != NULL && actual->adress != adress)
        {
            prec = actual;
            actual = actual->next;
        }

        /* Le pointeur inconnu */
        if(actual == NULL)
        {
            add_unknown_freed(line, file);
            res = adress;
        }
        else
        {
            res = my_malloc(size, line, file);

            if(res == NULL)
            {
                res = adress;
            }
            else
            {
                /* Recopie des données et libération */
                if(actual->size < size)
                    memcpy(res, adress, actual->size);
                else
                    memcpy(res, adress, size);

                my_free(adress, line, file);
            }
        }
    }

    return res;
}


void *my_calloc(size_t size, unsigned int line, char *file)
{
    size_t i;
    void *res = my_malloc(size, line, file);

    for(i = 0; i < size; i++)
        ((char *)res)[i] = 0;

    return res;
}


char *find_filename(char *str)
{
    char path[FILENAME_SIZE];
    char *filename = str;
    char *temp;

    strcpy(path, str);
    temp = strtok(path, "\\/");

    while(temp != NULL)
    {
        temp = strtok(NULL, "\\/");

        if(temp)
        {
            filename = temp;
        }
    }

    return filename;
}


#endif
