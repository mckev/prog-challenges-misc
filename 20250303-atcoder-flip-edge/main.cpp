// Flip Edge : https://atcoder.jp/contests/abc395/tasks/abc395_e

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <vector>


struct Node {
    int node;
    bool is_flipped;
    bool operator<(const Node& other) const {
        return std::tie(node, is_flipped) < std::tie(other.node, other.is_flipped);
    }
};


long long solve(int target, long long flip_cost, const std::map<Node, std::vector<int>>& edges) {
    const int start = 1;
    struct Queue {
        int node;
        bool is_flipped;
        long long current_cost;
        bool operator<(const Queue& other) const {
            return current_cost > other.current_cost;
        }
    };
    std::priority_queue<Queue> queues;
    queues.push({start, false, 0});
    queues.push({start, true, flip_cost});

    // Speed up
    std::set<Node> visited;

    while (! queues.empty()) {
        Queue queue = queues.top(); queues.pop();
        if (queue.node == target) {
            return queue.current_cost;
        }
        // Speed up
        bool is_visited = visited.find({queue.node, queue.is_flipped}) != visited.end();
        if (is_visited) continue;
        visited.insert({queue.node, queue.is_flipped});
        // Main algorithm (priority queue)
        bool is_exist = edges.find({queue.node, queue.is_flipped}) != edges.end();
        if (is_exist) {
            for (int node : edges.at({queue.node, queue.is_flipped})) {
                queues.push({node, queue.is_flipped, queue.current_cost + 1});
            }
        }
        queues.push({queue.node, !queue.is_flipped, queue.current_cost + flip_cost});
    }
    assert(false);
}


void test() {
    {
        std::map<Node, std::vector<int>> edges = {
            {{1, false}, {2, }},
            {{1, true}, {3, }},
            {{2, false}, {4, }},
            {{2, true}, {1, 5, }},
            {{3, false}, {1, 5, }},
            {{3, true}, {4, }},
            {{4, false}, {3, }},
            {{4, true}, {2, }},
            {{5, false}, {2, }},
            {{5, true}, {3, }},
        };
        int target = 5;
        int flip_cost = 5;
        long long answer = solve(target, flip_cost, edges);
        assert(answer == 4);
    }
    {
        std::map<Node, std::vector<int>> edges = {
            {{1, false}, {2, }},
            {{1, true}, {3, }},
            {{2, false}, {4, }},
            {{2, true}, {1, 5, }},
            {{3, false}, {1, 5, }},
            {{3, true}, {4, }},
            {{4, false}, {3, }},
            {{4, true}, {2, }},
            {{5, false}, {2, }},
            {{5, true}, {3, }},
        };
        int target = 5;
        int flip_cost = 1;
        long long answer = solve(target, flip_cost, edges);
        assert(answer == 3);
    }
    {
        std::map<Node, std::vector<int>> edges = {
            {{1, true}, {2, }},
            {{2, false}, {1, 3, }},
            {{3, true}, {2, 4, }},
            {{4, false}, {3, 5, }},
            {{5, true}, {4, 6, }},
            {{6, false}, {5, 7}},
            {{7, true}, {6, 8, }},
            {{8, false}, {7, }},
        };
        int target = 8;
        int flip_cost = 613566756;
        long long answer = solve(target, flip_cost, edges);
        assert(answer == 4294967299);
    }
    {
        std::map<Node, std::vector<int>> edges = {
            {{1, false}, {3, }},
            {{2, true}, {14, }},
            {{3, false}, {5, }},
            {{3, true}, {1, }},
            {{4, false}, {6, 9, }},
            {{5, true}, {3, 8, }},
            {{6, true}, {4, }},
            {{8, false}, {5, 14, }},
            {{8, true}, {14, }},
            {{9, true}, {13, 4, }},
            {{12, false}, {19, }},
            {{13, false}, {9, }},
            {{14, false}, {18, 2, 8, }},
            {{14, true}, {8, }},
            {{17, true}, {18, }},
            {{18, false}, {17, }},
            {{18, true}, {14, 20, }},
            {{19, true}, {12, }},
            {{20, false}, {18, }},
        };
        int target = 20;
        int flip_cost = 5;
        long long answer = solve(target, flip_cost, edges);
        assert(answer == 21);
    }
}

int main() {
    // test();
    int target; std::cin >> target;
    int M; std::cin >> M;
    int flip_cost; std::cin >> flip_cost;
    std::map<Node, std::vector<int>> edges;
    for (int m = 0; m < M; m++) {
        int u; std::cin >> u;
        int v; std::cin >> v;
        edges[{u, false}].push_back(v);
        edges[{v, true}].push_back(u);
    }
    long long answer = solve(target, flip_cost, edges);
    std::cout << answer << std::endl;
    return 0;
}
