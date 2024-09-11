//
// Luis

#ifndef DOUBLY_LINKED_LIST_STACK_H
#define DOUBLY_LINKED_LIST_STACK_H


#include <iostream>
#define MAX_STACK_SIZE 100

// Implementación de una pila genérica usando templates
template <typename T>
class Stack {
private:
    T array[MAX_STACK_SIZE];  // Array para almacenar los elementos de la pila
    int top;                  // Índice del elemento superior

public:
    // Constructor: inicializa la pila
    Stack() : top(-1) {}

    // Método para verificar si la pila está llena
    bool isFull() const {
        return top >= MAX_STACK_SIZE - 1;
    }

    // Método para verificar si la pila está vacía
    bool isEmpty() const {
        return top == -1;
    }

    // Método para devolver el elemento superior de la pila
    T getTop() const {
        if (isEmpty()) {
            std::cerr << "Error: La pila está vacía." << std::endl;
            exit(EXIT_FAILURE);  // Termina el programa si la pila está vacía
        }
        return array[top];
    }

    // Método para insertar un elemento en la pila
    void push(const T& element) {
        if (isFull()) {
            std::cerr << "Error: La pila está llena." << std::endl;
            exit(EXIT_FAILURE);  // Termina el programa si la pila está llena
        }
        array[++top] = element;  // Incrementa top y luego asigna el elemento
    }

    // Método para eliminar el elemento superior de la pila
    void pop() {
        if (isEmpty()) {
            std::cerr << "Error: La pila está vacía." << std::endl;
            exit(EXIT_FAILURE);  // Termina el programa si la pila está vacía
        }
        --top;  // Decrementa top para "eliminar" el elemento superior
    }
};

int main() {
    // Ejemplo de uso de la pila con enteros
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    std::cout << "Top element: " << intStack.getTop() << std::endl;  // Debería imprimir 30
    intStack.pop();
    std::cout << "Top element after pop: " << intStack.getTop() << std::endl;  // Debería imprimir 20

    return 0;
}


#endif //DOUBLY_LINKED_LIST_STACK_H
