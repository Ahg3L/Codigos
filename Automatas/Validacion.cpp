#include <iostream>
#include <string>
using namespace std;

bool tiene_mayuscula(string &password)
{
    for (char letra : password)
    {
        if (isupper(letra))
        {
            return true;
        }
    }
    return false;
}

bool tiene_minuscula(string &password)
{
    for (char letra : password)
    {
        if (islower(letra))
        {
            return true;
        }
    }
    return false;
}

bool tiene_numero(string &password)
{
    for (char letra : password)
    {
        if (isdigit(letra))
        {
            return true;
        }
    }
    return false;
}

bool cumple_reglas(string &password)
{
    return tiene_mayuscula(password) &&
           tiene_minuscula(password) &&
           tiene_numero(password);
}

int main()
{
    string password;
    int opc = 1;

    while (opc == 1)
    {
        system("cls");
        cout << "Ingrese la contrasena:  " << endl;
        cout << "Mayuscula, minuscula, numero" << endl;
        cin >> password;

        if (cumple_reglas(password))
        {
            cout << "La contrasena cumple con las reglas.\n";
        }
        else
        {
            cout << "La contrasena no cumple con las reglas de seguridad.\n";
        }

        cout << "Quieres introducir otra contraseña? \n";
        cout << "1.- SI    2.-NO\n";
        cin >> opc;
    }

    return 0;
}
