#include <iostream>
#include <vector>

using namespace std;

vector<vector<double>> A = {{0, 1}, {1, 3}, {2, 0}};

double productivo(int x) {
    double resultado = 0;
    
    for (int i = 0; i < 3; i++) {
        double r = 1;
        double r2 = 1;
        
        for (int j = 0; j < 3; j++) {
            if (j == i) {
                continue;
            } else {
                r *= (x - A[j][0]);
            }
        }
        
        for (int j = 0; j < 3; j++) {
            if (j == i) {
                continue;
            } else {
                r2 *= (A[i][0] - A[j][0]);
            }
        }
        
        resultado += A[i][1] * (r / r2);
    }
    
    return resultado;
}

double funcion(int x) {
    return productivo(x);
}

int main() {
    for (int i = -5; i < 5; i++) {
        cout << funcion(i) << endl;
    }
    return 0;  // Agregar un valor de retorno para la función main()
}

