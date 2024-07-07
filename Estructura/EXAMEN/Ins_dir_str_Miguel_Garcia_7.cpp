#include <iostream>
using namespace std;

struct nodo {
    int dato;
    struct nodo* siguiente;
};

void insertar(nodo *&nuevo_num) {
    int n;
    cout << "Escribe un numero :"; cin>>n;
    nodo* nuevo=new nodo();
    nuevo->dato=n;
    nodo* aux1_nuevo=nuevo_num;
    nodo* aux2_nuevo =NULL;

    while(aux1_nuevo != NULL){
        aux2_nuevo=aux1_nuevo;
        aux1_nuevo=aux1_nuevo->siguiente;
    }
    if(nuevo_num==aux1_nuevo){
        nuevo_num=nuevo;
    }else{
        aux2_nuevo->siguiente=nuevo;
    }
    nuevo->siguiente=aux1_nuevo;
}

void ordenar(nodo *&nuevo_num) {
    if (nuevo_num==NULL || nuevo_num->siguiente==NULL) {
        return; 
    }
    nodo* aux1=NULL;
    nodo* aux2=nuevo_num;
    while (aux2 != NULL) {
        nodo* next=aux2->siguiente;

        if (aux1==NULL || aux1->dato>=aux2->dato) {
            aux2->siguiente=aux1;
            aux1=aux2;
        }else{
            nodo* temp=aux1;
            while (temp->siguiente != NULL && temp->siguiente->dato<aux2->dato){
                temp=temp->siguiente;
            }
            aux2->siguiente=temp->siguiente;
            temp->siguiente=aux2;
        }
        aux2=next;
    }
    nuevo_num =aux1;
}

void mostrar(nodo* nuevo){
    nodo* actual=nuevo;
    while (actual !=NULL){
        cout<<actual->dato<<" - ";
        actual =actual->siguiente;
    }
    cout<<endl;
}

int main() {
    nodo *nuevo_num = NULL;
    int ops;
    while (true) {
        system("cls");
        mostrar(nuevo_num);
        cout<<"\n\nQue quieres hacer ? "<<endl;
        cout<<"1. Ingresar nuevo numero."<<endl;
        cout<<"2. Ordenar. "<<endl;
        cin>>ops;
        switch(ops){
            case 1:
                insertar(nuevo_num);
                break;
            case 2:
                ordenar(nuevo_num);
                break;
            default:
                break;
        }
    }
    return 0;
}
