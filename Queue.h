//
// Created by Luis on 10/09/2024.
//

#ifndef DOUBLY_LINKED_LIST_QUEUE_H
#define DOUBLY_LINKED_LIST_QUEUE_H


#include <iostream>
#define MAX_Q_SIZE 100

template <typename T>
class Queue {
private:
    T array[MAX_Q_SIZE];   // Array circular para almacenar los elementos de la cola
    int front, rear;       // Índices para la cabeza y la cola

public:
    // Constructor: inicializa la cola
    Queue() : front(0), rear(0) {}

    // Método para verificar si la cola está llena
    bool isFull() const {
        return front == (rear + 1) % MAX_Q_SIZE;
    }

    // Método para verificar si la cola está vacía
    bool isEmpty() const {
        return front == rear;
    }

    // Método para devolver el elemento al frente de la cola
    T getFront() const {
        if (isEmpty()) {
            std::cerr << "Error: La cola está vacía." << std::endl;
            exit(EXIT_FAILURE);  // Terminar el programa si la cola está vacía
        }
        return array[(front + 1) % MAX_Q_SIZE];  // El siguiente de 'front' es el primero válido
    }

    // Método para insertar un elemento en la cola
    void enqueue(const T& element) {
        if (isFull()) {
            std::cerr << "Error: La cola está llena." << std::endl;
            exit(EXIT_FAILURE);  // Terminar el programa si la cola está llena
        }
        rear = (rear + 1) % MAX_Q_SIZE;
        array[rear] = element;
    }

    // Método para eliminar el elemento al frente de la cola
    void dequeue() {
        if (isEmpty()) {
            std::cerr << "Error: La cola está vacía." << std::endl;
            exit(EXIT_FAILURE);  // Terminar el programa si la cola está vacía
        }
        front = (front + 1) % MAX_Q_SIZE;  // Avanzar el puntero 'front'
    }
};

int main() {
    // Ejemplo de uso de la cola con enteros
    Queue<int> intQueue;

    intQueue.enqueue(10);
    intQueue.enqueue(20);
    intQueue.enqueue(30);

    std::cout << "Front element: " << intQueue.getFront() << std::endl;  // Debería imprimir 10
    intQueue.dequeue();
    std::cout << "Front element after dequeue: " << intQueue.getFront() << std::endl;  // Debería imprimir 20

    return 0;
}

#include <iostream>
#define MAX_Q_SIZE 100

// Definición de un elemento genérico utilizando templates
template <typename T>
struct element {
    T key;  // Puedes definir otros campos según lo que quieras almacenar
};

// Definición de la cola circular
template <typename T>
struct queue {
    element<T> list[MAX_Q_SIZE];  // Array de elementos (tipo genérico)
    int front;  // Índice de la cabeza de la cola
    int rear;   // Índice de la cola (último elemento)
};

// Inicializar la cola
template <typename T>
void initializeQueue(queue<T> *q) {
    q->front = 0;
    q->rear = 0;
}

// Verificar si la cola está llena
template <typename T>
int isFull(queue<T> *q) {
    return q->front == ((q->rear + 1) % MAX_Q_SIZE);
}

// Verificar si la cola está vacía
template <typename T>
int isEmpty(queue<T> *q) {
    return q->front == q->rear;
}

// Obtener el elemento al frente de la cola
template <typename T>
element<T> getFront(queue<T> *q) {
    if (isEmpty(q)) {
        std::cerr << "Error: La cola está vacía." << std::endl;
        exit(EXIT_FAILURE);
    }
    return q->list[(q->front + 1) % MAX_Q_SIZE];
}

// Insertar un elemento en la cola
template <typename T>
void enqueue(queue<T> *q, element<T> e) {
    if (isFull(q)) {
        std::cerr << "Error: La cola está llena." << std::endl;
        exit(EXIT_FAILURE);
    }
    q->rear = (q->rear + 1) % MAX_Q_SIZE;
    q->list[q->rear] = e;
}

// Eliminar el elemento al frente de la cola
template <typename T>
void dequeue(queue<T> *q) {
    if (isEmpty(q)) {
        std::cerr << "Error: La cola está vacía." << std::endl;
        exit(EXIT_FAILURE);
    }
    q->front = (q->front + 1) % MAX_Q_SIZE;
}

int main() {
    // Declaración de una cola de enteros
    queue<int> Q;

    // Inicializar la cola
    initializeQueue(&Q);

    // Crear algunos elementos
    element<int> e1 = {10};
    element<int> e2 = {20};
    element<int> e3 = {30};

    // Insertar elementos en la cola
    enqueue(&Q, e1);
    enqueue(&Q, e2);
    enqueue(&Q, e3);

    // Mostrar el elemento al frente de la cola
    std::cout << "Front element: " << getFront(&Q).key << std::endl;  // Debería imprimir 10

    // Eliminar el elemento del frente
    dequeue(&Q);
    std::cout << "Front element after dequeue: " << getFront(&Q).key << std::endl;  // Debería imprimir 20

    return 0;
}

#endif //DOUBLY_LINKED_LIST_QUEUE_H
