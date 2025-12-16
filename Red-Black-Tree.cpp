#include <iostream>
using namespace std;

class Node{
    public:
        int value;
        bool color; // Black = 0 
        Node* left;
        Node* right;
        Node* parent;
    
    
        Node(int val, bool cl, Node* lt, Node* rt, Node* pt){
            value = val;
            color = cl;
            left = lt;
            right = rt;
            parent = pt;
        }
};


class RedBlackTree{
    private:
        Node* Root = nullptr;

        Node* insertHelper(Node* root, Node* parent, int val){
            if (root == nullptr){  // Base Case
                Node* newNode = new Node(val, 1, nullptr, nullptr, parent);
                FixViolation(newNode);
                return newNode;
            } 
            if (val < root->value) root->left = insertHelper(root->left, root, val);
            else if (val > root->value) root->right = insertHelper(root->right, root, val);
            return root;
        }
        
        void LeftRotation(Node*& node) { 
            Node* right_child = node->right; 
            node->right = right_child->left;
            
            if (node->right != nullptr) node->right->parent = node;

            right_child->parent = node->parent;

            if (node->parent == nullptr) Root = right_child; 
            else if (node == node->parent->left) node->parent->left = right_child;
            else node->parent->right = right_child;

            right_child->left = node;
            node->parent = right_child;
        }

        void RightRotation(Node*& ptr) { 
            Node* left_child = ptr->left; 
            ptr->left = left_child->right;

            if (ptr->left != nullptr) ptr->left->parent = ptr;

            left_child->parent = ptr->parent;

            if (ptr->parent == nullptr) Root = left_child; 
            else if (ptr == ptr->parent->left) ptr->parent->left = left_child; 
            else ptr->parent->right = left_child;
            
            left_child->right = ptr;
            ptr->parent = left_child;
        }

        void FixViolation(Node*& node){
            Node* parent = nullptr;
            Node* GrandParent = nullptr;
            while ((node != Root) && (node->color == 1) && (node->parent->color == 1)){
                parent = node->parent;
                GrandParent = parent->parent;

                // if Parent is the left for the grandparent
                if(GrandParent->left == parent ){
                    Node* uncle = GrandParent->right;
                    if (uncle != nullptr && uncle->color == 1) {
                        GrandParent->color = 1;
                        parent->color = 0;      
                        uncle->color = 0;      
                        node = GrandParent;     
                    }
                    else{
                        if (node == parent->right){ // triangle Case
                            LeftRotation(parent);
                            node = parent;
                            parent = node->parent;
                        }
                        
                        // Line Case
                        RightRotation(GrandParent);
                        int tempColor = parent->color;
                        parent->color = GrandParent->color;
                        GrandParent->color = tempColor;
                        node = parent;
                        
                    }

                }
                else{
                    Node* uncle = GrandParent->left;
                    if ((uncle != nullptr) && (uncle->color == 1)) {
                        GrandParent->color = 1;
                        parent->color = 0;
                        uncle->color = 0;
                        node = GrandParent;
                    }
                    else { 
                        if (node == parent->left) {  // triangle Case
                            RightRotation(parent);
                            node = parent;
                            parent = node->parent;
                        }
                        
                        // Line Case
                        LeftRotation(GrandParent);
                        int tempColor = parent->color;
                        parent->color = GrandParent->color;
                        GrandParent->color = tempColor;
                        node = parent;
                    }
                }

            }
            Root->color = 0;
        }

    public:
        

        void insert(int val){
            if (Root == nullptr) Root = new Node(val,0,nullptr,nullptr,nullptr);
            else insertHelper(Root, nullptr, val); 
        }
};

int main(){

}