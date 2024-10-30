#include <vector>
using namespace std;

template <typename V, typename E>
class Graph
{
public:
    typedef Graph<V, E> self; // Definimos un tipo que se refiere a la misma clase para usarlo internamente
    typedef Node<self> Node;  // Definimos un tipo para representar nodos en el grafo
    typedef Edge<self> Edge;  // Definimos un tipo para representar aristas en el grafo
    typedef vector<Node *> NodeSeq; // Secuencia de nodos representada como un vector de punteros a Node
    typedef vector<vector<Edge *>> EdgeMatrix; // Matriz de adyacencia usando vector de vectores de punteros a Edge
    
private:
    NodeSeq nodes;         // Vector para almacenar los nodos del grafo
    EdgeMatrix edges;      // Matriz de adyacencia para almacenar las aristas

public:
    // Método para agregar un nuevo nodo al grafo
    void addNode(V data) {
        Node* newNode = new Node(data); // Creamos un nuevo nodo con el dato dado
        nodes.push_back(newNode);       // Añadimos el nodo al vector de nodos
        edges.resize(nodes.size());     // Redimensionamos la matriz de adyacencia
        for (auto& row : edges) {
            row.resize(nodes.size(), nullptr); // Añadimos una nueva columna con nullptr para cada fila
        }
    }
    
    // Método para agregar una arista entre dos nodos
    void addEdge(int from, int to, E weight) {
        if (from < nodes.size() && to < nodes.size()) { // Verificamos que los índices existan
            Edge* newEdge = new Edge(nodes[from], nodes[to], weight); // Creamos la arista
            edges[from][to] = newEdge; // Asignamos la arista en la posición de la matriz de adyacencia
        }
    }
};

template <typename G>
class Node
{
public:
    typedef typename G::V V;     // Tipo para el dato almacenado en el nodo
    typedef typename G::Edge Edge; // Tipo para representar una arista en el grafo

    V data; // Dato almacenado en el nodo
    
    // Constructor del nodo
    Node(V value) : data(value) {} // Inicializamos el nodo con el dato proporcionado
};

template <typename G>
class Edge
{
public:
    typedef typename G::E E;    // Tipo de dato para el peso de la arista
    typedef typename G::Node Node; // Tipo para representar un nodo en el grafo

    Node* from;  // Nodo de inicio de la arista
    Node* to;    // Nodo de destino de la arista
    E weight;    // Peso de la arista

    // Constructor de la arista
    Edge(Node* start, Node* end, E w) : from(start), to(end), weight(w) {} // Inicializamos la arista
};
