#include <iostream>

using namespace std;

void intercambiar(int &a, int &b) {
    int aux = a;
    a=b;
    b=aux;
}  

void Burbuja(int arr[], int n) {
    for (int i=0; i<n-1; i++) {
        for (int j=0;j <n-i-1; j++) {
            if (arr[j]>arr[j+1]) {
                intercambiar(arr[j],arr[j+1]);
            }
        }
    }
}

void imprimir(int arr[], int n) {
    for (int i=0; i<n; i++) {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

int main() {
    int numero;
    system("cls");
    cout<<"\nCuantos datos quieres ordenar? : "<<endl;
    cin>>numero;
    int arr[numero] = {0};

    cout<<"\nIngresa los datos. \n";
    for(int i = 0; i<numero; i++){
        int num;cin>>num;
        arr[i] = num;
    }

    cout<<"Array original: ";
    imprimir(arr, numero);

    Burbuja(arr,numero);

    cout<<"Array ordenado: ";
    imprimir(arr,numero);

    return 0;
}

