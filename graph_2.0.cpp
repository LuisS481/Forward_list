#include <vector>
#include <limits>
#include <iostream>

using namespace std;

struct Edge {
    int to, weight;
    Edge(int v, int w) : to(v), weight(w) {}
};

class Queue {
private:
    vector<int> data; // Almacenamiento de los elementos de la cola
    int frontIndex; // Índice del frente de la cola
    int backIndex; // Índice del final de la cola

public:
    Queue() : frontIndex(0), backIndex(0) {}

    // Agregar un elemento al final de la cola
    void push(int value) {
        data.push_back(value);
        backIndex++;
    }

    // Eliminar y retornar el elemento del frente de la cola
    int pop() {
        if (empty()) {
            throw runtime_error("La cola está vacía.");
        }
        return data[frontIndex++];
    }

    // Verificar si la cola está vacía
    bool empty() const {
        return frontIndex >= backIndex;
    }

    // Obtener el tamaño de la cola
    int size() const {
        return backIndex - frontIndex;
    }
};

class Graph {
public:
    vector<vector<Edge>> adj; // Lista de adyacencia
    int n; // Número de nodos en el grafo

    Graph(int n) : n(n) {
        adj.resize(n);
    }

    // Agregar una arista al grafo
    void addEdge(int from, int to, int weight) {
        adj[from].push_back(Edge(to, weight));
        adj[to].push_back(Edge(from, weight)); // Grafo no dirigido
    }

    // Obtener nodos adyacentes a un nodo dado
    vector<int> getAdjacentNodes(int node) {
        vector<int> neighbors;
        for (const auto& edge : adj[node]) {
            neighbors.push_back(edge.to);
        }
        return neighbors;
    }

    // BFS para recorrer el grafo desde un nodo inicial
    void bfs(int start) {
        vector<bool> visited(n, false);
        Queue q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.pop();
            cout << node << " "; // Procesar el nodo (imprimirlo en este caso)

            for (const auto& edge : adj[node]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
        cout << endl;
    }

    // DFS para recorrer el grafo desde un nodo inicial
    void dfsUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " "; // Procesar el nodo

        for (const auto& edge : adj[node]) {
            if (!visited[edge.to]) {
                dfsUtil(edge.to, visited);
            }
        }
    }

    void dfs(int start) {
        vector<bool> visited(n, false);
        dfsUtil(start, visited);
        cout << endl;
    }

    // Dijkstra para encontrar el camino más corto desde un nodo inicial
    vector<int> dijkstra(int start) {
        vector<int> distance(n, numeric_limits<int>::max()); // Distancias inicializadas a infinito
        vector<bool> visited(n, false); // Indicar si se ha visitado el nodo
        vector<int> parent(n, -1); // Arreglo para almacenar los padres en el camino
        distance[start] = 0;

        // Implementación del MinHeap manual para Dijkstra
        MinHeap minHeap(n);
        minHeap.insert(start, 0);

        while (!minHeap.empty()) {
            int u = minHeap.extractMin().first;

            if (visited[u]) continue;
            visited[u] = true;

            for (const auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;

                if (!visited[v] && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;

                    // Actualizamos el MinHeap con la nueva distancia
                    if (minHeap.contains(v)) {
                        minHeap.decreaseKey(v, distance[v]);
                    } else {
                        minHeap.insert(v, distance[v]);
                    }
                }
            }
        }
        return parent; // Retorna los padres de cada nodo en el camino más corto
    }
};
