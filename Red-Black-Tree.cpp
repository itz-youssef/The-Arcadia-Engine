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

    
    bool isRed(Node* n) {
        return n != nullptr && n->color == RED;
    }

    bool isBlack(Node* n) {
        return n == nullptr || n->color == BLACK;
    }

    
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

    
    void FixViolation(Node* node) {
        Node* parent = nullptr;
        Node* GrandParent = nullptr;

        while ((node != Root) && (node->color == RED) && (node->parent->color == RED)) {
            parent = node->parent;
            GrandParent = parent->parent;

            if (parent == GrandParent->left) {
                Node* uncle = GrandParent->right;
                if (uncle != nullptr && uncle->color == RED) {
                    GrandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = GrandParent;
                }
                else {
                    if (node == parent->right) {
                        LeftRotation(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    RightRotation(GrandParent);
                    bool tempColor = parent->color;
                    parent->color = GrandParent->color;
                    GrandParent->color = tempColor;
                    node = parent;
                }
            }
            else {
                Node* uncle = GrandParent->left;
                if ((uncle != nullptr) && (uncle->color == RED)) {
                    GrandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = GrandParent;
                }
                else {
                    if (node == parent->left) {
                        RightRotation(parent);
                        node = parent;
                        parent = node->parent;
                    }
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

    
    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            Root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    
    Node* minimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }


    void deleteFix(Node* x) {
        Node* s;
        while (x != Root && isBlack(x)) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (isRed(s)) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    LeftRotation(x->parent);
                    s = x->parent->right;
                }

                if (isBlack(s->left) && isBlack(s->right)) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (isBlack(s->right)) {
                        if (s->left) s->left->color = BLACK;
                        s->color = RED;
                        RightRotation(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (s->right) s->right->color = BLACK;
                    LeftRotation(x->parent);
                    x = Root;
                }
            }
            else {
                s = x->parent->left;
                if (isRed(s)) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    RightRotation(x->parent);
                    s = x->parent->left;
                }

                if (isBlack(s->right) && isBlack(s->left)) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (isBlack(s->left)) {
                        if (s->right) s->right->color = BLACK;
                        s->color = RED;
                        LeftRotation(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (s->left) s->left->color = BLACK;
                    RightRotation(x->parent);
                    x = Root;
                }
            }
        }
        if (x != nullptr) x->color = BLACK;
    }

    
    void deleteNodeHelper(Node* z) {
        Node* x = nullptr;
        Node* y = z;
        bool y_original_color = y->color;

        Node* dummy = new Node(0, BLACK, nullptr, nullptr, nullptr);
        bool usedDummy = false;

        if (z->left == nullptr) {
            x = z->right;
            if (x == nullptr) { x = dummy; usedDummy = true; } 
            rbTransplant(z, x);
        }
        else if (z->right == nullptr) {
            x = z->left;
            if (x == nullptr) { x = dummy; usedDummy = true; } 
            rbTransplant(z, x);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (x == nullptr) { x = dummy; usedDummy = true; } 

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                rbTransplant(y, x);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (y_original_color == BLACK) {
            deleteFix(x);
        }

        
        if (usedDummy) {
            if (x->parent != nullptr) {
                if (x->parent->left == x) x->parent->left = nullptr;
                else if (x->parent->right == x) x->parent->right = nullptr;
            }
            delete dummy;
        }
    }

    void inorderHelper(Node* root) {
        if (root == nullptr) return;
        inorderHelper(root->left);
        cout << root->value << "(" << (root->color == RED ? "R" : "B") << ") ";
        inorderHelper(root->right);
    }

public:
    RedBlackTree() {
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

    void remove(int val) {
        Node* temp = Root;
        while (temp != nullptr) {
            if (val == temp->value) {
                deleteNodeHelper(temp);
                return;
            }
            else if (val < temp->value) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        cout << "Value " << val << " not found!" << endl;
    }

    void printInorder() {
        if (Root == nullptr)
            cout << "Tree is empty";
        else
            inorderHelper(Root);
        cout << endl;
    }
};

// --- Main Function for Testing ---
int main() {
    RedBlackTree rb;

    rb.insert(30);
    rb.insert(20);
    rb.insert(40);
    rb.insert(10);
    rb.insert(25);
    
    cout << "Original Tree: \n";
    rb.printInorder();

    cout << "\nDeleting 20... \n";
    rb.remove(20);
    rb.printInorder();

    cout << "\nDeleting 10 (Leaf)... \n";
    rb.remove(10);
    rb.printInorder();
    
    cout << "\nDeleting 30 (Root)... \n";
    rb.remove(30);
    rb.printInorder();

    return 0;
}