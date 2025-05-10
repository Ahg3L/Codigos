// Automata finito Determinista
#include <iostream>
#include <string.h>
using namespace std;

// Definimos los estados para entrada y salida
#define Q0 0
#define Q1 1
#define Q2 2
#define QF 3 // Estado final para entrada

#define QS 0
#define QS1 1
#define QS2 2
#define QSF 3 // Estado final para salida

// Función para las transiciones de entrada (de izquierda a derecha)
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
            return QF;
        else
            return -1;
        break;

    default:
        return -1;
    }
    return -1;
}

// Función para las transiciones de salida (de derecha a izquierda)
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
            return QSF;
        break;
    default:
        return -1;
    }
    return -1;
}

// Verifica si una cadena es aceptada por ambos procesos
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

    cout << "Estado final entrada: " << estado << endl;
    cout << "Estado final salida: " << estadoSalida << endl;

    if (estado == QF && estadoSalida == QSF)
        return true;
    else
        return false;
}

int main()
{
    char cadena[] = "00100000";
    system("cls"); // Limpiar pantalla (solo en Windows)

    if (aceptarCadena(cadena))
        printf("La cadena es aceptada.\n");
    else
        printf("La cadena no es aceptada.\n");

    return 0;
}
