#include <iostream>
using namespace std;

void ordenar(float a[], int c)
{
    for (int i = 1; i < c; i++)
    {
        float aux = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > aux)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = aux;
    }

    for (int k = 0; k < c; k++)
    {
        cout << a[k] << " ";
    }
    cout << endl;
}

void imprimir(float a[], int c)
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

    cout << "Cuantos numeros quieres ingresar ? " << endl;
    cin >> n;

    float *arr = new float[n]();

    for (int i = 0; i < n; i++)
    {
        system("cls");
        imprimir(arr, n);
        cout << " Ingresa un numero para la lista" << endl;
        cin >> arr[i];
    }
    system("cls");
    imprimir(arr, n);
    cout << "Lista ordenada: " << endl;
    ordenar(arr, n);

    return 0;
}
