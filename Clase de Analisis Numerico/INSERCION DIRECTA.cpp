#include <iostream>
using namespace std;

void ordenar(float a[], int c) {
    for (int i = 1; i < c; i++) {
        float aux = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > aux) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = aux;

        // Mostrar el arreglo después de cada iteración
        for (int k = 0; k < c; k++) {
            cout << a[k] << " ";
        }
        cout << endl;
    }
}

int main() {
    float arr[] = {10, 15, 21, 4, 16, 75, 94, 8};

    ordenar(arr, 8);

    return 0;
}
