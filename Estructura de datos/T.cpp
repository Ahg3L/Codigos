#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

double aleatorio(double min, double max){
	double numero = rand();
	return min + (max-min) * numero;
}

main(){
	
	double rangoMinimo = -1.0;
	double rangoMaximo = 1.0;
	
	for (int i = 0; i < 100; i++){
		double resultado = aleatorio(rangoMinimo, rangoMaximo);
		cout<<"Numero aleatorio de rango : "<<rangoMinimo<<" , "<<rangoMaximo<<" es : "<<resultado<<endl;
	}
	
	
	
	
}