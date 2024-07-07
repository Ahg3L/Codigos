#include <iostream>
using namespace std;

struct nodo{
    int dato;
    struct nodo* siguiente;
    struct nodo* anterior;
};

void insertar(nodo *&nuevo_num) {
    int n;
    cout<<"Escribe un numero :"; 
    cin>>n;

    nodo* nuevo=new nodo();
    nuevo->dato=n;
    nuevo->siguiente=NULL; 

    if(nuevo_num ==NULL){
        nuevo_num=nuevo;  
    }else{
        nodo* aux1_nuevo=nuevo_num;
        while(aux1_nuevo->siguiente != NULL) {
            aux1_nuevo=aux1_nuevo->siguiente;
        }
        aux1_nuevo->siguiente=nuevo;  
        nuevo->anterior =aux1_nuevo;   
    }
}

void mostrar(struct nodo* cabeza){
    cout<<"\nCodigo ascendente : ";
    struct nodo* actual=cabeza;
    while (actual !=NULL){
        cout<<actual->dato;
        actual =actual->siguiente;
    }  
}

void mostrarR(struct nodo* cabeza){
    cout<<"\nCodigo descendent : ";
    struct nodo* actual=cabeza;
    while(actual->siguiente != NULL) {
        actual=actual->siguiente;
    }

    while (actual != NULL) {
        cout<<actual->dato;
        actual=actual->anterior;
    }
    cout<<endl;
    system("pause");
}

int main() {
    nodo *nuevo_num = NULL;
    int ops;
    while (true) {
        system("cls");
        cout<<"\n\nQue quieres hacer ? "<<endl;
        cout<<"1. Ingresar nuevo numero."<<endl;
        cout<<"2. Ver. "<<endl;
        cin>>ops;
        switch(ops){
            case 1:
                insertar(nuevo_num);
                break;
            case 2:
                mostrar(nuevo_num);
                mostrarR(nuevo_num);
                break;
            default:
                break;
        }
    }
    return 0;
}