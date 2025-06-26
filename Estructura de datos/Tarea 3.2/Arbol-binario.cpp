#include "Libreria.h"

struct nodo{
    int dato;
    nodo *der;
    nodo *izq;
};

nodo *crear(int n){
    nodo *nuevo = new nodo();
    nuevo->dato =n;
    nuevo->der =NULL;
    nuevo->izq =NULL;
    return nuevo;
}

nodo *arbol = NULL;
void insertar(nodo *&arbol, int n){
    if (arbol == NULL){
        nodo *nuevo = crear(n);
        arbol = nuevo;
    } else{
        int valorR = arbol->dato;
        if (n<valorR){
            insertar(arbol->izq, n);
        } else{
            insertar(arbol->der, n);
        }
    }
}

void mostrar(nodo *arbol, int cont){
    if(arbol==NULL){
        return;
    }else{
        mostrar(arbol->der, cont + 1);
        for (int i = 0; i < cont; i++) {
            cout<<"  ";
        }
        cout<<arbol->dato <<endl;
        mostrar(arbol->izq, cont + 1);
    }
}

int menu(){
    int cont = 0;
    system("cls");
    cout<<"\tMENU"<<endl;
    cout<<"1.-Ingresar un nodo."<<endl;
    cout<<"2.-Mostrar." <<endl;
    cout<<"3.-Salir. "<<endl;
    
    int opc = ingresar(3);
    switch (opc){
        case 1:
            system("cls");
            cout<<"Ingresa el dato que quieres ingresar : ";
            insertar(arbol, ingresar(1000));
            system("pause");
            return menu();
        case 2:
            system("cls");
            mostrar(arbol, cont);
            system("pause");
            break;
        case 3:
            return 0;   
    }
}

int main() {
    menu();
    return 0;
}
