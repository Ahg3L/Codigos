#include <iostream>
#include <cmath>

using namespace std;

double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

double calcularRaizDe2(int iteraciones) {
    double resultado = 0.0;
    for (int n = 0; n < iteraciones; ++n) {
      double numerador = pow(-1, n) * tgamma(2 * n + 1); 
      double denominador = (1 - 2 * n) * pow(factorial(n), 2) * pow(4, n);
        resultado += numerador / denominador;
    }

    return resultado;
}

int main() {
    int iteraciones;
    cout << "Ingrese el número de iteraciones para calcular la raíz cuadrada de 2: ";
    cin >> iteraciones;

    double raizCuadradaDe2 = calcularRaizDe2(iteraciones);

    cout << "La aproximación de la raíz cuadrada de 2 con " << iteraciones << " iteraciones es: " << raizCuadradaDe2 << endl;

    return 0;
}
