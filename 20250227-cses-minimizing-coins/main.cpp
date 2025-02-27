// Minimizing Coins: https://cses.fi/problemset/task/1634

#include <cassert>
#include <iostream>
#include <vector>


int solve(int target, const std::vector<int>& coins) {
    std::vector<int> amounts(target + 1);
    for (int coin : coins) {
        if (coin > target) continue;
        amounts[coin] = 1;
    }
    for (int i = 1; i <= target; i++) {
        for (int coin : coins) {
            if (i - coin < 1) continue;
            if (amounts[i - coin] == 0) continue;   // No path to achieve "i"
            amounts[i] = amounts[i] == 0 ? amounts[i - coin] + 1 : std::min(amounts[i], amounts[i - coin] + 1);
        }
    }
    return amounts[target] == 0 ? -1 : amounts[target];
}


void test() {
    {
        std::vector<int> coins = {1, 5, 7};
        int answer = solve(11, coins);
        assert(answer == 3);
    }
    {
        std::vector<int> coins = {1000000};
        int answer = solve(1, coins);
        assert(answer == -1);
    }
    {
        std::vector<int> coins = {2, 3, 4, 8, 9, 11, 14, 16, 17, 19};
        int answer = solve(200, coins);
        assert(answer == 11);
    }
}


int main() {
    // test();
    int N; std::cin >> N;
    int target; std::cin >> target;
    std::vector<int> coins;
    for (int n = 0; n < N; n++) {
        int coin; std::cin >> coin;
        coins.push_back(coin);
    }
    std::cout << solve(target, coins) << std::endl;
    return 0;
}
