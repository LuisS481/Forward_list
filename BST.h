#include <iostream>
#include <vector>

template<typename T>
class BSTree {
private:
    struct NodeBT {
        T data;
        NodeBT* left;
        NodeBT* rigth;
//        auto fb;
        NodeBT(T value): data(value), left(nullptr), rigth(nullptr) {}
    };

    NodeBT* root;
public:
    BSTree() : root(nullptr) {}
//  insertar y borrar. Además, con un iterador se debería poder recorrer
    bool find(NodeBT* node, T value) {
        if(node == nullptr) // BASE CASE
            return false;
        else if(value < node->data)
            return find(node->left, value);
        else if(value > node->data)
            return find(node->rigth, value);
        else return true;
    }

    void insert(NodeBT* node, T value) {
        NodeBT* leaf;
        leaf->data = value;

        NodeBT* father = insertaux(node, value);
        if(value < father->data)
            father->left = node;
        else if(value > father->data)
            father->rigth = node;
    }

    NodeBT* insertaux(NodeBT* node, T value){
        if(value < node->data && node->left != nullptr)
            return insertaux(node->left, value);
        else if(value > node->data && node->rigth != nullptr)
            return insertaux(node->rigth, value);
        else
            return node;
    }

    void remove(NodeBT* node){
        if(node->left == nullptr && node->rigth == nullptr) {
            delete node;
            return;
        }
        else if(node->left != nullptr && node->rigth == nullptr) {
            node = node->left;
            node->left = nullptr;
            return;
        }
        else if(node->rigth != nullptr && node->left == nullptr) {
            node = node->rigth;
            node->rigth = nullptr;
            return;
        }
        else if(node->left != nullptr && node->rigth != nullptr){

        }
    }

    std::vector<T> in_order(BSTree* node){
        if(node->left != nullptr)
            in_order(node->left);

    }

    //Iterador?
};

int main() {
    BSTree<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    cout << "In-order traversal: ";
    bst.inorder(); // Debería imprimir los valores en orden ascendente

    cout << "Find 40: " << (bst.find(40) ? "Found" : "Not found") << endl;
    cout << "Find 100: " << (bst.find(100) ? "Found" : "Not found") << endl;

    return 0;
}
