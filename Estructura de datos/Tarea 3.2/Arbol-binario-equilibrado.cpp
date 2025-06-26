#include "Libreria.h"

struct nodo{
    int dato;
    nodo *der;
    nodo *izq;
};

nodo *crear(int n){
    nodo *nuevo=new nodo();
    nuevo->dato= n;
    nuevo->der = NULL;
    nuevo->izq= NULL;
    return nuevo;
}

nodo *arbol = NULL;

void insertar(nodo *&arbol, int n){
    if(arbol == NULL){
        nodo *nuevo =crear(n);
        arbol= nuevo;
    }else{
        int valorR = arbol->dato;
        if (n<valorR) {
            insertar(arbol->izq, n);
        }else{
            insertar(arbol->der, n);
        }
    }
}

void mostrar(nodo *arbol, int cont){
    if(arbol == NULL){
        return;
    }else{
        mostrar(arbol->der, cont + 1);
        for (int i = 0; i < cont; i++){
            cout<<"  ";
        }
        cout<<arbol->dato<<endl;
        mostrar(arbol->izq,cont + 1);
    }
}


int main(){
    
    system("cls");
    int cont = 0;

    insertar(arbol, 5);
    insertar(arbol, 3);
    insertar(arbol, 1);
    insertar(arbol, 4);
    insertar(arbol, 7);
    insertar(arbol, 6);
    insertar(arbol, 9);
    mostrar (arbol, cont);
    return 0;
}
