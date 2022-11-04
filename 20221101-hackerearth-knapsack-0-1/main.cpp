// https://www.hackerearth.com/problem/algorithm/01-knapsack-problem-5a88b815/

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

struct Item {
    int value;
    int weight;
    bool operator<(const Item& other) const {
        // The most valuable first
        int value_reversed = -value, other_value_reversed = -other.value;
        // We need to compare all elements, otherwise "set" will consider them the same
        return std::tie(value_reversed, weight) < std::tie(other_value_reversed, other.weight);
    }
};

int solve(int C, const std::vector<Item>& items) {
    // Transform items into map of weight
    std::map<int, std::set<Item>> items_weights;
    for (const Item& item : items) {
        items_weights[item.weight].insert(item);    // We use "set" to sort items by their value. Gotcha: Cannot have items with same value and weight.
    }
    // Traverse the total weight from 0 to C
    int max_value = 0;
    struct Queue {
        int cur_value;
        int cur_weight;
        std::map<int, std::set<Item>> items_weights;
        bool operator<(const Queue& other) const {
            // Priority Queue sorted by highest first
            // Smaller weight first
            int cur_weight_reversed = -cur_weight, other_cur_weight_reversed = -other.cur_weight;
            return std::tie(cur_weight_reversed, cur_value) < std::tie(other_cur_weight_reversed, other.cur_value);
        }
    };
    std::priority_queue<Queue> queues;
    queues.push({ 0, 0, items_weights });
    int prev_weight = -1;
    while (! queues.empty()) {
        Queue queue = queues.top(); queues.pop();
        if (queue.cur_weight > C) break;
        if (queue.cur_weight == prev_weight) continue; prev_weight = queue.cur_weight;  // Skip all other items configurations that has same weight (only process the first one that has highest overall value)
        max_value = std::max(max_value, queue.cur_value);
        for (const auto& [weight, items_weight] : queue.items_weights) {
            // Only consider the most valuable item from each weight
            if (items_weight.empty()) continue;
            Item most_valuable_item_of_a_weight = *(items_weight.begin());
            int next_value = queue.cur_value + most_valuable_item_of_a_weight.value;
            int next_weight = queue.cur_weight + most_valuable_item_of_a_weight.weight;
            std::map<int, std::set<Item>> next_items_weights = queue.items_weights; next_items_weights[weight].erase(most_valuable_item_of_a_weight);
            queues.push({ next_value, next_weight, next_items_weights });
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
