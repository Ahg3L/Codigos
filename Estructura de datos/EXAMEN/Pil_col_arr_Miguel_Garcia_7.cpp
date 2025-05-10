#include <iostream>
using namespace std;

int pila[15] = {0};
int cola[15] = {0};
int contaP = 0;
int contaC = 0;

void elimina(){
    int opcionEliminar = 0;

    cout<<"Eliminar de:" << endl;
    cout<<"1. Pila" << endl;
    cout<< "2. Cola" << endl;
    cout<< "Elija una opcion: ";
    cin>> opcionEliminar;

    switch (opcionEliminar){
    case 1:
        if (contaP>0){
            pila[contaP -1] = 0; 
            contaP--;
        }
        else{
            cout << "La pila está vacía." << endl;
        }
        break;

    case 2:
        if(contaC>0){
            for (int i = 0; i <=contaC; i++){
                cola[i] = cola[i+1];
            }
            contaC--;
        }else{
            cout<<"La cola está vacía." << endl;
        }
        break;

    default:
        cout<<"Opción no válida." << endl;
        break;
    }
}


void anadir(){
    int opsa = 0;
    int valor = 0;
    cout<<"Pila o cola ?" << endl;
    cout<<"1.- Pila" << endl;
    cout<<"2.- Cola" << endl;
    cout<<"Ingresa una opcion : ";
    cin>>opsa;
    switch(opsa){
    case 1:
        cout<< endl<<"Ingrese un valor: : ";
        cin>>valor;
        pila[contaP] =valor;
        contaP++;
    break;
    case 2:
        cout <<endl<<"Ingrese un valor: : ";
        cin>>valor;
        cola[contaC] = valor;
        contaC++;
    break;

    default:
        cout<<"Opción no válida." << endl;
    break;
    }
}

int menu(){
    while (true){
        int opcion;
        system("cls");
        cout<<"  Pila  :  ";
        for (int i=0;i<=14; i++){
            cout<<pila[i]<<" | ";
        }
        cout<<endl<< "  Cola  :  ";
        for(int j = 0; j<=14; j++){
            cout<<cola[j] << " | ";
        }
        cout<<endl<<endl<<"Elija una opcion."<<endl<<endl;
        cout<<"1. Anadir un elemento."<<endl;
        cout<<"2. Eliminar"<<endl;
        cout<<"3. Salir" <<endl;
        cin>>opcion;

        switch (opcion){
            case 1:
                anadir();
            break;

            case 2:
                elimina();
            break;

            case 3:
                return 0;
            break;

            default:
                cout << "Opción no válida." << endl;
            break;
        }
    }
    return 0;
}

int main(){
    menu();
    return 0;
}
