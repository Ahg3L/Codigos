#include <iostream>
using namespace std;

struct lista {
    int n;
};

int main() {
    int num;
    system("cls");
    cout<<"Cuantos datos quieres? : "; cin>>num;
    lista numeros[num];

    cout << "Escribe un numero :  " <<endl;
    for (int i = 0; i < num; ++i) {
        int numero;
        cin>>numero;
        numeros[i].n = numero; 
    }

    cout<<"\nLa lista es  : ";
    for(int i = 0; i<num; i++){
        cout<<numeros[i].n;
    }

    return 0;
}
