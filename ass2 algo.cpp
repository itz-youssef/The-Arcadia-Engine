#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <limits>
using namespace std;

struct Player {
    int id;
    string name;
    int score;
    Player() : id(-1), name(""), score(0) {}
    Player(int i, const std::string& n, int s) : id(i), name(n), score(s) {}
};

class PlayerTable {
private:
    static const int TABLE_SIZE = 101;
    std::vector<Player> table;
    std::vector<bool> occupied;

    int hash1(int key) const {
        return key % TABLE_SIZE;
    }

    int hash2(int key) const {
        return 1 + (key % (TABLE_SIZE - 1));
    }

public:
    PlayerTable() : table(TABLE_SIZE), occupied(TABLE_SIZE, false) {}

    void insert(const Player& p) {
        int h1 = hash1(p.id);
        int h2 = hash2(p.id);

        for (int i = 0; i < TABLE_SIZE; ++i) {
            int idx = (h1 + i * h2) % TABLE_SIZE;
            if (!occupied[idx]) {
                table[idx] = p;
                occupied[idx] = true;
                return;
            }
        }
        throw std::runtime_error("Table is full");
    }

    Player* find(int playerID) {
        int h1 = hash1(playerID);
        int h2 = hash2(playerID);

        for (int i = 0; i < TABLE_SIZE; ++i) {
            int idx = (h1 + i * h2) % TABLE_SIZE;
            if (!occupied[idx]) return nullptr;
            if (table[idx].id == playerID) return &table[idx];
        }
        return nullptr;
    }
};

class SkipList {
private:
    struct Node {
        int playerID;
        int score;
        vector<Node*> forward;

        Node(int id, int s, int level)
            : playerID(id), score(s), forward(level, nullptr) {
        }
    };

    static const int MAX_LEVEL = 16;
    int level;
    Node* header;
    int randomLevel() {
        int lvl = 1;
        while ((rand() % 2) && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }
    bool lessThan(int score1, int id1, int score2, int id2) const {
        if (score1 != score2) return score1 > score2;
        return id1 < id2;
    }

public:
    SkipList() : level(1) {
        header = new Node(-1, numeric_limits<int>::max(), MAX_LEVEL);
        srand(time(NULL));
    }

    void insert(int playerID, int score) {
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* current = header;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] &&
                lessThan(current->forward[i]->score,
                    current->forward[i]->playerID,
                    score, playerID)) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        int newLevel = randomLevel();
        if (newLevel > level) {
            for (int i = level; i < newLevel; ++i)
                update[i] = header;
            level = newLevel;
        }

        Node* newNode = new Node(playerID, score, newLevel);
        for (int i = 0; i < newLevel; ++i) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    bool searchByScore(int score) const {
        Node* current = header;
        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->score > score) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return current && current->score == score;
    }

    void removePlayer(int playerID) {
        Node* current = header->forward[0];
        int score = -1;
        while (current) {
            if (current->playerID == playerID) {
                score = current->score;
                break;
            }
            current = current->forward[0];
        }
        if (score == -1) return;

        vector<Node*> update(MAX_LEVEL, nullptr);
        current = header;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] &&
                lessThan(current->forward[i]->score,
                    current->forward[i]->playerID,
                    score, playerID)) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        Node* target = current->forward[0];
        if (!target || target->playerID != playerID) return;

        for (int i = 0; i < level; ++i) {
            if (update[i]->forward[i] != target) break;
            update[i]->forward[i] = target->forward[i];
        }
        delete target;

        while (level > 1 && header->forward[level - 1] == nullptr)
            --level;
    }

    vector<int> getTopN(int n) const {
        vector<int> result;
        Node* current = header->forward[0];

        while (current && n--) {
            result.push_back(current->playerID);
            current = current->forward[0];
        }
        return result;
    }
};
