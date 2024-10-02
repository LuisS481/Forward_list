#include <iostream>

struct TreeNode {
    int Entry;         // llave
    int count;        // contador
    int bal;          // -1, 0, +1
    TreeNode *LeftNode, *RightNode; //subárboles
};
typedef TreeNode* TreePointer;

void SearchInsert(int x, TreePointer &pA, bool &h) {
    TreePointer pB, pC;

    if(pA == nullptr) { // insertar
        pA = new TreeNode;
        h = true;
        pA->Entry = x;
        pA->count = 1;
        pA->LeftNode = pA->RightNode = nullptr;
        pA->bal = 0;
    }
    else
        if(x < pA->Entry) {
            SearchInsert(x, pA->LeftNode, h);
            if(h) {                              // el subárbol izquierdo creció
                switch (pA->bal) {
                    case -1: pA->bal = 0; h = false; break;
                    case  0: pA->bal = +1;           break;
                    case +1: pB = pA->LeftNode;
                             if(pB->bal == +1) { // rotación LL
                                pA->LeftNode = pB->RightNode;  pB->RightNode = pA;
                                pA->bal = 0;                   pA = pB;
                             }
                             else {              // rotación LR
                                pC = pB->RightNode;   pB->RightNode = pC->LeftNode;
                                pC->LeftNode  = pB;   pA->LeftNode = pC->RightNode;
                                pC->RightNode = pA;
                                if(pC->bal == +1) pA->bal = -1; else pA->bal = 0;
                                if(pC->bal == -1) pB->bal = +1; else pB->bal = 0;
                                pA = pC;
                             }
                             pA->bal = 0; h = false;
                } // switch
            }
        }
    else
        if(x > pA->Entry) {
            SearchInsert(x, pA->RightNode, h);
            if(h) {                          // el subárbol derecho creció
                switch (pA->bal) {
                    case +1: pA->bal = 0; h = false; break;
                    case  0: pA->bal = -1;           break;
                    case -1: pB = pA->RightNode;
                        if(pB->bal == -1) {  // rotación RR
                            pA->RightNode = pB->LeftNode;  pB->LeftNode = pA;
                            pA->bal = 0;                   pA = pB;
                        }
                        else {               // rotación RL
                            pC = pB->LeftNode;    pB->LeftNode = pC->RightNode;
                            pC->RightNode = pB;   pA->RightNode = pC->LeftNode;
                            pC->LeftNode = pA;
                            if(pC->bal == -1) pA->bal = +1; else pA->bal = 0;
                            if(pC->bal == +1) pB->bal = -1; else pB->bal = 0;
                            pA = pC;
                        }
                        pA->bal = 0; h = false;
                } // switch
            }
        }
        else // elemento encontrado
            pA->count++;
}

void balanceL(TreePointer &pA, bool &h) {
    TreePointer pB, pC;
    int balB, balC;
    // subárbol izquierdo reducido
    switch(pA->bal) {
        case +1: pA->bal = 0;             break;
        case  0: pA->bal = -1; h = false; break;
        case -1:
            pB = pA->RightNode; balB = pB->bal;
            if (balB <= 0) { // rotación RR
                pA->RightNode = pB->LeftNode;
                pB->LeftNode = pA;
                if (balB == 0) {
                    pA->bal = -1; pB->bal = +1; h = false;
                }
                else {
                    pA->bal = 0; pB->bal = 0;
                }
                pA = pB;
            }
            else { // rotación RL
                pC = pB->LeftNode; balC = pC->bal;
                pB->LeftNode = pC->RightNode;
                pC->RightNode = pB;
                pA->RightNode = pC->LeftNode;
                pC->LeftNode = pA;
                if (balC == -1) pA->bal = +1; else pA->bal = 0;
                if (balC == +1) pB->bal = -1; else pB->bal = 0;
                pA = pC; pC->bal = 0;
            }
    }
}

void balanceR(TreePointer &pA, bool &h) {
    TreePointer pB, pC;
    int balB, balC;
    // subárbol derecho reducido
    switch(pA->bal) {
        case -1: pA->bal = 0;             break;
        case  0: pA->bal = +1; h = false; break;
        case +1:
            pB = pA->LeftNode; balB = pB->bal;
            if(balB >= 0) { // rotación LL
                pA->LeftNode = pB->RightNode;
                pB->RightNode = pA;
                if(balB == 0) {
                    pA->bal = +1; pB->bal = -1; h = false;
                }
                else {
                    pA->bal = 0;  pB->bal = 0;
                }
                pA = pB;
            }
            else { // rotación LR
                pC = pB->RightNode; balC = pC->bal;
                pB->RightNode = pC->LeftNode;
                pC->LeftNode = pB;
                pA->LeftNode = pC->RightNode;
                pC->RightNode = pA;
                if(balC==+1) pA->bal=-1; else pA->bal=0;
                if(balC==-1) pB->bal=+1; else pB->bal=0;
                pA = pC;  pC->bal = 0;
            }
    }
}

void DelMin(TreePointer &q, TreePointer &r, bool &h) {
    if(r->LeftNode != nullptr) {
        DelMin(q, r->LeftNode, h);
        if(h)
            balanceL(r,h);
    }
    else {
        q->Entry = r->Entry;
        q->count = r->count;
        q = r;
        r = r->RightNode;
        h = true;
    }
}

void Delete(int x, TreePointer &p, bool &h) {
    TreePointer q;

    if(p == nullptr) {
        std::cout << "Elemento inexistente";
        abort();
    }
    if(x < p->Entry) {
        Delete(x,p->LeftNode,h);
        if(h)
            balanceL(p,h);
    }
    else
        if(x > p->Entry) {
            Delete(x,p->RightNode,h);
            if(h)
                balanceR(p,h);
        }
        else { // remover p->
            q = p;
            if(q->RightNode == nullptr) {
                p = q->LeftNode;
                h = true;
            }
            else
                if(q->LeftNode == nullptr) {
                    p = q->RightNode;
                    h = true;
                }
                else {
                    DelMin(q,q->RightNode,h);
                    if(h)
                        balanceR(p,h);
                }
            delete q;
        }
}
