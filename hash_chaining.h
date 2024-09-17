#include "forward_list.h"

#include <iostream>
#include <random> // Para generación de números aleatorios

template<typename T>
class Hash_table{
    List<T>* linked_list; // Array de listas enlazadas
    int sz;
    int cont;

    // Parámetros aleatorios
    size_t a, b, p;

    // Número primo mayor que el tamaño de la tabla
    static const size_t prime = 4294967311; // Puedes usar un primo grande como 2^32 - 5

    // Función hash que convierte cualquier objeto en un valor de hash usando bytes
    int hash_funct(const T& key, size_t sz) const {
        const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
        size_t dataSize = sizeof(key);
        unsigned long hash = 5381;  // Inicializamos con un valor base (DJB2)

        for (size_t i = 0; i < dataSize; i++) {
            hash = ((hash << 5) + hash) + data[i];  // hash * 33 + data[i]
        }

        return ((a * hash + b) % p) % sz;
    }

    void rehash(int nwSize) {
        List<T>* oldTable = linked_list;
        int oldsz = sz;
        sz = nwSize;
        linked_list = new List<T>[sz];
        cont = 0;

        for (int i = 0; i < oldsz; i++) {
            Nodo<T>* current = oldTable[i].head;
            while (current != nullptr) {
                insert(current->data);
                current = current->next;
            }
        }
        delete[] oldTable;
    }

public:
    Hash_table(int size): sz(size), cont(0) {
        linked_list = new List<T>[sz];

        // Seleccionar aleatoriamente los parámetros a y b
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(1, prime - 1);

        a = dis(gen);
        b = dis(gen);
        p = prime;
    }

    void insert(const T& key) {
        int index = hash_funct(key, sz);
        if (!linked_list[index].find(key)) {
            linked_list[index].push_front(key);
            cont++;
            // Redimensionar si la carga de la tabla es mayor al 75%
            if(cont > 0.75 * sz) {
                rehash(2 * sz);
            }
        }
    }

    bool search(const T& key) const {
        int index = hash_funct(key, sz);
        return linked_list[index].find(key);
    }

    void remove(const T& key) {
        int index = hash_funct(key, sz);
        if(linked_list[index].find(key)) {
            linked_list[index].remove(key);
            cont--;
            // Redimensionar si la carga de la tabla es menor al 25% después de eliminar
            if (cont < 0.25 * sz && sz > 10) {
                rehash(sz / 2);
            }
        }
    }

    bool empty() const {
        return cont == 0;
    }

    // Imprimir la tabla hash
    void print() const {
        for (int i = 0; i < sz; i++) {
            std::cout << "Bucket " << i << ": ";
            linked_list[i].print();
            std::cout << std::endl;
        }
    }

    ~Hash_table() {
        delete[] linked_list;
    }
};

int main() {
    // Crear una tabla hash con 10 buckets
    HashTable<int> hashTable(10);

    // Insertar algunos valores en la tabla hash
    hashTable.insert(10);
    hashTable.insert(20);
    hashTable.insert(30);
    hashTable.insert(40);
    hashTable.insert(50);

    // Imprimir el contenido de la tabla hash
    std::cout << "Contenido de la tabla hash después de insertar algunos valores:" << std::endl;
    hashTable.print();

    // Buscar algunos valores en la tabla hash
    std::cout << "Buscar 20: " << (hashTable.search(20) ? "Encontrado" : "No encontrado") << std::endl;
    std::cout << "Buscar 100: " << (hashTable.search(100) ? "Encontrado" : "No encontrado") << std::endl;

    // Eliminar un valor de la tabla hash
    hashTable.remove(20);
    std::cout << "Contenido de la tabla hash después de eliminar 20:" << std::endl;
    hashTable.print();

    // Verificar si la tabla está vacía
    std::cout << "La tabla está vacía: " << (hashTable.empty() ? "Sí" : "No") << std::endl;

    return 0;
}
