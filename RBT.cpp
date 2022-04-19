#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Node {
   int ID;
   bool color; // false is red true is black
   Node* left = nullptr;
   Node* right = nullptr;
   Node* parent = nullptr;
   int height = 0;
   int balanceFactor = 0;

   Node(int ID) {
       this->ID = ID;
       this->color = false; //nodes start off as red in rbt
   }
};

class RBT
{
    Node* root = nullptr;
    int nodeCount;
    
    void rotateRight(Node* root, Node* node) {
        Node* temp = node->left;
        node->left = temp->right;
        if (node->left != nullptr) {
            node->left->parent = node;
        }
        temp->parent = node->parent;
        if (node->parent == nullptr) {
            root = temp;
        }
        else if (node == node->parent->left) {
            node->parent->left = temp;
        }
        else {
            node->parent->right = temp;
        }
        temp->right = node;
        node->parent = temp;
    }

    void rotateLeft(Node* root, Node* node) {
        Node* temp = node->right;
        node->right = temp->left;
        if (node->right != nullptr) {
            node->right->parent = node;
        }
        temp->parent = node->parent;
        if (node->parent == nullptr) {
            root = temp;
        }
        else if (node == node->parent->left) {
            node->parent->left = temp;
        }
        else {
            node->parent->right = temp;
        }
        temp->left = node;
        node->parent = temp;
    }

public:
    RBT() {
        this->nodeCount = 0;
    }
    
    void balancer(Node* root, Node* node) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color != true && node->parent->color == false) {
            parent = node->parent;
            grandparent = node->parent->parent;

            // Parent of node is left child of grandparent 
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                //uncle of node is also red which means need to recolor
                if (uncle != nullptr && uncle->color == false) {
                    grandparent->color = false;
                    parent->color = true;
                    uncle->color = true;
                    node = grandparent;
                }
                else {
                    //node is right of parent which needs a left rotation
                    if (node == parent->right) {
                        rotateLeft(this->root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    // node is left of its parent which needs right rotation
                    rotateRight(this->root, grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            //parent of node is right of grandparent
            else {
                Node* uncle = grandparent->left;

                //uncle of node is also red whcih requires recolor
                if (uncle != nullptr && uncle->color == false) {
                    grandparent->color = false;
                    parent->color = true;
                    uncle->color = true;
                    node = grandparent;
                }
                else {
                    //node is left of parent requires right rotation
                    if (node == parent->left) {
                        rotateRight(this->root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    // node is right child of parent
                    rotateLeft(this->root, grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = true;
    }

    Node* findByID(Node* root, const int &ID) {
        Node* temp = root;
        if (temp == nullptr) {
            cout << "";
        }
        else if (ID == temp->ID) {
            return temp;
        }
        else if (ID < temp->ID) {
            temp = findByID(temp->left, ID);
        }
        else if (ID > temp->ID) {
            temp = findByID(temp->right, ID);
        }
        return temp;
    }
    void searchID(int ID) {
        auto start = high_resolution_clock::now();
        Node* temp = findByID(this->root, ID);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds> (stop - start);
        if (temp == nullptr) {
            cout << "Recipe ID not found" << endl;
            cout << "Searching whole tree took " << duration.count() << " microseconds" << endl;
        }
        else {
            cout << "Recipe found" << endl;
            cout << "Searching for this recipe took " << duration.count() << " microseconds" << endl;
        }
    }

    Node* helperInsert(Node* helpRoot, Node* temp) {
        if (helpRoot == nullptr) {
            return temp;
        }
        if (temp->ID < helpRoot->ID) {
            helpRoot->left = helperInsert(helpRoot->left, temp);
            helpRoot->left->parent = helpRoot;
        }
        else if (temp->ID > helpRoot->ID) {
            helpRoot->right = helperInsert(helpRoot->right, temp);
            helpRoot->right->parent = helpRoot;
        }
        return helpRoot;
    }

    void insert(const int &ID) {
        Node* temp = new Node(ID);
        this->root = helperInsert(this->root, temp);
        this->nodeCount++;
        balancer(root, temp);
    }

    void printPreOrder(Node* root) {
        //Utilized similar structure to the preorder traversal code in the lecture slides
        if (root == nullptr) {
            cout << "";
        }
        else {
            //cant just have comma after every name because the rightmost leaf.
            //for preorder the second check after left half of tree will put the comma for the child
            //from the parent if the child is a leaf and is also not the last one
            cout << root->ID;
            if (root->right != nullptr || root->left != nullptr) {
                cout << ", ";
            }
            printPreOrder(root->left);
            if (root->right != nullptr && root->left != nullptr) {
                cout << ", ";
            }
            printPreOrder(root->right);
        }
    }
    
    Node* getRoot() {
        return root;
    }

    ~RBT() {}
};

void loadData(string filePath, RBT& rbt) {
	ifstream inFile(filePath);
	if (inFile.is_open()) {
		// each file has a header 
		string lineFromFile;
		getline(inFile, lineFromFile);
        getline(inFile, lineFromFile);

		//int index = 0;
		while (getline(inFile, lineFromFile)) {
			istringstream stream(lineFromFile);
	        string token1; // ID
            string restOfLine; // rest
            getline(stream, token1, ',');
            getline(stream, restOfLine);
			rbt.insert(stoi(token1));
            //index++;
            //if (index > 100000)
            //{
            //    break;
            //}
		}
	}
}

int main() {
    RBT rbt;
    loadData("PP_recipes.csv", rbt);
    // cout << "inorder of rbt: " << endl;
    // rbt.printPreOrder(rbt.getRoot());

    cout << "\nData Loaded. Search for something now (input an integer)" << endl;
    int input;
    cin >>input;
    rbt.searchID(input);
}