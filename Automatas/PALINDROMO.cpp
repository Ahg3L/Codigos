#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3
#define Q4 4
#define Q5 5
#define Q6 6

int transicion(int estado, char simbolo)
{
    switch (estado)
    {
    case Q0:
        if (simbolo == 'a')
            return Q1;
        else if (simbolo == 'b')
            return Q2;
        break;
    case Q1:
        if (simbolo == 'a')
            return Q3;
        else if (simbolo == 'b')
            return Q4;
        break;
    case Q2:
        if (simbolo == 'a')
            return Q4;
        else if (simbolo == 'b')
            return Q3;
        break;
    case Q3:
        if (simbolo == 'a' || simbolo == 'b')
            return Q5;
        break;
    case Q4:
        if (simbolo == 'a' || simbolo == 'b')
            return Q6;
        break;
    case Q5:
    case Q6:
        if (simbolo == 'a' || simbolo == 'b')
            return estado;
        break;
    default:
        return -1;
    }
    return -1;
}

bool aceptarCadena(char cadena[])
{
    int estado = Q0;
    int longitud = strlen(cadena);
    for (int i = 0; i < longitud; i++)
    {
        estado = transicion(estado, cadena[i]);
        if (estado == -1)
            return false;
    }
    return estado == Q5 || estado == Q6;
}

int main()
{
    char cadena[] = "aabbccbbaa";
    if (aceptarCadena(cadena))
        printf("La cadena es un palíndromo. \n");
    else
        printf("La cadena no es un palíndromo válido.\n");
    return 0;
}
