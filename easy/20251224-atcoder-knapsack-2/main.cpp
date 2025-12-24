// https://atcoder.jp/contests/dp/tasks/dp_e
// Hint: https://www.youtube.com/live/FAQxdm0bTaw?t=1668

#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#pragma GCC optimize "trapv"


struct Item {
    int id;
    int weight;
    int value;
};


class SolutionBruteforce {
private:
    long long best_value;
    std::vector<int> best_items_to_pick;

    void dfs(int pos, int cur_weight, long long cur_value, const std::vector<Item>& items, const int max_weight, std::vector<int>& items_to_pick) {
        if (cur_weight > max_weight) return;
        if (cur_value > best_value) {
            best_value = cur_value;
            best_items_to_pick = items_to_pick;
        }
        if (pos >= items.size()) return;
        // Case we pick
        Item item = items.at(pos);
        items_to_pick.push_back(item.id);
        dfs(pos + 1, cur_weight + item.weight, cur_value + item.value, items, max_weight, items_to_pick);
        items_to_pick.pop_back();
        // Case we skip
        dfs(pos + 1, cur_weight, cur_value, items, max_weight, items_to_pick);
    }

public:
    long long solve(const std::vector<Item>& items, const int max_weight) {
        best_value = 0;
        best_items_to_pick = {};
        std::vector<int> items_to_pick;
        dfs(0, 0, 0, items, max_weight, items_to_pick);
        return best_value;
    }
};


class SolutionEfficient {
public:
    long long INF = std::numeric_limits<long long>::max() - (1000000000 * 100);
    long long solve(const std::vector<Item>& items, const int max_weight) {
        int max_value = 0; for (const Item& item : items) max_value += item.value;
        std::vector<long long> dp(max_value + 1, INF);                  // dp[] tracks the minimum weight to achieve a value
        dp[0] = 0;
        for (const Item& item : items) {
            for (int v = max_value; v >= item.value; v--) {
                dp.at(v) = std::min(dp.at(v), dp.at(v - item.value) + item.weight);
            }
        }
        for (int v = max_value; v >= 0; v--) {
            if (dp.at(v) <= max_weight) {
                return v;
            }
        }
        assert(false);
    }
};


void test() {
    {
        std::vector<Item> items = { {0, 3, 30}, {1, 4, 50}, {2, 5, 60} };
        int max_weight = 8;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        assert(solution_bruteforce.solve(items, max_weight) == 90);
        SolutionEfficient solution_efficient = SolutionEfficient();
        assert(solution_efficient.solve(items, max_weight) == 90);
    }
    {
        std::vector<Item> items = { {0, 1000000000, 10} };
        int max_weight = 1000000000;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        assert(solution_bruteforce.solve(items, max_weight) == 10);
        SolutionEfficient solution_efficient = SolutionEfficient();
        assert(solution_efficient.solve(items, max_weight) == 10);
    }
    {
        std::vector<Item> items = { {0, 6, 5}, {1, 5, 6}, {2, 6, 4}, {3, 6, 6}, {4, 3, 5}, {5, 7, 2} };
        int max_weight = 15;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        assert(solution_bruteforce.solve(items, max_weight) == 17);
        SolutionEfficient solution_efficient = SolutionEfficient();
        assert(solution_efficient.solve(items, max_weight) == 17);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);             // [0, 1)
        for (int t = 0; t < 1000; t++) {
            std::vector<Item> items;
            int N = int(rnd(mt) * 20) + 1;
            for (int n = 0; n < N; n++) {
                int weight = int(rnd(mt) * 1000) + 1;
                int value = int(rnd(mt) * 1000) + 1;
                Item item = {n, weight, value};
                items.push_back(item);
            }
            int max_weight = int(rnd(mt) * 1000) + 1;
            SolutionBruteforce solution_bruteforce = SolutionBruteforce();
            long long answer_bruteforce = solution_bruteforce.solve(items, max_weight);
            SolutionEfficient solution_efficient = SolutionEfficient();
            long long answer_efficient = solution_efficient.solve(items, max_weight);
            assert(answer_efficient == answer_bruteforce);
        }
    }
}


int main() {
    // test();
    int N; std::cin >> N;
    int max_weight; std::cin >> max_weight;
    std::vector<Item> items;
    for (int n = 0; n < N; n++) {
        int w; std::cin >> w;
        int v; std::cin >> v;
        Item item = {n, w, v};
        items.push_back(item);
    }
    SolutionEfficient solution_efficient = SolutionEfficient();
    long long answer = solution_efficient.solve(items, max_weight);
    std::cout << answer << std::endl;
    return 0;
}
