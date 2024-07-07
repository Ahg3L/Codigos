#include <iostream>
using namespace std;

void ascendente( int tam, int a[]){
    for(int i=0; i<tam;i++){
        int pos = i; 
        int aux = a[i];
        while (pos>0 && (a[pos-1]>aux)){
            a[pos] = a[pos-1];
            pos--;
        }
        a[pos] = aux;
    } 
}

void imprimir(int tam, int a[]){
    cout<<"\nLista ordenada : ";
    for(int i=0; i<tam; i++){
        cout<<a[i];
    }
}

int main(){
    int tamano;
    system("cls");
    cout <<"Cuantos datos quieres tener en tu lista ?"<<endl;cin>>tamano;
    int numeros[tamano];
    cout<<"Escribe los datos : \n";    
    for(int i= 0; i<tamano; i++){
        cin>>numeros[i];
    }
    ascendente(tamano, numeros);
    imprimir(tamano,numeros);
    return 0;
}
