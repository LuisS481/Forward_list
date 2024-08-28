//
// Created by Luis on 28/08/2024.
//

#ifndef UNTITLED24_FORWARD_LIST_H
#define UNTITLED24_FORWARD_LIST_H

#include <iostream>

template<typename T>
struct Node {
    T data;
    Node* next;

    Node(T data) : data(data), next(nullptr) {}  // Constructor para facilitar la creación de nodos
};

template<typename T>
class List {
private:
    Node<T>* head;
    int list_sz; // variable para el tamaño

    // Función para dividir la lista en dos mitades
    Node<T>* split(Node<T>* head) {
        Node<T>* fast = head->next;
        Node<T>* slow = head;

        // Fast avanza dos nodos y slow avanza uno
        while (fast != nullptr && fast->next != nullptr) {
            fast = fast->next->next;
            slow = slow->next;
        }

        Node<T>* mid = slow->next;
        slow->next = nullptr;  // Cortar la lista en dos mitades
        return mid;
    }

    // Función para fusionar dos sublistas ordenadas
    Node<T>* merge(Node<T>* left, Node<T>* right) {
        if (!left) return right;
        if (!right) return left;

        Node<T>* result;
        if (left->data <= right->data) {
            result = left;
            result->next = merge(left->next, right);
        } else {
            result = right;
            result->next = merge(left, right->next);
        }
        return result;
    }

    // Función recursiva para aplicar Merge Sort
    Node<T>* mergeSort(Node<T>* head) {
        if (!head || !head->next)
            return head;

        Node<T>* mid = split(head);
        Node<T>* left = mergeSort(head);
        Node<T>* right = mergeSort(mid);

        return merge(left, right);
    }


public:
    List() : head(nullptr), list_sz(0) {}  // Constructor que inicializa la cabeza a nullptr

    ~List() {  // Destructor para limpiar toda la lista al finalizar
        clear();
    }

    // Retorna el elemento al comienzo de la lista
    T front() {
        if (head != nullptr)
            return head->data;
        throw std::out_of_range("Lista vacia");
    }

    // Retorna el elemento al final de la lista
    T back() {
        if (head == nullptr)
            throw std::out_of_range("List is empty");

        Node<T>* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;

        return temp->data;
    }

    // Método para insertar un nodo al frente de la lista
    void push_front(int value) {
        Node<T>* nodo = new Node<T>(value);
        nodo->next = head;
        head = nodo;
        list_sz++;
    }

    // Método para insertar un nodo al final de la lista
    void push_back(int value) {
        Node<T>* nodo = new Node<T>(value);
        if (head == nullptr) {
            head = nodo;
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = nodo;
        }
        list_sz++;
    }

    // Método para eliminar el primer nodo de la lista
    // Remueve y retorna el elemento al comienzo de la lista
    void pop_front() {
        if (head == nullptr)
            throw std::out_of_range("Lista vacia");

        Node<T>* temp = head;
        T value = head->data;
        head = head->next;
        delete temp;
        list_sz--;
        return value;
    }

    // Método para eliminar el último nodo de la lista
    // Remueve y retorna el elemento al final de la lista
    void pop_back() {
        if (head == nullptr)
            throw std::out_of_range("Lista vacia");

        if (head->next == nullptr) {
            T value = head->data;
            delete head;
            head = nullptr;
            list_sz--;
            return value;
        }

        Node<T>* temp = head;
        while (temp->next->next != nullptr)
            temp = temp->next;

        T value = temp->next->data;
        delete temp->next;
        temp->next = nullptr;
        list_sz--;
        return value;
    }

    // Retorna el elemento en la posición indicada (indexación con operador [])
    T operator[](int index) {
        if (index < 0 || index >= list_sz)
            throw std::out_of_range("Indice fuera de rango");

        Node<T>* temp = head;
        for (int i = 0; i < index; i++)
            temp = temp->next;

        return temp->data;
    }

    // Retorna si la lista está vacía o no
    bool empty() const {
        return head == nullptr;
    }

    // Retorna el tamaño de la lista
    int size() const {
        return list_sz;
    }

//    // Método para insertar un nodo en una posición específica
//    void insert(int value, int pos) {
//        if (pos < 0) return;  // Validar posición no negativa
//
//        Node<T>* nodo = new Node<T>(value);
//        if (pos == 0) {
//            nodo->next = head;
//            head = nodo;
//            return;
//        }
//
//        Node<T>* temp = head;
//        for (int i = 0; i < pos - 1 && temp != nullptr; i++) {
//            temp = temp->next;
//        }
//
//        if (temp == nullptr) {
//            delete nodo;  // La posición es mayor al tamaño de la lista
//        } else {
//            nodo->next = temp->next;
//            temp->next = nodo;
//        }
//    }

    // Ordena la lista utilizando Merge Sort
    void sort() {
        head = mergeSort(head);
    }

    // Método para limpiar toda la lista
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        list_sz = 0;
    }

    // Revierte la lista
    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;

        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }

    // Método para imprimir la lista
    void print() const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }
};

#endif //UNTITLED24_FORWARD_LIST_H
