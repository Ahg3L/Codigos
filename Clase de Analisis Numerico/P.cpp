#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

void imprimirMatriz(const vector<vector<double>> &matriz)
{
    for (const auto &fila : matriz)
    {
        for (double elemento : fila)
        {
            cout << elemento << " ";
        }
        cout << endl;
    }
}

void ingresarDatosMatriz(vector<vector<double>> &matriz, vector<double> &R)
{
    system("cls");
    cout << "Matriz actual:" << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << " =  " << R[i] << endl;
    }
    cout << "Ingresa los datos de la matriz en orden :" << endl;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            cout << "[" << i << "]"
                 << "[" << j << "] = ";
            cin >> matriz[i][j];
        }
    }
    cout << "Ahora ingresa los resultados : " << endl;
    for (int i = 0; i < 3; i++)
    {
        cin >> R[i];
    }
}

void LU(vector<vector<double>> &A, vector<double> &R)
{
    vector<vector<double>> L, U;
    int n = A.size();
    float x, y, z = 0;

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

    x = R[0];
    y = (R[1] - L[1][0] * x);
    z = R[2] - (L[2][0] * (x)) - (L[2][1] * y);

    R[2] = z / U[2][2];
    R[1] = (y - (U[1][2] * R[2])) / U[1][1];
    R[0] = (x - (U[0][2] * R[2]) - (U[0][1] * R[1])) / U[0][0];

    cout << "x = " << R[0] << endl;
    cout << "y = " << R[1] << endl;
    cout << "z = " << R[2] << endl;
}

vector<vector<double>> matrizAdjunta(const vector<vector<double>> &matriz)
{
    size_t n = matriz.size();
    vector<vector<double>> adjunta(n, vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            vector<vector<double>> cofactor(n - 1, vector<double>(n - 1, 0.0));
            for (size_t k = 0; k < n; ++k)
            {
                if (k == i)
                    continue;
                for (size_t l = 0; l < n; ++l)
                {
                    if (l == j)
                        continue;
                    cofactor[k < i ? k : k - 1][l < j ? l : l - 1] = matriz[k][l];
                }
            }
            double signo = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            double determinanteCofactor = 1.0;
            for (size_t k = 0; k < n - 1; ++k)
            {
                determinanteCofactor *= cofactor[k][k];
            }
            adjunta[i][j] = signo * determinanteCofactor;
        }
    }

    return adjunta;
}

vector<vector<double>> matrizInversa(const vector<vector<double>> &matriz)
{
    size_t n = matriz.size();

    if (n != matriz[0].size())
    {
        cerr << "La matriz no es cuadrada. No se puede calcular la inversa." << endl;
        return {};
    }
    double determinante = 1.0;
    for (size_t i = 0; i < n; ++i)
    {
        determinante *= matriz[i][i];
    }

    if (determinante == 0.0)
    {
        cerr << "La matriz es singular. No se puede calcular la inversa." << endl;
        return {};
    }

    vector<vector<double>> adjunta = matrizAdjunta(matriz);
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            adjunta[i][j] /= determinante;
        }
    }
    return adjunta;
}

bool menuMatx(vector<vector<double>> &matriz, vector<double> &R)
{
    vector<vector<double>> inversa = matrizInversa(matriz);
    while (true)
    {
        system("cls");
        cout << "Ingresa una opcion\n\n";
        cout << "1.-Ingresar datos" << endl;
        cout << "2.-Resolver sistema de la matriz" << endl;
        cout << "3.-Matriz inversa" << endl;
        cout << "4.-Salir" << endl;
        char opc;
        cin >> opc;
        switch (opc)
        {
        case '1':
            ingresarDatosMatriz(matriz, R);

        case '2':
            LU(matriz, R);
            system("pause");
            break;
        case '3':
            cout << "Matriz Inversa:" << endl;
            imprimirMatriz(inversa);
            system("pause");
            break;
        case '4':
            system("cls");
            cout << "Adios :) ";
            system("pause");
            return false;
        default:
            cout << "Opcion no valida ";
            system("pause");
            break;
        }
    }
}

void menu()
{
    vector<vector<double>> matriz = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<double> R = {1, 2, 3};

    while (true)
    {
        system("cls");
        cout << "\nMenu:" << endl;
        cout << "1. Matrices" << endl;
        cout << "2. Interpolacion y aproximacion" << endl;
        cout << "3. Salir" << endl;

        cout << "Selecciona una opcion: ";
        int opcion;
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            menuMatx(matriz, R);
            break;
        case 2:
            int n;
            cout << "¿Cuantos datos vas a ingresar ?";
            cin >> n;
            int equis[n];
            int ye[n];
            for (int i = 0; i < n; i++)
            {
                cout << "Dato x [" << i << "]"
                     << " = ";
                cin >> equis[i];
                cout << "Dato y [" << i << "]"
                     << " = ";
                cin >> ye[i];
            }
            break;
        }
    }
}

int main()
{
    menu();
    return 0;
}
