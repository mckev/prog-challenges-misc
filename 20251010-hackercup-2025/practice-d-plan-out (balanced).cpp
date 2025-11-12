// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/D
// How to run:
//   $ g++ -Ofast -o main.out main.cpp
//   $ ulimit -s unlimited
//   $ cat plan_out_input.txt | time ./main.out > plan_out_output.txt

#include <algorithm>
#include <bitset>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>


const int MAX_EDGES = 200000;

class SolutionBruteforce {
public:
    struct Answer {
        long long best_cost;
        std::vector<int> best_schedules;
    };
    Answer solve(int N, const std::vector<std::pair<int, int>>& activities) {
        struct Queue {
            int cur_activity;
            std::map<int, int> day1;        // key: coder, value: number of activities
            std::map<int, int> day2;
            long long cost;
            std::vector<int> schedules;
            bool operator<(const Queue& other) const {
                // Minimize cost
                return cost > other.cost;
            }
        };
        std::priority_queue<Queue> queues;
        queues.push({ -1 });
        while (! queues.empty()) {
            Queue queue; queue = queues.top(); queues.pop();
            if (queue.cur_activity == activities.size() - 1) {
                // This is optimal
                assert(queue.schedules.size() == activities.size());
                Answer answer = { queue.cost, queue.schedules };
                return answer;
            }
            int cur_activity = queue.cur_activity + 1;
            auto [coder1, coder2] = activities.at(cur_activity);
            assert(coder1 != coder2);
            {
                std::map<int, int> day1; day1 = queue.day1;
                day1[coder1]++;
                day1[coder2]++;
                long long cost = queue.cost
                    - queue.day1[coder1] * queue.day1[coder1] /* previous */ + day1[coder1] * day1[coder1]
                    - queue.day1[coder2] * queue.day1[coder2] /* previous */ + day1[coder2] * day1[coder2];
                // Schedules
                std::vector<int> schedules = queue.schedules; schedules.push_back(1);
                queues.push({ cur_activity, day1, queue.day2, cost, schedules });
            }
            {
                std::map<int, int> day2; day2 = queue.day2;
                day2[coder1]++;
                day2[coder2]++;
                long long cost = queue.cost
                    - queue.day2[coder1] * queue.day2[coder1] /* previous */ + day2[coder1] * day2[coder1]
                    - queue.day2[coder2] * queue.day2[coder2] /* previous */ + day2[coder2] * day2[coder2];
                // Schedules
                std::vector<int> schedules = queue.schedules; schedules.push_back(2);
                queues.push({ cur_activity, queue.day1, day2, cost, schedules });
            }
        }
        assert(false);
    }
};


class SolutionEfficient {
private:
    struct Edge {
        int edge_id;
        int node_id_to;
    };
    static const int DAY_1 = 0;
    static const int DAY_2 = 1;

    static bool assign_colors_dfs(int node_id, const std::vector<std::vector<Edge>>& edges_of_a_node, std::bitset<MAX_EDGES>& edges_color, std::vector<bool>& is_edge_processed, std::vector<int>& balances_of_a_node) {
        for (const Edge& edge : edges_of_a_node.at(node_id)) {
            if (is_edge_processed.at(edge.edge_id)) continue;
            // Process a single edge
            if (balances_of_a_node.at(node_id) > 0) {
                // Too much Day 1 activities
                edges_color[edge.edge_id] = DAY_2;
                balances_of_a_node.at(node_id)--;
                balances_of_a_node.at(edge.node_id_to)--;
            } else {
                // Too much Day 2 activities
                edges_color[edge.edge_id] = DAY_1;
                balances_of_a_node.at(node_id)++;
                balances_of_a_node.at(edge.node_id_to)++;
            }
            is_edge_processed.at(edge.edge_id) = true;
            assign_colors_dfs(edge.node_id_to, edges_of_a_node, edges_color, is_edge_processed, balances_of_a_node);
            return true;
        }
        return false;                                                           // No more edges to be processed
    }

    static int count_balance(int node_id, const std::vector<std::vector<Edge>>& edges_of_a_node, const std::bitset<MAX_EDGES>& edges_color) {
        // Note: All edges must already been processed, as bitset default is 0 which is translated to DAY_1
        int balance = 0;
        for (const Edge& edge : edges_of_a_node.at(node_id)) {
            if (edges_color[edge.edge_id] == DAY_1) {
                balance++;
            } else if (edges_color[edge.edge_id] == DAY_2) {
                balance--;
            } else {
                assert(false);
            }
        }
        return balance;
    }

    static long long count_total_cost(int N, const std::vector<std::vector<Edge>>& edges_of_a_node, const std::bitset<MAX_EDGES>& edges_color) {
        long long total_cost = 0;
        for (int node_id = 0; node_id < N; node_id++) {
            int d1 = 0, d2 = 0;
            for (const Edge& edge : edges_of_a_node.at(node_id)) {
                if (edges_color[edge.edge_id] == DAY_1) {
                    d1++;
                } else if (edges_color[edge.edge_id] == DAY_2) {
                    d2++;
                } else {
                    assert(false);
                }
            }
            total_cost += 1LL * d1 * d1 + 1LL * d2 * d2;
        }
        return total_cost;
    }

    static long long count_sub_cost(int N, const std::vector<std::vector<Edge>>& edges_of_a_node, const std::bitset<MAX_EDGES>& edges_color, const std::vector<int>& node_ids) {
        long long sub_cost = 0;
        for (int node_id : node_ids) {
            int d1 = 0, d2 = 0;
            for (const Edge& edge : edges_of_a_node.at(node_id)) {
                if (edges_color[edge.edge_id] == DAY_1) {
                    d1++;
                } else if (edges_color[edge.edge_id] == DAY_2) {
                    d2++;
                } else {
                    assert(false);
                }
            }
            sub_cost += 1LL * d1 * d1 + 1LL * d2 * d2;
        }
        return sub_cost;
    }

    static std::bitset<MAX_EDGES> balance_colors(int N, const std::vector<std::vector<Edge>>& edges_of_a_node, const std::bitset<MAX_EDGES>& initial_edges_color) {
        std::bitset<MAX_EDGES> best_edges_color = initial_edges_color;

        struct Queue {
            int node_id;
            long long cost;
            int balance_of_a_node;
            std::bitset<MAX_EDGES> edges_color;
            bool operator<(const Queue& other) const {
                return std::tie(cost, balance_of_a_node) < std::tie(other.cost, other.balance_of_a_node);
            }
        };
        std::priority_queue<Queue> queues;
        long long initial_total_cost = count_total_cost(N, edges_of_a_node, initial_edges_color);
        for (int node_id = 0; node_id < N; node_id++) {
            int balance = count_balance(node_id, edges_of_a_node, initial_edges_color);
            Queue queue = {
                node_id,
                initial_total_cost,
                balance,
                initial_edges_color,
            };
            if (std::abs(balance) > 1) {
                // To save memory we only put into queues the ones we want to explore
                queues.push(queue);
            }
        }

        struct History {
            std::bitset<MAX_EDGES> edges_color;
            bool operator==(const History& other) const noexcept {
                return edges_color == other.edges_color;
            }
        };
        struct HistoryHash {
            std::size_t operator()(const History& h) const noexcept {
                std::size_t seed = 0;
                const size_t num_blocks = (MAX_EDGES + 63) / 64;
                for (size_t block = 0; block < num_blocks; block++) {
                    uint64_t value = 0;
                    for (size_t bit = 0; bit < 64; ++bit) {
                        size_t index = block * 64 + bit;
                        if (index >= MAX_EDGES) break;
                        if (h.edges_color[index]) value |= (1ULL << bit);
                    }
                    // Combine the hashes (same technique as Boost's hash_combine)
                    seed ^= std::hash<uint64_t>{}(value) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
                }
                return seed;
            }
        };
        std::unordered_set<History, HistoryHash> history;

        while (! queues.empty()) {
            Queue queue = queues.top(); queues.pop();
            best_edges_color = queue.edges_color;
            if (queue.balance_of_a_node > 1) {
                // Too much Day 1 activities
                for (const Edge& edge : edges_of_a_node.at(queue.node_id)) {
                    if (queue.edges_color[edge.edge_id] == DAY_1) {
                        std::bitset<MAX_EDGES> edges_color = queue.edges_color;
                        long long sub_cost1 = count_sub_cost(N, edges_of_a_node, edges_color, { queue.node_id, edge.node_id_to });
                        edges_color[edge.edge_id] = DAY_2;
                        long long sub_cost2 = count_sub_cost(N, edges_of_a_node, edges_color, { queue.node_id, edge.node_id_to });
                        long long total_cost = queue.cost - sub_cost1 + sub_cost2;
                        // assert(total_cost == count_total_cost(N, edges_of_a_node, edges_color));
                        bool is_found = history.find({ edges_color }) != history.end(); if (is_found) continue; history.insert({ edges_color });
                        Queue new_queue = {
                            edge.node_id_to,
                            total_cost,
                            count_balance(edge.node_id_to, edges_of_a_node, edges_color),
                            edges_color,
                        };
                        queues.push(new_queue);
                    }
                }
            } else if (queue.balance_of_a_node < -1) {
                // Too much Day 2 activities
                for (const Edge& edge : edges_of_a_node.at(queue.node_id)) {
                    if (queue.edges_color[edge.edge_id] == DAY_2) {
                        std::bitset<MAX_EDGES> edges_color = queue.edges_color;
                        long long sub_cost1 = count_sub_cost(N, edges_of_a_node, edges_color, { queue.node_id, edge.node_id_to });
                        edges_color[edge.edge_id] = DAY_1;
                        long long sub_cost2 = count_sub_cost(N, edges_of_a_node, edges_color, { queue.node_id, edge.node_id_to });
                        long long total_cost = queue.cost - sub_cost1 + sub_cost2;
                        // assert(total_cost == count_total_cost(N, edges_of_a_node, edges_color));
                        bool is_found = history.find({ edges_color }) != history.end(); if (is_found) continue; history.insert({ edges_color });
                        Queue new_queue = {
                            edge.node_id_to,
                            total_cost,
                            count_balance(edge.node_id_to, edges_of_a_node, edges_color),
                            edges_color,
                        };
                        queues.push(new_queue);
                    }
                }
            } else {
                // Accepted balance (i.e. -1 <= balance <= 1)
            }
        }
        return best_edges_color;
    }

public:
    struct Answer {
        long long best_cost;
        std::vector<int> best_schedules;
    };
    Answer solve(int N, const std::vector<std::pair<int, int>>& activities) {
        // Edges
        int M = activities.size();
        std::vector<std::vector<Edge>> edges_of_a_node(N);                      // const
        for (int edge_id = 0; edge_id < M; edge_id++) {
            std::pair<int, int> activity = activities.at(edge_id);
            int node1 = activity.first; node1--;
            int node2 = activity.second; node2--;
            edges_of_a_node.at(node1).push_back({ edge_id, node2 });
            edges_of_a_node.at(node2).push_back({ edge_id, node1 });
        }

        // Firstly, let's do initial color assignment of all edges
        std::bitset<MAX_EDGES> initial_edges_color;
        {
            std::vector<bool> is_edge_processed(M, false);
            std::vector<int> balances_of_a_node(N, 0);                          // Positive: too much Day 1 activities. Negative: too much Day 2 activities. Zero: equal.
            for (int node_id = 0; node_id < N; node_id++) {
                if (assign_colors_dfs(node_id, edges_of_a_node, initial_edges_color, is_edge_processed, balances_of_a_node)) {
                    // Stay on same node_id if we can still process its edges
                    node_id--;
                }
            }
            // Make sure all edges have been colored
            for (int edge_id = 0; edge_id < M; edge_id++) {
                assert(is_edge_processed.at(edge_id) == true);
            }
        }

        // Secondly, balance the edge colors for each node
        std::bitset<MAX_EDGES> best_edges_color;
        // First pass solves most cases. However few edge cases such as Two triangles and Five triangles are unsolved.
        best_edges_color = balance_colors(N, edges_of_a_node, initial_edges_color);
        // Second pass seems to solve all cases so far. This is still a conjecture.
        best_edges_color = balance_colors(N, edges_of_a_node, best_edges_color);

        // Answer
        Answer answer;
        answer.best_cost = count_total_cost(N, edges_of_a_node, best_edges_color);
        for (int edge_id = 0; edge_id < M; edge_id++) {
            if (best_edges_color[edge_id] == DAY_1) {
                answer.best_schedules.push_back(1);
            } else if (best_edges_color[edge_id] == DAY_2) {
                answer.best_schedules.push_back(2);
            } else {
                assert(false);
            }
        }
        return answer;
    }
};


void test() {
    {
        int N = 5;
        std::vector<std::pair<int, int>> activities = {
            { 1, 2 },
            { 4, 2 },
            { 1, 5 },
            { 2, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 10);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 1, 5 },
            { 6, 3 },
            { 2, 3 },
            { 1, 2 },
            { 4, 3 },
            { 2, 6 },
            { 5, 2 },
            { 2, 4 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 26);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 5;
        std::vector<std::pair<int, int>> activities = {
            { 4, 5 },
            { 1, 3 },
            { 1, 5 },
            { 3, 2 },
            { 2, 1 },
            { 1, 4 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 16);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 3;
        std::vector<std::pair<int, int>> activities = {
            { 3, 1 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 2);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 8;
        std::vector<std::pair<int, int>> activities = {
            { 2, 4 },
            { 4, 5 },
            { 2, 3 },
            { 1, 6 },
            { 6, 7 },
            { 3, 5 },
            { 3, 4 },
            { 6, 8 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 22);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 11;
        std::vector<std::pair<int, int>> activities = {
            { 1, 2 },
            { 2, 3 },
            { 3, 4 },
            { 4, 5 },
            { 5, 3 },
            { 3, 6 },
            { 6, 7 },
            { 7, 3 },
            { 3, 8 },
            { 8, 9 },
            { 9, 3 },
            { 3, 10 },
            { 10, 11 },
            { 11, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 60);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin: Pascal triangle
        int N = 7;
        std::vector<std::pair<int, int>> activities = {
            { 5, 4 },
            { 6, 7 },
            { 5, 2 },
            { 7, 2 },
            { 1, 5 },
            { 1, 7 },
            { 5, 7 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 22);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin: Pentagon with 2 inner triangles
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 4, 2 },
            { 3, 4 },
            { 2, 5 },
            { 1, 6 },
            { 1, 3 },
            { 6, 2 },
            { 6, 5 },
            { 4, 5 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 24);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin: A triangle and an open rectangle
        int N = 7;
        std::vector<std::pair<int, int>> activities = {
            { 7, 4 },
            { 7, 1 },
            { 2, 7 },
            { 7, 3 },
            { 4, 1 },
            { 6, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 16);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin: Two triangles
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 3, 5 },
            { 4, 6 },
            { 6, 5 },
            { 2, 5 },
            { 3, 2 },
            { 1, 6 },
            { 1, 4 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 18);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin: Five triangles
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 2, 5 },
            { 3, 1 },
            { 6, 1 },
            { 4, 6 },
            { 6, 2 },
            { 3, 2 },
            { 6, 3 },
            { 2, 4 },
            { 5, 6 },
            { 4, 5 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 38);
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Stress test
        for (int t = 0; t < 100000; t++) {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
            int N = int(rnd(mt) * (10 + 1 - 2)) + 2;                        // [2, 10]
            int M = int(rnd(mt) * 20) + 1;                                  // [1, 20]
            std::set<std::pair<int, int>> activities_history;
            std::vector<std::pair<int, int>> activities;
            for (int m = 0; m < M; m++) {
                int coder1 = int(rnd(mt) * N) + 1;                          // [1, N]
                int coder2 = int(rnd(mt) * N) + 1;
                if (coder1 == coder2) continue;
                std::pair<int, int> activity = { coder1, coder2 };
                {
                    // No same activity (i.e. same coders pair)
                    bool is_exist1 = activities_history.find({ coder1, coder2 }) != activities_history.end(); if (is_exist1) continue;
                    bool is_exist2 = activities_history.find({ coder2, coder1 }) != activities_history.end(); if (is_exist2) continue;
                }
                activities_history.insert(activity);
                activities.push_back(activity);
            }
            M = activities.size();
            SolutionBruteforce solution_bruteforce = SolutionBruteforce();
            SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
            SolutionEfficient solution_efficient = SolutionEfficient();
            SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
            assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int M; std::cin >> M;
        std::vector<std::pair<int, int>> activities;
        for (int m = 0; m < M; m++) {
            int coder1; std::cin >> coder1;
            int coder2; std::cin >> coder2;
            activities.push_back({ coder1, coder2 });
        }
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(N, activities);
        std::cout << "Case #" << t + 1 << ": " << answer_efficient.best_cost << " ";
        for (int m = 0; m < M; m++) {
            std::cout << answer_efficient.best_schedules.at(m);
        }
        std::cout << std::endl;
    }
    return 0;
}
