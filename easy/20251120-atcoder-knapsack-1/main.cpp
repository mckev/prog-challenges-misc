// https://atcoder.jp/contests/dp/tasks/dp_d

#include <cassert>
#include <iostream>
#include <random>
#include <vector>


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
    long long solve(const std::vector<Item>& items, const int max_weight) {
        std::vector<long long> dp(max_weight + 1);
        for (const Item& item : items) {
            for (int w = max_weight; w >= item.weight; w--) {
                dp.at(w) = std::max(dp.at(w), dp.at(w - item.weight) + item.value);
            }
        }
        return dp.at(max_weight);
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
        std::vector<Item> items = { {0, 1, 1000000000}, {1, 1, 1000000000}, {2, 1, 1000000000}, {3, 1, 1000000000}, {4, 1, 1000000000} };
        int max_weight = 5;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        assert(solution_bruteforce.solve(items, max_weight) == 5000000000);
        SolutionEfficient solution_efficient = SolutionEfficient();
        assert(solution_efficient.solve(items, max_weight) == 5000000000);
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
        // Kevin
        std::vector<Item> items = { {0, 220, 170}, {1, 84, 19}, {2, 876, 56}, {3, 965, 261}, {4, 956, 49}, {5, 298, 538} };
        int max_weight = 952;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        assert(solution_bruteforce.solve(items, max_weight) == 727);
        SolutionEfficient solution_efficient = SolutionEfficient();
        assert(solution_efficient.solve(items, max_weight) == 727);
    }
    {
        // Kevin
        std::vector<Item> items = { {0, 866, 24}, {1, 159, 714}, {2, 39, 549}, {3, 420, 657}, {4, 662, 794}, {5, 375, 726}, {6, 593, 999}, {7, 588, 58}, {8, 140, 578}, {9, 265, 189}, {10, 163, 677}, {11, 131, 560}, {12, 768, 397}, {13, 151, 568}, {14, 628, 184}, {15, 820, 340} };
        int max_weight = 615;
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long answer_bruteforce = solution_bruteforce.solve(items, max_weight);
        assert(answer_bruteforce == 2537);
        SolutionEfficient solution_efficient = SolutionEfficient();
        long long answer_efficient = solution_efficient.solve(items, max_weight);
        assert(answer_efficient == 2537);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);             // [0, 1)
        for (int t = 0; t < 100000; t++) {
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
    test();
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
