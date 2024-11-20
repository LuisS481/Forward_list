#include <iostream>
using namespace std;

struct Cell {
    int row, col, value;
    Cell* nextRow;
    Cell* nextCol;
    Cell(int r, int c, int v) : row(r), col(c), value(v), nextRow(nullptr), nextCol(nullptr) {}
};

struct Header {
    int index;
    Cell* next;
    Header* nextHeader;
    Header(int i) : index(i), next(nullptr), nextHeader(nullptr) {}
};

class SparseMatrix {
private:
    Header* rowHead;
    Header* colHead;
    int rows, cols;

    Header* getOrCreateRowHeader(int row) {
        Header* prev = nullptr;
        Header* curr = rowHead;
        while (curr && curr->index < row) {
            prev = curr;
            curr = curr->nextHeader;
        }
        if (!curr || curr->index != row) {
            Header* newHeader = new Header(row);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                rowHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }

    Header* getOrCreateColHeader(int col) {
        Header* prev = nullptr;
        Header* curr = colHead;
        while (curr && curr->index < col) {
            prev = curr;
            curr = curr->nextHeader;
        }
        if (!curr || curr->index != col) {
            Header* newHeader = new Header(col);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                colHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }

public:
    SparseMatrix(int r, int c) : rowHead(nullptr), colHead(nullptr), rows(r), cols(c) {}

    // Insertar un valor
    void insert(int row, int col, int value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) return;
        Header* rowHeader = getOrCreateRowHeader(row);
        Header* colHeader = getOrCreateColHeader(col);
        Cell* newCell = new Cell(row, col, value);

        // Insertar en la fila
        if (!rowHeader->next) {
            rowHeader->next = newCell;
            newCell->nextRow = newCell; // Cíclico
        } else {
            Cell* rowPrev = nullptr;
            Cell* rowCurr = rowHeader->next;
            do {
                rowPrev = rowCurr;
                rowCurr = rowCurr->nextRow;
            } while (rowCurr != rowHeader->next && rowCurr->col < col);

            newCell->nextRow = rowCurr;
            if (rowPrev->nextRow == rowCurr) {
                rowPrev->nextRow = newCell;
            } else {
                rowHeader->next = newCell;
            }
        }

        // Insertar en la columna
        if (!colHeader->next) {
            colHeader->next = newCell;
            newCell->nextCol = newCell; // Cíclico
        } else {
            Cell* colPrev = nullptr;
            Cell* colCurr = colHeader->next;
            do {
                colPrev = colCurr;
                colCurr = colCurr->nextCol;
            } while (colCurr != colHeader->next && colCurr->row < row);

            newCell->nextCol = colCurr;
            if (colPrev->nextCol == colCurr) {
                colPrev->nextCol = newCell;
            } else {
                colHeader->next = newCell;
            }
        }
    }

    // Eliminar un valor
    void remove(int row, int col) {
        Header* rowHeader = rowHead;
        while (rowHeader && rowHeader->index != row) {
            rowHeader = rowHeader->nextHeader;
        }
        if (!rowHeader || !rowHeader->next) return;

        Header* colHeader = colHead;
        while (colHeader && colHeader->index != col) {
            colHeader = colHeader->nextHeader;
        }
        if (!colHeader || !colHeader->next) return;

        // Eliminar de fila
        Cell* rowPrev = nullptr;
        Cell* rowCurr = rowHeader->next;
        do {
            if (rowCurr->col == col) {
                if (rowPrev) {
                    rowPrev->nextRow = rowCurr->nextRow;
                } else {
                    rowHeader->next = (rowCurr->nextRow == rowCurr) ? nullptr : rowCurr->nextRow;
                }
                break;
            }
            rowPrev = rowCurr;
            rowCurr = rowCurr->nextRow;
        } while (rowCurr != rowHeader->next);

        // Eliminar de columna
        Cell* colPrev = nullptr;
        Cell* colCurr = colHeader->next;
        do {
            if (colCurr->row == row) {
                if (colPrev) {
                    colPrev->nextCol = colCurr->nextCol;
                } else {
                    colHeader->next = (colCurr->nextCol == colCurr) ? nullptr : colCurr->nextCol;
                }
                delete rowCurr;
                break;
            }
            colPrev = colCurr;
            colCurr = colCurr->nextCol;
        } while (colCurr != colHeader->next);
    }

    // Buscar un valor
    int search(int row, int col) const {
        Header* rowHeader = rowHead;
        while (rowHeader && rowHeader->index != row) {
            rowHeader = rowHeader->nextHeader;
        }
        if (!rowHeader || !rowHeader->next) return 0;

        Cell* cell = rowHeader->next;
        do {
            if (cell->col == col) {
                return cell->value;
            }
            cell = cell->nextRow;
        } while (cell != rowHeader->next);
        return 0;
    }

    // Sobrecargar operador para obtener valor
    int operator()(int row, int col) const {
        return search(row, col);
    }

    // Comparar matrices
    bool operator==(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (search(i, j) != other(i, j)) return false;
            }
        }
        return true;
    }

    // Suma de matrices
    SparseMatrix operator+(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) throw invalid_argument("Dimensiones no coinciden");
        SparseMatrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int sum = search(i, j) + other(i, j);
                if (sum != 0) result.insert(i, j, sum);
            }
        }
        return result;
    }

    // Resta de matrices
    SparseMatrix operator-(const SparseMatrix& other) const {
        if (rows != other.rows || cols != other.cols) throw invalid_argument("Dimensiones no coinciden");
        SparseMatrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int diff = search(i, j) - other(i, j);
                if (diff != 0) result.insert(i, j, diff);
            }
        }
        return result;
    }

    // Transpuesta de una matriz
    SparseMatrix transpose() const {
        SparseMatrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int value = search(i, j);
                if (value != 0) result.insert(j, i, value);
            }
        }
        return result;
    }

    // Multiplicación de matrices
    SparseMatrix operator*(const SparseMatrix& other) const {
        if (cols != other.rows) throw invalid_argument("Dimensiones no válidas para la multiplicación");
        SparseMatrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                int value = 0;
                for (int k = 0; k < cols; ++k) {
                    value += search(i, k) * other(k, j);
                }
                if (value != 0) result.insert(i, j, value);
            }
        }
        return result;
    }

    // Mostrar la matriz
    void display() {
        for (int i = 0; i < rows; ++i) {
            Header* rowHeader = getOrCreateRowHeader(i);
            Cell* cell = rowHeader->next;
            for (int j = 0; j < cols; ++j) {
                if (cell && cell->col == j) {
                    cout << cell->value << " ";
                    cell = cell->nextRow;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }

    ~SparseMatrix() {
        Header* rowCurr = rowHead;
        while (rowCurr) {
            Cell* cellCurr = rowCurr->next;
            do {
                Cell* toDelete = cellCurr;
                cellCurr = cellCurr->nextRow;
                delete toDelete;
            } while (cellCurr && cellCurr != rowCurr->next);
            Header* toDelete = rowCurr;
            rowCurr = rowCurr->nextHeader;
            delete toDelete;
        }
        Header* colCurr = colHead;
        while (colCurr) {
            Header* toDelete = colCurr;
            colCurr = colCurr->nextHeader;
            delete toDelete;
        }
    }
};

int main() {
    SparseMatrix sm1(3, 3);
    SparseMatrix sm2(3, 3);
    sm1.insert(0, 0, 1);
    sm1.insert(1, 1, 2);
    sm1.insert(2, 2, 3);

    sm2.insert(0, 0, 4);
    sm2.insert(1, 1, 5);
    sm2.insert(2, 2, 6);

    cout << "Matriz 1:" << endl;
    sm1.display();
    cout << "Matriz 2:" << endl;
    sm2.display();

    SparseMatrix sum = sm1 + sm2;
    cout << "Suma de Matrices:" << endl;
    sum.display();

    SparseMatrix transpose = sm1.transpose();
    cout << "Transpuesta de Matriz 1:" << endl;
    transpose.display();

    return 0;
}
