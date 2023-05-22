/* MUSAT-BURCEA Adrian - 312CB */

#include "Trie.h"

/*functie pt aflarea indicelui unui caracter din alfabetul dat*/
int getIndex(char c)
{
    char *a = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";
    int i;
    for(i = 0; i < ALPHABET_SIZE; i++)
        if(c == a[i])
            return i;
    return -1;
}

/*functie initializare trie*/
TTrie *InitTrie()
{
    ATrie p;
    p = (ATrie)malloc(sizeof(TTrie));
    if(!p)
        return NULL;
    
    int i;
    p->isEoW = 0;
    //initializare vector copii cu NULL
    for(i = 0; i < ALPHABET_SIZE; i++)
        p->children[i] = NULL;

    return p;
}

/*functie inserare in trie
  daca exista deja cheia, structura nu se modifica*/
int InsTrie(ATrie t, char *key, void *info)
{
    int i, index;
    int length = strlen(key);
    ATrie p = t;

    for(i = 0; i < length; i++)
    {
        index = getIndex(key[i]);
        if(!p->children[index]) //daca nodul nu exista, se creeaza
            p->children[index] = InitTrie();
        p = p->children[index];
    }

    if(p->isEoW == 1) //cheia exista deja, nu mai inseram info
        return 0;
    //marcare ultimul nod ca frunza
    p->isEoW = 1;
    //inserare informatie
    p->info = info;
    return 1;
}

/*functie de cautare in trie cu returnare info*/
void *SearchTrie(ATrie t, char *key)
{
    int i, index;
    int length = strlen(key);
    ATrie p = t;

    for(i = 0; i < length; i++)
    {
        index = getIndex(key[i]);
        if(!p->children[index])
            return NULL; //cheia nu exista
        p = p->children[index];
    }

    /*daca se ajunge la sfarsitul cheii verificam daca ultima
    litera este frunza*/
    if(p != NULL && p->isEoW == 1)
        return p->info;
    return NULL;
}

/*functie de cautare a unui prefix in trie cu returnare subarbore*/
TTrie *SearchPrefix(ATrie t, char *prefix)
{
    int i, index;
    int length = strlen(prefix);
    ATrie p = t;

    for(i = 0; i < length; i++)
    {
        index = getIndex(prefix[i]);
        if(!p->children[index])
            return NULL; //prefixul nu exista
        p = p->children[index];
    }

    /*daca se ajunge la sfarsit returnam subarborele dat de prefix*/
    return p;
}

/*functie afisare elemente din frunzele trielui*/
void PrintTrie(TTrie *t, FILE *g, TFP f)
{
    if(!t)
        return;
    int i;
    ATrie p = t;

    if(p->isEoW == 1) //afisare element daca este sfarsit de cheie
        f(p->info, g);

    //afisare elemente recursiva
    for(i = 0; i < ALPHABET_SIZE; i++)
        if(p->children[i])
            PrintTrie(t->children[i], g, f);
    
}

/*functie afisare un singur element*/
void Print1Elem(TTrie *t, FILE *g, TFP f)
{
    int i;
    ATrie p = t;

    for(i = 0; i < ALPHABET_SIZE;) //parcurgere pana la prima frunza gasita
        if(p->children[i])
        {
            p = p->children[i];
            i = 0;
        }
        else
            i++;
    
    if(p->isEoW) //afisare element gasit
        f(p->info, g);
    
}

/*functie care afiseaza primele 3 rezultate date de 
cautarea unui prefix in trie*/
void PrintAutocomplete(TTrie *t, FILE *g, TFP f, int *count)
{
    int i;
    ATrie p = t;

    if(*count >= 3) //am facut deja 3 afisari
        return;

    if(p->isEoW == 1) //afisare element daca este sfarsit de cheie
    {
        f(p->info, g);
        (*count)++;
    }

    //afisare recursiva
    for(i = 0; i < ALPHABET_SIZE; i++)
        if(p->children[i])
            PrintAutocomplete(p->children[i], g, f, count);
    
}

/*functie care aplica alta functie pe primele 3 rezultate
date de cautarea unui prefix*/
void Autocomplete(TTrie *t, FILE *g, TFP f, int *count)
{
    int i;
    ATrie p = t;

    if(*count >= 3) //am facut deja 3 afisari
        return;

    if(p->isEoW == 1) //afisare element daca este sfarsit de cheie
    {
        Print1Elem(p->info, g, f);
        (*count)++;
    }

    //afisare recursiva
    for(i = 0; i < ALPHABET_SIZE; i++)
        if(p->children[i])
            Autocomplete(p->children[i], g, f, count);
    
}

/*functie care verifica daca un nod e gol sau nu*/
int EmptyNode(TTrie *a)
{
    int i;
    for(i = 0; i < ALPHABET_SIZE; i++)
        if(a->children[i])
            return 0;
    return 1;
}

/*functie care elimina o cheie din trie*/
int RemTrie(TTrie **t, char *key)
{
    if(*t == NULL) //trie gol
        return 0;

    //cazul in care nu am ajuns la sfarsitul cheii
    if(*key)
    {
        //stergere recursiva a nodurilor
        int index = getIndex(*key);
        if(*t != NULL && (*t)->children[index] != NULL)
        {
            int res = RemTrie(&((*t)->children[index]), key + 1);
            /*daca s-a sters nodul copil si nodul curent nu e frunza
            putem sterge nodul curent*/ 
            if(res && (*t)->isEoW == 0)
            {
                if(EmptyNode(*t)) //daca nodul nu mai are copii, il eliberam
                {
                    free(*t);
                    *t = NULL;
                    return 1;
                }
                else
                    return 0; //nodul mai avea copii si nu l-am sters
            }
        }
    }

    //cazul in care suntem la ultimul caracter din cheie
    if(*key == '\0' && (*t)->isEoW)
    {
        //daca nodul curent e frunza si nu are copii
        if(EmptyNode(*t))
        {
            free(*t); //eliberam nodul
            *t = NULL;
            return 1; //putem sterge nodul anterior
        }
        else //daca nodul curent e frunza si are copii
        {
            (*t)->isEoW = 0; //nodul curent nu mai este sfarsit de cheie
            return 0; //nodul aveam copii si nu l am sters
        }
    }
    return 0;
}

/*functie stergere trie*/
void DestrTrie(TTrie *a, TF fe)
{
    int i;
    if(!a)
        return;

    //eliberare recursiva de la frunze spre inceput
    for(i = 0; i < ALPHABET_SIZE; i++)
        DestrTrie(a->children[i], fe);

    if(a->isEoW) //daca este frunza, eliberam info
        fe(a->info);
    free(a);
}

/*functie auxiliara eliberare trie cu info deja eliberat*/
void DestrTrie2(void *a)
{
    ATrie t = (ATrie)a;
    int i;
    if(!t)
        return;

    //eliberare recursiva de la frunze spre inceput
    for(i = 0; i < ALPHABET_SIZE; i++)
        DestrTrie2(t->children[i]);

    free(t);
}
