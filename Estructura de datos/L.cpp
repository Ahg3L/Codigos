#include <iostream>
using namespace std;

struct Nodo
{
    int dato;
    Nodo *next;
    Nodo(int val) : dato(val), next(nullptr) {}
};

struct ingresar(Nodo *&head, int val)
{
    Nodo *newnodo = new Nodo(val);
    newnodo->next = head;
    head = newnodo
};

struct borrar(Nodo *&head)
{
    if (head == nullptr)
    {
        return;
    }
    else
    {
        Nodo *temp = head;
        head = head->next;
        delate temp;
    }
}

struct imprimir(Nodo *&head)
{

}

int
main()
{
    Nodo *head = nullptr;

    insert(head, 3);
    insert(head, 1);
    insert(head, 5);
    insert(head, 2);
    insert(head, 4);

    cout << "Lista enlazada no ordenada: ";
    printList(head);

    return 0;
}
