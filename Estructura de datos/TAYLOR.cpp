#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double calcularRaizCuadrada(double x, int n) {
    
	double resultado = 1;
    
	for (int i = 0; i < n; i++) {
        resultado = resultado + (x - 1.0) / (2.0 * resultado);
    }
	return resultado;
}

int main(){
	
	double raiz = 0;
	double intervalos = 0;
	
	cout<<"Escribe el numero del que quieres su raiz :  "<<endl; 
    cin>>raiz;
	cout<<"Con cuantos intervalos¿? "<<endl; 
	cin>>intervalos;
	
    double raizCuadradaDe2 = calcularRaizCuadrada(raiz, intervalos); 
    
	cout << "Aproximación de la raíz cuadrada de 2: " <<setprecision(6)<<raizCuadradaDe2 <<endl;
    
	return 0;

}