#include <iostream>
#include <cmath>
using namespace std;

double calcularRaizCuadradaDe3(int iteraciones) {
    double x = 1.0; // valor inicial
    for (int i = 0; i < iteraciones; ++i) {
        x = 0.5 * (x + 3 / x);
    }
    return x;
}

int main() {
    int iteraciones;
    cout << "Ingrese el numero de iteraciones para calcular la raiz cuadrada de 3: ";
    cin >> iteraciones;

    double raizCuadradaDe3 = calcularRaizCuadradaDe3(iteraciones);
    cout << "Aproximacion de la raíz cuadrada de 3 con " << iteraciones << " iteraciones: " << raizCuadradaDe3 << endl;
    cout << "Valor real con sqrt(3): " << sqrt(3.0) << endl;

    return 0;
}
