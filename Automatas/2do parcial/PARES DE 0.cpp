#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define Q0 0
#define Q1 1

int transicion(int estado, char simbolo)
{
    switch (estado)
    {
    case Q0:
        if (simbolo == '0')
            return Q1;
        else if (simbolo == '1')
            return Q0;
        break;
    case Q1:
        if (simbolo == '0')
            return Q0;
        else if (simbolo == '1')
            return Q1;
        break;
    default:
        return -1;
    }
    return -1;
}

bool aceptarCadena(char cadena[])
{
    int estado = Q0;
    int contador_ceros = 0;
    int longitud = strlen(cadena);
    for (int i = 0; i < longitud; i++)
    {
        estado = transicion(estado, cadena[i]);
        if (estado == Q1 && cadena[i] == '0')
            contador_ceros++;
        if (estado == -1)
            return false;
    }
    return contador_ceros % 2 == 0 && estado == Q0;
}

int main()
{
    char cadena[] = "1010010";
    if (aceptarCadena(cadena))
        printf("La cadena contiene una cantidad par de signos alfabéticos '0'.\n");
    else
        printf("La cadena no contiene una cantidad par de signos alfabéticos '0'.\n");
    return 0;
}
