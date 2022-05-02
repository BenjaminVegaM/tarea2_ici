#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"

typedef struct{
    char title[50];
    int year;
}Book;

Book* createBook(char* title, int year){
    Book* b = (Book*) malloc (sizeof(Book));
    strcpy(b->title,title);
    b->year=year;
    return b;
}

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo string
  retorna 1 si son key1<key2
*/
int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son iguales
*/
int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son key1<key2
*/
int lower_than_int(void * key1, void * key2) {
    if(*(int*)key1 < *(int*)key2) return 1;
    return 0;
}

int main(int argc, const char * argv[]) {

    //mapa con clave int
    Map * books_by_year = createMap(is_equal_int);

    //mapa con clave string
    Map * books_by_title = createMap(is_equal_string);
    setSortFunction(books_by_title,lower_than_string);

    Book* b = createBook("The Lion, the Witch and the Wardrobe",1950);
    insertMap(books_by_title, b->title, b);
    insertMap(books_by_year, &b->year, b);

    b = createBook("Ender's Game",1985);
    insertMap(books_by_title, b->title, b);
    insertMap(books_by_year, &b->year, b);

    b = createBook("The Tibetan Book of Living and Dying",1992);
    insertMap(books_by_title, b->title, b);
    insertMap(books_by_year, &b->year, b);

    b = createBook("Solaris",1961);
    insertMap(books_by_title, b->title, b);
    insertMap(books_by_year, &b->year, b);


    printf("Libros ordeandos por nombre:\n");
    b = firstMap(books_by_title);
    while (b) {
        printf("%s (%d)\n", b->title, b->year);
        b = nextMap(books_by_title);
    }

    int year=1950;
    printf("\nBuscando libro del anno %d:\n",year);
    b = searchMap(books_by_year,&year);
    if(b)
        printf("%s (%d)\n", b->title, b->year);
    else 
        printf("No existe el libro\n");


    return 0;
}
