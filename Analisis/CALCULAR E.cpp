zz #include<iostream>
#include <cmath>

    using namespace std;

double calcularEuler(int iteraciones)
{
    double euler = 1.0;
    double factorial = 1.0;

    for (int i = 1; i <= iteraciones; ++i)
    {
        factorial *= i;
        euler += 1.0 / factorial;
    }

    return euler;
}

int main()
{
    int iteraciones;
    cout << "Ingrese el numero de iteraciones para calcular e: ";
    cin >> iteraciones;

    double e = calcularEuler(iteraciones);

    cout << "El número de Euler (e) calculado con " << iteraciones << " iteraciones es: " << e;
    return 0;
}
