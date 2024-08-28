#include <iostream>

struct Node {
    int data;
    Node* next;
};

class List {
private:
    Node* head;
    …
};

//cout
Node* temp = head;
while (temp != NULL) {
cout<< temp->data;
temp =  temp->next;
}

// push_front
Node* nodo = new Node;
nodo->data = 5;
nodo->next = head;
head = nodo;

// push_back
Node* nodo = new Node;
nodo->data = 5;
Node* temp = head;
while (temp->next !=NULL)
    temp = temp->next;
temp->next = nodo;
nodo->next = NULL;

//pop front
Node* temp = head;
head = head->next;
delete temp;

//pop back
if(head->next == NULL)
{
delete head;
head = NULL;
}

else
{
}
Node* temp = head;
while(temp->next->next != NULL)
temp = temp->next;
delete temp->next;
temp->next = NULL;

// insert
Node* nodo = new Node(5);
Node* temp = head;
int i = 0;
while(i++ < pos - 1)
    temp = temp->next;
nodo->next = temp->next;
temp->next = nodo;

// clear
while(head != NULL)
{
Node* temp = head;
head = head->next;
delete temp;
}


int main() {
    List<int> list;

    list.push_front(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(15);

    list.print();  // Imprimir la lista: 5 10 20 15

    cout << "Front: " << list.front() << endl;  // Debe imprimir 5
    cout << "Back: " << list.back() << endl;    // Debe imprimir 15

    list.sort();  // Ordena la lista
    list.print();  // Imprimir la lista ordenada: 5 10 15 20

    list.reverse();  // Revierte la lista
    list.print();  // Imprimir la lista revertida: 20 15 10 5

    list.pop_front();  // Remueve el primer elemento
    list.print();  // Imprimir la lista: 15 10 5

    list.pop_back();  // Remueve el último elemento
    list.print();  // Imprimir la lista: 15 10

    cout << "Elemento en la posición 1: " << list[1] << endl;  // Debe imprimir 10

    cout << "Size: " << list.size() << endl;  // Debe imprimir 2

    list.clear();  // Limpia la lista
    cout << "Size after clear: " << list.size() << endl;  // Debe imprimir 0

    List<int> list;

    list.push_back(10);
    list.push_back(3);
    list.push_back(5);
    list.push_back(8);
    list.push_back(1);

    cout << "Lista antes de ordenar: ";
    list.print();  // Imprime la lista desordenada

    list.sort();  // Ordena la lista usando Merge Sort

    cout << "Lista después de ordenar: ";
    list.print();  // Imprime la lista ordenada
    return 0;
}
