#include <iostream>
#include <vector>

using namespace std;

vector<vector<double>> A = {{0, 1}, {1, 3}, {2, 0}}; // Puntos (x, y)

double interpolarLagrange(double x)
{
    double resultado = 0;
    int n = A.size();

    for (int i = 0; i < n; i++)
    {
        double numerador = 1.0;
        double denominador = 1.0;

        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                numerador *= (x - A[j][0]);
                denominador *= (A[i][0] - A[j][0]);
            }
        }

        resultado += A[i][1] * (numerador / denominador);
    }

    return resultado;
}

int main()
{
    for (int i = -5; i <= 5; i++)
    {
        cout << "f(" << i << ") = " << interpolarLagrange(i) << endl;
    }

    return 0;
}
