#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // Semilla para la generación de números aleatorios
    srand(time(0));

    // Solicitar al usuario el rango
    double rangoInicio, rangoFin;
    cout << "Introduce el rango inicial: ";
    cin >> rangoInicio;
    cout << "Introduce el rango final: ";
    cin >> rangoFin;

    // Verificar que el rango sea válido
    if (rangoInicio >= rangoFin) {
        cout << "Error: El rango inicial debe ser menor que el rango final." << endl;
        return 1; // Salir del programa con código de error
    }

    // Generar números aleatorios en el rango especificado
    for (int i = 0; i < 10; ++i) {
        // Genera un número aleatorio en el rango [0, 1]
        double random_number = rand() / static_cast<double>(RAND_MAX);

        // Escala y desplaza el número al rango especificado
        double scaled_number = random_number * (rangoFin - rangoInicio) + rangoInicio;

        cout << scaled_number << " ";
    }

    cout << endl;

    return 0;
}




