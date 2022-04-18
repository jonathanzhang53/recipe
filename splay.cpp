#include <iostream>
#include <fstream>
// SPLAY TREE PSEUDOCODE ADOPTED FROM https://algorithmtutor.com/Data-Structures/Tree/Splay-Trees/

using namespace std;

struct Node {
	int val;
	Node* p;
	Node* l;
	Node* r;
};

typedef Node* nptr;

class Splay {
private:
    nptr root;

    nptr searchHelper(nptr node, int key);
    nptr join(nptr node1, nptr node2);
    void deleteHelper(nptr node, int key);
    void rotateLeft(nptr node);
    void rotateRight(nptr node);
    void splay(nptr node);
    void split(nptr& n1, nptr& n2, nptr& n3);

public:
    Splay();
    nptr getRoot();
    nptr search(int key);
    nptr minKey(nptr node);
    nptr maxKey(nptr node);
    nptr successor(nptr node);
    nptr predecessor(nptr node);
    void preorder(nptr node);
    void inorder(nptr node);
    void postorder(nptr node);
    void insert(int key);
    void deleteN(int val);
};

Splay::Splay() {
	root = nullptr;
}

nptr Splay::getRoot() {
    return this->root;
}

nptr Splay::search(int key) {
    nptr found = searchHelper(getRoot(), key);
    if (found != nullptr) {
        splay(found);
    }
    return found;
}

nptr Splay::minKey(nptr node) {
    while (node->l != nullptr) {
        node = node->l;
    }
    return node;
}

nptr Splay::maxKey(nptr node) {
    while (node->r != nullptr) {
        node = node->r;
    }
    return node;
}

nptr Splay::successor(nptr node) {
    if (node->r != nullptr) {
        return minKey(node->r);
    }
    nptr ancestor = node->p;
    while (ancestor != nullptr && node == ancestor->r) {
        node = ancestor;
        ancestor = ancestor->p;
    }
    return ancestor; 
}

nptr Splay::predecessor(nptr node) {
    if (node->l != nullptr) {
        return maxKey(node->l);
    }
    nptr ancestor = node->p;
    while (ancestor != nullptr && node == ancestor->l) {
        node = ancestor;
        ancestor = ancestor->p;
    }
    return ancestor;
}

void Splay::preorder(nptr node) {
    if (node != nullptr) {
        cout << node->val << " ";
        preorder(node->l);
        preorder(node->r);
    } 
}

void Splay::inorder(nptr node) {
    if (node != nullptr) {
        inorder(node->l);
        cout<< node->val << " ";
        inorder(node->r);
    } 
}

void Splay::postorder(nptr node) {
    if (node != nullptr) {
        postorder(node->l);
        postorder(node->r);
        cout << node->val << " ";
    } 
}

void Splay::insert(int key) {
    nptr node = new Node;
    node->p = nullptr;
    node->l = nullptr;
    node->r = nullptr;
    node->val = key;
    nptr n1 = nullptr;
    nptr iroot = getRoot();
    while (iroot != nullptr) {
        n1 = iroot;
        if (node->val < iroot->val) {
            iroot = iroot->l;
        } 
        else {
            iroot = iroot->r;
        }
    }
    node->p = n1;
    if (n1 == nullptr) {
        root = node;
    } 
    else if (node->val < n1->val) {
        n1->l = node;
    } 
    else {
        n1->r = node;
    }
    splay(node);
}

void Splay::deleteN(int val) {
    deleteHelper(getRoot(), val);
}

nptr Splay::searchHelper(nptr node, int key) {
    if (node == nullptr || key == node->val) {
        return node;
    }
    if (key < node->val) {
        return searchHelper(node->l, key);
    } 
    return searchHelper(node->r, key);
}

nptr Splay::join(nptr node1, nptr node2) {
    if (node1 == nullptr) {
        return node2;
    }
    if (node2 == nullptr) {
        return node1;
    }
    nptr maxN = maxKey(node1);
    splay(maxN);
    maxN->r = node2;
    node2->p = maxN;
    return maxN;
}

void Splay::deleteHelper(nptr node, int key) {
    nptr search = nullptr;
    while (node != nullptr){
        if (node->val == key) {
            search = node;
        }
        if (node->val <= key) {
            node = node->r;
        } 
        else {
            node = node->l;
        }
    }
    if (search == nullptr) {
        cout << "Key not found." << endl;
        return;
    }
    nptr n1, n2;
    split(search, n1, n2);
    if (n1->l){
        n1->l->p = nullptr;
    }
    root = join(n1->l, n2);
}

void Splay::rotateLeft(nptr node) {
    nptr rnode = node->r;
    node->r = rnode->l;
    if (rnode->l != nullptr) {
        rnode->l->p = node;
    }
    rnode->p = node->p;
    if (node->p == nullptr) {
        nptr rroot = getRoot();
        rroot = rnode;
    } 
    else if (node == node->p->l) {
        node->p->l = rnode;
    } 
    else {
        node->p->r = rnode;
    }
    rnode->l = node;
    node->p = rnode;
}

void Splay::rotateRight(nptr node) {
    nptr lnode = node->l;
    node->l = lnode->r;
    if (lnode->r != nullptr) {
        lnode->r->p = node;
    }
    lnode->p = node->p;
    if (node->p == nullptr) {
        nptr rroot = getRoot();
        rroot = lnode;
    } 
    else if (node == node->p->r) {
        node->p->r = lnode;
    } 
    else {
        node->p->l = lnode;
    }
    lnode->r = node;
    node->p = lnode;
}

void Splay::splay(nptr node) {
    while (node->p) {
        if (!node->p->p) {
            // zig
            if (node == node->p->l) {
                rotateRight(node->p);
            }
            // zag 
            else {
                rotateLeft(node->p);
            }
        }
        // zig-zig
        else if (node == node->p->l && node->p == node->p->p->l) {
            rotateRight(node->p->p);
            rotateRight(node->p);
        } 
        // zag-zag
        else if (node == node->p->r && node->p == node->p->p->r) {
            rotateLeft(node->p->p);
            rotateLeft(node->p);
        } 
        // zig-zag
        else if (node == node->p->r && node->p == node->p->p->l) {
            rotateLeft(node->p);
            rotateRight(node->p);
        } 
        // zag-zig
        else {
            rotateRight(node->p);
            rotateLeft(node->p);
        }
    }
}

void Splay::split(nptr& n1, nptr& n2, nptr& n3) {
    splay(n1);
    if (n1->r) {
        n3 = n1->r;
        n3->p = nullptr;
    } 
    else {
        n3 = nullptr;
    }
    n2 = n1;
    n2->r = nullptr;
    n1 = nullptr;
}


int main() {
	Splay tree;
	tree.insert(33);
	tree.preorder(tree.getRoot());
    cout << endl;
	tree.deleteN(31);
	tree.search(33);
	tree.deleteN(33);
	tree.preorder(tree.getRoot());
    cout << endl;

	// load in csv
	// insert all rows into the tree

    fstream dat;
    dat.open("PP_recipes.csv", ios::in);
    

	return 0;
}