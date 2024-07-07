#include <stdio.h>
#define m (a*b)
#define d (a/c)

int main() {
    int a, b;
	float c;

    printf("Ingresa el valor A: ");
    scanf("%i", &a); 
    printf("Ingresa el valor B: ");
    scanf("%i", &b); 
    printf("Ingresa el valor C: ");
    scanf("%f", &c);
    
    printf("\nA*B : %i", m);
    printf("\nA/B : %.3f", d);
    
    return 0;
}
