#include <iostream>
using namespace std;

struct nodo {
    int dato;
    struct nodo*siguente;
    struct nodo*anterior;
};

void imprimirAscendente(struct nodo* cabeza){
    struct nodo* actual = cabeza;
    while (actual != NULL){
        cout<<actual->dato;
        actual = actual->siguente;
    }
    cout<<endl;
} 
    
void imprimirDescendente(struct nodo* rabo){
    struct nodo* actual = rabo;
    while (actual != NULL){
        cout<<actual->dato;
        actual = actual->anterior;
    }
    cout<<endl;
} 

main(){
    struct nodo* nodo1 = (struct  nodo*)malloc(sizeof(struct nodo));
    struct nodo* nodo2 = (struct  nodo*)malloc(sizeof(struct nodo));
    struct nodo* nodo3 = (struct  nodo*)malloc(sizeof(struct nodo));

    nodo1->dato = 1;
    nodo2->dato = 2;
    nodo3->dato = 3; 

    nodo1 ->anterior = NULL;
    nodo1 ->siguente = nodo2;
    nodo2 ->anterior = nodo1;
    nodo2 ->siguente = nodo3;
    nodo3 ->anterior = nodo2;
    nodo3 ->siguente = NULL;
    cout<<"Lista en orden Ascendente : ";
    imprimirAscendente(nodo1);
    cout<<"Lista en orden Descendente : ";
    imprimirDescendente(nodo3);

    
    free (nodo1);
    free (nodo2);
    free (nodo3);


}