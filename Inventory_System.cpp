#include <iostream>
#include "ArcadiaEngine.h" 

using namespace std;

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

