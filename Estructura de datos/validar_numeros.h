#ifndef LIBRERIA_H
#define LIBRERIA_H

#include <iostream>
#include <string>
#include <cctype>

int ingresar_numero()
{
    std::string entrada;
    while (true)
    {
        std::cin >> entrada;
        bool valido = true;

        for (size_t i = 0; i < entrada.length(); i++)
        {
            if (!isdigit(entrada[i]) && !(i == 0 && entrada[i] == '-'))
            {
                valido = false;
                break;
            }
        }

        if (entrada == "-" || entrada == "")
        {
            valido = false;
            break;
        }

        if (valido)
        {
            return std::stoi(entrada);
        }

        std::cout << "Error: Ingresa un numero valido: ";
    }
}

int validador(int min, int max)
{
    int numero;

    while (true)
    {
        numero = ingresar_numero();

        if (numero >= min && numero <= max)
            return numero;

        std::cout << "Error: Ingresa un numero valido ("
                  << min << " - " << max << "): ";
    }
}

int validador()
{
    return ingresar_numero();
}

#endif