#include <iostream>
#include <cmath>

using namespace std;

double calcularPi(int iteraciones) {
    double pi = 0.0;
    double sign = 1.0;

    for (int i = 0; i < iteraciones; ++i) {
        double term = 1.0 / (2 * i + 1);
        pi += sign * term;
        sign = -sign;
    }

    return 4.0 * pi;
}

int main() {
    int iteraciones;
    cout << "Ingrese el numero de iteraciones para calcular pi: ";
    cin >> iteraciones;

    double pi = calcularPi(iteraciones);

    cout << "La aproximacion de pi calculada con " << iteraciones << " iteraciones es: " << pi << endl;

    return 0;
}
