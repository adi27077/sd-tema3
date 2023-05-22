/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _TRIE_
#define _TRIE_

#define ALPHABET_SIZE 68

typedef struct trie
{
    struct trie *children[ALPHABET_SIZE]; //vector copii
    int isEoW;                            //este sau nu end of word
    void *info;                           //adresa catre info frunza
}TTrie, *ATrie;

typedef void (*TF)(void *); //tip functie eliberare info
typedef void (*TFP)(void *, FILE *); //tip functie afisare elem 

TTrie *InitTrie(); //initializare trie
int InsTrie(ATrie t, char *key, void *info); //inserare in trie
void *SearchTrie(ATrie t, char *key); //cautare in trie
void PrintTrie(TTrie *a, FILE *g, TFP f); //afisare elem din frunze
TTrie *SearchPrefix(ATrie t, char *prefix); //cautare prefix
void PrintAutocomplete(TTrie *t, FILE *g, TFP f, int *count);
//^afisare primele 3 elemente din frunze
void Autocomplete(TTrie *t, FILE *g, TFP f, int *count);
//^aplicare alta functie de afisare pe primele 3 elemente din frunze
int RemTrie(TTrie **t, char *key); //stergere cheie din trie
void DestrTrie(TTrie *a, TF fe); //eliberare memorie trie
void DestrTrie2(void *a); //eliberare memorie trie fara info

#endif
