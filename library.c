/* MUSAT-BURCEA Adrian - 312CB */

#include "library.h"

/*functie alocare si creare carte*/
Tbook *createBook(char *title, char *author, int rating, int pages)
{
    Tbook *b = (Tbook *)malloc(sizeof(Tbook));
    if(!b)
        return NULL;
    b->name = strdup(title);
    b->author = strdup(author);
    b->rating = rating;
    b->pages = pages;
    return b;
}

/*functie pt afisarea numelui cartii*/
void ShowBookName(void *b, FILE *g)
{
    Tbook *bb = (Tbook *)b;
    fprintf(g, "%s\n", bb->name);
}

/*functie pt afisarea autorului cartii*/
void ShowBookAuthor(void *b, FILE *g)
{
    Tbook *bb = (Tbook *)b;
    fprintf(g, "%s\n", bb->author);
}

/*functie eliberare memorie carte*/
void freeBook(void *b)
{
    Tbook *bb = (Tbook *)b;
    if(!bb)
        return;
    free(bb->name);
    free(bb->author);
    free(bb);
}
