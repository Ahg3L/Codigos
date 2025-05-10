#include <iostream>
#include <string.h>
using namespace std;

#define Q0  0
#define Q1  1
#define Q2  2
#define Q22 3
#define Q3  4
#define Q33 5
#define Q4  6
#define Q5  7
#define Q6  8
#define Q7  9
#define Q8  10
#define Q9  11

int transicion(int estado, char simbolo)
{
    switch (estado)
    {
    case Q0:
        if (simbolo == 'a')
            return Q1;
        break;
    case Q1:
        if (simbolo == 'a')
            return Q2;
        else if (simbolo == 'b')
            return Q22;
        break;

    case Q2:
        if (simbolo == 'b')
            return Q3;
        break;
    case Q22:
        if (simbolo == 'b')
            return Q33;
        break;

    case Q3:
        if (simbolo == 'b')
            return Q4;
        break;
    case Q33:
        if (simbolo == 'a')
            return Q4;
        break;

    case Q4:
        if (simbolo == 'a')
            return Q5;
        break;
    case Q5:
        if (simbolo == 'b')
            return Q6;
        break;
    case Q6:
        if (simbolo == 'b')
            return Q7;
        break;
    case Q7:
        if (simbolo == 'a')
            return Q8;
        break;
    case Q8:
        if (simbolo == 'a')
            return Q9;
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
    return true;
}

int main()
{
    char cadena[] = "abbaabbaa";
    system("cls");
    if (aceptarCadena(cadena))
        printf("La cadena es aceptada.\n");
    else
        printf("La cadena no es aceptada.\n");
    return 0;
}
