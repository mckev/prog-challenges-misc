// https://leetcode.com/problems/coin-change/

#include <array>
#include <iostream>
#include <vector>


class Solution {
public:
    const int NA = -1;
    int coinChange(std::vector<int>& coins, int target) {
        std::array<int, 10000 + 1> minCoins; minCoins.fill(NA); minCoins[0] = 0;
        for (int i = 0; i <= target; i++) {
            if (i > 0 && minCoins[i] == NA) continue;           // there is no path to "i", however if i == 0 let it proceed to initialize
            for (int coin : coins) {
                if (i > 10000 - coin) continue;                 // prevent array out of bound
                minCoins[i + coin] = minCoins[i + coin] == NA ? minCoins[i] + 1 : std::min(minCoins[i + coin], minCoins[i] + 1);
            }
        }
        return minCoins[target];
    }
};


int main() {
    {
        Solution solution = Solution();
        std::vector<int> coins = {200, 500};
        std::cout << solution.coinChange(coins, 800) << std::endl;              // ans: 4
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {1, 2, 5};
        std::cout << solution.coinChange(coins, 11) << std::endl;               // ans: 3
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {1, 2147483647};
        std::cout << solution.coinChange(coins, 2) << std::endl;                // ans: 2
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {1};
        std::cout << solution.coinChange(coins, 0) << std::endl;                // ans: 0
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {2};
        std::cout << solution.coinChange(coins, 3) << std::endl;                // ans: -1
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {186, 419, 83, 408};
        std::cout << solution.coinChange(coins, 6249) << std::endl;             // ans: 20
    }
    {
        Solution solution = Solution();
        std::vector<int> coins = {357, 239, 73, 52};
        std::cout << solution.coinChange(coins, 9832) << std::endl;             // ans: 35
    }
}
