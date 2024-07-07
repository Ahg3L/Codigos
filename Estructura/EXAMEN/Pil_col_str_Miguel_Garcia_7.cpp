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

void eliminar(nodo*& cola, nodo*& pila, int n) {
    int ops;
    cout << "Donde quieres eliminar el dato?\n1.-Pila.\n2.-Cola.\n"; 
    cin >> ops;

    switch (ops) {
    case 1:
        
        if (cola != nullptr) {
            nodo* borrar = cola;
            nodo* anterior = nullptr;

            while ((borrar != nullptr) && (borrar->dato != n)) {
                anterior = borrar;
                borrar = borrar->siguiente;
            }

            if (borrar == nullptr) {
                cout << "\nEl elemento no se encuentra en la cola." << endl;
            } else if (anterior == nullptr) {
                cola = cola->siguiente;
                delete borrar;
                cout << "\nElemento eliminado de la cola correctamente." << endl;
            } else {
                anterior->siguiente = borrar->siguiente;
                delete borrar;
                cout << "\nElemento eliminado de la cola correctamente." << endl;
            }
        } else {
            cout << "\nLa cola está vacía." << endl;
        }
        break;

    case 2:
        
        if (pila != nullptr) {
            nodo* borrar = pila;
            pila = pila->siguiente;
            delete borrar;
            cout << "\nElemento eliminado de la pila correctamente." << endl;
        } else {
            cout << "\nLa pila está vacía." << endl;
        }
        break;

    default:
        cout << "\nOpción no válida." << endl;
        break;
    }
}

void buscar(nodo* cola, nodo* pila, int n) {
    int ops = 0;
    cout << "En donde quieres buscar \n1.-Pila.\n2.-Cola.\n";
    cin >> ops;

    if(ops == 1){
         bool variable_pila = false;
        int contador_pila = 1;
        int aux_pila = 0;
        nodo* actual_pila = pila;

        while(actual_pila != nullptr) {
            if (actual_pila->dato == n) {
                variable_pila = true;
                aux_pila = contador_pila;
        
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
       
    }if( ops == 2){   
    
        bool variable_cola = false;
        int contador_cola = 1;
        int aux_cola = 0;
        nodo* actual_cola = cola;

        while(actual_cola != nullptr) {
            if(actual_cola->dato == n) {
                variable_cola = true;
                aux_cola = contador_cola;
                break;
            }
            actual_cola = actual_cola->siguiente;
            contador_cola++;
        }

        if(variable_cola){
            cout << "\nEl elemento " << n << " se encuentra en la cola, en la posicion " << aux_cola << endl;
            system("pause");
        } else{
            cout << "\nEl elemento " << n << " no se encuentra en la cola : ( " << endl;
            system("pause");
        }
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
        cout<<"1.- Ingresar dato." << endl;
        cout<<"2.- Buscar numero." << endl;
        cout<<"3.- Eliminar numero." << endl;
        cout<<"4.- Salir." << endl;
        cin >>ops;
        switch (ops) {
            case 1:
                cout<<"Ingresa un numero :";
                cin>>dato;
                insertar(cola, pila, dato);
                break;
            case 2:
                cout<<"Ingresa el numero que quieres buscar : ";
                cin>>num;
                buscar(cola,pila, num);
                break;
            case 3:
                cout<<"Ingresa el numero que quieres eliminar  : ";
                cin >> num;
                eliminar(cola,pila, num);
                break;
            default:
                break;
        }
    } while (ops !=4);
}

int main() {
    nodo *cola = nullptr;
    nodo *pila = nullptr;
    menu(cola,pila);
    return 0;
}

