#include <iostream>
#include <random> // Para generación de números aleatorios
#include <utility> // Para std::pair

template<typename T>
class Hash_table{
    T* table; // Array de la tabla hash
    bool* occupied; // Array para rastrear si la posición está ocupada
    bool* deleted; // Array para rastrear si la posición está eliminada

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
        T* oldTable = table;
        bool* oldOccupied = occupied;
        bool* oldDeleted = deleted;
        int oldsz = sz;
        sz = nwSize;
        table = new T[sz];
        occupied = new bool[sz]{false};
        deleted = new bool[sz]{false};
        cont = 0;

        for (int i = 0; i < oldsz; i++) {
            if (oldOccupied[i] && !oldDeleted[i]) {
                insert(oldTable[i]);
            }
        }
        delete[] oldTable;
        delete[] oldOccupied;
        delete[] oldDeleted;
    }

public:
    Hash_table(int size): sz(size), cont(0) {
        table = new T[sz];
        occupied = new bool[sz]{false};
        deleted = new bool[sz]{false};

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
        int start = index;

        do {
            if (!occupied[index] || deleted[index]) {
                table[index] = key;
                occupied[index] = true;
                deleted[index] = false;
                cont++;
                // Redimensionar si la carga de la tabla es mayor al 75%
                if(cont > 0.75 * sz) {
                    rehash(2 * sz);
                }
                return;
            } else if (table[index] == key) {
                // Si el elemento ya existe, no se hace nada
                return;
            }

            index = (index + 1) % sz;
        } while (index != start);
    }

    bool search(const T& key) const {
        int index = hash_funct(key, sz);
        int start = index;

        do {
            if (!occupied[index]) {
                return false;
            } else if (table[index] == key) {
                return true;
            }

            index = (index + 1) % sz;
        } while (index != start);

        return false;
    }

    void remove(const T& key) {
        int index = hash_funct(key, sz);
        int start = index;

        do {
            if (!occupied[index]) {
                return;
            } else if (table[index] == key) {
                deleted[index] = true;
                cont--;
                // Redimensionar si la carga de la tabla es menor al 25% después de eliminar
                if (cont < 0.25 * sz && sz > 10) {
                    rehash(sz / 2);
                }
                return;
            }

            index = (index + 1) % sz;
        } while (index != start);
    }

    bool empty() const {
        return cont == 0;
    }

    // Imprimir la tabla hash
    void print() const {
        for (int i = 0; i < sz; i++) {
            if (occupied[i] && !deleted[i]) {
                std::cout << "Bucket " << i << ": " << table[i] << std::endl;
            } else if (deleted[i]) {
                std::cout << "Bucket " << i << ": [Deleted]" << std::endl;
            } else {
                std::cout << "Bucket " << i << ": [Empty]" << std::endl;
            }
        }
    }


    ~Hash_table() {
        delete[] table;
        delete[] occupied;
        delete[] deleted;
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
