#include <iostream>
#include <cmath>

using namespace std;

double calcularRaizCuadradaDe2(int iteraciones) {
    double resultado = 0.0;
    int factorial = 1;
	
    for (int i = 1; i <= iteraciones; ++i) {
    	factorial = factorial * i;
        resultado = resultado +(2*( ((2 * i) - 3) / (pow(4, i) * factorial)));
        
    }

    return resultado;
}

int main() {
    int iteraciones;
    cout << "Ingrese el número de iteraciones para calcular la raíz cuadrada de 2: ";
    cin >> iteraciones;

    double raizCuadradaDe2 = 1-calcularRaizCuadradaDe2(iteraciones);
    
    cout << "La aproximación de la raíz cuadrada de 2 con " << iteraciones << " iteraciones es: " << raizCuadradaDe2 << endl;

    return 0;
}
