#ifndef FORWARD_LIST_HASH_TABLE_H
#define FORWARD_LIST_HASH_TABLE_H

#include "forward_list.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <random>

template<typename T>
class Hash_Table{
private:
    std::vector<List<T>> hash_table;
    int HTsz;
    int capacity;

    double am = 0.6180339887; // Constante (0 < a < 1)
    int a, b; // Parámetros aleatorios
    int p;

    // Número primo mayor que el tamaño de la tabla
    const size_t prime = 1000003;

    // Función para convertir cualquier tipo a un valor numérico
    int K_bytes(const T& key) const {
        std::ostringstream oss;
        oss.write(reinterpret_cast<const char*>(&key), sizeof(key));
        std::string bytes = oss.str();
        int hash = 0;
        for (unsigned char b : bytes) {
            hash += b; // Sumar valores de bytes
        }
        return hash;
    }

    int div_hash_funct(const T key){
        int hkey = K_bytes(key);
        return hkey % HTsz;
    }
    int mult_hash_funct(const T key, int HTsz){
        int hkey = K_bytes(key);
        int w = sizeof(T) * 8; // Numero de bytes en T
        int r = static_cast<int>(log2(HTsz));
        return static_cast<int>(static_cast<int>((am * hkey)) % static_cast<int>((pow(2,w)))) >> (w - r);
    }
    int u_hash_funct(const T key){
        int hkey = K_bytes(key);
        return ((a * hkey + b) % p) % HTsz;
    }

    void rehash(int Newsz){
        std::vector<List<T>> new_hash_table(Newsz);
        int old_HTsz = HTsz;
        HTsz = Newsz;

        // Reinsertar todos los elementos en la nueva tabla
        for (int i = 0; i < old_HTsz; ++i) {
            if (!hash_table[i].empty()) {
                Nodo<T>* current = hash_table[i].front_nodo(); // Obtener la lista en la posición i
                while (current != nullptr) {
                    int new_index = u_hash_funct(current->data); // Calcular nueva posición
                    new_hash_table[new_index].push_front(current->data); // Insertar en la nueva tabla
                    current = current->next;
                }
            }
        }

        hash_table = std::move(new_hash_table); // Reemplazar la tabla vieja por la nueva
    }
public:
    Hash_Table(int size): HTsz(size), capacity(0){
        hash_table.resize(HTsz); // Inicializar el vector de listas con el tamaño de la tabla

        // Seleccionar aleatoriamente los parámetros a y b
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(1, prime - 1);

        a = dis(gen);
        b = dis(gen);
        p = prime;
    }

    void insert(const T& key){
        int index = u_hash_funct(key);
        List<T>& list_at_index = hash_table[index];  // Obtener la lista en esa posición

        // Verificar si el elemento ya está en la lista
        if (!list_at_index.find(key)) {
            list_at_index.push_back(key);  // Insertar el elemento en la lista enlazada
            capacity++;  // Aumentar el número de elementos en la tabla
        }
        // Redimensionar si la carga de la tabla es mayor al 75%
        if(capacity > 0.75 * HTsz) {
            rehash(2 * HTsz);
        }
    }

    bool search(const T& key){
        int index = u_hash_funct(key);
        List<T>& list_at_index = hash_table[index];

        // Buscar el elemento en la lista enlazada
        return list_at_index.find(key);  // Si lo encuentra, retorna true; de lo contrario, false
    }

    void remove(const T& key){ // void delete(const T& key){
        int index = u_hash_funct(key);
        List<T>& list_at_index = hash_table[index];

        // Intentar eliminar el elemento de la lista enlazada
        if (list_at_index.remove(key)) {  // Si la eliminación fue exitosa
            capacity--;
        }
        // Redimensionar si la carga de la tabla es menor al 25% después de eliminar
        if (capacity < 0.25 * HTsz && HTsz > 10) {
            rehash(HTsz / 2);
        }
    }

    bool empty(){
        return capacity == 0;
    }
};

// Especialización de K_bytes para std::string
template<>
int Hash_Table<std::string>::K_bytes(const std::string& key) const {
    int hash = 0;
    for (char c : key) {
        hash += static_cast<unsigned char>(c); // Sumar cada byte de la cadena
    }
    return hash;
}
#endif //FORWARD_LIST_HASH_TABLE_H

int main() {
    Hash_Table<std::string> hashTable(10); // Crear una tabla hash con tamaño 10

    // Casos de prueba para insertar elementos
    std::cout << "Insertando elementos:\n";
    hashTable.insert("apple");
    hashTable.insert("banana");
    hashTable.insert("orange");
    hashTable.insert("grape");
    hashTable.insert("pear");

    // Comprobar la búsqueda de elementos
    std::cout << "\nBuscando elementos:\n";
    std::cout << "Esta 'banana'? " << (hashTable.search("banana") ? "Si" : "No") << std::endl;
    std::cout << "Esta 'kiwi'? " << (hashTable.search("kiwi") ? "Si" : "No") << std::endl;

    // Comprobar la eliminación de elementos
    std::cout << "\nEliminando elementos:\n";
    hashTable.remove("banana");
    std::cout << "Esta 'banana' despues de eliminarla? " << (hashTable.search("banana") ? "Si" : "No") << std::endl;

    // Verificar comportamiento al eliminar un elemento que no existe
    std::cout << "Eliminando 'kiwi' (no existe):\n";
    hashTable.remove("kiwi"); // No debería causar problemas

    // Comprobar la carga de la tabla
    std::cout << "\nLa tabla esta vacia? " << (hashTable.empty() ? "Si" : "No") << std::endl;

    return 0;
}

