#include <vector>
#include <limits>

using namespace std;

// Estructura para representar una arista con su peso
struct Edge {
    int to, weight;
    Edge(int v, int w) : to(v), weight(w) {}
};

// Implementación manual de un MinHeap
class MinHeap {
private:
    vector<pair<int, int>> heap; // Arreglo de pares {nodo, distancia}
    vector<int> pos; // Arreglo para guardar las posiciones de los nodos en el heap

    // Función para mantener la propiedad del min-heap al subir un nodo
    void siftUp(int idx) {
        while (idx > 0 && heap[idx].second < heap[(idx - 1) / 2].second) {
            swap(pos[heap[idx].first], pos[heap[(idx - 1) / 2].first]);
            swap(heap[idx], heap[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }

    // Función para mantener la propiedad del min-heap al bajar un nodo
    void siftDown(int idx) {
        int n = heap.size();
        while (2 * idx + 1 < n) {
            int j = 2 * idx + 1;
            if (j + 1 < n && heap[j + 1].second < heap[j].second) j++;
            if (heap[idx].second <= heap[j].second) break;
            swap(pos[heap[idx].first], pos[heap[j].first]);
            swap(heap[idx], heap[j]);
            idx = j;
        }
    }

public:
    MinHeap(int n) : pos(n, -1) {}

    // Insertar un nuevo nodo con su distancia en el heap
    void insert(int node, int dist) {
        heap.push_back({node, dist});
        pos[node] = heap.size() - 1;
        siftUp(heap.size() - 1);
    }

    // Actualizar la distancia de un nodo existente en el heap
    void decreaseKey(int node, int newDist) {
        int idx = pos[node];
        heap[idx].second = newDist;
        siftUp(idx);
    }

    // Extraer el nodo con la menor distancia
    pair<int, int> extractMin() {
        pair<int, int> minNode = heap[0];
        pos[heap[0].first] = -1;
        heap[0] = heap.back();
        pos[heap[0].first] = 0;
        heap.pop_back();
        siftDown(0);
        return minNode;
    }

    // Verificar si el heap está vacío
    bool empty() const {
        return heap.empty();
    }

    // Verificar si un nodo está en el heap
    bool contains(int node) const {
        return pos[node] != -1;
    }
};

class Graph {
public:
    vector<vector<Edge>> adj; // Lista de adyacencia para el grafo
    int n; // Número de nodos en el grafo

    // Constructor para inicializar un grafo con `n` nodos
    Graph(int n) : n(n) {
        adj.resize(n);
    }

    // Método para agregar una arista al grafo
    void addEdge(int from, int to, int weight) {
        adj[from].push_back(Edge(to, weight));
        adj[to].push_back(Edge(from, weight)); // Grafo no dirigido
    }

    // Algoritmo de Prim para hallar el MST
    vector<int> prim(int start) {
        vector<int> parent(n, -1); // Arreglo para almacenar los padres del MST
        vector<int> distance(n, numeric_limits<int>::max()); // Distancia inicializada a infinito
        vector<bool> inMST(n, false); // Indica si el nodo ya está en el MST
        MinHeap minHeap(n); // Nuestro MinHeap manual

        // Inicialización: distancia del nodo fuente a 0
        distance[start] = 0;
        minHeap.insert(start, 0);

        while (!minHeap.empty()) {
            // Extraemos el nodo con la menor distancia
            int u = minHeap.extractMin().first;
            inMST[u] = true;

            // Explorar nodos adyacentes a `u`
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;

                // Si `v` no está en el MST y encontramos una mejor distancia
                if (!inMST[v] && weight < distance[v]) {
                    distance[v] = weight;
                    parent[v] = u;
                    
                    // Actualizamos el MinHeap con la nueva distancia
                    if (minHeap.contains(v)) {
                        minHeap.decreaseKey(v, weight);
                    } else {
                        minHeap.insert(v, weight);
                    }
                }
            }
        }

        return parent; // Retorna los padres de cada nodo en el MST
    }
};
