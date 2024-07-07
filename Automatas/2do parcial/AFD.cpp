#include <iostream>
#include <string.h>
using namespace std;

#define Q0 0
#define Q1 1
#define Q2 2
#define QS 3
#define QS1 4
#define QS2 5

int transicionEntrada(int estado, char simbolo)
{
    switch (estado)
    {
    case Q0:
        if (simbolo == '0')
            return Q1;
        else
            return -1;
        break;
    case Q1:
        if (simbolo == '0')
            return Q2;
        else
            return -1;
        break;
    case Q2:
        if (simbolo == '1')
            return 3;
        else
            return -1;
        break;

    default:
        return -1;
    }
    return -1;
}

int transicionSalida(int estadoSalida, char simboloS)
{
    switch (estadoSalida)
    {
    case QS:
        if (simboloS == '0')
            return QS1;
        break;
    case QS1:
        if (simboloS == '0')
            return QS2;
        break;

    case QS2:
        if (simboloS == '0')
            return 3;
        break;

    default:
        return -1;
    }
    return -1;
}


bool aceptarCadena(char cadena[])
{
    int estado = Q0;
    int estadoSalida = QS;
    int longitud = strlen(cadena) - 1;

    for (int i = 0; i < 3; i++)
    {

        estado = transicionEntrada(estado, cadena[i]);
        estadoSalida = transicionSalida(estadoSalida, cadena[longitud]);
        longitud--;
    }
    cout << estado << endl;
    cout << estadoSalida << endl;

    if (!(estado == 3 && estadoSalida == 3))
        return false;
    else
        return true;
}

int main()
{
    char cadena[] = "00100000";
    system("cls");
    if (aceptarCadena(cadena))
        printf("La cadena es aceptada.\n");
    else
        printf("La cadena no es aceptada.\n");
    return 0;
}
