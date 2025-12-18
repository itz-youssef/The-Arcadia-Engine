// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // TODO: Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    // TODO: Define your skip list node structure and necessary variables
    // Hint: You'll need nodes with multiple forward pointers

public:
    ConcreteLeaderboard() {
        // TODO: Initialize your skip list
    }

    void addScore(int playerID, int score) override {
        // TODO: Implement skip list insertion
        // Remember to maintain descending order by score
    }

    void removePlayer(int playerID) override {
        // TODO: Implement skip list deletion
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        return {};
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---
class Node {
public:
    int id;
    int price;
    bool color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int i_d, int val, bool cl, Node* lt, Node* rt, Node* pt) {
        id = i_d;
        price = val;
        color = cl;
        left = lt;
        right = rt;
        parent = pt;
    }
};


class ConcreteAuctionTree : public AuctionTree {
private:
    Node* Root = nullptr;
    const bool RED = 1;
    const bool BLACK = 0;

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

        
        Node* dummy = nullptr;
        bool usedDummy = false;

        if (z->left == nullptr) {
            x = z->right;
            if (x == nullptr) { 
                x = new Node(0,0, BLACK, nullptr, nullptr, nullptr);
                usedDummy = true; 
            } 
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
    // TODO: Define your Red-Black Tree node structure
    // Hint: Each node needs: id, price, color, left, right, parent pointers

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
        Root = nullptr;
    }

    void insertItem(int itemID, int price) override {
        Node* newNode = new Node(itemID,price, RED, nullptr, nullptr, nullptr);
        if (Root == nullptr) {
            newNode->color = BLACK;
            Root = newNode;
            return;
        }

        Node* temp = Root;
        Node* parent = nullptr;

        while (temp != nullptr) {
            parent = temp;
            if (itemID < temp->id)
                temp = temp->left;
            else
                temp = temp->right;
        }

        newNode->parent = parent;
        if (itemID < parent->id)
            parent->left = newNode;
        else
            parent->right = newNode;

        FixViolation(newNode);
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
    }

    void deleteItem(int itemID) override {
         Node* temp = Root;
        while (temp != nullptr) {
            if (itemID == temp->id) {
                deleteNodeHelper(temp);
                return;
            }
            else if (itemID < temp->id) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        cout << "Item ID " << itemID << " not found!" << endl;
        // TODO: Implement Red-Black Tree deletion
        // This is complex - handle all cases carefully
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += coins[i];
    }
    int half = total / 2;
    vector<int> dp(half + 1, 0);
    dp[0] = 1;
    for (int c : coins) {
        for (int s = half; s >= c; s--) {
            if (dp[s - c]) {
                dp[s] = 1;
            }
        }
    }
    int best = 0;
    for (int s = half; s >= 0; s--) {
        if (dp[s]) {
            best = s;
            break;
        }
    }
    return abs(total - 2 * best);
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    vector<int> dp(capacity + 1, 0);
    for (auto& it : items) {
        int w = it.first;
        int v = it.second;
        for (int c = capacity; c >= w; c--) {
            dp[c] = max(dp[c], dp[c - w] + v);
        }
    }
    return dp[capacity];
}

long long InventorySystem::countStringPossibilities(string s) {
    const long long MOD = 1000000007;
    int n = s.size();

    vector<long long> dp(n + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        dp[i] = dp[i - 1];
        if (i >= 2) {
            if (s[i - 2] == 'u' && s[i - 1] == 'u') {
                dp[i] = (dp[i] + dp[i - 2]) % MOD;
            }
            if (s[i - 2] == 'n' && s[i - 1] == 'n') {
                dp[i] = (dp[i] + dp[i - 2]) % MOD;
            }
        }
    }

    return dp[n];
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    if (source == dest) return true;

        // Build Adjacency List
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // BFS Initialization
        vector<bool> visited(n, false);
        queue<int> q;

        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == dest) return true;

            for (int neighbor : adj[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

    return false;
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {
    struct DSU {
        vector<int> parent;
        DSU(int n) {
            parent.resize(n);
            for (int i = 0; i < n; i++) parent[i] = i;
        }
        int find(int x) {
            if (parent[x] == x) return x;
            return parent[x] = find(parent[x]); // Path compression
        }
        bool unite(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY) {
                parent[rootY] = rootX;
                return true;
            }
            return false;
        }
    };

    struct Edge {
        int u, v;
        long long totalCost;
        bool operator<(const Edge& other) const {
            return totalCost < other.totalCost;
        }
    };

    vector<Edge> edges;
    for (const auto& row : roadData) {
        int u = row[0];
        int v = row[1];
        long long gold = row[2];
        long long silver = row[3];
        long long total = (gold * goldRate) + (silver * silverRate);
        edges.push_back({u, v, total});
    }


    sort(edges.begin(), edges.end());

    DSU dsu(n);
    long long minTotalCost = 0;
    int edgesIncluded = 0;

    for (const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            minTotalCost += edge.totalCost;
            edgesIncluded++;
        }
    }

    if (edgesIncluded == n - 1) {
        return minTotalCost;
    } else {
        return -1; 
    }
}



string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
 
    const long long INF = 1e15; 
    vector<vector<long long>> dist(n, vector<long long>(n, INF));

    for (int i = 0; i < n; i++) dist[i][i] = 0;

    
    for (const auto& road : roads) {
        int u = road[0];
        int v = road[1];
        long long w = road[2];
        if (w < dist[u][v]) {
            dist[u][v] = dist[v][u] = w;
        }
    }

    
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    long long totalSum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (dist[i][j] != INF) {
                totalSum += dist[i][j];
            }
        }
    }

    if (totalSum == 0) return "0";
    
    string binary = "";
    while (totalSum > 0) {
        binary = (totalSum % 2 == 0 ? "0" : "1") + binary;
        totalSum /= 2;
    }

    return binary;
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting
    return 0;
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
    PlayerTable* createPlayerTable() { 
        return new ConcretePlayerTable(); 
    }

    Leaderboard* createLeaderboard() { 
        return new ConcreteLeaderboard(); 
    }

    AuctionTree* createAuctionTree() { 
        return new ConcreteAuctionTree(); 
    }
}
