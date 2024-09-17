#include <iostream>
//#include <cmath>

template<typename T>
struct Nodo {
    T data;
    Nodo<T>* next;
};

template<typename T>
class List {
private:
    Nodo<T>* head;

public:
    List() : head(nullptr) {}

    T front() {
//        if (head == nullptr) {
//            std::cout << "La lista esta vacia" << std::endl;;
//        }
        return head->data;
    }

    T back() {
//        if (head == nullptr) {
//            std::cout << "La lista esta vacia" << std::endl;;
//        }
        Nodo<T>* temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        return temp->data;
    }

    void push_front(T t) {
        Nodo<T>* nodo = new Nodo<T>;
        nodo->data = t;
        nodo->next = head;
        head = nodo;
    }

    void push_back(T t) {
        Nodo<T>* nodo = new Nodo<T>;
        nodo->data = t;

//        nodo->next = nullptr;
//
//        if(head == nullptr){
//            head = nodo;
//        }
//        else{
            Nodo<T>* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = nodo;
//        }
        nodo->next = NULL;
    }

    T pop_front() {
//        if (head == nullptr) {
//            std::cout << "La lista esta vacia" << std::endl;;
//        }
        Nodo<T>* temp = head;
//        T value = temp->data;
        head = head->next;
        delete temp;

//        return value;
        return head->data;
    }

    T pop_back() {
//        if (head == nullptr) {
//            std::cout << "La lista esta vacia" << std::endl;;
//        }

        if (head->next == NULL) {
            T value = head->data;
            delete head;
            head = NULL;
            return value;
        }
        else {
            Nodo<T>* temp = head;
//            Node<T>* tprev = nullptr;
//            while (temp->next != nullptr) {
//                tprev = temp;
//                temp = temp->next;
//            }

            while(temp->next->next != NULL)
                temp = temp->next;
            T value = temp->data;
            delete temp->next;
            temp->next = NULL;

            return value;
        }
    }

    T operator[](int pos) {
        Nodo<T>* temp = head;
        int j = 0;
//        while(temp != nullptr){
//
//            if(j == pos){
//                return temp->data;
//            }
//            temp = temp->next;
//            j++;
//        }
        for(int i = 0; i<pos; ++i) {
            temp = temp->next;
        }
        return temp->data;
//        return -1; //fuera de rango
    }

    bool empty() {
        return head == nullptr;
    }

    int size() {
        Nodo<T>* temp = head;
        int cont = 0; //(1)
        while(temp != nullptr) {
            temp = temp->next;
            ++cont;
        }
        return cont;
    }

//    void clear() {
//        Nodo<T>* current = head;
//        while (current != nullptr) {
//            Nodo<T>* next = current->next;
//            delete current;
//            current = next;
//        }
//        head = nullptr;
//    }

    void clear() {
        while(head != NULL) {
            Nodo<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

//  MERGE SORT ----------------------------------------

    Nodo<T>* mergeSort(Nodo<T>* head) {
        if (head == NULL || head->next == NULL) {
            return head;
        }


        Nodo<T>* middle = getMiddle(head);
        Nodo<T>* nextOfMiddle = middle->next;

        middle->next = NULL;


        Nodo<T>* left = mergeSort(head);
        Nodo<T>* right = mergeSort(nextOfMiddle);


        Nodo<T>* sortedList = merge(left, right);

        return sortedList;
    }

    Nodo<T>* merge(Nodo<T>* left, Nodo<T>* right) {

        if (left == NULL) return right;
        if (right == NULL) return left;

        Nodo<T>* result = NULL;


        if (left->data <= right->data) {
            result = left;
            result->next = merge(left->next, right);
        } else {
            result = right;
            result->next = merge(left, right->next);
        }

        return result;
    }

    Nodo<T>* getMiddle(Nodo<T>* head) {
        if (head == NULL) return head;

        Nodo<T>* slow = head;
        Nodo<T>* fast = head->next;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    void sort() {
        head = mergeSort(head);
    }

//  --------------------------------------------------

//    void sort(){
//
//    }

    void reverse(){
        Nodo<T>* prev = NULL;
        Nodo<T>* curr = head;
        Nodo<T>* next = NULL;

        while(curr != NULL) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    void insert(Nodo<T>* nodo, int pos) {
        Nodo<T>* temp = head;
        int i = 0;
        while(i++ < pos - 1) temp = temp->next;
        nodo->next = temp->next;
        temp->next = nodo;
    }

    void print() {
        Nodo<T>* temp = head;
        while (temp != NULL) {
            std::cout<< temp->data;
            temp = temp->next;
        }
    }
};
