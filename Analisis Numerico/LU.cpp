#include <iostream>
#include <vector>

using namespace std;

void LU(vector<vector<double>> &A, vector<vector<double>> &L, vector<vector<double>> &U)
{
    int n = A.size();
    L = vector<vector<double>>(n, vector<double>(n, 0.0));
    U = A;

    for (int i = 0; i < n; i++)
    {
        L[i][i] = 1.0;
        for (int j = i + 1; j < n; j++)
        {
            double factor = U[j][i] / U[i][i];
            L[j][i] = factor;
            for (int k = i; k < n; k++)
            {
                U[j][k] -= factor * U[i][k];
            }
        }
    }
}

vector<double> resolverLU(vector<vector<double>> &L, vector<vector<double>> &U, vector<double> &b)
{
    int n = b.size();
    vector<double> y(n), x(n);

    // Sustitución hacia adelante: Ly = b
    for (int i = 0; i < n; i++)
    {
        y[i] = b[i];
        for (int j = 0; j < i; j++)
        {
            y[i] -= L[i][j] * y[j];
        }
        // L[i][i] es 1, así que no se divide
    }

    // Sustitución hacia atrás: Ux = y
    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++)
        {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }

    return x;
}

int main()
{
    vector<vector<double>> A = {{1, -1, 3}, {1, 1, 1}, {2, 2, -1}};
    vector<double> b = {13, 11, 7};

    vector<vector<double>> L, U;
    LU(A, L, U);

    vector<double> x = resolverLU(L, U, b);

    int n = A.size();

    cout << "\nMatriz L:\n";
    for (auto &fila : L)
    {
        for (auto &val : fila)
            cout << val << "\t";
        cout << endl;
    }

    cout << "\nMatriz U:\n";
    for (auto &fila : U)
    {
        for (auto &val : fila)
            cout << val << "\t";
        cout << endl;
    }

    cout << "\nSolución del sistema:\n";
    for (int i = 0; i < n; i++)
    {
        cout << "x" << i + 1 << " = " << x[i] << endl;
    }

    return 0;
}
