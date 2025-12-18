#include <iostream>
using namespace std;

const bool RED = 1;
const bool BLACK = 0;

class Node {
public:
    int value;
    bool color; 
    Node* left;
    Node* right;
    Node* parent;

    Node(int val, bool cl, Node* lt, Node* rt, Node* pt) {
        value = val;
        color = cl;
        left = lt;
        right = rt;
        parent = pt;
    }
};

class RedBlackTree {
private:
    Node* Root = nullptr;

    void LeftRotation(Node* node) {
        Node* right_child = node->right;
        node->right = right_child->left;

        if (node->right != nullptr)
            node->right->parent = node;

        right_child->parent = node->parent;

        if (node->parent == nullptr)
            Root = right_child;
        else if (node == node->parent->left)
            node->parent->left = right_child;
        else
            node->parent->right = right_child;

        right_child->left = node;
        node->parent = right_child;
    }

    void RightRotation(Node* node) {
        Node* left_child = node->left;
        node->left = left_child->right;

        if (node->left != nullptr)
            node->left->parent = node;

        left_child->parent = node->parent;

        if (node->parent == nullptr)
            Root = left_child;
        else if (node == node->parent->left)
            node->parent->left = left_child;
        else
            node->parent->right = left_child;

        left_child->right = node;
        node->parent = left_child;
    }

    // --- Rebalancing Logic ---

    void FixViolation(Node* node) {
        Node* parent = nullptr;
        Node* GrandParent = nullptr;

        // Run while the parent is RED
        while ((node != Root) && (node->color == RED) && (node->parent->color == RED)) {
            parent = node->parent;
            GrandParent = parent->parent;

            // Parent is the LEFT child of GrandParent
            if (parent == GrandParent->left) {
                Node* uncle = GrandParent->right;

                // Case 1: Uncle is RED (Recolor only)
                if (uncle != nullptr && uncle->color == RED) {
                    GrandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = GrandParent; 
                }
                else {
                    // Case 2: Uncle is BLACK and Node is RIGHT child (Triangle Case)
                    // We need a Left Rotation to transform it into a Line Case
                    if (node == parent->right) {
                        LeftRotation(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // Case 3: Uncle is BLACK and Node is LEFT child (Line Case)
                    // We need a Right Rotation on GrandParent and swap colors
                    RightRotation(GrandParent);
                    bool tempColor = parent->color;
                    parent->color = GrandParent->color;
                    GrandParent->color = tempColor;
                    node = parent;
                }
            }
            // Parent is the RIGHT child of GrandParent 
            else {
                Node* uncle = GrandParent->left;

                // Case 1: Uncle is RED
                if ((uncle != nullptr) && (uncle->color == RED)) {
                    GrandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = GrandParent;
                }
                else {
                    // Case 2: Uncle is BLACK and Node is LEFT child (Triangle Case)
                    if (node == parent->left) {
                        RightRotation(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // Case 3: Uncle is BLACK and Node is RIGHT child (Line Case)
                    LeftRotation(GrandParent);
                    bool tempColor = parent->color;
                    parent->color = GrandParent->color;
                    GrandParent->color = tempColor;
                    node = parent;
                }
            }
        }
        
        Root->color = BLACK;
    }

    
    void inorderHelper(Node* root) {
        if (root == nullptr) return;
        inorderHelper(root->left);
        cout << root->value << "(" << (root->color == RED ? "R" : "B") << ") ";
        inorderHelper(root->right);
    }

public:
    RedBlackTree(){ 
        Root = nullptr; 
    }

    void insert(int val) {
        Node* newNode = new Node(val, RED, nullptr, nullptr, nullptr); 
        if (Root == nullptr) {
            newNode->color = BLACK;
            Root = newNode;
            return;
        }

        Node* temp = Root;
        Node* parent = nullptr;

        while (temp != nullptr) {
            parent = temp;
            if (val < temp->value)
                temp = temp->left;
            else
                temp = temp->right;
        }

        newNode->parent = parent;
        if (val < parent->value)
            parent->left = newNode;
        else
            parent->right = newNode;

        FixViolation(newNode);
    }

    void printInorder() {
        if (Root == nullptr) 
            cout << "Tree is empty";
        else 
            inorderHelper(Root);
        cout << endl;
    }
};

int main() {
    RedBlackTree rb;

    rb.insert(1);
    rb.insert(2);
    rb.insert(3); 
    rb.insert(6);
    rb.insert(7);
    rb.insert(4);
    rb.insert(5);


    cout << "Inorder Traversal (Value + Color): \n";
    rb.printInorder();

    return 0;
}