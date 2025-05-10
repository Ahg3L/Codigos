#include <stdio.h>
#include <ctype.h>

int main()
{
    char palabra[100];
    printf("Dame una palabra: ");
    scanf("%s", palabra);

    int i = 0, valida = 1;

    while (palabra[i] != '\0')
    {
        switch (isalpha(palabra[i]))
        {
        case 1: // Es una letra
            printf("Es una palabra\n");
            break;
        default: // No es una letra
            printf("No es una palabra\n");
            valida = 0;
            break;
        }
        i++;
    }

    if (valida)
        printf("Cadena aceptada\n");
    else
        printf("Cadena no aceptada\n");

    return 0;
}