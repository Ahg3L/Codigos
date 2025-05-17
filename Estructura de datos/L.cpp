#include <iostream>
using namespace std;

struct Nodo
{
    int dato;
    Nodo *next;
    Nodo(int val) : dato(val), next(nullptr) {}
};

struct ingresar
{
};

int main()
{
    int num = 10;
    int *p = &num;
    cout << &p;

    return 0;
}
