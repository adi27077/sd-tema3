/* MUSAT-BURCEA Adrian - 312CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOOK_NAME_LENGTH 50
#define AUTHOR_NAME_LENGTH 40

typedef struct book
{
    char *name;
    char *author;
    int rating;
    int pages;
}Tbook;

//functie pt alocare si creare carte
Tbook *createBook(char *title, char *author, int rating, int pages);
void ShowBookName(void *b, FILE *g); //afisare nume carte
void ShowBookAuthor(void *b, FILE *g); //afisare autor carte
void freeBook(void *b); //eliberare memorie carte
