#include <stdio.h>
#define AND  bool1 && bool2
#define OR  bool1 || bool2
#define NOT1  !bool1

int main() {
    int bool1,bool2;
    
    printf("Ingresa el primer valor : ");
    scanf("%d", &bool1);

    printf("Ingresa el segundo valor : ");
    scanf("%d", &bool2);

    printf("Resultado de AND: %d\n", AND);
    printf("Resultado de OR: %d\n", OR);
    printf("Resultado de NOT : %d", NOT1);
    
    return 0;
}
