#ifndef LIBRERIA_H
#define LIBRERIA_H

#include <iostream>
#include <sstream>

using namespace std;

int ingresar(int t){
    string num;
    int numero;
    cout<<"  ";
    cin>>num;
    istringstream n(num);
    if (n >> numero&&numero < t&&numero >0){
        return numero;
    }else{
        cout<<"Error: Debes ingresar una opción válida."<<endl;
        return ingresar(t);
    }
}
#endif 
