#include <iostream>

using namespace std;

// Definición de la estructura de un nodo
struct Node {
    int data;
    Node* next;  
    Node(int val) : data(val), next(NULL) {}
};


void insert(Node*& head, int val) {
    Node* newNode = new Node(val);
    newNode->next = head;
    head = newNode;
}

// Función para imprimir la lista enlazada
void printList(Node* head) {
    while (head != nullptr) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

void insertionSort(Node*& head) {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Node* sorted = nullptr;
    Node* current = head;

    while (current != nullptr) {
        Node* next = current->next;

        if (sorted == nullptr || current->data <= sorted->data) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* search = sorted;
            while (search->next != nullptr && current->data > search->next->data) {
                search = search->next;
            }
            current->next = search->next;
            search->next = current;
        }

        current = next;
    }

    head = sorted;
}

int main() {
    Node* head = nullptr;

    // Inserta elementos en la lista enlazada
    insert(head, 3);
    insert(head, 1);
    insert(head, 5);
    insert(head, 2);
    insert(head, 4);

    cout << "Lista enlazada no ordenada: ";
    printList(head);

    // Ordena la lista enlazada utilizando el algoritmo de ordenamiento por inserción
    insertionSort(head);

    cout << "Lista enlazada ordenada: ";
    printList(head);

    return 0;
}