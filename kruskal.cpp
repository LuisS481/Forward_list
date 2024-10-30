#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Clase de conjuntos disjuntos para Kruskal
class DisjointSet {
public:
    vector<int> parent, rank;

    // Constructor para inicializar `n` conjuntos
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i; // Cada nodo es su propio padre
    }

    // Método Find: encuentra el representante del conjunto
    int Find(int x) {
        if (parent[x] != x) parent[x] = Find(parent[x]); // Compresión de ruta
        return parent[x];
    }

    // Método Union: une dos conjuntos por rango
    void Union(int x, int y) {
        int rootX = Find(x);
        int rootY = Find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY])
                parent[rootY] = rootX;
            else if (rank[rootX] < rank[rootY])
                parent[rootX] = rootY;
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

// Estructura para representar una arista con su peso
struct Edge {
    int from, to, weight;
    Edge(int u, int v, int w) : from(u), to(v), weight(w) {}
};

template <typename V>
class Graph {
public:
    vector<V> nodes;                // Vector para almacenar los nodos
    vector<Edge> edges;             // Vector para almacenar las aristas con sus pesos

    // Método para agregar un nodo al grafo
    void addNode(V data) {
        nodes.push_back(data);
    }

    // Método para agregar una arista entre dos nodos
    void addEdge(int from, int to, int weight) {
        edges.emplace_back(from, to, weight); // Añade la arista con el peso dado
    }

    // Algoritmo de Kruskal para hallar el árbol de expansión mínima (MST)
    vector<Edge> kruskalMST() {
        vector<Edge> mst; // Vector para almacenar las aristas del MST
        DisjointSet ds(nodes.size()); // Conjunto disjunto para manejar los conjuntos de nodos

        // Ordenar las aristas por peso
        sort(edges.begin(), edges.end(), [](Edge& a, Edge& b) { return a.weight < b.weight; });

        // Iterar sobre las aristas ordenadas
        for (const auto& edge : edges) {
            int u = edge.from;
            int v = edge.to;

            // Solo agregar la arista si los nodos están en conjuntos separados
            if (ds.Find(u) != ds.Find(v)) {
                mst.push_back(edge); // Añadir la arista al MST
                ds.Union(u, v); // Unir los conjuntos de los nodos u y v
            }
        }

        return mst; // Retornar las aristas del MST
    }
};
