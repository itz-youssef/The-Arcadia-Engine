#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <string>
#include <climits>
#include <bitset>

using namespace std;

// ==========================================
// Part 1: Safe Passage (Path Existence)
// ==========================================
class SafePassage {
public:
    // Structure to represent a simple edge
    struct Edge {
        int u, v;
    };

    bool hasPath(int n, const vector<Edge>& edges, int source, int dest) {
        if (source == dest) return true;

        // Build Adjacency List
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge.u].push_back(edge.v);
            adj[edge.v].push_back(edge.u);
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
};

// ==========================================
// Part 2: The Bribe (Min Cost Path/MST Variant)
// ==========================================
class TheBribe {
public:
    struct Road {
        int u, v;
        int goldCost;
        int silverCost;
    };

private:
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

        void unite(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY) {
                parent[rootY] = rootX;
            }
        }
    };

public:
    long long getMinConnectivityCost(int n, const vector<Road>& roads, int goldRate, int silverRate) {
        // 1. Calculate weighted edges based on current rates
        struct WeightedEdge {
            int u, v;
            long long cost;
            bool operator<(const WeightedEdge& other) const {
                return cost < other.cost;
            }
        };

        vector<WeightedEdge> edges;
        for (const auto& road : roads) {
            long long totalCost = (long long)road.goldCost * goldRate + (long long)road.silverCost * silverRate;
            edges.push_back({road.u, road.v, totalCost});
        }

        // 2. Sort edges by cost (Ascending)
        sort(edges.begin(), edges.end());

        // 3. Kruskal's Algorithm
        DSU dsu(n);
        long long minTotalCost = 0;
        int edgesCount = 0;

        for (const auto& edge : edges) {
            if (dsu.find(edge.u) != dsu.find(edge.v)) {
                dsu.unite(edge.u, edge.v);
                minTotalCost += edge.cost;
                edgesCount++;
            }
        }
        return minTotalCost;
    }
};

// ==========================================
// Part 3: The Teleporter Network (All-Pairs Shortest Path)
// ==========================================
class TeleporterNetwork {
public:
    struct WeightedRoad {
        int u, v, weight;
    };

    string getAllPairsSumBinary(int n, const vector<WeightedRoad>& roads) {
        // Initialize Distance Matrix with Infinity
        // Using a large number but safe from overflow when adding
        const long long INF = 1e15; 
        vector<vector<long long>> dist(n, vector<long long>(n, INF));

        // Distance to self is 0
        for (int i = 0; i < n; i++) {
            dist[i][i] = 0;
        }

        // Populate initial edges
        for (const auto& road : roads) {
            dist[road.u][road.v] = min(dist[road.u][road.v], (long long)road.weight);
            dist[road.v][road.u] = min(dist[road.v][road.u], (long long)road.weight); 
        }

        // Floyd-Warshall Algorithm
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        // Calculate Sum of all reachable pairs
        long long totalSum = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j && dist[i][j] != INF) {
                    totalSum += dist[i][j];
                }
            }
        }

        return toBinary(totalSum);
    }

private:
    // Helper to convert decimal to binary string (removing leading zeros)
    string toBinary(long long n) {
        if (n == 0) return "0";
        string binary = "";
        while (n > 0) {
            binary = (n % 2 == 0 ? "0" : "1") + binary;
            n /= 2;
        }
        return binary;
    }
};

// ==========================================
// Main
// ==========================================
int main() {

    
    // 1. Safe Passage
    SafePassage solver1;
    vector<SafePassage::Edge> p1Edges = {{0,1}, {1,2}};
    bool exists = solver1.hasPath(3, p1Edges, 0, 2);

    // 2. The Bribe
    TheBribe solver2;
    vector<TheBribe::Road> p2Roads = {{0,1,10,0}, {1,2,5,0}, {0,2,20,0}};
    long long cost = solver2.getMinConnectivityCost(3, p2Roads, 1, 1);

    // 3. Teleporter
    TeleporterNetwork solver3;
    vector<TeleporterNetwork::WeightedRoad> p3Roads = {{0,1,2}, {0,2,8}};
    string binSum = solver3.getAllPairsSumBinary(3, p3Roads);
    
    cout << exists <<endl;
    cout << cost <<endl;
    cout << binSum <<endl;

    return 0;
}