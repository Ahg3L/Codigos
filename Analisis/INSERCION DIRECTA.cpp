#include <iostream>
using namespace std;

void ordenar(float a[], int c) {
    for (int i = 1; i < c; i++) {
        float aux = a[i];
        int j= i- 1;

        while(a[j] >aux) {
            a[j+1] = a[j];
            j--;
        }
  
        a[j+1] = aux;
        
        for (int i = 0; i < 8; i++) {
        cout << a[i] << " ";
         }
         cout<<endl;
    }
}

int main() {
    float arr[] = {10, 15, 21, 4, 16, 75, 94, 8};
    
    ordenar(arr, 8);
   

    return 0;
}