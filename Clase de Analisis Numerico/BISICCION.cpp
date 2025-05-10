#include <iostream>
#include <cmath>
using namespace std;

double funcion(double x) {
    return pow(cos(x), 2) - 2 * x + 6;
}

int main() {
    double a, b, c, fa, fb, fc, error = 1e-6;
    int i = 0;

    cout << "Ingresa el rango 1: ";
    cin >> a;
    cout << "Ingresa el rango 2: ";
    cin >> b;

    fa = funcion(a);
    fb = funcion(b);

    if (fa * fb > 0) {
        cout << "No se puede aplicar el método de bisección. f(a) y f(b) tienen el mismo signo." << endl;
        return 1;
    }

    cout << "\nIter\t a\t\t b\t\t c\t\t f(c)\t\t Error\n";
    do {
        c = (a + b) / 2;
        fc = funcion(c);

        cout << i << "\t" << a << "\t" << b << "\t" << c << "\t" << fc;
        if (i > 0) {
            cout << "\t" << fabs(c - (a + b) / 2);
        }
        cout << endl;

        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        i++;
    } while (fabs(fc) > error && i < 50);

    cout << "\nLa raíz aproximada es: " << c << endl;
    return 0;
}
