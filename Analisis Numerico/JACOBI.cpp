#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<vector<double>> A = {
        {4, -1, 2},
        {2, 5, 1},
        {3, 2, 8}
    };
    vector<double> R = {7, 5, 10};

    double semilla[3] = {0};     // Valores anteriores
    double nueva[3] = {0};       // Nuevos valores

    cout << "\n\tIteraciones - Método de Jacobi\n\n";

    for (int iter = 0; iter < 5; iter++) {
        nueva[0] = (R[0] - A[0][1] * semilla[1] - A[0][2] * semilla[2]) / A[0][0];
        nueva[1] = (R[1] - A[1][0] * semilla[0] - A[1][2] * semilla[2]) / A[1][1];
        nueva[2] = (R[2] - A[2][0] * semilla[0] - A[2][1] * semilla[1]) / A[2][2];

        cout << "Iteración " << iter + 1 
             << " | x = " << nueva[0]
             << " | y = " << nueva[1]
             << " | z = " << nueva[2] << endl;

        // Actualiza los valores para la siguiente iteración
        for (int i = 0; i < 3; i++) {
            semilla[i] = nueva[i];
        }
    }

    return 0;
}
