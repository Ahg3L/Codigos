#include <iostream>
#include <cstdio>
#include <vector>
#include <math.h>
using namespace std;

void imprimirMatriz(vector<vector<double>> &matriz)
{
    for (auto &fila : matriz)
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
    int n = A.size();
    vector<vector<double>> L(n, vector<double>(n, 0.0));
    vector<vector<double>> U(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++)
    {
        L[i][i] = 1.0;

        for (int j = i + 1; j < n; j++)
        {
            double factor = A[j][i] / A[i][i];

            L[j][i] = factor;
            for (int k = i; k < n; k++)
            {
                A[j][k] -= factor * A[i][k];
            }
        }
    }

    U = A;

    for (int i = 0; i < n; ++i)
    {
        double suma = 0.0;
        for (int j = 0; j < i; ++j)
        {
            suma += L[i][j] * R[j];
        }
        R[i] -= suma;
    }

    for (int i = n - 1; i >= 0; --i)
    {
        double suma = 0.0;
        for (int j = i + 1; j < n; ++j)
        {
            suma += U[i][j] * R[j];
        }
        R[i] = (R[i] - suma) / U[i][i];
    }

    cout << "x = " << R[0] << endl;
    cout << "y = " << R[1] << endl;
    cout << "z = " << R[2] << endl;
}

double funcion(int x, vector<vector<double>> &A, int n)
{
    double resultado = 0;

    for (int i = 0; i < n; i++)
    {
        double r = 1;
        double r2 = 1;

        for (int j = 0; j < n; j++)
        {
            if (j == i)
            {
                continue;
            }
            else
            {
                r *= (x - A[j][0]);
            }
        }

        for (int j = 0; j < n; j++)
        {
            if (j == i)
            {
                continue;
            }
            else
            {
                r2 *= (A[i][0] - A[j][0]);
            }
        }

        resultado += A[i][1] * (r / r2);
    }
    return resultado;
}

double evaluarPolinomio(const double coeficientes[], double potencia, double x)
{
    double resultado = 0;
    for (int i = potencia; i >= 0; i--)
    {
        resultado += coeficientes[i] * pow(x, i);
    }
    return resultado;
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

void menuMatx(vector<vector<double>> &matriz, vector<double> &R)
{
    vector<vector<double>> inversa = matrizInversa(matriz);
    vector<vector<double>> adjunta = matrizAdjunta(matriz);
    system("cls");
    int i = 0;
    while (i == 0)
    {
        cout << "Ingresa una opcion\n\n";
        cout << "1.-Ingresar datos" << endl;
        cout << "2.-Resolver sistema de la matriz" << endl;
        cout << "3.-Matriz inversa" << endl;
        cout << "4.-Matriz adjunta " << endl;
        cout << "5.-Volver al menu principal " << endl;
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
            cout << "Matriz Adjunta:" << endl;
            imprimirMatriz(adjunta);
            system("pause");
        case '5':
            system("cls");
            cout << "Adios :) ";
            system("pause");
            i++;
            break;
        default:
            cout << "Opcion no valida ";
            system("pause");
            break;
        }
    }
}

void Interpolacion()
{
    int n;
    double resul;

    cout << "¿Cuantos datos vas a ingresar ?";
    cin >> n;
    vector<vector<double>> equis(n, vector<double>(2, 0));

    for (int i = 0; i < n; i++)
    {
        cout << "Ingresa el dato en x : ";
        cin >> equis[i][0];
        cout << "Ingresa el dato en y : ";
        cin >> equis[i][1];
    }
    cout << "Ingresa un numero X te muestro su valor en Y : ";
    cin >> resul;
    cout << "\n"
         << resul << " , " << funcion(resul, equis, n);
    cout << endl;
    cout << "Quieres hacer otra coordenada ? ";
    system("pause");
}

void biseccion()
{
    int potencia;
    double inter1, inter2, contador = 0;
    double total, total2, total3;

    cout << "¿Qué potencia tiene el polinomio? : ";
    cin >> potencia;
    double coeficientes[potencia + 1];

    for (int i = potencia; i >= 0; i--)
    {
        cout << "Coeficiente para x^" << i << " = ";
        cin >> coeficientes[i];
    }

    cout << "Ingrese los extremos del intervalo inicial: " << endl;
    cout << "Extremo 1: ";
    cin >> inter1;
    cout << "Extremo 2: ";
    cin >> inter2;

    cout << "\n\t| No. | Intervalo 1 | Intervalo 2 | Punto medio | F(punto medio) | " << endl;

    while (true)
    {
        double puntoMedio = (inter1 + inter2) / 2;
        total = evaluarPolinomio(coeficientes, potencia, inter1);
        total2 = evaluarPolinomio(coeficientes, potencia, inter2);
        total3 = evaluarPolinomio(coeficientes, potencia, puntoMedio);

        cout << "\t " << contador << " " << inter1 << " " << inter2 << " " << puntoMedio << " " << total3 << endl;

        if (total3 == 0.0 || (inter2 - inter1) / 2 < 0.0001)
        {
            cout << "Raíz encontrada en: " << puntoMedio << endl;
            break;
        }

        if (total3 < 0 && total2 > 0)
        {
            inter1 = puntoMedio;
        }
        else
        {
            inter2 = puntoMedio;
        }

        contador++;
    }
    system("pause");
}

int main()
{
    vector<vector<double>> matriz = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<double> R = {1, 2, 3};
    int opcion;
    while (true)
    {
        system("cls");
        cout << "\nMenu:" << endl;
        cout << "1. Matrices" << endl;
        cout << "2. Interpolacion " << endl;
        cout << "3. Encontrar la raiz de una funcion" << endl;
        cout << "4. salir" << endl;

        cout << "Selecciona una opcion: ";
        cin >> opcion;
        switch (opcion)
        {
        case 1:
            menuMatx(matriz, R);
            break;
        case 2:
            Interpolacion();
            break;
        case 3:
            biseccion();
            break;
        case 4:
            return 0;
        }
    }
    return 0;
}
