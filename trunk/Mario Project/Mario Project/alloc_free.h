/******************************************************************************/
/*	alloc_free.h	v0.8	04/02/2010			Guittonneau '[ZBM]' Olivier   */
/******************************************************************************/

#ifdef _DEBUG

#ifndef ALLOC_FREE_H
#define ALLOC_FREE_H

/*			=======================[Defines/Enums]====================		  */

#define FILENAME_SIZE		    	256
#define OVERFLOW_SIZE       		16
#define UNDERFLOW_SIZE              16
#define FLOWS_SIZE                  (OVERFLOW_SIZE + UNDERFLOW_SIZE)
#define MAGIC_CST                   '«'
#define MAGIC_FREE_CST              'Í'


/*			=======================[Includes]=========================		  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*			=======================[Macros]============================		  */

//#define main my_main

#define malloc(taille)              my_malloc((taille), __LINE__, __FILE__)

#define free(adresse)               my_free((adresse), __LINE__, __FILE__)

#define realloc(adresse, taille)    my_realloc((adresse), (taille), __LINE__, __FILE__)

#define calloc(nombre, taille)      my_calloc(((nombre) * (taille)), __LINE__, __FILE__)

/* Macros pratiques */
#ifndef min
#define min(a, b) (((a)<(b))?(a):(b))
#endif

#ifndef max
#define max(a, b) (((a)>(b))?(a):(b))
#endif


/*			=======================[Structures]=======================		  */

FILE *fic;


typedef struct ptr
{
	void *adress;
	size_t size;
	char file[FILENAME_SIZE];
	unsigned int alloc_line;
	struct ptr *next;
	int overflow;
    int underflow;
} ptr;


typedef struct null_ptr_freed
{
    char filename[FILENAME_SIZE];
    unsigned int line;
    struct null_ptr_freed *next;
} null_ptr_freed;


typedef struct unknown_ptr_freed
{
    char filename[FILENAME_SIZE];
    unsigned int line;
    struct unknown_ptr_freed *next;
} unknown_ptr_freed;


typedef struct allocations_list
{
	ptr *first;
    null_ptr_freed *first_null;
    unknown_ptr_freed *first_unknown;

	unsigned int allocated_blocs;
	unsigned int freed_blocs;
	size_t alloc_size;
    size_t max_alloc_size;
	size_t free_size;
    unsigned int underflows;
    unsigned int overflows;
    unsigned int null_freed;
    unsigned int unknown_freed;
} allocations_list;


/*			=======================[Prototypes]=======================		  */

/* Se substitue au main du programme, il faut passer par la forme avec arguments,
sinon la macro ne fonctionne pas. Incompatible avec la SDL, ou tout autre lib
qui utilise ce procede */
int main(int argc, char *argv[]);

/* Ajoute la nouvelle allocation a la liste des blocs alloues */
void add_list(size_t size, void *p, unsigned int line, char *file);

/* Supprime une allocation de la liste des blocs alloues lors de la demande  de
liberation. Inscrit dans le fichier allocation.txt si un debordement a eu lieu.
Elle retourne 0 si la suppression a eu lieu, -1 sinon (pointeur inconnu) */
int remove_from_list(void *adress, unsigned int line, char *filename);

/* Incrit dans le fichier allocation.txt les anomalies detectees, les blocs non
liberes, ainsi que le resume de l'execution*/
void affich_list(void);

/* Verifie si un buffer overflow a eu lieu, et met a jour la structure si
necessaire */
void test_overflow(ptr *p);

/* Verifie si un buffer underflow a eu lieu, et met a jour la structure si
necessaire */
void test_underflow(ptr *p);

/* Se substitue au malloc() standard, trace les allocations realisées ainsi que
les allocations de taille nulle et les echecs d'allocation */
void *my_malloc(size_t size, unsigned int line, char *file);

/* Se substitue au free() standard, trace les desallocations ainsi que les
liberations de pointeurs NULL ou non alloués via my_malloc() */
void my_free(void *adress, unsigned int line, char *file);

/* Se substitue au realloc() standard, il s'appuie sur my_malloc()
et my_free() pour proposer les mêmes services */
void *my_realloc(void *adress, size_t size, unsigned int line, char *file);

/* Se substitue au calloc() standard, équivalent à my_malloc(), mais
avec une mise à 0 de la mémoire allouée */
void *my_calloc(size_t size, unsigned int line, char *file);

/* Recupere le nom du fichier a partir de son chemin complet */
char *find_filename(char *str);

/* Ajoute la tentative de desallocation de pointeur NULL a la liste */
void add_null_freed(unsigned int line, char *filename);


#endif /* fin protection inclusions multiples */

#endif /* fin _DEBUG */
