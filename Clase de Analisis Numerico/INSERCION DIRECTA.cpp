#include <iostream>
using namespace std;

void ordenar(int a[], int c)
{
    for (int i = 1; i < c; i++)
    {
        int aux = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > aux)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = aux;

        for (int k = 0; k < c; k++)
        {
            cout << a[k] << " ";
        }
        cout << endl;
    }
}

void imprimir(int a[], int c)
{
    for (int i = 0; i < c; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main()
{
    int n;

    cout << "ORDENAMIENTO POR INSERCION DIRECTA" << endl;
    cout << "-------------------------------" << endl;

    cout << "Cuantos numeros quieres ingresar? ";
    cin >> n;

    int *arr = new int[n](); // arreglo dinámico inicializado en 0

    for (int i = 0; i < n; i++)
    {
        system("cls"); // puedes quitar esto si no estás en Windows
        imprimir(arr, n);
        cout << "Ingresa un numero para la lista: ";
        cin >> arr[i];
    }

    system("cls");
    cout << "Lista original: ";
    imprimir(arr, n);

    cout << "Lista ordenada paso a paso:" << endl;
    ordenar(arr, n);

    delete[] arr; // libera la memoria dinámica

    return 0;
}
