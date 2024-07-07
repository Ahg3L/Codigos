#include <iostream>
#include <stdio.h>
#include <stdlib.h>

struct nodo{
    int dato;
    struct nodo*siguente;
};
main(){
    struct nodo* nodo1 = (struct nodo*)malloc(sizeof(struct nodo));
    struct nodo* nodo2 = (struct nodo*)malloc(sizeof(struct nodo));
    struct nodo* nodo3 = (struct nodo*)malloc(sizeof(struct nodo));

    nodo1->dato = 1;
    nodo2->dato = 2;
    nodo3->dato = 3;

    nodo1->siguente = nodo2;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    nodo2->siguente = nodo3;
    nodo3->siguente = NULL;

    struct nodo*actual = nodo1;
    
    while(actual != NULL){
        printf("%d", actual->dato);
        actual = actual ->siguente;
    }
    free(nodo1);
    free(nodo2);
    free(nodo3);
}