#ifndef FORWARD_LIST_HASH_TABLE_OPEN_H
#define FORWARD_LIST_HASH_TABLE_OPEN_H

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <random>
#include <optional>

template<typename T>
class Hash_Table {
private:
    struct Entry {
        T data;
        bool occupied = false; // Indica si la entrada está ocupada
        bool deleted = false;  // Indica si la entrada fue eliminada
    };

    std::vector<Entry> hash_table;
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

    int u_hash_funct(const T key, int attempt) {
        int hkey = K_bytes(key);
        int hash_index = (hkey + attempt) % HTsz; // Sondeo lineal
        return hash_index;
    }

    void rehash(int Newsz) {
        std::vector<Entry> new_hash_table(Newsz);
        int old_HTsz = HTsz;
        HTsz = Newsz;
        capacity = 0; // Reiniciar capacidad

        // Reinsertar todos los elementos en la nueva tabla
        for (const auto& entry : hash_table) {
            if (entry.occupied && !entry.deleted) {
                insert(entry.data); // Reinsertar elementos
            }
        }

        hash_table = std::move(new_hash_table); // Reemplazar la tabla vieja por la nueva
    }

public:
    Hash_Table(int size) : HTsz(size), capacity(0) {
        hash_table.resize(HTsz); // Inicializar el vector de entradas con el tamaño de la tabla

        // Seleccionar aleatoriamente los parámetros a y b
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(1, prime - 1);

        a = dis(gen);
        b = dis(gen);
    }

    void insert(const T& key) {
        if (capacity >= 0.75 * HTsz) {
            rehash(2 * HTsz); // Redimensionar si la carga es mayor al 75%
        }

        int attempt = 0;
        while (attempt < HTsz) {
            int index = u_hash_funct(key, attempt);
            if (!hash_table[index].occupied || hash_table[index].deleted) {
                hash_table[index].data = key;
                hash_table[index].occupied = true;
                hash_table[index].deleted = false;
                capacity++;
                return;
            }
            attempt++;
        }
    }

    bool search(const T& key) {
        int attempt = 0;
        while (attempt < HTsz) {
            int index = u_hash_funct(key, attempt);
            if (!hash_table[index].occupied) {
                return false; // Entrada vacía, no está presente
            }
            if (hash_table[index].data == key && !hash_table[index].deleted) {
                return true; // Elemento encontrado
            }
            attempt++;
        }
        return false; // Elemento no encontrado
    }

    void remove(const T& key) {
        int attempt = 0;
        while (attempt < HTsz) {
            int index = u_hash_funct(key, attempt);
            if (!hash_table[index].occupied) {
                return; // Entrada vacía, no se puede eliminar
            }
            if (hash_table[index].data == key && !hash_table[index].deleted) {
                hash_table[index].deleted = true; // Marcar como eliminado
                capacity--;
                if (capacity < 0.25 * HTsz && HTsz > 10) {
                    rehash(HTsz / 2);
                }
                return;
            }
            attempt++;
        }
    }

    bool empty() {
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

#endif //FORWARD_LIST_HASH_TABLE_OPEN_H

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
    std::cout << "Esta 'banana' después de eliminarla? " << (hashTable.search("banana") ? "Si" : "No") << std::endl;

    // Verificar comportamiento al eliminar un elemento que no existe
    std::cout << "Eliminando 'kiwi' (no existe):\n";
    hashTable.remove("kiwi"); // No debería causar problemas

    // Comprobar la carga de la tabla
    std::cout << "\nLa tabla está vacía? " << (hashTable.empty() ? "Si" : "No") << std::endl;

    return 0;
}
