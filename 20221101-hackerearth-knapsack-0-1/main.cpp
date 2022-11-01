// https://www.hackerearth.com/problem/algorithm/01-knapsack-problem-5a88b815/

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <vector>

struct Item {
    int value;
    int weight;
};

int solve(int C, const std::vector<Item>& items) {
    // Transform items into map of weight
    std::map<int, std::deque<int>> items_weights;
    for (int index = 0; index < items.size(); index++) {
        items_weights[items[index].weight].push_back(index);
    }
    // For each weight sort the items by its value, the most valuable first
    for (auto& [weight, items_weight] : items_weights) {
        std::sort(items_weight.begin(), items_weight.end(), [items](const int& a, const int& b) -> bool {
            return items[a].value > items[b].value;
        });
    }
    // Traverse the total weight from 0 to C
    // We use the feature of "map" that always have its key sorted
    struct Queue {
        int cur_value;
        std::map<int, std::deque<int>> items_weights;
    };
    std::map<int, Queue> queues;
    queues[0] = { 0, items_weights };
    int max_value = 0;
    while (! queues.empty()) {
        int cur_weight = queues.begin()->first; Queue queue = queues[cur_weight]; queues.erase(cur_weight);
        if (cur_weight > C) break;
        max_value = std::max(max_value, queue.cur_value);
        for (const auto& [weight, items_weight] : queue.items_weights) {
            // Only consider the most valuable item (i.e. front()) of each weight
            if (items_weight.empty()) continue;
            std::map<int, std::deque<int>> next_items_weights = queue.items_weights;
            int next_weight = cur_weight + weight;
            int next_value = queue.cur_value + items[next_items_weights[weight].front()].value;
            next_items_weights[weight].pop_front();

            bool is_in = queues.find(next_weight) != queues.end();
            if (!is_in || queues[next_weight].cur_value < next_value) {
                queues[next_weight] = { next_value, next_items_weights };
            }
        }
    }
    return max_value;
}

int main() {
    int N, C;
    std::vector<Item> items;

    std::cin >> N >> C;
    while (N--) {
        items.push_back({-1, -1});
    }
    for (Item& item : items) {
        std::cin >> item.value;
    }
    for (Item& item : items) {
        std::cin >> item.weight;
    }

    // N = 4;
    // C = 20;
    // items.push_back({10, 10});
    // items.push_back({2, 5});
    // items.push_back({1, 10});
    // items.push_back({3, 10});

    int answer = solve(C, items);
    std::cout << answer << std::endl;

    return 0;
}
