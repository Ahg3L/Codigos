#include <iostream>
#include <map>
#include "validar_numeros.h"

using namespace std;

// (Contadores de elementos en pila y cola)
int contadorP = 0, contadorC = 0;

int pila[15] = {0};
int cola[15] = {0};

void anadir()
{
    int opc;
    int valor;

    cout << "Pila o cola?\n1.-Pila\n2.-Cola\n";

    opc = validador(1, 2); // valor válidado

    switch (opc)
    {
    case 1:
        if (contadorP >= 15)
        {
            cout << "Pila llena." << endl;
            system("pause");
            return;
        }

        cout << "Ingrese un valor:";
        valor = validador(); //  aquí obtienes el número válido

        pila[contadorP++] = valor;
        break;

    case 2:
        if (contadorC >= 15)
        {
            cout << "Cola llena." << endl;
            system("pause");
            return;
        }

        cout << "Ingrese un valor:";
        valor = validador(); //  aquí obtienes el número válido

        cola[contadorC++] = valor;
        break;
    }
}

// Estructura para almacenar valores máximos y minimos
struct Maximos_y_minimos
{
    int maximo = 0, minimo = 0;
} PilaM, ColaM;

// Función para encontrar el valor máximo en pila y cola
void maximo()
{
    PilaM.maximo = pila[0];
    ColaM.maximo = cola[0];

    for (int i = 0; i < 15; i++)
    {
        if (pila[i] > PilaM.maximo)
        {
            PilaM.maximo = pila[i];
        }
        if (cola[i] > ColaM.maximo)
        {
            ColaM.maximo = cola[i];
        }
    }
}

// Función para encontrar el valor mínimo en pila y cola
void minimo()
{
    PilaM.minimo = pila[0];
    ColaM.minimo = cola[0];
    for (int j = 0; j < contadorP; j++)
    {
        if (pila[j] < PilaM.minimo)
        {
            PilaM.minimo = pila[j];
        }
    }

    for (int i = 0; i < contadorC; i++)
    {
        if (cola[i] < ColaM.minimo)
        {
            ColaM.minimo = cola[i];
        }
    }
}

// Estructura para almacenar los valores de la moda
struct moda
{
    int modaP, modaC;
};

// Función para calcular la moda de pila y cola

void moda()
{
    map<int, int> frequency;
    map<int, int> frequencyC;
    int maxCount = 0, mode = -1, maxCount2 = 0, mode2 = -1;

    for (int i = 0; i < contadorP; i++)
    {
        frequency[pila[i]]++;
        if (frequency[pila[i]] > maxCount)
        {
            maxCount = frequency[pila[i]];
            mode = pila[i];
        }
    }

    if (maxCount > 1)
    {
        cout << endl
             << "La moda de la pila es: " << mode << " con " << maxCount << " repeticiones." << endl;
    }
    else
    {
        cout << "No hay una moda en el arreglo de la pila." << endl;
    }

    for (int i = 0; i < contadorC; i++)
    {
        frequencyC[cola[i]]++;
        if (frequencyC[cola[i]] > maxCount2)
        {
            maxCount2 = frequencyC[cola[i]];
            mode2 = cola[i];
        }
    }
    if (maxCount2 > 1)
    {
        cout << endl
             << "La moda de la cola es: " << mode2 << " con " << maxCount2 << " repeticiones." << endl
             << endl;
    }
    else
    {
        cout << "No hay una moda en el arreglo de la cola." << endl;
    }
}

// Estructura para almacenar la suma
struct suma
{
    float sumapila;
    float sumacola;
} S;

// Función para calcular la suma de elementos en pila y cola
void sumaT()
{
    S.sumapila = 0;
    S.sumacola = 0;

    for (int j = 0; j < 15; j++)
    {
        S.sumapila = S.sumapila + pila[j];
        S.sumacola = S.sumacola + cola[j];
    }
}

// Estructura para almacenar el promedio
struct prom
{
    float prompila;
    float promcola;
} P;

// Función para calcular el promedio de pila y cola
void promedio()
{
    sumaT();
    P.prompila = 0;
    P.promcola = 0;
    P.prompila = S.sumapila / contadorP;
    P.promcola = S.sumacola / contadorC;
}

// Función para calcular el rango de pila y cola
void rango()
{
    minimo();
    maximo();
    cout << "El rango de la pila: " << (PilaM.maximo - PilaM.minimo) << endl;
    cout << "El rango de la cola: " << (ColaM.maximo - ColaM.minimo) << endl;
}

int main()
{
    int opcion = 0;

    while (true)
    {
        system("cls");

        cout << "  Pila  :  ";
        for (int i = 0; i < 15; i++)
        {
            cout << pila[i] << " | ";
        }
        cout << endl
             << "  Cola  :  ";
        for (int i = 14; i >= 0; i--)
        {
            cout << cola[i] << " | ";
        }
        cout << endl
             << endl
             << "Elija una opcion" << endl;
        cout << "1. Anadir un elemento" << endl;
        cout << "2. Encontrar el numero mayor y el menor" << endl;
        cout << "3. Encontrar la moda" << endl;
        cout << "4. Encontrar la sumatoria" << endl;
        cout << "5. Encontrar el promedio" << endl;
        cout << "6. Desplegar el rango" << endl;
        cout << "7. Salir" << endl;

        opcion = validador(1, 7); // valor validado
        switch (opcion)
        {
        case 1:
            anadir();
            break;

        case 2: // Encontrar el máximo
            maximo();
            minimo();
            cout << "Maximo Pila: " << PilaM.maximo << "  Maximo Cola: " << ColaM.maximo << endl;
            cout << "Minimo Pila: " << PilaM.minimo << "  Minimo Cola: " << ColaM.minimo << endl;
            system("pause");
            break;

        case 3: // Encontrar la moda
            moda();
            system("pause");
            break;

        case 4: // Encontrar la sumatoria
            sumaT();
            cout << "La sumatoria de la pila es: " << S.sumapila << " y el de cola es: " << S.sumacola << endl;
            system("pause");
            break;

        case 5: // Encontrar el promedio
            promedio();
            cout << "Promedio pila: " << P.prompila << endl;
            cout << "Promedio cola: " << P.promcola << endl;
            system("pause");
            break;

        case 6: // Desplegar el rango
            rango();
            system("pause");
            break;

        case 7: // Termina el programa
            return 0;
            break;

        default:
            cout << "Opción no válida." << endl;
            break;
        }
    }
    return 0;
}
