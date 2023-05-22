/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie.h"
#include "library.h"

void addBook(ATrie T1, ATrie T2, Tbook *b)
{
    int rez = InsTrie(T1, b->name, (void *)b); //ins carte in T1
    if(!rez) //daca inserarea nu se face dezalocam memoria pt cartea citita
    {
        freeBook(b);
        return; //nu mai este nevoie sa incercam inserare in T2
    }
    ATrie tr_auth = (ATrie)SearchTrie(T2, b->author); //cautare autor in T2
    if(tr_auth == NULL) //daca autorul nu exista
    {
        tr_auth = InitTrie(); //cream trie pt cartile lui
        InsTrie(tr_auth, b->name, (void *)b); //ins cartea in trie-ul lui
        //^nu mai trebuie verificata inserarea
        InsTrie(T2, b->author, (void *)tr_auth);
        /*^inserare autor in T2 si adaugare trie cu cartile lui
        ca informatie*/
    }
    else //autorul exista deja
        InsTrie(tr_auth, b->name, (void *)b); //ins cartea in trie-ul lui
}

void searchBook(ATrie T1, char *title, FILE *g)
{
    //cautare carte in arborele T1
    Tbook *book = (Tbook *)SearchTrie(T1, title);
    if(book != NULL)
        //afisare informatii despre carte
        fprintf(g, "Informatii recomandare: %s, %s, %d, %d\n", 
                book->name, book->author, book->rating, book->pages);
    else
        fprintf(g, "Cartea %s nu exista in recomandarile tale.\n", title);
}

void listAuthor(ATrie T2, char *author, FILE *g)
{
    //cautare autor in arorele T2
    ATrie tr_auth = (ATrie)SearchTrie(T2, author);
    if(tr_auth != NULL)
        //afiseaza toate cartile autorului
        PrintTrie(tr_auth, g, ShowBookName);
    else
        fprintf(g, "Autorul %s nu face parte din recomandarile tale.\n", author);
}

void searchbyAuthor(ATrie T2, char *author, char *title, FILE *g)
{
    //gasire arbore asociat autorului
    ATrie tr_auth = (ATrie)SearchTrie(T2, author);
    if(tr_auth != NULL)
    {
        //cautare carte in arborele gasit
        Tbook *book = (Tbook *)SearchTrie(tr_auth, title);
        if(book != NULL)
            fprintf(g, "Informatii recomandare: %s, %s, %d, %d\n", 
                    book->name, book->author, book->rating, book->pages);
        else
            fprintf(g, "Cartea %s nu exista in recomandarile tale.\n", title);
    }
    else
        fprintf(g, "Autorul %s nu face parte din recomandarile tale.\n", author);
}

void searchBookAutocomplete(ATrie T1, char *prefix, FILE *g)
{
    //gasire subarbore de la sfarsitul prefixului
    ATrie suggestions = SearchPrefix(T1, prefix);
    if(suggestions != NULL)
    {
        //afisarea primelor 3 sugestii de autocompletare
        int count = 0;
        PrintAutocomplete(suggestions, g, ShowBookName, &count);
    }
    else
        fprintf(g, "Nicio carte gasita.\n");
}

void listAuthorAutocomplete(ATrie T2, char *prefix, FILE *g)
{
    //gasire subarbore de la sfarsitul prefixului
    ATrie suggestions = SearchPrefix(T2, prefix);
    if(suggestions != NULL)
    {
        //afisarea primelor 3 sugestii de autocompletare
        int count = 0;
        Autocomplete(suggestions, g, ShowBookAuthor, &count);
    }
    else
        fprintf(g, "Niciun autor gasit.\n");
}

void searchbyAuthorAutocomplete(ATrie T2, char *author, char *prefix, FILE *g)
{
    //gasire arbore corespunzator autorului
    ATrie tr_auth = (ATrie)SearchTrie(T2, author);
    if(tr_auth != NULL)
        //cautam prefixul cartii in arborele gasit
        searchBookAutocomplete(tr_auth, prefix, g);
    else
        fprintf(g, "Autorul %s nu face parte din recomandarile tale.\n", author);
}

void deleteBook(ATrie T1, ATrie T2, char *title, FILE *g)
{
    //verificare daca exista cartea
    Tbook *b = (Tbook *)SearchTrie(T1, title);
    if(b == NULL)
    {
        fprintf(g, "Cartea %s nu exista in recomandarile tale.\n", title);
        return;
    }
    char *author = strdup(b->author);
    //eliberare memorie carte
    freeBook((void *)b);
    //daca exista cartea, o stergem din T1 si din arborele autorului
    RemTrie(&T1, title);
    ATrie tr_auth = (ATrie)SearchTrie(T2, author);
    RemTrie(&tr_auth, title);
    /*daca autorul respectiv nu mai are nicio carte, eliberam
    arborele si il eliminam pe autor din T2*/
    if(tr_auth == NULL)
    {
        DestrTrie2((void *)tr_auth);
        RemTrie(&T2, author);
    }
    free(author);
}

int main(int argc, char *argv[])
{
    char *input_file = argv[1];
    char *output_file = argv[2];
    size_t n = 2;
    
    char *line = malloc(n * sizeof(char));
    if(line == NULL)
        return 1;
    char *operation;

    //initializare trie T1 pt carti
    ATrie T1 = InitTrie();
    if(T1 == NULL)
    {
        free(line);
        return 1;
    }
    //initializare trie T2 pt autori
    ATrie T2 = InitTrie();
    if(T2 == NULL)
    {
        free(line);
        DestrTrie(T1, freeBook);
        return 1;
    }
    
    FILE *f = fopen(input_file, "rt");
    FILE *g = fopen(output_file, "wt");

    //citim fiecare linie din fisier
    while(getline(&line, &n, f) != -1)
    {
        if(strcmp(line, "\n") == 0) //verificare linie goala
            continue;
        
        operation = strtok(line, " "); //citire operatie

        if(strcmp(operation, "add_book") == 0)
        {
            char *title = strtok(NULL, ":");
            char *author = strtok(NULL, ":");
            int rating = atoi(strtok(NULL, ":"));
            int pages = atoi(strtok(NULL, ":"));
            Tbook *book = createBook(title, author, rating, pages);
            addBook(T1, T2, book); //inserare carte
            continue;
        }

        if(strcmp(operation, "search_book") == 0)
        {
            char *title = strtok(NULL, ":\n");
            if(title[strlen(title)-1] == '~') //caz autocomplete
            {
                title[strlen(title)-1] = '\0';
                searchBookAutocomplete(T1, title, g);
                continue;
            }
            searchBook(T1, title, g); //caz fara autocomplete
            continue;
        }

        if(strcmp(operation, "list_author") == 0)
        {
            char *author = strtok(NULL, ":\n");
            if(author[strlen(author)-1] == '~') //caz autocomplete
            {
                author[strlen(author)-1] = '\0';
                listAuthorAutocomplete(T2, author, g);
                continue;
            }
            listAuthor(T2, author, g); //caz fara autocomplete
            continue;
        }

        if(strcmp(operation, "search_by_author") == 0)
        {
            char *author = strtok(NULL, ":\n");
            if(author[strlen(author)-1] == '~') //caz autocomplete autor
            {
                author[strlen(author)-1] = '\0';
                listAuthorAutocomplete(T2, author, g);
                continue;
            }
            char *title = strtok(NULL, ":\n");
            if(title[strlen(title)-1] == '~') //caz autocomplete titlu
            {
                title[strlen(title)-1] = '\0';
                searchbyAuthorAutocomplete(T2, author, title, g);
                continue;
            }
            searchbyAuthor(T2, author, title, g); //caz fara autocomplete
            continue;
        }

        if(strcmp(operation, "delete_book") == 0)
        {
            char *title = strtok(NULL, ":\n");
            deleteBook(T1, T2, title, g);
            continue;
        }
    }

    //eliberare memorie
    free(line);
    DestrTrie(T1, freeBook);
    DestrTrie(T2, DestrTrie2);
    fclose(f);
    fclose(g);
    return 0;
}
       