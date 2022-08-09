// https://leetcode.com/problems/coin-change/

// #include <algorithm>
#include <deque>
#include <iostream>
#include <set>
#include <vector>
#pragma GCC optimize "trapv"


class Solution {
private:
    std::set<int> processed;
    struct QUEUE_T {
        long long cur;
        int nCoin;
    };

public:
    int coinChange(std::vector<int>& coins, int target) {
        // std::sort(coins.begin(), coins.end(), std::greater<>());    // Sort the coins descending
        std::deque<QUEUE_T> queues;
        queues.push_back({0, 0});
        while (! queues.empty()) {
            QUEUE_T queue = queues.front(); queues.pop_front();
            if (queue.cur == target) return queue.nCoin;
            if (queue.cur > target) continue;
            bool isIn = processed.find(queue.cur) != processed.end();
            if (isIn) continue;
            processed.insert(queue.cur);
            for (int coin : coins) {
                queues.push_back({queue.cur + coin, queue.nCoin + 1});
            }
        }
        return -1;
    }
};


int main() {
    Solution solution = Solution();
    // std::vector<int> coins = {200, 500};
    // int minCoin = solution.coinChange(coins, 800);                  // ans: 4
    // std::vector<int> coins = {1, 2, 5};
    // int minCoin = solution.coinChange(coins, 11);                   // ans: 3
    // std::vector<int> coins = {1, 2147483647};
    // int minCoin = solution.coinChange(coins, 2);                    // ans: 2
    // std::vector<int> coins = {1};
    // int minCoin = solution.coinChange(coins, 0);                    // ans: 0
    // std::vector<int> coins = {186, 419, 83, 408};
    // int minCoin = solution.coinChange(coins, 6249);                 // ans: 20
    std::vector<int> coins = {357, 239, 73, 52};
    int minCoin = solution.coinChange(coins, 9832);                 // ans: 35
    std::cout << minCoin << std::endl;
}
