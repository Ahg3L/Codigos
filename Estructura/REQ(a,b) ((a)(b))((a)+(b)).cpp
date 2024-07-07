#include <stdio.h>
#define REQ(a,b) ((a)*(b))/((a)+(b))

int main() {
    float a, b;

    printf("REQ = (a * b) / (a + b)\n");
    printf("Ingresa el valor A: ");
    scanf("%f", &a); 
    printf("Ingresa el valor B: ");
    scanf("%f", &b); 

    printf("El valor de REQ : %f", REQ(a,b));
    
    return 0;
}
