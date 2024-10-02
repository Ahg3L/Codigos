#include <iostream>
#include <cmath>

using namespace std;

double calcularRaizCuadradaDe3(int iteraciones) {
    double resultado = 0.0;
    int factorial = 1;
	
    for (int i = 1; i <= iteraciones; ++i) {
    	factorial = factorial * i;
        resultado = resultado +(( ((2 * i) - 3) / (pow(8, i) * factorial))*(pow(-1,i)));
        
    }

    return resultado;
}

int main() {
    int iteraciones;
    cout << "Ingrese el número de iteraciones para calcular la raíz cuadrada de 3: ";
    cin >> iteraciones;

    double raizCuadradaDe3 = 2-2*calcularRaizCuadradaDe3(iteraciones);
    
    cout << "La aproximación de la raíz cuadrada de 3 con " << iteraciones << " iteraciones es: " << raizCuadradaDe3 << endl;

    return 0;
}
