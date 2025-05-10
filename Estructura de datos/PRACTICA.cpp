#include <iostream>
#include <cstdlib>  
using namespace std;

struct nodo {
    int dato;
    struct nodo* siguiente;
    struct nodo* anterior;
};

void insertar(nodo *&cola, nodo *&pila, int n) {
    int ops;
    cout << "En donde quieres ingresar el dato ?" << endl;
    cout << "1.-Pila" << endl;
    cout << "2.-Cola" << endl;
    cin>> ops;
    if (ops ==1){
        nodo* new_nodo_cola = new nodo();
        new_nodo_cola->dato = n;
        nodo* aux1_cola = cola;
        nodo* aux2_cola = nullptr;

        while (aux1_cola != nullptr) {
            aux2_cola = aux1_cola;
            aux1_cola = aux1_cola->siguiente;
        }

        if (cola == aux1_cola) {
            cola = new_nodo_cola;
        } else {
            aux2_cola->siguiente = new_nodo_cola;
        }

        new_nodo_cola->siguiente = aux1_cola;
        cout << endl << "Dato ingresado correctamente en la cola : )" << endl;
    }

    if(ops ==2){
        nodo* new_nodo_pila = new nodo();
        new_nodo_pila->dato = n;
        new_nodo_pila->siguiente = pila;
        pila = new_nodo_pila;
        cout << endl << "Dato ingresado correctamente en la pila : )" << endl;
    }else{
        cout<<"Opcion invalida :( ";
    }
}


void mostrar(nodo* cola) {
    nodo* actual = cola;
    while (actual != nullptr) {
        cout << actual->dato << " - ";
        actual = actual->siguiente;
    }
    cout << endl;
}

void eliminar(nodo*& cola, int n) {
    if (cola != nullptr) {
        nodo* borrar;
        nodo* anterior = nullptr;
        borrar = cola;

        while ((borrar != nullptr) && (borrar->dato != n)) {
            anterior = borrar;
            borrar = borrar->siguiente;
        }

        if (borrar == nullptr) {
            cout << "\nEl elemento no se encuentra " << endl;
        } else if (anterior == nullptr) {
            cola = cola->siguiente;
            delete borrar;  
        } else {
            anterior->siguiente = borrar->siguiente;
            delete borrar;  
        }
    }
}

void buscar(nodo* cola, nodo* pila, int n) {
    int ops;
    cout << "En donde quieres buscar \n1.-Pila.\n2.-Cola.";
    cin >> ops;

    switch (ops) {
    case 1:
       
        bool variable_pila = false;
        int contador_pila = 1;
        int aux_pila = 0;
        nodo* actual_pila = pila;

        while (actual_pila != nullptr) {
            if (actual_pila->dato == n) {
                variable_pila = true;
                aux_pila = contador_pila;
                break;
            }
            actual_pila = actual_pila->siguiente;
            contador_pila++;
        }

        if (variable_pila) {
            cout << "\nEl elemento " << n << " se encuentra en la pila, en la posicion " << aux_pila << endl;
            system("pause");
        } else {
            cout << "\nEl elemento " << n << " no se encuentra en la pila : ( " << endl;
            system("pause");
        }
        
    break

    case 2:
        bool variable_cola = false;
        int contador_cola = 1;
        int aux_cola = 0;
        nodo* actual_cola = cola;

        while (actual_cola != nullptr) {
            if (actual_cola->dato == n) {
                variable_cola = true;
                aux_cola = contador_cola;
                break;
            }
            actual_cola = actual_cola->siguiente;
            contador_cola++;
        }

        if (variable_cola) {
            cout << "\nEl elemento " << n << " se encuentra en la cola, en la posicion " << aux_cola << endl;
            system("pause");
        } else {
            cout << "\nEl elemento " << n << " no se encuentra en la cola : ( " << endl;
            system("pause");
        }
    break;

    default:
    break;
    }
}


void menu(nodo*  cola, nodo*pila) {
    int ops, dato, num;
    do {
        system("cls");
        cout << "\tMenu \n";
        cout<<"\nPILA : "; mostrar(cola);
        cout<<"\nCOLA : "; mostrar(pila);
        cout<<endl;
        cout << "1.- Ingresar dato." << endl;
        cout << "2.- Buscar numero." << endl;
        cout << "3.- Eliminar numero." << endl;
        cout << "4.- Salir." << endl;
        cin >> ops;
        switch (ops) {
            case 1:
                cout << "Ingresa un numero :";
                cin >> dato;
                insertar(cola, pila, dato);
                break;
            case 2:
                cout << "Ingresa el numero que quieres buscar : ";
                cin >> num;
                buscar(cola, num);
                break;
            case 3:
                cout << "Ingresa el numero que quieres eliminar  : ";
                cin >> num;
                eliminar(cola, num);
                break;
            default:
                break;
        }
    } while (ops != 4);
}

int main() {
    nodo *cola = nullptr;
    nodo *pila = nullptr;
    menu(cola,pila);
    return 0;
}






















