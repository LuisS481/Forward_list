#include <iostream> // Incluye la biblioteca estándar de entrada y salida
using namespace std; // Permite usar elementos del espacio de nombres estándar sin calificarlos

// Estructura para representar una celda no cero en la matriz
struct Cell {
    int row, col, value; // Fila, columna y valor de la celda
    Cell* nextRow; // Puntero al siguiente elemento en la misma fila
    Cell* nextCol; // Puntero al siguiente elemento en la misma columna
    // Constructor para inicializar una celda con fila, columna y valor
    Cell(int r, int c, int v) : row(r), col(c), value(v), nextRow(nullptr), nextCol(nullptr) {}
};

// Estructura para representar la cabecera de filas o columnas
struct Header {
    int index; // Índice de la fila o columna
    Cell* next; // Puntero al primer nodo asociado (inicio de la fila o columna)
    Header* nextHeader; // Puntero a la siguiente cabecera (siguiente fila o columna)
    // Constructor para inicializar una cabecera con índice
    Header(int i) : index(i), next(nullptr), nextHeader(nullptr) {}
};

// Clase principal para manejar matrices dispersas
class SparseMatrix {
private:
    Header* rowHead; // Puntero a la lista de cabeceras de filas
    Header* colHead; // Puntero a la lista de cabeceras de columnas
    int rows, cols;  // Dimensiones de la matriz (filas y columnas)

    // Función para obtener o crear la cabecera de una fila específica
    Header* getOrCreateRowHeader(int row) {
        Header* prev = nullptr; // Puntero al nodo anterior en la lista de cabeceras
        Header* curr = rowHead; // Puntero al nodo actual en la lista de cabeceras
        // Avanza hasta encontrar la posición adecuada o hasta el final
        while (curr && curr->index < row) {
            prev = curr;
            curr = curr->nextHeader;
        }
        // Si no existe una cabecera para esta fila, la crea
        if (!curr || curr->index != row) {
            Header* newHeader = new Header(row); // Crea una nueva cabecera
            if (prev) { // Si hay una cabecera previa, enlaza la nueva cabecera
                prev->nextHeader = newHeader;
            } else { // Si no hay previa, actualiza la cabeza de la lista de filas
                rowHead = newHeader;
            }
            newHeader->nextHeader = curr; // Enlaza con la siguiente cabecera
            return newHeader; // Retorna la nueva cabecera
        }
        return curr; // Retorna la cabecera existente
    }

    // Función para obtener o crear la cabecera de una columna específica
    Header* getOrCreateColHeader(int col) {
        Header* prev = nullptr; // Puntero al nodo anterior en la lista de cabeceras
        Header* curr = colHead; // Puntero al nodo actual en la lista de cabeceras
        // Avanza hasta encontrar la posición adecuada o hasta el final
        while (curr && curr->index < col) {
            prev = curr;
            curr = curr->nextHeader;
        }
        // Si no existe una cabecera para esta columna, la crea
        if (!curr || curr->index != col) {
            Header* newHeader = new Header(col); // Crea una nueva cabecera
            if (prev) { // Si hay una cabecera previa, enlaza la nueva cabecera
                prev->nextHeader = newHeader;
            } else { // Si no hay previa, actualiza la cabeza de la lista de columnas
                colHead = newHeader;
            }
            newHeader->nextHeader = curr; // Enlaza con la siguiente cabecera
            return newHeader; // Retorna la nueva cabecera
        }
        return curr; // Retorna la cabecera existente
    }

public:
    // Constructor para inicializar la matriz con dimensiones dadas
    SparseMatrix(int r, int c) : rowHead(nullptr), colHead(nullptr), rows(r), cols(c) {}

    // Inserta un valor en la matriz dispersa
    void insert(int row, int col, int value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || value == 0) return; // Validación de entrada
        Header* rowHeader = getOrCreateRowHeader(row); // Obtiene/crea cabecera de fila
        Header* colHeader = getOrCreateColHeader(col); // Obtiene/crea cabecera de columna
        Cell* newCell = new Cell(row, col, value); // Crea la nueva celda

        // Inserta la celda en la lista de la fila
        Cell* rowPrev = nullptr;
        Cell* rowCurr = rowHeader->next; // Primer nodo de la fila
        while (rowCurr && rowCurr->col < col) { // Busca posición correcta
            rowPrev = rowCurr;
            rowCurr = rowCurr->nextRow;
        }
        if (rowPrev) { // Inserta la celda después de un nodo existente
            rowPrev->nextRow = newCell;
        } else { // Inserta la celda como el primer nodo de la fila
            rowHeader->next = newCell;
        }
        newCell->nextRow = rowCurr; // Enlaza la celda con el siguiente nodo en la fila

        // Inserta la celda en la lista de la columna
        Cell* colPrev = nullptr;
        Cell* colCurr = colHeader->next; // Primer nodo de la columna
        while (colCurr && colCurr->row < row) { // Busca posición correcta
            colPrev = colCurr;
            colCurr = colCurr->nextCol;
        }
        if (colPrev) { // Inserta la celda después de un nodo existente
            colPrev->nextCol = newCell;
        } else { // Inserta la celda como el primer nodo de la columna
            colHeader->next = newCell;
        }
        newCell->nextCol = colCurr; // Enlaza la celda con el siguiente nodo en la columna
    }

    // Muestra la matriz en formato tabular
    void display() {
        for (int i = 0; i < rows; ++i) { // Itera por cada fila
            Header* rowHeader = getOrCreateRowHeader(i); // Obtiene la cabecera de la fila
            Cell* cell = rowHeader->next; // Primer nodo de la fila
            for (int j = 0; j < cols; ++j) { // Itera por cada columna
                if (cell && cell->col == j) { // Si hay un nodo en esta posición
                    cout << cell->value << " "; // Muestra el valor
                    cell = cell->nextRow; // Avanza al siguiente nodo en la fila
                } else {
                    cout << "0 "; // Si no hay nodo, muestra un cero
                }
            }
            cout << endl; // Salto de línea después de cada fila
        }
    }

    // Destructor para limpiar la memoria
    ~SparseMatrix() {
        Header* rowCurr = rowHead; // Puntero para iterar por las cabeceras de fila
        while (rowCurr) {
            Cell* cellCurr = rowCurr->next; // Puntero para iterar por las celdas
            while (cellCurr) {
                Cell* toDelete = cellCurr; // Guarda el nodo a eliminar
                cellCurr = cellCurr->nextRow; // Avanza al siguiente nodo en la fila
                delete toDelete; // Libera memoria de la celda
            }
            Header* toDelete = rowCurr; // Guarda la cabecera a eliminar
            rowCurr = rowCurr->nextHeader; // Avanza a la siguiente cabecera de fila
            delete toDelete; // Libera memoria de la cabecera
        }
        Header* colCurr = colHead; // Puntero para iterar por las cabeceras de columna
        while (colCurr) {
            Header* toDelete = colCurr; // Guarda la cabecera a eliminar
            colCurr = colCurr->nextHeader; // Avanza a la siguiente cabecera de columna
            delete toDelete; // Libera memoria de la cabecera
        }
    }
};

int main() {
    SparseMatrix sm(5, 5); // Crea una matriz dispersa de 5x5
    sm.insert(1, 1, 10); // Inserta el valor 10 en la posición (1, 1)
    sm.insert(2, 3, 20); // Inserta el valor 20 en la posición (2, 3)
    sm.insert(4, 0, 30); // Inserta el valor 30 en la posición (4, 0)
    cout << "Matriz dispersa:" << endl;
    sm.display(); // Muestra la matriz
    sm.insert(3, 2, 8); // Inserta el valor 8 en la posición (3, 2)
    cout << "Matriz dispersa:" << endl;
    sm.display(); // Muestra la matriz
    return 0; // Finaliza el programa
}
